/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/src/mass_body_attach.cc
 * Define methods for the mass body initialization class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((body_attach.o)
   (body_action.o)
   (body_action_messages.o)
   (dynamics/mass/mass_point_state.o)
   (dynamics/dyn_manager/dyn_manager.o)
   (dynamics/dyn_body/dyn_body.o)
   (utils/message/message_handler.o))



*******************************************************************************/


// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "dynamics/mass/include/mass.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/body_action_messages.hh"
#include "../include/body_attach.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a MassBodyAttach.
 */
BodyAttach::BodyAttach (
   void)
:
   BodyAction(),
   parent(nullptr),
   dyn_parent(nullptr),
   succeeded(false)
{
   active = true;

   return;
}



/**
 * Initialize a MassBodyAttach.
 * \param[in,out] dyn_manager Dynamics manager
 */
void
BodyAttach::initialize (
   DynManager & dyn_manager)
{

   // Forward the request up the chain.
   BodyAction::initialize (dyn_manager);

   // Sanity check: set subject
   if (subject == nullptr) {
      subject = &(dyn_subject->mass);
   }
   else if( dyn_subject == nullptr )
   {
       dyn_subject = dyn_manager.find_dyn_body( subject->name.c_str() );
   }

   // Sanity check: set parent
   if( (parent == nullptr) && (dyn_parent != nullptr) )
   {
      parent = &(dyn_parent->mass);
   }
   else if( (parent != nullptr) && (dyn_parent == nullptr) )
   {
       dyn_parent = dyn_manager.find_dyn_body( parent->name.c_str() );
   }
   else if( (parent == nullptr) && (dyn_parent == nullptr) )
   {
      MessageHandler::fail (
         __FILE__, __LINE__, BodyActionMessages::null_pointer,
         "%s failed:\n"
         "The parent object was not assigned.",
         action_identifier);
   }

   return;
}


/**
 * A derived class presumably has performed the attachment, which may
 * not have worked, and forwarded the apply call to this method.
 * This method acts on the status from that child class attachment.
 * \param[in,out] dyn_manager Jeod manager
 */
void
BodyAttach::apply (
   DynManager & dyn_manager)
{

   // Attachment succeeded: Debug.
   if (succeeded) {
      MessageHandler::debug (
         __FILE__, __LINE__, BodyActionMessages::trace,
         "%s: %s attached to %s.",
         action_identifier, subject->name.c_str(), parent->name.c_str());
   }

   // Attachment failed: Terminate the sim if terminate_on_error is set.
   else if (terminate_on_error) {
      MessageHandler::fail (
         __FILE__, __LINE__, BodyActionMessages::fatal_error,
         "'%s' failed to attach '%s' to '%s'. "
         "The terminate_on_failure flag set and an attachment error occurred.\n"
         "The attachment error described above is fatal per this setting.",
         action_identifier, subject->name.c_str(), parent->name.c_str());
   }

   // Attachment failed, terminate_ not set: Tell the user about the problem.
   else {
      MessageHandler::error (
         __FILE__, __LINE__, BodyActionMessages::not_performed,
         "%s failed to attach %s to %s.",
         action_identifier, subject->name.c_str(), parent->name.c_str());
   }


   // Forward the action up the chain.
   BodyAction::apply (dyn_manager);

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
