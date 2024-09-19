//=============================================================================
// Notices:
//
// Copyright © 2023 United States Government as represented by the Administrator
// of the National Aeronautics and Space Administration.  All Rights Reserved.
//
//
// Disclaimers:
//
// No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF
// ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED
// TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO SPECIFICATIONS, ANY
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
// FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL BE ERROR
// FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO THE
// SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN
// ENDORSEMENT BY GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS,
// RESULTING DESIGNS, HARDWARE, SOFTWARE PRODUCTS OR ANY OTHER APPLICATIONS
// RESULTING FROM USE OF THE SUBJECT SOFTWARE.  FURTHER, GOVERNMENT AGENCY
// DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING THIRD-PARTY SOFTWARE,
// IF PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT "AS IS."
//
// Waiver and Indemnity:  RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS AGAINST THE
// UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
// PRIOR RECIPIENT.  IF RECIPIENT'S USE OF THE SUBJECT SOFTWARE RESULTS IN ANY
// LIABILITIES, DEMANDS, DAMAGES, EXPENSES OR LOSSES ARISING FROM SUCH USE,
// INCLUDING ANY DAMAGES FROM PRODUCTS BASED ON, OR RESULTING FROM, RECIPIENT'S
// USE OF THE SUBJECT SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE
// UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
// PRIOR RECIPIENT, TO THE EXTENT PERMITTED BY LAW.  RECIPIENT'S SOLE REMEDY FOR
// ANY SUCH MATTER SHALL BE THE IMMEDIATE, UNILATERAL TERMINATION OF THIS
// AGREEMENT.
//
//=============================================================================
//
//
/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Integration
 * @{
 *
 * @file models/utils/integration/include/jeod_integration_group.hh
 * Define the extensible class IntegrationGroup, an instance of which is
 * responsible for integrating the states of a set of DynBody objects.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/jeod_integration_group.cc))



*******************************************************************************/

#ifndef JEOD_INTEGRATION_GROUP_HH
#define JEOD_INTEGRATION_GROUP_HH

// Local includes
#include "time_change_subscriber.hh"

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

#include "utils/container/include/pointer_vector.hh"
#include "utils/sim_interface/include/jeod_integrator_interface.hh"

// ER7 utilities includes
#include "er7_utils/integration/core/include/base_integration_group.hh"
#include "er7_utils/integration/core/include/integrable_object.hh"
#include "er7_utils/integration/core/include/integrator_constructor.hh"
#include "er7_utils/integration/core/include/integrator_result_merger_container.hh"

// System includes
#include <cstddef>

//! Namespace jeod
namespace jeod
{

// Forward declarations
class JeodIntegrationGroup;
class JeodIntegrationTime;

/**
 * The abstract class IntegrationGroupOwner contains an IntegrationGroup.
 * This simple interface class has no data members.
 */
class JeodIntegrationGroupOwner
{
public:
    /**
     * Destructor.
     */
    virtual ~JeodIntegrationGroupOwner() = default;

