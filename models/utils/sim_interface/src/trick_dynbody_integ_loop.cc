/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup SimInterface
 * @{
 *
 * @file models/utils/sim_interface/src/trick_dynbody_integ_loop.cc
 * Define JeodDynbodyIntegrationLoop methods.
 */

/*******************************************************************************

Purpose:
  ()

Assumptions and limitations:
  ((Take great care in mixing Trick integration and JEOD integration.))

Library dependencies:
  ((trick_dynbody_integ_loop.cc))


*******************************************************************************/

#ifdef TRICK_VER

// Local includes
#include "../include/trick_dynbody_integ_loop.hh"
#include "../include/sim_interface_messages.hh"

// JEOD includes
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "environment/time/include/time_manager.hh"
#include "utils/message/include/message_handler.hh"

// Trick includes
#include "sim_services/Executive/include/exec_proto.h"

// System includes
#include <cstddef>
#include <string>
#include <vector>

extern Trick::Integrator * trick_curr_integ;

//! Namespace jeod
namespace jeod
{

// JeodDynbodyIntegrationLoop default constructor.
JeodDynbodyIntegrationLoop::JeodDynbodyIntegrationLoop()
    : Trick::IntegLoopScheduler(0.0, nullptr)
{
}

// JeodDynbodyIntegrationLoop non-default constructor.
JeodDynbodyIntegrationLoop::JeodDynbodyIntegrationLoop(double cycle,
                                                       Trick::SimObject & sim_object_in,
                                                       TimeManager & time_manager_in,
                                                       DynManager & dyn_manager_in,
                                                       GravityManager & grav_manager_in,
                                                       er7_utils::IntegratorConstructor *& integ_cotr_in,
                                                       DynamicsIntegrationGroup & integ_group_factory_in)
    : Trick::IntegLoopScheduler(cycle, &sim_object_in),
      loop_sim_object(&sim_object_in),
      dyn_manager(&dyn_manager_in),
      time_manager(&time_manager_in),
      gravity_manager(&grav_manager_in),
      integ_constructor(&integ_cotr_in),
      integ_group_factory(&integ_group_factory_in)
{
    // The containing sim object should have derivative class jobs that
    // call this loop's gravitation and collect_derivative methods.
    // Register the containing sim object as an object to be integrated.
    int return_code = add_sim_object(*loop_sim_object);

    // Multiple integration loops in the same sim object is a fatal error.
    if(return_code != 0)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             SimInterfaceMessages::integration_error,
                             "Multiple integration loops in the same sim object is not allowed.");
    }
}

// JeodDynbodyIntegrationLoop destructor.
JeodDynbodyIntegrationLoop::~JeodDynbodyIntegrationLoop()
{
    JEOD_DELETE_OBJECT(integ_group);
}

// Initialize an integration loop object.
void JeodDynbodyIntegrationLoop::initialize_integ_loop()
{
    // The integrator_constructor should have been populated at this point.
    if(*integ_constructor == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             SimInterfaceMessages::integration_error,
                             "The integrator constructor for a JeodDynbodyIntegrationLoop "
                             "has not been initialized.");
        return;
    }

    // Create the loop's integration group object by
    // using the provided integration group as a factory.
    integ_group = integ_group_factory->create_group(*this,
                                                    **integ_constructor,
                                                    integ_interface,
                                                    time_manager->get_jeod_integration_time());
    integ_group->deriv_ephem_update = deriv_ephem_update;

    // Let the dynamics manager know about the group.
    dyn_manager->add_integ_group(*integ_group);

    // Construct this loops derivative/integration/etc. job queues.
    rebuild_jobs();
}

// Find the sim object that contains the specified integrable object.
Trick::SimObject * JeodDynbodyIntegrationLoop::find_containing_sim_object(
    er7_utils::IntegrableObject & integrable_object)
{
    // Get the simulation engine's name for the specified integrable object.
    std::string obj_name = JeodSimulationInterface::get_name_at_address(&integrable_object,
                                                                        JeodMemoryManager::get_type_descriptor(
                                                                            typeid(er7_utils::IntegrableObject)));

    if(obj_name[0] == '&')
    {
        obj_name = obj_name.substr(1);
    }

    Trick::SimObject * sim_object = nullptr;

    // Find the sim object using the integrable object's name, starting with the
    // outermost parent name and proceeding to more basic names.
    // For example, if the integrable_object's name is foo.bar.baz, look for
    // a SimObject named foo.bar, and if that fails, one named foo.
    for(std::size_t pos = obj_name.find_last_of('.'); pos < obj_name.npos; pos = obj_name.find_last_of('.'))
    {
        obj_name.erase(pos);
        sim_object = manager.find_sim_object(JeodSimulationInterface::get_address_at_name(obj_name));
        if(sim_object != nullptr)
        {
            break;
        }
    }

    return sim_object;
}

// Add the specified integrable object to the integration group.
void JeodDynbodyIntegrationLoop::add_integrable_object(er7_utils::IntegrableObject & integrable_object)
{
    // The object must not be a DynBody.
    if(dynamic_cast<DynBody *>(&integrable_object))
    {
        Trick::SimObject * sim_obj = find_containing_sim_object(integrable_object);
        add_sim_object(*sim_obj);
    }
    else
    {
        // Add the object to the loop's integration group.
        integ_group->add_integrable_object(integrable_object);
    }
}

