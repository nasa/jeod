/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynManager
 * @{
 *
 * @file models/dynamics/dyn_manager/src/gravitation.cc
 * Compute gravitational acceleration.
 */

/*****************************************************************************
Purpose:
  ()

Library dependencies:
  ((gravitation.o)
   (dynamics/dyn_body/dyn_body.o)
   (dynamics/mass/mass_point_state.o)
   (environment/gravity/gravity_manager.o)
   (utils/ref_frames/ref_frame.o))



******************************************************************************/


// System includes

// JEOD includes
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "environment/gravity/include/gravity_manager.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/dyn_manager.hh"
#include "../include/dyn_manager_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Initialize the gravity controls for each dynamic body.
 *
 * \par Assumptions and Limitations
 *  - Not called in empty space mode.
 */
void
DynManager::initialize_gravity_controls (
   void)
{
   // Sanity check:
   // The loop that follows will drop core if there is no Gravity Manager.
   // That this method was called is highly suspect.
   // It means the call comes from outside the dynamics manager.
   if (gravity_manager == NULL) {
      MessageHandler::error (
         __FILE__, __LINE__, DynManagerMessages::inconsistent_setup,
         "DynManager::initialize_gravity_controls() should not be called\n"
         "in non-gravitational simulations.");

      // Turn gravity off just to be sure.
      gravity_off = true;
      return;
   }

   // Initialize the gravity controls for each dynamic body.
   for (std::vector<DynBody *>::const_iterator it = dyn_bodies.begin();
        it != dyn_bodies.end();
        ++it) {
      DynBody * body = *it;
      body->initialize_controls (*gravity_manager);
   }
}


/**
 * Reset the gravity controls for each dynamic body.
 *
 * \par Assumptions and Limitations
 *  - Not called in empty space mode.
 */
void
DynManager::reset_gravity_controls (
   void)
{
   // Sanity check:
   // The loop that follows will drop core if there is no Gravity Manager.
   // This method is intended to be called at the S_define level, so
   // the call itself is not suspect.
   if (gravity_manager == NULL) {

      // Use the gravity_off flag to limit the spew.
      if (! gravity_off) {
         MessageHandler::error (
            __FILE__, __LINE__, DynManagerMessages::inconsistent_setup,
            "DynManager::reset_gravity_controls() should not be called\n"
            "in non-gravitational simulations.");
         gravity_off = true;
      }
   }

   // Silently return if gravity is off.
   if (gravity_off) {
      return;
   }


   // Initialize the gravity controls for each dynamic body.
   for (std::vector<DynBody *>::const_iterator it = dyn_bodies.begin();
        it != dyn_bodies.end();
        ++it) {
      DynBody * body = *it;
      body->reset_controls ();
   }
}


/**
 * Compute gravitational acceleration on each root body.
 */
void
DynManager::gravitation (
   void)
{
   // Sanity check:
   // The loop that follows will drop core if there is no Gravity Manager.
   // A message will already have been issued if the model is initialized,
   // so don't complain in such a case (we would get spew otherwise).
   if (gravity_manager == NULL) {
      if (! initialized) {
         MessageHandler::error (
            __FILE__, __LINE__, DynManagerMessages::inconsistent_setup,
            "The dynamics manager has no Gravity Manager\n"
            "and has not been initialized.\n"
            "Please read the User Manual.");

         // Setting initialized will prevent a spew, and will also result
         // in a warning about an even later registration of a Gravity Manager.
         initialized = true;
         gravity_off = true;
      }
      return;
   }

   // Sanity check:
   // The model should have been initialized as well.
   if (! initialized) {
      MessageHandler::error (
         __FILE__, __LINE__, DynManagerMessages::inconsistent_setup,
         "The dynamics manager has not been properly initialized.\n"
         "Please read the User Manual.");

      // Setting initialized and clearing the Gravity Manager will ensure that
      // subsequent calls to this method will do nothing, but silently.
      initialized = true;
      gravity_off = true;
      gravity_manager = NULL;
      return;
   }

   // Silently return if gravity is off.
   if (gravity_off) {
      return;
   }

   // Let the default integration group do the real work.
   default_integ_group->deriv_ephem_update = deriv_ephem_update;
   default_integ_group->gravitation (*this, *gravity_manager);
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
