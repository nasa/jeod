/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynManager
 * @{
 *
 * @file models/dynamics/dyn_manager/src/dynamics_integration_group.cc
 * Define DynamicsIntegrationGroup methods.
 */

/*****************************************************************************
Purpose:
  ()

Library dependencies:
  ((dynamics_integration_group.cc)
   (dyn_manager.cc)
   (dyn_manager_messages.cc)
   (dynamics/dyn_body/src/dyn_body.cc)
   (environment/gravity/src/gravity_manager.cc)
   (environment/time/src/time_manager.cc)
   (utils/integration/src/jeod_integration_group.cc)
   (utils/message/src/message_handler.cc)
   (utils/named_item/src/named_item.cc))


******************************************************************************/


// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "environment/gravity/include/gravity_manager.hh"
#include "utils/integration/include/jeod_integration_time.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/named_item/include/named_item.hh"

// Model includes
#include "../include/dyn_manager.hh"
#include "../include/dyn_manager_messages.hh"
#include "../include/dynamics_integration_group.hh"


//! Namespace jeod
namespace jeod {

/**
 * DynamicsIntegrationGroup default constructor, needed for checkpoint/restart.
 */
DynamicsIntegrationGroup::DynamicsIntegrationGroup ()
:
   JeodIntegrationGroup (),
   deriv_ephem_update (false),
   dyn_bodies (),
   bodies_integrated_separately (true)
{
   register_base_contents();
}


/**
 * DynamicsIntegrationGroup non-default constructor,
 *  used to create the default integration group.
 * @param[in] owner        The new group's owner
 * @param[in] integ_cotr   Integrator constructor
 * @param[in] integ_inter  Simulation engine integration interface
 * @param[in] time_mngr    Time manager
 */
DynamicsIntegrationGroup::DynamicsIntegrationGroup (
   JeodIntegrationGroupOwner & owner,
   er7_utils::IntegratorConstructor & integ_cotr,
   JeodIntegratorInterface & integ_inter,
   JeodIntegrationTime & time_mngr)
:
   JeodIntegrationGroup (owner, integ_cotr, integ_inter, time_mngr),
   deriv_ephem_update (false),
   dyn_bodies (),
   bodies_integrated_separately (true)
{
   register_base_contents();
}


/**
 * Register types and containers.
 */
void
DynamicsIntegrationGroup::register_base_contents ()
{
   JEOD_REGISTER_CLASS (DynamicsIntegrationGroup);
   JEOD_REGISTER_CLASS (DynBody);
   JEOD_REGISTER_CHECKPOINTABLE (this, dyn_bodies);
}

/**
 * DynamicsIntegrationGroup destructor.
 */
DynamicsIntegrationGroup::~DynamicsIntegrationGroup ()
{
   JEOD_DEREGISTER_CHECKPOINTABLE (this, dyn_bodies);
}


/**
 * Create an integration group object that can be used as the
 * dynamic manager's default integration group.
 * @param[in] owner        The new group's owner
 * @param[in] integ_cotr   Integrator constructor
 * @param[in] integ_inter  Simulation engine integration interface
 * @param[in] time_mngr    Time manager
 * @return Created DynamicsIntegrationGroup.
 */
DynamicsIntegrationGroup *
DynamicsIntegrationGroup::create_group (
   JeodIntegrationGroupOwner & owner,
   er7_utils::IntegratorConstructor & integ_cotr,
   JeodIntegratorInterface & integ_inter,
   JeodIntegrationTime & time_mngr)
const
{
   return JEOD_ALLOC_CLASS_OBJECT (
             DynamicsIntegrationGroup,
             (owner, integ_cotr, integ_inter, time_mngr));
}


/**
 * Pre-initialize the group and register it with the dynamics manager.
 * This function is to be called early in the initialization process.
 * Overrides should not depend on the dyn_bodies vector having any members.
 * @param[in] dyn_manager  Dynamics manager.
 */
void
DynamicsIntegrationGroup::register_group (
   DynManager & dyn_manager)
{
   if (dyn_manager.is_integ_group_registered (this)) {
   }

   dyn_manager.add_integ_group (*this);
}


/**
 * Complete the initialization of the group.
 * For overriders:
 * This function is called by DynManager::initialize_simulation.
 * At the point of this call, the dyn_bodies vector is populated with the
 * bodies that are to be integrated by this group.
 * Note well: That vector can still be empty.
 */
void
DynamicsIntegrationGroup::initialize_group (
   DynManager &)
{
   if (jeod_time_manager == nullptr) {
      MessageHandler::fail (
         __FILE__, __LINE__, DynManagerMessages::null_pointer,
         "Improper use of DynamicsIntegrationGroup.\n"
         "The time manager must not be NULL.\n"
         "The DynamicsIntegrationGroup was not properly constructed.");
      return;
   }

   initialize_group();

   if (bodies_integrated_separately) {
      for (std::vector<DynBody *>::const_iterator it = dyn_bodies.begin();
           it != dyn_bodies.end();
           ++it) {
         DynBody * body = *it;
         body->create_body_integrators (
            *integ_constructor, *integ_controls, *jeod_time_manager);
      }
   }

   update_from_owner();
}


/**
 * Add a DynBody to the set of bodies whose states are integrated by this group.
 * @param dyn_body  DynBody to be added to the group.
 */
void
DynamicsIntegrationGroup::add_dyn_body (
   DynBody & dyn_body)
{
   bool found = false;

   // Check if the body is already in our list of bodies.
   for (std::vector<DynBody *>::const_iterator it = dyn_bodies.begin();
        it != dyn_bodies.end();
        ++it) {
      DynBody * body = *it;
      if (body == &dyn_body) {
         found = true;
         break;
      }
   }

   // Don't add the body if it is already here.
   if (found) {
      MessageHandler::error (
         __FILE__, __LINE__, DynManagerMessages::duplicate_entry,
         "DynBody '%s' is already registered with this integration group.",
         dyn_body.name.c_str());
      return;
   }

   // Not a duplicate. Add the body to the list.
   dyn_bodies.push_back (&dyn_body);

   // Let the body know it was assigned to this group.
   dyn_body.set_integration_group (*this);

   // Create the integrators for this body,
   // but only if the bodies are to be integrated via DynBody::integrate().
   if ((integ_controls != nullptr) && bodies_integrated_separately) {

      // Create the integrators for this body.
      dyn_body.create_body_integrators (
         *integ_constructor, *integ_controls, *jeod_time_manager);

      // Add any integrable objects associated with the dyn_body.
      JeodPointerVector<er7_utils::IntegrableObject>::type associated_objects =
         dyn_body.get_integrable_objects();

      for (std::vector<er7_utils::IntegrableObject *>::const_iterator it =
              associated_objects.begin();
           it != associated_objects.end();
           ++it) {
         add_integrable_object(**it);
      }
   }
}


/**
 * Remove a DynBody from the set of bodies whose states are integrated by
 * this group.
 * @param dyn_body  DynBody to be removed from the group.
 */
void
DynamicsIntegrationGroup::delete_dyn_body (
   DynBody & dyn_body)
{
   std::vector<DynBody *>::iterator it;

   // Find the body in our list of bodies.
   for (it = dyn_bodies.begin(); it != dyn_bodies.end(); ++it) {
      DynBody * body = *it;
      if (body == &dyn_body) {
         break;
      }
   }

   // Don't delete the body if it isn't ours.
   if (it == dyn_bodies.end()) {
      MessageHandler::error (
         __FILE__, __LINE__, DynManagerMessages::inconsistent_setup,
         "DynBody '%s' is not registered with this integration group.",
         dyn_body.name.c_str());
      return;
   }

   // It's one of ours. Delete the body from the list.
   dyn_bodies.erase (it);

   // Let the body know it has been removed from this group.
   dyn_body.clear_integration_group ();
}


/**
 * Perform actions that need to be taken before entering the
 * derivative / integration loop.
 * The base action is to set the time model to the time at
 * the start of the integration loop.
 * @param sim_endtime  End time of integration loop.
*/
void
DynamicsIntegrationGroup::prepare_for_integ_loop (
  double sim_endtime)
{
   jeod_time_manager->update_time (sim_endtime - integ_interface->get_dt());
}


/**
 * Compute the gravitational acceleration of each root dynamic body.
 * @param dyn_manager    Dynamics manager.
 * @param gravity_manager  Gravity Manager.
 */
void
DynamicsIntegrationGroup::gravitation (
   DynManager & dyn_manager,
   GravityManager & gravity_manager)
{
   // Update ephemerides if this is to be done at the derivative rate
   // or if the reference frame tree is out of whack.
   if (deriv_ephem_update || dyn_manager.ref_frame_tree_needs_rebuild()) {
      dyn_manager.update_ephemerides ();
   }

   // Compute gravitational effects on each root body.
   for (std::vector<DynBody *>::const_iterator it = dyn_bodies.begin();
        it != dyn_bodies.end();
        ++it) {
      DynBody * body = *it;

      // Only process root bodies.
      // The gravitational acceleration is not needed for child bodies.
      if (body->is_root_body()) {

         // Ask the Gravity Manager to compute the acceleration.
         gravity_manager.gravitation (
            body->composite_body,
            body->grav_interaction);
      }
   }
}


/**
 * Collect the forces and torques acting on each root dynamic body.
 */
void
DynamicsIntegrationGroup::collect_derivatives ()
{
   // Collect forces and torques on each root body.
   for (std::vector<DynBody *>::const_iterator it = dyn_bodies.begin();
        it != dyn_bodies.end();
        ++it) {
      DynBody * body = *it;

      // Only process root bodies.
      // The forces and torques on non-root bodies are collected
      // within the root body collection.
      if (body->is_root_body()) {

         // Collect the forces and torques acting on the body as a whole.
         body->collect_forces_and_torques ();
      }
   }
}


/**
 * Force all integrators to reset themselves.
 */
void
DynamicsIntegrationGroup::reset_body_integrators ()
{
   // Reset each body integrator.
   for (std::vector<DynBody *>::const_iterator it = dyn_bodies.begin();
        it != dyn_bodies.end();
        ++it) {
      DynBody * body = *it;
      body->reset_integrators ();
   }

   // Reset the non-body integrators.
   JeodIntegrationGroup::reset_body_integrators ();
}


/**
 * Integrate the states of the DynBody objects that comprise the group.
 * @param[in]     cycle_dyndt   Dynamic time step, in dynamic time seconds.
 * @param[in]     target_stage  The stage of the integration process
 *                              that the integrator should try to attain.
 * @return The status (time advance, pass/fail status) of the integration.
 */
er7_utils::IntegratorResult
DynamicsIntegrationGroup::integrate_bodies (
   double cycle_dyndt,
   unsigned int target_stage)
{
   er7_utils::IntegratorResult status (false);

   // This method requires that bodies_integrated_separately be set.
   if (! bodies_integrated_separately) {

      // We should *never* get here. If we do, we've got trouble, big time.
      MessageHandler::fail (
         __FILE__, __LINE__, DynManagerMessages::inconsistent_setup,
         "Bodies must be integrated separately in this context.\n");
   }

   // Integrate non-DynBody objects that are to be integrated as a part
   // of this integration group.
   if (! integrable_objects.empty()) {
      status = integrate_container (
                 cycle_dyndt, target_stage, integrable_objects);

   }

   // Propagate state of each body to the end of the intermediate step.
   for (std::vector<DynBody *>::const_iterator it = dyn_bodies.begin();
        it != dyn_bodies.end();
        ++it) {
      DynBody * body = *it;

      // Only process root bodies.
      // The state of a non-root body is updated by virtue of the
      // propagate_state() method of that body's root body.
      if (body->is_root_body()) {

         // Integrate the body's state and merge the integration status.
         integ_merger.merge_integrator_result (
            body->integrate (cycle_dyndt, target_stage),
            status);
      }
   }

   return status;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
