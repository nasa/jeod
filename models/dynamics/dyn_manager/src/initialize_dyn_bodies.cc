/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynManager
 * @{
 *
 * @file models/dynamics/dyn_manager/src/initialize_dyn_bodies.cc
 * Define DynManager::initialize_dyn_bodies.
 */

/*******************************************************************************
Purpose:
  ()

Library dependencies:
  ((initialize_dyn_bodies.cc)
   (dyn_manager_messages.cc)
   (dynamics/body_action/src/body_action.cc)
   (dynamics/body_action/src/dyn_body_init.cc)
   (dynamics/body_action/src/mass_body_init.cc)
   (dynamics/body_action/src/body_attach.cc)
   (dynamics/dyn_body/src/dyn_body.cc)
   (dynamics/mass/src/mass_point_state.cc)
   (utils/message/src/message_handler.cc)
   (utils/ref_frames/src/ref_frame.cc))


*******************************************************************************/


// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/body_action/include/body_action.hh"
#include "dynamics/body_action/include/body_attach.hh"
#include "dynamics/body_action/include/mass_body_init.hh"
#include "dynamics/body_action/include/dyn_body_init.hh"
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/ref_frames/include/ref_frame_items.hh"

/* Model includes */
#include "../include/dyn_manager.hh"
#include "../include/dyn_manager_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Initialize dynamic bodies.
 */
void
DynManager::initialize_dyn_bodies (
   void)
{

   // Initialize mass body mass properties and
   // perform initialization-time attachments.
   perform_mass_body_initializations (nullptr);
   perform_mass_attach_initializations ();



   // Update the body mass trees.
   for (std::vector<DynBody *>::const_iterator it = dyn_bodies.begin();
        it != dyn_bodies.end();
        ++it) {
      DynBody * body = *it;
      if (body->is_root_body()) {
         body->mass.compute_inverse_inertia = true;
         body->mass.update_mass_properties ();
      }
   }


   // Initialize dynamic body states and
   // check that all required states have been initialized.
   perform_dyn_body_initializations (nullptr);
   check_for_uninitialized_states ();


   // Initialize all body actions that remain in the queue.
   // Note that attachments have already been initialized.
   for (std::list<BodyAction *>::const_iterator it = body_actions.begin();
        it != body_actions.end();
        ++it) {
      BodyAction * action = *it;

      // Initialize the action if it *does not* derives from MassBodyAttach.
      if (dynamic_cast<BodyAttach *> (action) == nullptr) {
         action->initialize (*this);
      }
   }
}


/**
 * Initialize a specific dynamic body.
 *
 * \par Assumptions and Limitations
 *  - The body in question is assumed to be an isolated body.
 * \param[in,out] body Body to be initialized
 */
void
DynManager::initialize_dyn_body (
   DynBody & body)
{

   // Apply mass initializations that pertain to the body in question.
   perform_mass_body_initializations (&body.mass);

   // Update the mass tree for the subject body.
   body.mass.update_mass_properties ();

   // Initialize the state of the subject body.
   perform_dyn_body_initializations (&body);
}


/**
 * Initialize all queued body actions that derive from MassBodyInit
 * and apply those that are immediately ready to be applied.
 * \param[in,out] body Body to be initialized
 */
void
DynManager::perform_mass_body_initializations (
   MassBody * body)
{

   // Initialize and then apply body actions that derive from MassBodyInit.
   // Note that the increment is in (and must be in) the loop body as
   // actions are to be deleted from the queue once applied.
   for (std::list<BodyAction *>::iterator it = body_actions.begin();
        it != body_actions.end();
        /* No increment -- increment is in loop body */ ) {
      BodyAction * action = *it;

      // Only process instances of MassBodyInit and its derived classes.
      // If MassBody is NULL (default), then process all ready actions.
      // If MassBody is specified, process only the ready associated actions.
      if (    ( dynamic_cast<MassBodyInit *> (action) != nullptr      )
           && ( (body == nullptr) || (action->is_same_subject_body(*body)) )   )
      {

         // Initialize the action.
         action->initialize (*this);

         // Perform the action if it is ready.
         if (action->is_ready()) {

            // Apply the action ...
            action->apply (*this);

            // ... and delete it from the queue.
            body_actions.erase (it++);
         }

         // Attachment is not ready: Skip over it.
         // A MassBodyInit action might not be ready because
         //   - The user has set the active flag to false.
         //     For example, the subject MassBody object might not be used
         //     in this particular simulation run.
         //   - The action is a user-defined extension to MassBodyInit.
         //     That the action is not ready is not our problem.
         // Whatever the reason, the response here is to pass over the action.
         else {
            ++it;
         }
      }

      // Simply skip over actions that do not derive from MassBodyInit.
      else {
         ++it;
      }
   }

   return;
}


/**
 * Initialize all queued body actions that derive from MassBodyAttach
 * and apply those that are immediately ready to be applied.
 */
void
DynManager::perform_mass_attach_initializations (
   void)
{

   // Initialize and then apply body actions that derive from MassBodyAttach.
   // Note that the increment is in (and must be in) the loop body as
   // actions are to be deleted from the queue once applied.
   for (std::list<BodyAction *>::iterator it = body_actions.begin();
        it != body_actions.end();
        /* No increment -- increment is in loop body */ ) {
      BodyAction * action = *it;

      // Only process instances of MassBodyAttach and its derived classes.
      if (dynamic_cast<BodyAttach *> (action) != nullptr) {

         // Initialize the action.
         action->initialize (*this);

         // Perform the action if the initializer is ready.
         // Note: Attachments that are not ready will presumably be processed
         // by DynManager::perform_actions() method post-initialization.
         if (action->is_ready()) {

            // Apply the action ...
            action->apply (*this);

            // ... and delete it from the queue.
            body_actions.erase (it++);
         }

         // Attachment is not ready: Skip over it.
         // Note: There is nothing wrong with an attachment that is not ready at
         // initialization time.
         else {
            ++it;
         }
      }

      // Action does not derive from MassBodyAttach: Skip over it.
      else {
         ++it;
      }
   }

   return;
}


/**
 * Initialize dynamic bodies.
 * \param[in,out] body Body to be initialized
 */
void
DynManager::perform_dyn_body_initializations (
   DynBody * body)
{
   // Initialize the queued actions that derive from DynBodyInit.

   for (std::list<BodyAction *>::iterator it = body_actions.begin();
           it != body_actions.end();
           ++it) {
       // Initialize the action if it derives from DynBodyInit.
       BodyAction * action = *it;
       if( dynamic_cast<DynBodyInit*>(action) != nullptr )
       {
           // If body is NULL (default), then initialize any action.
           // If body is specified, initialize only the associated action
           if(    ( body == nullptr )
               || ( action->is_same_subject_body(body->mass) )  )
           {
               // Initialize the action.
               action->initialize (*this);
           }
       }
   }

   // If body is NULL (default):
   // Perform remaining DynBodyInit actions.
   // Keep trying to perform pending actions of type DynBodyInit until
   // the queue is drained of all such initializations (nominal completion) or
   // the unprocessed actions of this type can never be performed (abort).
   bool more_inits_to_do = true; // Set true to enter loop
   bool any_inits_performed = true; // Set true to enter loop
   while (more_inits_to_do && any_inits_performed) {
       more_inits_to_do = any_inits_performed = false;

       for(std::list<BodyAction *>::iterator it = body_actions.begin();
           it != body_actions.end();
           /* No increment -- increment is in loop body */ )
       {
           BodyAction * action = *it;

           // Only process instances of DynBodyInit and its derived classes.
           if(   ( dynamic_cast<DynBodyInit *>(action) != nullptr )
              && ( (body==nullptr) || ( action->is_same_subject_body(body->mass) ) ))
           {

            // Perform the action if the initializer is ready.
            // Note: Body initializations that are not ready will presumably
            // be performed on some subsequent pass through this double loop.
            if (action->is_ready()) {

                // Note that an initialization was performed, which will force
                // another pass through the outer loop if some initializations
                // remain unprocessed after completing the inner loop.
                any_inits_performed = true;

                // Apply the action ...
                action->apply (*this);

                // ... and delete it from the queue.
                // Note that a side effect (via it++) is to advance the iterator.
                body_actions.erase (it++);

                // Update the ephemerides if the above did something weird.
                if ((body == nullptr) && regenerate_ref_frame_tree) {
                    update_ephemerides ();
                }
            }

            // Initializer is *not* ready:
            // Skip over this initializer for this pass.
            else {

                // Note that some initializations remain unprocessed.
                // This initializer will be applied on a subsequent pass if other
                // initializers still in the queue set the requisite knowledge
               // needed by this initializer.
               more_inits_to_do = true;

               // Advance to the next action.
               ++it;
            }
         }

         // Action is not a DynBodyInit. Skip over it.
         else {
            ++it;
         }
      }
   }


   // Active initializations that could not be performed are a non-fatal error.
   if (more_inits_to_do) {
      more_inits_to_do = false;

      // Report the initializers that didn't run.
      for (std::list<BodyAction *>::iterator it = body_actions.begin();
           it != body_actions.end();
           ++it) {
         BodyAction * action = *it;
         DynBodyInit * body_init = dynamic_cast<DynBodyInit *> (action);

         // Action is a DynBodyInit: Report failure to initialize.
         if ((body_init != nullptr) && body_init->active) {
            body_init->report_failure ();
            more_inits_to_do = true;
         }
      }
   }

   if (more_inits_to_do) {
      MessageHandler::error (
         __FILE__, __LINE__, DynManagerMessages::inconsistent_setup,
         "Unable to perform all active initializations.");
   }

   return;
}


/**
 * Ensure that all of the required states have been set.
 */
void
DynManager::check_for_uninitialized_states (
   void)
{
   bool uninitialized_bodies;


   // Check that all bodies have been initialized.
   uninitialized_bodies = false;
   for (std::vector<DynBody *>::const_iterator it = dyn_bodies.begin();
        it != dyn_bodies.end();
        ++it) {
      DynBody * body = *it;
      RefFrameItems missing_states;

      // First denote all of the required states for this body as missing.
      // Which states are required depends on which aspects of the
      // state are to be propagated.
      if (body->translational_dynamics && body->rotational_dynamics) {
         missing_states.set (RefFrameItems::Pos_Vel_Att_Rate);
      }
      else if (body->translational_dynamics) {
         missing_states.set (RefFrameItems::Pos_Vel);
      }
      else if (body->rotational_dynamics) {
         missing_states.set (RefFrameItems::Att_Rate);
      }

      // Now mark the items that have been set as present.
      missing_states.remove (body->get_initialized_states().get());

      // Any missing items are problematic.
      if (! missing_states.is_empty()) {
         uninitialized_bodies = true;
         MessageHandler::error (
            __FILE__, __LINE__, DynManagerMessages::inconsistent_setup,
            "Body '%s' %s not initialized",
            body->name.c_str(), missing_states.to_string());
      }
   }


   // That required states were not initialized is a fatal error.
   if (uninitialized_bodies) {
      MessageHandler::fail (
         __FILE__, __LINE__, DynManagerMessages::inconsistent_setup,
         "Uninitialized states");
   }

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
