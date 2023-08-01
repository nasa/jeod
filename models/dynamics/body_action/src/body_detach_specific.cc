/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/src/body_detach_specific.cc
 * Define methods for the BodyDetachSpecific class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((body_detach_specific.cc)
   (body_action.cc)
   (body_action_messages.cc)
   (dynamics/mass/src/mass_point_state.cc)
   (dynamics/dyn_body/src/dyn_body_detach.cc)
   (utils/message/src/message_handler.cc))



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
   mass_detach_from(nullptr),
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

   validate_body_inputs(dyn_detach_from, mass_detach_from, "detach_from");

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
       succeeded = dyn_detach_from->remove_mass_body(*mass_subject);
       name_subject = mass_subject->name.get_name();
       name_detach  = dyn_detach_from->name.get_name();
   }
   // Detach MassBody Subject from its parent
   else if( dyn_subject == nullptr && dyn_detach_from == nullptr )
   {
       succeeded = mass_subject->detach (*mass_detach_from);
       name_subject = mass_subject->name.get_name();
       name_detach  = mass_detach_from->name.get_name();
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
         action_identifier.c_str(), name_subject.c_str(), name_detach.c_str());
   }

   // Detachment failed: Terminate the sim if terminate_on_error is set.
   else if (terminate_on_error) {
      MessageHandler::fail (
         __FILE__, __LINE__, BodyActionMessages::fatal_error,
         "%s failed to detach %s from %s."
         "The terminate_on_failure flag set and a detachment error occurred.\n"
         "The detachment error described above is fatal per this setting.",
         action_identifier.c_str(), name_subject.c_str(), name_detach.c_str());
   }

   // Detachment failed, terminate_ not set: Tell the user about the problem.
   else {
      MessageHandler::error (
         __FILE__, __LINE__, BodyActionMessages::not_performed,
         "%s failed to detach %s from %s.",
         action_identifier.c_str(), name_subject.c_str(), name_detach.c_str());
   }


   // Forward the action up the chain.
   BodyAction::apply (dyn_manager);

   return;
}

void BodyDetachSpecific::set_detach_from_body(MassBody &mass_body_in)
{
    mass_detach_from = &mass_body_in;
    dyn_detach_from = nullptr;
}

void BodyDetachSpecific::set_detach_from_body(DynBody &dyn_body_in)
{
    dyn_detach_from = &dyn_body_in;
    mass_detach_from = nullptr;
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
