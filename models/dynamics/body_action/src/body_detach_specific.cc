/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/src/mass_body_detach_specific.cc
 * Define methods for the BodyDetachSpecific class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((body_detach_specific.o)
   (body_action.o)
   (body_action_messages.o)
   (dynamics/mass/mass_point_state.o)
   (dynamics/dyn_body/dyn_body_detach.o)
   (utils/message/message_handler.o))



*******************************************************************************/


// System includes
#include <cstddef>
#include <string>

// JEOD includes
#include "dynamics/mass/include/mass.hh"
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/body_action_messages.hh"
#include "../include/body_detach_specific.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a BodyDetachSpecific.
 */
BodyDetachSpecific::BodyDetachSpecific (
   void)
:
   BodyAction(),
   detach_from(nullptr),
   dyn_detach_from(nullptr)
{
    active = false;
    return;
}


/**
 * Initialize a BodyDetachSpecific.
 * \param[in,out] dyn_manager Dynamics manager
 */
void
BodyDetachSpecific::initialize (
   DynManager & dyn_manager)
{

   // Forward the request up the chain.
   BodyAction::initialize (dyn_manager);
   std::string fail_reason("");

   // Sanity check: Detach-from must not be null.
   if (detach_from == nullptr && dyn_detach_from == nullptr )
   {
       fail_reason += "The detaching body was not assigned\n";
   }
   else if( detach_from != nullptr )
   {
       if( dyn_detach_from == nullptr )
       {
           dyn_detach_from = detach_from->dyn_owner;
       }
       else if( dyn_detach_from != detach_from->dyn_owner )
       {
           fail_reason += "The detach_from and dyn_detach_from bodies are "
                          "assigned, but refer to different bodies\n";
       }
   }

   // Sanity check: subject must not be null
   if( subject == nullptr && dyn_subject == nullptr )
   {
       fail_reason += "The subject body was not assigned\n";
   }
   else if( subject != nullptr )
   {
       if( dyn_subject == nullptr )
       {
           dyn_subject = subject->dyn_owner;
       }
       else if( dyn_subject != subject->dyn_owner )
       {
           fail_reason += "The subject and dyn_subject bodies are "
                          "assigned, but refer to different bodies\n";
       }
   }

   // Sanity check cannot detach a DynBody from a MassBody
   if( dyn_subject != nullptr && dyn_detach_from == nullptr )
   {
       fail_reason += "The subject refers to a dynamic body, but "
                      "detach_from is not a dynamic body";
   }

   if( !fail_reason.empty() )
   {
       MessageHandler::fail (
           __FILE__, __LINE__, BodyActionMessages::null_pointer,
           "%s failed:\n"
           "%s",
           action_identifier, fail_reason.c_str() );
   }

   return;
}


/**
 * Detach the body from its parent.
 * \param[in,out] dyn_manager Dynamics manager
 */
void
BodyDetachSpecific::apply (
   DynManager & dyn_manager)
{
   bool succeeded = false;
   std::string name_detach = "(?detach_from)";
   std::string name_subject = "(?subject)";

   // Detach DynBody subject from its parent
   if(dyn_subject != nullptr && dyn_detach_from != nullptr )
   {
       succeeded = dyn_subject->detach (*dyn_detach_from);
       name_subject = dyn_subject->name.get_name();
       name_detach  = dyn_detach_from->name.get_name();
   }
   // Jettison MassBody from DynBody
   else if( dyn_subject == nullptr && dyn_detach_from != nullptr )
   {
       succeeded = dyn_detach_from->remove_mass_body(*subject);
       name_subject = subject->name.get_name();
       name_detach  = dyn_detach_from->name.get_name();
   }
   // Detach MassBody Subject from its parent
   else if( subject != nullptr && detach_from != nullptr )
   {
       succeeded = subject->detach (*detach_from);
       name_subject = subject->name.get_name();
       name_detach  = detach_from->name.get_name();
   }
   // Inconsistent setup not detected by BodyDetachSpecific::initialize()
   else
   {
       succeeded = false;
   }

   // Detachment succeeded: Debug.
   if (succeeded) {
      MessageHandler::debug (
         __FILE__, __LINE__, BodyActionMessages::trace,
         "%s: %s detached from %s.",
         action_identifier, name_subject.c_str(), name_detach.c_str());
   }

   // Detachment failed: Terminate the sim if terminate_on_error is set.
   else if (terminate_on_error) {
      MessageHandler::fail (
         __FILE__, __LINE__, BodyActionMessages::fatal_error,
         "%s failed to detach %s from %s."
         "The terminate_on_failure flag set and a detachment error occurred.\n"
         "The detachment error described above is fatal per this setting.",
         action_identifier, name_subject.c_str(), name_detach.c_str());
   }

   // Detachment failed, terminate_ not set: Tell the user about the problem.
   else {
      MessageHandler::error (
         __FILE__, __LINE__, BodyActionMessages::not_performed,
         "%s failed to detach %s from %s.",
         action_identifier, name_subject.c_str(), name_detach.c_str());
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
BodyDetachSpecific::is_ready (
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
