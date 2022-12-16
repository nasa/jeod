/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/src/mass_body_detach.cc
 * Define methods for the MassBodyDetach class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((body_detach.o)
   (body_action.o)
   (body_action_messages.o)
   (dynamics/mass/mass_point_state.o)
   (utils/message/message_handler.o))




*******************************************************************************/


// System includes

// JEOD includes
#include "dynamics/mass/include/mass.hh"
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/body_action_messages.hh"
#include "../include/body_detach.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a MassBodyDetach.
 */
BodyDetach::BodyDetach (
   void)
:
   BodyAction()
{
   active = false;

   return;
}


/**
 * Initialize a MassBodyDetach.
 * \param[in,out] dyn_manager Dynamics manager
 */
void
BodyDetach::initialize (
   DynManager & dyn_manager)
{
   // Forward the request up the chain.
   BodyAction::initialize (dyn_manager);

   // Sanity check: set subject
   if (subject == nullptr)
   {
       subject = &(dyn_subject->mass);
   }
   else if( dyn_subject == nullptr )
   {
       dyn_subject = subject->dyn_owner;
   }

   return;
}


/**
 * Detach the body from its parent.
 * \param[in,out] dyn_manager Jeod manager
 */
void
BodyDetach::apply (
   DynManager & dyn_manager)
{
   bool succeeded;

   // Detach the subject from its parent.
   if( dyn_subject != nullptr )
   {
       succeeded = dyn_subject->detach();
   }
   else
   {
       succeeded = subject->detach ();
   }

   // Detachment succeeded: Debug.
   if(succeeded)
   {
      MessageHandler::debug (
         __FILE__, __LINE__, BodyActionMessages::trace,
         "%s: %s detached.",
         action_identifier, subject->name.c_str());
   }

   // Detachment failed: Terminate the sim if terminate_on_error is set.
   else if (terminate_on_error) {
      MessageHandler::fail (
         __FILE__, __LINE__, BodyActionMessages::fatal_error,
         "%s failed to detach %s."
         "The terminate_on_failure flag set and a detachment error occurred.\n"
         "The detachment error described above is fatal per this setting.",
         action_identifier, subject->name.c_str());
   }

   // Detachment failed, terminate_ not set: Tell the user about the problem.
   else {
      MessageHandler::error (
         __FILE__, __LINE__, BodyActionMessages::not_performed,
         "%s failed to detach %s.",
         action_identifier, subject->name.c_str());
   }

   // Forward the action up the chain.
   BodyAction::apply (dyn_manager);

   return;
}


/**
 * Queries whether the "active" flag has been set.
 * @return Can detach process run?
 */
bool
BodyDetach::is_ready (
   void)
{
   return active;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
