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
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynManager
 * @{
 *
 * @file models/dynamics/dyn_manager/include/dyn_manager.hh
 * Define the DynManager class, which manages the planets and vehicles
 * modeled in a JEOD-based simulation.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/dyn_manager.cc))



*******************************************************************************/

#ifndef JEOD_DYN_MANAGER_HH
#define JEOD_DYN_MANAGER_HH

// system includes
#include <list>
#include <vector>

// JEOD includes
#include "environment/ephemerides/ephem_manager/include/ephem_manager.hh"
#include "environment/planet/include/planet.hh"
#include "utils/integration/include/jeod_integration_group.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "base_dyn_manager.hh"
#include "dyn_manager_init.hh"
#include "dynamics_integration_group.hh"

//! Namespace jeod
namespace jeod
{

class BodyAction;
class DynamicsIntegrationGroup;
class DynBody;
class JeodIntegratorInterface;
class MassBody;
class GravityManager;
class Planet;
class TimeManager;
class SinglePointEphemeris;

/**
 * The DynManager class manages the dynamic elements of a simulation.
 * The primary functions of a DynManager are to:
 *   - Dynamically determine which ephemerides are needed in a simulation.
 *   - Initialize ephemeris models and keep them in sync with the rest
 *     of the simulation.
 *   - Initialize mass bodies and dynamic bodies independently of the order
 *     in which these bodies are declared in the S_define file.
 *   - Coordinate the computation of the cumulative forces and torques and
 *     gravitational effects on the dynamic bodies in a simulation.
 *   - Coordinate the integration of time and and of dynamic body states.
 *   - Apply asynchronous actions to bodies.
 *
 * The DynManager can operate in one of three modes: empty space,
 * single planet, and ephemeris mode.  The DynManager inherits from
 * EphemerisInterface so that when it operates in empty space or
 * single-planet mode it can properly register itself as
 * the owner of the reference frame tree root node.
 */
class DynManager : virtual public JeodIntegrationGroupOwner,
                   virtual public BaseDynManager,
                   public EphemeridesManager
{
    JEOD_MAKE_SIM_INTERFACES(jeod, DynManager)

public:
    // Member functions

    // Constructor and destructor
    DynManager();
    ~DynManager() override;

    DynManager(const DynManager &) = delete;
    DynManager & operator=(const DynManager &) = delete;

    /**
     * Determine if the manager has been initialized.
     * @return Initialization status
     */
    bool is_initialized()
    {
        return initialized;
    }

    // Initialize the DynManager (but not much else).
    void initialize_model(DynManagerInit & init, TimeManager & time_mngr);

    void initialize_model(JeodIntegratorInterface & integ_if, DynManagerInit & init, TimeManager & time_mngr);

    // Initialize the simulation.
    void initialize_simulation();

    // Set the Gravity Manager.
    void set_gravity_manager(GravityManager & gravity) override;

    // Initialize the gravity model controls.
    void initialize_gravity_controls() override;

    // Reset the gravity model controls.
    void reset_gravity_controls() override;

    // Compute gravity. Note: NOT virtual.
    void gravitation();

    // Add a mass body to the list of such.
    void add_mass_body(MassBody & mass_body) override;
#ifndef SWIG
    void add_mass_body(MassBody * mass_body) override;
#endif

    // Find a mass body.
    MassBody * find_mass_body(const std::string & name) const override;

    // Check if a mass body has been registered with the dynamics manager.
    bool is_mass_body_registered(const MassBody * mass_body) const override;

    // Add a dynamic body to the list of such.
    void add_dyn_body(DynBody & dyn_body) override;

    // Find a dynamic body.
    DynBody * find_dyn_body(const std::string & name) const override;

    /**
     * Return a copy of the list of registered dynamic bodies.
     * @return Copy of dyn_bodies data member
     */
    std::vector<DynBody *> get_dyn_bodies() const override
    {
        return dyn_bodies;
    }

    // Check if a dynamic body has been registered with the dynamics manager.
    bool is_dyn_body_registered(const DynBody * dyn_body) const override;

    // Add an integration group to the list of such.
    void add_integ_group(DynamicsIntegrationGroup & integ_group) override;

    // Check if an integration group has been registered.
    bool is_integ_group_registered(const DynamicsIntegrationGroup * integ_group) const override;

    // Enqueue a body action.
    void add_body_action(BodyAction & body_action);

    // remove a body action from the queue based on its name.
    void remove_body_action(const std::string & action_name_in);

    // Perform body actions that are ready to be applied.
    void perform_actions();

    // Initialize the integration groups.
    void initialize_integ_groups();

    // Update the specified integ group (which should be the default group).
    void update_integration_group(JeodIntegrationGroup & group) override;

    // Initialize all registered dynamic bodies.
    void initialize_dyn_bodies();

    // Initialize a specific dynamic body.
    void initialize_dyn_body(DynBody & body);

    /**
     * Collect forces and torques on each body and compute derivatives.
     */
    void compute_derivatives()
    {
        default_integ_group->collect_derivatives();
    }

    /**
     * Force all integrators to reset themselves.
     */
    void reset_integrators() override;

    /**
     * Instruct specific integrator to reset itself.
     * @param integ_group  Integration group to be reset.
     */
    void reset_integrators(DynamicsIntegrationGroup & integ_group) override
    {
        integ_group.reset_integrators();
    }

    /**
     * Propagate all vehicles and propagate time.
     * @param to_sim_time Simulation time seconds of end of integration interval.
     * @return zero if complete, non-zero if incomplete.
     */
    int integrate(double to_sim_time, TimeManager &)
    {
        return default_integ_group->integrate_group_to(to_sim_time);
    }

    // Get the time at which the manager was last updated.
    double timestamp() const override;

    // Get the manager's name.
    const std::string name() const;

    // Shutdown the dynamics manager.
    void shutdown();

    // Member data

    /**
     * Update ephemerides at the derivative rate?
     */
    bool deriv_ephem_update{}; //!< trick_units(--)

    /**
     * This flag exists primarily to support unit tests. Typical simulations
     * should not set this flag. The intent is to support simulations that use
     * planetary ephemerides but neither need nor have a gravity model.
     */
    bool gravity_off{}; //!< trick_units(--)

    /**
     * The ephemeris mode in which the dynamics manager operates.
     */
    DynManagerInit::EphemerisMode mode{DynManagerInit::EphemerisMode_Ephemerides}; //!< trick_units(--)

    /**
     * Pointer to the integration object used by the simulation engine itself.
     */
    JEOD_SIM_INTEGRATOR_POINTER_TYPE sim_integrator{}; //!< trick_units(--)

protected:
    // Member functions

    // Initialize the DynManager (but not much else).
    virtual void initialize_model_internal(DynManagerInit & init, TimeManager & time_mngr);

    // initialize_dyn_bodies support methods
    void perform_mass_body_initializations(MassBody * body = nullptr);
    void perform_mass_attach_initializations();
    void perform_dyn_body_initializations(DynBody * body = nullptr);
    void check_for_uninitialized_states();

    // Member data

    /**
     * Have all initializations been performed?
     */
    bool initialized{}; //!< trick_units(--)

    /**
     * The model that encapsulates all of the gravity models.
     */
    GravityManager * gravity_manager{}; //!< trick_units(--)

    /**
     * Integrator generator
     */
    er7_utils::IntegratorConstructor * integ_constructor{}; //!< trick_units(--)

    /**
     * Interface with the simulation integration structure
     */
    JeodIntegratorInterface * integ_interface{}; //!< trick_units(--)

    /**
     * The integration group used for simple monolithic simulations.
     */
    DynamicsIntegrationGroup * default_integ_group{}; //!< trick_units(--)

    /**
     * Simple ephemeris for use in empty space and single planet modes
     */
    SinglePointEphemeris * simple_ephemeris{}; //!< trick_units(--)

    /**
     * List of vehicle models.
     */
    std::vector<MassBody *> mass_bodies;

    /**
     * List of vehicle models.
     */
    std::vector<DynBody *> dyn_bodies;

    /**
     * List of integration groups.
     */
    std::vector<DynamicsIntegrationGroup *> integ_groups;

    /**
     * List of body initializers.
     */
    std::list<BodyAction *> body_actions;
};

} // namespace jeod

#ifdef TRICK_VER
#include "environment/ephemerides/ephem_interface/include/simple_ephemerides.hh"
#include "er7_utils/integration/core/include/integrator_constructor_factory.hh"
#endif

#endif

/**
 * @}
 * @}
 * @}
 */
