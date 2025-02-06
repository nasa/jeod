/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/src/body_detach.cc
 * Define methods for the MassBodyDetach class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((body_detach.cc)
   (body_action.cc)
   (body_action_messages.cc)
   (dynamics/mass/src/mass_point_state.cc)
   (utils/message/src/message_handler.cc))




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
       succeeded = mass_subject->detach();
   }

   // Detachment succeeded: Debug.
   if(succeeded)
   {
      MessageHandler::debug (
         __FILE__, __LINE__, BodyActionMessages::trace,
         "%s: %s detached.",
         action_identifier.c_str(), mass_subject->name.c_str());
   }

   // Detachment failed: Terminate the sim if terminate_on_error is set.
   else if (terminate_on_error) {
      MessageHandler::fail (
         __FILE__, __LINE__, BodyActionMessages::fatal_error,
         "%s failed to detach %s."
         "The terminate_on_failure flag set and a detachment error occurred.\n"
         "The detachment error described above is fatal per this setting.",
         action_identifier.c_str(), mass_subject->name.c_str());
   }

   // Detachment failed, terminate_ not set: Tell the user about the problem.
   else {
      MessageHandler::error (
         __FILE__, __LINE__, BodyActionMessages::not_performed,
         "%s failed to detach %s.",
         action_identifier.c_str(), mass_subject->name.c_str());
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
