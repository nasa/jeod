//=============================================================================
// Notices:
//
// Copyright © 2025 United States Government as represented by the Administrator
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
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynManager
 * @{
 *
 * @file models/dynamics/dyn_manager/include/dynamics_integration_group.hh
 * Define the extensible class DynamicsIntegrationGroup, an instance of which is
 * responsible for integrating the states of a set of DynBody objects.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/dynamics_integration_group.cc))



*******************************************************************************/

#ifndef JEOD_DYNAMICS_INTEGRATION_GROUP_HH
#define JEOD_DYNAMICS_INTEGRATION_GROUP_HH

// System includes

// JEOD includes
#include "utils/container/include/pointer_vector.hh"
#include "utils/integration/include/jeod_integration_group.hh"
#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod
{

// Forward declarations
class DynBody;
class DynManager;
class GravityManager;
class JeodIntegrationTime;
class JeodIntegratorInterface;

/**
 A DynamicsIntegrationGroup integrates the state of a set of DynBoby objects
 over time.
 The class provides implementations of all virtual functions listed below
 and the pure virtuals defined in the base class.
 This class is designed for extensibility. Authors of derived classes should
 follow the extension notes in the source file.
*/
class DynamicsIntegrationGroup : public JeodIntegrationGroup
{
    JEOD_MAKE_SIM_INTERFACES(jeod, DynamicsIntegrationGroup)
public:
    // Constructors

    // Default constructor.
    // This exists internally for checkpoint restart use.
    // Externally, the default constructor can be used to create a non-functional
    // integration group object that is assigned to the
    // DynManagerInit::integ_group_constructor. The dynamics manager will
    // call this object's create_group method to create a functional
    // integration group object as the dynamic manager's default integ group.
    DynamicsIntegrationGroup();

    // Non-default constructor.
    explicit DynamicsIntegrationGroup(JeodIntegrationGroupOwner & owner,
                                      er7_utils::IntegratorConstructor & integ_cotr,
                                      JeodIntegratorInterface & integ_inter,
                                      JeodIntegrationTime & time_mngr);

    // Destructor
    ~DynamicsIntegrationGroup() override;

    DynamicsIntegrationGroup(const DynamicsIntegrationGroup &) = delete;
    DynamicsIntegrationGroup & operator=(const DynamicsIntegrationGroup &) = delete;

    // Non-virtual methods.
    // That these are not virtual is intentional.

    /**
     * Query whether the group is void of registered bodies.
     * @return True if group is empty, false otherwise.
     */
    // That these are not virtual is intentional.

    /*
     Purpose:
       (Query whether the group is void of registered bodies.)
    */
    bool is_empty() const
    {
        return dyn_bodies.empty();
    }

    // Virtual methods.

    // Factory method.
    // This method provides a means for a non-functional integration group
    // object (one created by a default constructor) to create a functional
    // equivalent. Derived classes should override this method,
    // with the created object of the same type as the original.
    virtual DynamicsIntegrationGroup * create_group(JeodIntegrationGroupOwner & owner,
                                                    er7_utils::IntegratorConstructor & integ_cotr,
                                                    JeodIntegratorInterface & integ_inter,
                                                    JeodIntegrationTime & time_mngr) const;

    // Pre-initialize the group and register it with the dynamics manager.
    // Usage note:
    //   This function should be invoked as an S_define-level initialization job,
    //   and it must be called fairly early in the initialization process,
    //   between the calls to DynManager::initialize_model and
    //   DynManager::initialize_simulation.
    virtual void register_group(DynManager & dyn_manager);

    // Complete the group initialization process.
    // Usage note:
    //   This function is called by DynManager::initialize_simulation.
    //   Others should not call this.
    virtual void initialize_group(DynManager & dyn_manager);

    // Perform actions that need to be taken before entering the derivative /
    // integration loop. The base action is to set the time model to the time at
    // the start of the integration loop. This capability is essential for
    // a multiple integration rate simulation. Simple monolithic simulations
    // can sometimes get away with a scheduled job that sets time so long as
    // that job is called at the dynamics (integration) rate.
    virtual void prepare_for_integ_loop(double sim_endtime);

    // Compute the gravitational acceleration for each body in the group.
    virtual void gravitation(DynManager & dyn_manager, GravityManager & gravity_manager);

    // Collect derivatives for each root DynBody object.
    virtual void collect_derivatives();

    // Integrate the states of the DynBody objects that comprise the group.
    // This function should only be called by IntegrationControls::integrate
    // or by an override of that function.
    er7_utils::IntegratorResult integrate_bodies(double cycle_dyndt, unsigned int target_stage) override;

    using JeodIntegrationGroup::initialize_group;

    // Add a DynBody to the set of bodies that comprise the group.
    // Usage note:
    //   Moving a body into or out of an integration group must be coordinated
    //   with moving the corresponding derivative functions into or out of
    //   the integration loop object.
    virtual void add_dyn_body(DynBody & body);

    // Delete a DynBody from the set of bodies that comprise the group.
    virtual void delete_dyn_body(DynBody & body);

    // Member data

    /**
     * Update ephemerides at the derivative rate?
     */
    bool deriv_ephem_update{}; //!< trick_units(--)

protected:
    // Member functions

    // Reset the group's state integrators.
    // Resets can occur when time changes behavior (call is internal to the
    // integration process) or when some external event would render an
    // integrator's history invalid (call comes from outside).
    // When this happens, integrators that depend on history need to reset their
    // internal state to indicate that they have no history data.
    void reset_body_integrators() override;

    // Member data

    /**
     * List of vehicles whose state is integrated by this group.
     */
    JeodPointerVector<DynBody>::type dyn_bodies; //!< trick_io(**)

    /**
     * This flag is always true for JEOD integration groups.
     * Setting this flag to false results in bypassing the call in
     * DynamicsIntegrationGroup::add_dyn_body to DynBody::create_body_integrators.
     * This hook exists for derived classes that override
     * DynamicsIntegrationGroup::integrate_bodies in a way that does not involve
     * calling DynBody::integrate.
     */
    bool bodies_integrated_separately{true}; //!< trick_units(--)

private:
    // Register items in the base class with the memory manager
    void register_base_contents();
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
