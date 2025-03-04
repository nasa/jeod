/*
 * dyn_manager_ut.hh
 *
 *  Created on: Dec 13, 2023
 *      Author: tbrain
 */

#include "dynamics/dyn_manager/include/dyn_manager.hh"

using namespace jeod;

class DynManagerTest : public DynManager
{
public:
    void set_initialized(bool initialized_in)
    {
        initialized = initialized_in;
    }

    GravityManager * get_gravity_manager()
    {
        return gravity_manager;
    }

    void set_gravity_manager(GravityManager & gravity_manager_in)
    {
        gravity_manager = &gravity_manager_in;
    }

    er7_utils::IntegratorConstructor * get_integ_constructor()
    {
        return integ_constructor;
    }

    JeodIntegratorInterface * get_integ_interface()
    {
        return integ_interface;
    }

    void set_integ_interface(JeodIntegratorInterface * integ_interface_in)
    {
        integ_interface = integ_interface_in;
    }

    DynamicsIntegrationGroup * get_default_integ_group()
    {
        return default_integ_group;
    }

    void set_default_integ_group(DynamicsIntegrationGroup * default_integ_group_in)
    {
        default_integ_group = default_integ_group_in;
    }

    SinglePointEphemeris * get_simple_ephemeris()
    {
        return simple_ephemeris;
    }

    void set_simple_ephemeris(SinglePointEphemeris * simple_ephemeris_in)
    {
        simple_ephemeris = simple_ephemeris_in;
    }

    void set_dyn_bodies(const std::vector<DynBody *> & dyn_bodies_in)
    {
        dyn_bodies = dyn_bodies_in;
    }

    std::vector<MassBody *> get_mass_bodies()
    {
        return mass_bodies;
    }

    void set_mass_bodies(const std::vector<MassBody *> & mass_bodies_in)
    {
        mass_bodies = mass_bodies_in;
    }

    std::vector<DynamicsIntegrationGroup *> get_integ_groups()
    {
        return integ_groups;
    }

    void set_integ_groups(const std::vector<DynamicsIntegrationGroup *> & integ_groups_in)
    {
        integ_groups = integ_groups_in;
    }

    std::list<BodyAction *> get_body_actions()
    {
        return body_actions;
    }

    void set_body_actions(const std::list<BodyAction *> & body_actions_in)
    {
        body_actions = body_actions_in;
    }
};
