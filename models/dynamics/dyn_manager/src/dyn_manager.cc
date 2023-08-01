/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynManager
 * @{
 *
 * @file models/dynamics/dyn_manager/src/dyn_manager.cc
 * Define simple member functions for the DynManager and related classes.
 */

/*****************************************************************************
Purpose:
  ()

Library dependencies:
  ((dyn_manager.cc)
   (dyn_bodies_primitives.cc)
   (gravitation.cc)
   (initialize_dyn_bodies.cc)
   (initialize_model.cc)
   (initialize_simulation.cc)
   (integ_group_primitives.cc)
   (mass_bodies_primitives.cc)
   (perform_actions.cc)
   (dyn_manager_messages.cc)
   (dynamics_integration_group.cc)
   (dynamics/mass/src/mass.cc)
   (dynamics/dyn_body/src/dyn_body.cc)
   (dynamics/body_action/src/body_action.cc)
   (environment/ephemerides/ephem_interface/src/simple_ephemerides.cc)
   (utils/message/src/message_handler.cc))



******************************************************************************/


// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/body_action/include/body_action.hh"
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "dynamics/mass/include/mass.hh"
#include "environment/ephemerides/ephem_interface/include/simple_ephemerides.hh"
#include "environment/ephemerides/ephem_item/include/ephem_item.hh"
#include "environment/planet/include/planet.hh"
#include "utils/integration/include/jeod_integration_group.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/memory/include/jeod_alloc.hh"

// Model includes
#include "../include/dyn_manager.hh"
#include "../include/dyn_manager_messages.hh"
#include "../include/dynamics_integration_group.hh"


//! Namespace jeod
namespace jeod {

/**
 * DynManager default constructor.
 */
DynManager::DynManager (
   void)
:
   deriv_ephem_update (false),
   gravity_off (false),
   mode (DynManagerInit::EphemerisMode_Ephemerides),
   sim_integrator (nullptr),
   initialized (false),
   gravity_manager (nullptr),
   integ_constructor (nullptr),
   integ_interface (nullptr),
   default_integ_group(nullptr),
   simple_ephemeris (nullptr),
   integ_groups (),
   body_actions ()
{
   // Register types.
   JEOD_REGISTER_CLASS (EmptySpaceEphemeris);
   JEOD_REGISTER_CLASS (SinglePlanetEphemeris);
   JEOD_REGISTER_CLASS (DynManager);

   JEOD_REGISTER_CLASS (MassBody);
   JEOD_REGISTER_CLASS (DynBody);
   JEOD_REGISTER_CLASS (DynamicsIntegrationGroup);
   JEOD_REGISTER_CLASS (BodyAction);

   // Register types associated with integration.
   JeodIntegrationGroup::register_classes ();

}


/**
 * DynManager destructor.
 */
DynManager::~DynManager (
   void)
{
   // Free locally-allocated memory.
   if ((simple_ephemeris != nullptr) &&
       (JEOD_IS_ALLOCATED (simple_ephemeris))) {
      JEOD_DELETE_OBJECT (simple_ephemeris);
   }

   if ((integ_interface != nullptr) &&
       (JEOD_IS_ALLOCATED (integ_interface))) {
      JEOD_DELETE_OBJECT (integ_interface);
   }

   if ((default_integ_group != nullptr) &&
       (JEOD_IS_ALLOCATED (default_integ_group))) {
      JEOD_DELETE_OBJECT (default_integ_group);
   }

   // Free memory allocated by the ER7 numerical package.
   er7_utils::Er7UtilsDeletable::delete_instance (integ_constructor);
}


/**
 * Return last update time
 * @return Name
 */
double
DynManager::timestamp (
   void)
const
{
   return update_time;
}


/**
 * Return identifier.
 * @return Name
 */
const char *
DynManager::name (
   void)
const
{
   return "DynManager";
}


/**
 * Shutdown the manager. Empty for now.
 */
void
DynManager::shutdown (
   void)
{
   ; // Empty, for now
}


/**
 * Set the Gravity Manager to the specified reference.
 * \param[in] gravity Gravity Manager
 */
void
DynManager::set_gravity_manager (
   GravityManager & gravity)
{

   // Handle errors.
   if (gravity_manager != nullptr) {
      MessageHandler::error (
         __FILE__, __LINE__, DynManagerMessages::singleton_error,
         "A Gravity Manager was already registered with the dynamics manager.\n"
         "Duplicate registration request ignored.");
   }

   else if (initialized) {
      MessageHandler::error (
         __FILE__, __LINE__, DynManagerMessages::inconsistent_setup,
         "The call to DynManager::set_gravity_manager() must preceed the call\n"
         "to DynManager::initialize_simulation().\n"
         "Late registration request ignored.");
   }

   else if (gravity_off) {
      MessageHandler::error (
         __FILE__, __LINE__, DynManagerMessages::inconsistent_setup,
         "DynManager::set_gravity_manager() should not be called when\n"
         "the DynManager::gravity_off flag is set.\n"
         "Gravity Manager registration request ignored.");
   }

   // No errors: Set the Gravity Manager as requested.
   else {
      gravity_manager = &gravity;
   }
}


/**
 * Add a body action to the list of such.
 * \param[in,out] body_action Body action
 */
void
DynManager::add_body_action (
   BodyAction * body_action)
{

   // Handle errors.
   // Note well: These are treated as severe rather than fatal errors.

   // 1. The action must not be null.
   if (body_action == nullptr) {
      MessageHandler::error (
         __FILE__, __LINE__, DynManagerMessages::null_pointer,
         "Null pointer passed to add_body_action()\n"
         "Addition request ignored.");
      return;
   }

   // 2. The action must not yet be in the list of actions.
   for (std::list<BodyAction *>::iterator it = body_actions.begin();
        it != body_actions.end();
        ++it) {
      if (*it == body_action) {
         MessageHandler::error (
            __FILE__, __LINE__, DynManagerMessages::duplicate_entry,
            "Duplicate entry passed to add_body_action()\n"
            "Addition request ignored.");
         return;
      }
   }


   // Initialize the action if the DynManager has already been initialized.
   // (Enqueued body actions will be initialized en-masse during DynManager
   // initialization.)
   if (initialized) {
      body_action->initialize (*this);
   }

   // Add the action to the list of such.
   body_actions.push_back (body_action);
}

/**
 * Remove a body action to the list of such.
 * \param[in] action_name_in Name of the action to remove
 */
void
DynManager::remove_body_action (
         char * action_name_in)
{
  if (action_name_in == nullptr) {
    return;
  }
  for (std::list<BodyAction *>::iterator it = body_actions.begin();
       it != body_actions.end();
       ++it) {
    BodyAction * action = *it;
    // if the body-action has no name, it can't be the one we are looking for.
    if (action->action_name.empty()) {
      continue;
    }
    if (strcmp(action_name_in, action->action_name.c_str()) == 0) {
      action->shutdown(); // frees the memory allocated to action_identifier
      body_actions.erase(it); // remove from list.
      return;
    }
  }
}


/*
 * Force all integrators to reset themselves.
 */
void
DynManager::reset_integrators ()
{
   if (!integ_groups.empty()) {
     for (size_t ii = 0; ii < integ_groups.size(); ++ii) {
       if (integ_groups[ii] != nullptr) {
         reset_integrators( *integ_groups[ii]);
       }
     }
   }
   else if (default_integ_group != nullptr) {
     reset_integrators(*default_integ_group);
   }
   // there is no else, checking for default_integ_group is a safety check
   // before dereferencing it.  If there are no integ-groups and no default
   // integ group, something has gone wrong but the consequence is that there
   // are no groups to reset.
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
