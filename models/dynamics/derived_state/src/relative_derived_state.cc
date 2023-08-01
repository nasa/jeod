/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DerivedState
 * @{
 *
 * @file models/dynamics/derived_state/src/relative_derived_state.cc
 * Define methods for the base body initialization class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((relative_derived_state.cc)
   (derived_state.cc)
   (derived_state_messages.cc)
   (dynamics/mass/src/mass_point_state.cc)
   (utils/message/src/message_handler.cc)
   (utils/ref_frames/src/ref_frame.cc)
   (utils/ref_frames/src/ref_frame_compute_relative_state.cc))



*******************************************************************************/


// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/ref_frames/include/ref_frame.hh"

// Model includes
#include "../include/relative_derived_state.hh"
#include "../include/derived_state_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a RelativeDerivedState object.
 */
RelativeDerivedState::RelativeDerivedState (
   void)
:
   name(),
   subject_frame_name(nullptr),
   target_frame_name(nullptr),
   direction_sense(undefined),
   active(true),
   subject_frame(nullptr),
   target_frame(nullptr)
{
}



/**
 * Destruct a RelativeDerivedState object.
 */
RelativeDerivedState::~RelativeDerivedState (
   void)
{
   // Remove initialization-time subscriptions to the target and subject frames.
   if ((target_frame != nullptr) && (target_frame->subscriptions() > 0)) {
      target_frame->unsubscribe ();
   }
   if ((subject_frame != nullptr) && (subject_frame->subscriptions() > 0)) {
      subject_frame->unsubscribe ();
   }
}



/**
 * Begin initialization of a RelativeDerivedState.
 * The initialize method for all subclasses of DerivedState *must* pass
 * the initialize call to their immediate parent class, which in turn
 * must do the same, eventually invoking this method.
 * \param[in,out] subject_body Subject body
 * \param[in,out] dyn_manager Dynamics manager
 */
void
RelativeDerivedState::initialize (
   DynBody & subject_body,
   DynManager & dyn_manager)
{

   // Perform base initializations.
   DerivedState::initialize (subject_body, dyn_manager);

   // Perform initializations specific to this model.
   initialize (dyn_manager);
}



/**
 * Initialize a RelativeDerivedState, without a DynBody. No pass up
 * the initialization chain is possible for this method, since it
 * is not inherited.
 * \param[in,out] dyn_manager Dynamics manager
 */
void
RelativeDerivedState::initialize (
   DynManager & dyn_manager)
{

   // Sanity check: Subject and target names must be provided.

   // Find the subject and target frames, checking for errors.

   // Ensure that the subject_frame_name has something to it.
   if ((subject_frame_name == nullptr) || (subject_frame_name[0] == '\0')) {
      MessageHandler::fail (
         __FILE__, __LINE__, DerivedStateMessages::invalid_name,
         "Missing subject_frame_name for RelativeDerivedState '%s'",
         state_identifier);

      // Not reached
      return;
   }

   // Find the subject frame. For backwards compatibility, allow for
   // the possibility of an implied body name prefix, which might be
   // true if a DynBody is being used. Otherwise, a full frame name
   // must be supplied.
   if (subject != nullptr) {

      // There is a subject DynBody, so use it for frame lookup
      subject_frame = subject->find_body_frame (subject_frame_name);
   }

   else {

      // There is not a subject DynBody currently associated with
      // this instance, so treat given name as the full frame name.
      subject_frame = dynamic_cast <BodyRefFrame*> (
         dyn_manager.find_ref_frame (subject_frame_name));
   }


   // Ensure the above worked.
   if (subject_frame == nullptr) {
      MessageHandler::fail (
         __FILE__, __LINE__, DerivedStateMessages::invalid_name,
         "Invalid subject frame '%s' for RelativeDerivedState '%s'",
         subject_frame_name, state_identifier);

      // Not reached
      return;
   }

   // Ensure that the target_frame_name has something to it.
   if ((target_frame_name == nullptr) || (target_frame_name[0] == '\0')) {
      MessageHandler::fail (
         __FILE__, __LINE__, DerivedStateMessages::invalid_name,
         "Missing target_frame_name for RelativeDerivedState '%s'",
         state_identifier);

      // Not reached
      return;
   }

   // Find the target frame.
   target_frame = dyn_manager.find_ref_frame (target_frame_name);

   // Ensure the above worked.
   if (target_frame == nullptr) {
      MessageHandler::fail (
         __FILE__, __LINE__, DerivedStateMessages::invalid_name,
         "Invalid target frame '%s' for RelativeDerivedState '%s'",
         target_frame_name, state_identifier);

      // Not reached
      return;
   }

   // (Try to) ensure the target frame is computed.
   target_frame->subscribe();
   subject_frame->subscribe();
}



/**
 * Update the state.
 */
void
RelativeDerivedState::update (
   void)
{
   if (direction_sense == ComputeSubjectStateinTarget) {
      subject_frame->compute_relative_state (*target_frame, rel_state);
   }

   else if (direction_sense == ComputeTargetStateinSubject) {
      target_frame->compute_relative_state (*subject_frame, rel_state);
   }

   else {

      MessageHandler::fail (
         __FILE__, __LINE__, DerivedStateMessages::illegal_value,
         "Value of enum DirectionSense not set for RelativeDerivedState");
   }
}



/**
 * Setter for the activation flag to on or off and
 * If off, unsubscribes subject and target frames
 * /param raf  RelativeDerivedState activation flag
 * for RelKin manager
 */
void
RelativeDerivedState::set_activation_flag (
   bool raf)
{
   // true = on; false = off
   // Turns the activation flag for the relstate on/off
   active = raf;

   // If relstate turned on, subscribe subject and target
   // frames
   if (active == true) {
      subject_frame->subscribe();
      target_frame->subscribe();
   } else { // If relstate turned off, unsubscribe subject and target frames

      subject_frame->unsubscribe();
      target_frame->unsubscribe();
   }

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
