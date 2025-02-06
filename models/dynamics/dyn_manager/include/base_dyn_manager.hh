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
 * @file models/dynamics/dyn_manager/include/base_dyn_manager.hh
 * Define the BaseDynManager class, which defines the interfaces to the
 * class DynManager.
 */

/*******************************************************************************

Purpose:
  ()



*******************************************************************************/

#ifndef JEOD_BASE_DYN_MANAGER_HH
#define JEOD_BASE_DYN_MANAGER_HH

// JEOD includes
#include "environment/ephemerides/ephem_manager/include/base_ephem_manager.hh"
#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod
{

class BodyAction;
class DynamicsIntegrationGroup;
class DynBody;
class GravityManager;
class JeodIntegrationGroup;
class MassBody;

/**
 * The DynManager class augments the EphemManager with dynamics-related
 * items. This class defines the external interfaces to that class.
 */
class BaseDynManager : virtual public BaseEphemeridesManager
{
    JEOD_MAKE_SIM_INTERFACES(jeod, BaseDynManager)

public:
    // Member functions

    // Member functions
    // Note: The default constructor, copy constructor, and assignment operator
    // for this class are not declared. The C++ defaults are in force.
    ~BaseDynManager() override = default;

    /**
     * Set the Gravity Manager.
     * @param gravity link to the manager of gravity model.
     */
    virtual void set_gravity_manager(GravityManager & gravity) = 0;

    /**
     * Initialize the gravity model controls.
     */
    virtual void initialize_gravity_controls() = 0;

    /**
     * Reset the gravity model controls.
     */
    virtual void reset_gravity_controls() = 0;

    /**
     * Add a mass body to the list of such.
     * @param mass_body  Body to be added to the list of mass bodies.
     */
    virtual void add_mass_body(MassBody & mass_body) = 0;

#ifndef SWIG
    /**
     * Add a mass body to the list of such.
     * @param mass_body  Body to be added to the list of mass bodies.
     */
    virtual void add_mass_body(MassBody * mass_body) = 0;
#endif

    /**
     * Find a mass body.
     * @param name  Mass body name.
     * @return Pointer to the mass body with the given name.
     */
    virtual MassBody * find_mass_body(const std::string & name) const = 0;

    /**
     * Check if a mass body has been registered with the dynamics manager.
     * @param mass_body  Mass body to be checked.
     * @return True if the body is registered, false otherwise.
     */
    virtual bool is_mass_body_registered(const MassBody * mass_body) const = 0;

    /**
     * Add a dynamic body to the list of such.
     * @param dyn_body  Body to be added to the list of dynamic bodies.
     */
    virtual void add_dyn_body(DynBody & dyn_body) = 0;

    /**
     * Find a dynamic body.
     * @param name  Dynamic body name.
     * @return Pointer to the dynamic body with the given name.
     */
    virtual DynBody * find_dyn_body(const std::string & name) const = 0;

    /**
     * Return a copy of the list of registered dynamic bodies.
     * @return Copy of dyn_bodies data member
     */
    virtual std::vector<DynBody *> get_dyn_bodies() const = 0;

    /**
     * Check if a dynamic body has been registered with the dynamics manager.
     * @param dyn_body  Dynamic body to be checked.
     * @return True if the body is registered, false otherwise.
     */
    virtual bool is_dyn_body_registered(const DynBody * dyn_body) const = 0;

    /**
     * Add an integration group to the list of such.
     * @param integ_group  Group to be added to the list of integration groups.
     */
    virtual void add_integ_group(DynamicsIntegrationGroup & integ_group) = 0;

    /**
     * Check if an integration group has been registered.
     * @param integ_group  Integration group to be checked.
     * @return True if the group is registered, false otherwise.
     */
    virtual bool is_integ_group_registered(const DynamicsIntegrationGroup * integ_group) const = 0;

    /**
     * Force all integrators to reset themselves.
     */
    virtual void reset_integrators() = 0;

    /**
     * Instruct specific integration group to reset its integrators.
     * @param integ_group  Integration group to be reset.
     */
    virtual void reset_integrators(DynamicsIntegrationGroup & integ_group) = 0;

    /**
     * Get the time at which the manager was last updated.
     * @return Time at which the manager was last updated.
     */
    virtual double timestamp() const = 0;
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