// Remove the specified integrable object from the integration group.
void JeodDynbodyIntegrationLoop::remove_integrable_object(er7_utils::IntegrableObject & integrable_object)
{
    integ_group->remove_integrable_object(integrable_object);
}

// Set JEOD time to the time at the start of the current integration loop.
void JeodDynbodyIntegrationLoop::set_time_to_loop_start()
{
    time_manager->update(exec_get_sim_time() - next_cycle);
}

// Integrate over the specified time span.
int JeodDynbodyIntegrationLoop::integrate_dt(double beg_sim_time, double del_sim_time)
{
    int ipass = 0;
    bool need_derivs = integ_group->get_first_step_derivs_flag() || get_first_step_deriv_from_integrator();
    bool first_pass = true;

    trick_curr_integ = integ_ptr;

    // Integrate until the integrators say "we're done" by returning zero.
    do
    {
        if(need_derivs)
        {
            call_deriv_jobs();
        }
        need_derivs = true;

        // The bodies themselves are not represented by an integration job.
        ipass = integ_group->integrate_group(beg_sim_time, del_sim_time);

        // Perform any integration class jobs associated with this integrator.
        integ_jobs.reset_curr_index();
        Trick::JobData * curr_job;
        while((curr_job = integ_jobs.get_next_job()) != nullptr)
        {
            // Get the integrator object associated with this job.
            trick_curr_integ = *(static_cast<Trick::Integrator **>(curr_job->sup_class_data));
            if(trick_curr_integ == nullptr)
            {
                MessageHandler::error(__FILE__,
                                      __LINE__,
                                      SimInterfaceMessages::integration_error,
                                      "Integrate job has no associated Integrator.");
                return 1;
            }

            // Set the integrator's time, first pass only.
            if(first_pass)
            {
                trick_curr_integ->time = beg_sim_time;
                trick_curr_integ->dt = del_sim_time;
            }

            // Integrate, checking for agreement with ipass.
            int result = curr_job->call();

            if(result != ipass)
            {
                MessageHandler::error(__FILE__,
                                      __LINE__,
                                      SimInterfaceMessages::integration_error,
                                      "JEOD and Trick integrators are not in sync.");
                return 1;
            }
        }

        first_pass = false;
    } while(ipass != 0);

    return 0;
}

// Add a sim object to the set of objects to be integrated by this loop.
int JeodDynbodyIntegrationLoop::add_sim_object(Trick::SimObject & sim_obj)
{
    // Add the object using the base class method.
    // Failure (rc!=0) means stop, now.
    int return_code = Trick::IntegLoopScheduler::add_sim_object(sim_obj);
    if(return_code != 0)
    {
        return return_code;
    }

    // Add DynBody objects contained in the sim object if the dynamics
    // manager is initialized. (If not, the objects will be added during
    // the call DynManager::initialize_simulation.)
    if(dyn_manager->is_initialized())
    {
        add_sim_object_bodies(sim_obj);
    }
    return 0;
}

// Remove a sim object from the set of objects integrated by this loop object.
int JeodDynbodyIntegrationLoop::remove_sim_object(Trick::SimObject & sim_obj)
{
    // Remove the object using the base class method.
    // Failure (rc!=0) means stop, now.
    int return_code = Trick::IntegLoopScheduler::remove_sim_object(sim_obj);
    if(return_code != 0)
    {
        return return_code;
    }

    // Remove DynBody objects contained in the sim object if the dynamics
    // manager is initialized.
    if(dyn_manager->is_initialized())
    {
        remove_sim_object_bodies(sim_obj);
    }
    return 0;
}

// Add the DynBody objects contained in the specified sim object to the set of
// DynBody objects integrated by this integration loop.
void JeodDynbodyIntegrationLoop::add_sim_object_bodies(Trick::SimObject & sim_obj)
{
    std::vector<DynBody *> bodies = dyn_manager->get_dyn_bodies();

    for(auto body : bodies)
    {
        Trick::SimObject * containing_obj = find_containing_sim_object(*body);
        if(containing_obj == &sim_obj)
        {
            integ_group->add_dyn_body(*body);
        }
    }
}

// Add the DynBody objects contained in all sim objects integrated by this loop.
void JeodDynbodyIntegrationLoop::add_sim_object_bodies()
{
    std::vector<DynBody *> bodies = dyn_manager->get_dyn_bodies();

    for(auto body : bodies)
    {
        Trick::SimObject * sim_obj = find_containing_sim_object(*body);
        if((sim_obj != nullptr) && (find_sim_object(*sim_obj) != sim_objects.end()))
        {
            integ_group->add_dyn_body(*body);
        }
    }
}

// Remove the DynBody objects contained in the specified sim object from the set
// of DynBody objects integrated by this integration loop.
void JeodDynbodyIntegrationLoop::remove_sim_object_bodies(Trick::SimObject & sim_obj)
{
    std::vector<DynBody *> bodies = dyn_manager->get_dyn_bodies();

    for(auto body : bodies)
    {
        Trick::SimObject * containing_obj = find_containing_sim_object(*body);
        if(containing_obj == &sim_obj)
        {
            integ_group->delete_dyn_body(*body);
        }
    }
}

// Update the provided group.
void JeodDynbodyIntegrationLoop::update_integration_group(JeodIntegrationGroup & group)
{
    // The provided group must be this loop object's integration group.
    if(&group != integ_group)
    {
        MessageHandler::fail(__FILE__, __LINE__, SimInterfaceMessages::integration_error, "Internal error.");
    }

    add_sim_object_bodies();
}

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