    /**
     * Somehow update the specified integration group.
     * @param[in,out] group Integration group to be updated.
     */
    virtual void update_integration_group(JeodIntegrationGroup & group) = 0;
};

/**
 * A JeodIntegrationGroup integrates the state of a set of objects over time.
 * This class is designed for extensibility. Authors of derived classes should
 * follow the extension notes in the source file.
 */
class JeodIntegrationGroup : public er7_utils::BaseIntegrationGroup,
                             virtual public TimeChangeSubscriber
{
    JEOD_MAKE_SIM_INTERFACES(jeod, JeodIntegrationGroup)
public:
    // Static methods

    /**
     * Register classes associated with integration.
     * This is a static method, and is best called prior to initialization time.
     */
    static void register_classes();

    // Constructors and destructor.

    /**
     * JeodIntegrationGroup default constructor, needed for checkpoint/restart,
     * and to support derived classes' default constructors.
     */
    JeodIntegrationGroup();

    /**
     * JeodIntegrationGroup non-default constructor.
     * @param[in] owner       The object that contains this group.
     * @param[in] integ_cotr   Integrator constructor
     * @param[in] integ_inter  Integrator interface
     * @param[in] time_mngr    Time manager
     */
    JeodIntegrationGroup(JeodIntegrationGroupOwner & owner,
                         er7_utils::IntegratorConstructor & integ_cotr,
                         JeodIntegratorInterface & integ_inter,
                         JeodIntegrationTime & time_mngr);

    /**
     * JeodIntegrationGroup destructor
     */
    ~JeodIntegrationGroup() override;

    JeodIntegrationGroup(const JeodIntegrationGroup &) = delete;
    JeodIntegrationGroup & operator=(const JeodIntegrationGroup &) = delete;

    // Non-virtual methods.
    // That these are not virtual is intentional.

    /**
     * Indicate whether derivatives need to be calculated on the initial
     * step of an integration cycle.
     * @return Desired flag.
     */
    bool need_first_step_derivatives() const
    {
        return integ_constructor->need_first_step_derivatives();
    }

    /**
     *  Update the group via its owner.
     */
    void update_from_owner()
    {
        group_owner->update_integration_group(*this);
    }

    /**
     * Merge an IntegratorResult into another.
     * @return True if merger was successful, false if some error occurred.
     * @param[in]      new_result     Size of the generalized position vector
     * @param[in,out]  merged_result  Size of the generalized position vector
     */
    bool merge_integrator_result(const er7_utils::IntegratorResult & new_result,
                                 er7_utils::IntegratorResult & merged_result) const
    {
        return integ_merger.merge_integrator_result(new_result, merged_result);
    }

    // Virtual methods.

    /**
     * Respond to a change in the nature of time.
     */
    void respond_to_time_change() override
    {
        if(integ_controls != nullptr)
        {
            integ_controls->set_reset_needed();
        }
    }

    /**
     * Initialize the integration group.
     * Some integration techniques are configurable by user input, and thus
     * the creation of the controls and integrators needs to be delayed a bit.
     */
    void initialize_group() override;

    /**
     * Reset the integrators for the integrable objects managed by this group.
     * Resets can occur when time changes behavior (call is internal to the
     * integration process) or when some external event would render an
     * integrator's history invalid (call comes from outside).
     * When either happens, integrators that depend on history need to reset
     * their internal state to indicate that the saved data are invalid.)
     */
    void reset_body_integrators() override
    {
        reset_container(integrable_objects);
    }

    /**
     * Integrate the states of the integrable objects managed by this group.
     * This function should only be called by IntegrationControls::integrate
     * or by an override of that function.
     * Derived classes are free to override this default implementation.
     * However, those derived class overrides either must call this method to
     * integrate the states of the registered integrable bodies or must somehow
     * take on the burden of integrating those states.
     * @param[in]     cycle_dyndt   Dynamic time step, in dynamic time seconds.
     * @param[in]     target_stage  The stage of the integration process
     *                              that the integrator should try to attain.
     * @return The status (time advance, pass/fail status) of the integration.
     */
    er7_utils::IntegratorResult integrate_bodies(double cycle_dyndt, unsigned int target_stage) override
    {
        return integrate_container(cycle_dyndt, target_stage, integrable_objects);
    }

    /**
     * Add an integrable object to the vector of such.
     * @param[in] integrable_object  Object to be added.
     */
    virtual void add_integrable_object(er7_utils::IntegrableObject & integrable_object);

    /**
     * Remove an integrable object from the vector of such.
     * @param[in] integrable_object  Object to be removed.
     */
    virtual void remove_integrable_object(er7_utils::IntegrableObject & integrable_object);

protected:
    // Member functions

    /**
     * Issue a reset to each member of a container.
     * @tparam T          The container type.
     * @param[in,out]  container  The container to be reset.
     */
    template<typename T> void reset_container(const T & container)
    {
        // Reset each registered integrable object.
        for(auto iter = container.begin(); iter != container.end(); ++iter)
        {
            (*iter)->reset_integrators();
        }
    }

    /**
     * Integrate each member of a collection.
     * @tparam T            The container type.
     * @param[in]  dyn_dt       Dynamic time step, in dynamic time seconds.
     * @param[in]  target_stage The stage of the integration process
     *                          that the integrator should try to attain.
     * @param[in,out]  container    The container to be integrated.
     */
    template<typename T>
    er7_utils::IntegratorResult integrate_container(double dyn_dt, unsigned int target_stage, const T & container)
    {
        er7_utils::IntegratorResult status(false);

        // Integrate each registered integrable object.
        for(auto iter = container.begin(); iter != container.end(); ++iter)
        {
            integ_merger.merge_integrator_result((*iter)->integrate(dyn_dt, target_stage), status);
        }
        return status;
    }

    // Member data

    // Note: The first four of the following are const pointers rather than
    // references for the sake of checkpoint/restart. All of the pointers
    // will be non-null after construction.

    /**
     * The object that owns this integration group, typically by containment.
     */
    JeodIntegrationGroupOwner * const group_owner{}; //!< trick_units(--)

    /**
     * The object that merges results from multiple integrators.
     */
    er7_utils::IntegratorResultMergerContainer integ_merger; //!< trick_units(--)

    /**
     * The interface between the integration module and the simulation engine's
     * integration structure.
     */
    JeodIntegratorInterface * const jeod_integ_interface{}; //!< trick_units(--)

    /**
     * The interface between the integration module and the object that
     * represents time.
     */
    JeodIntegrationTime * const jeod_time_manager{}; //!< trick_units(--)

    /**
     * The objects whose states are integrated by this integration group.
     */
    JeodPointerVector<er7_utils::IntegrableObject>::type integrable_objects; //!< trick_io(**)
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
