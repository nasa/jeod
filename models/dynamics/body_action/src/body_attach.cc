/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/src/body_attach.cc
 * Define methods for the mass body initialization class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((body_attach.cc)
   (body_action.cc)
   (body_action_messages.cc)
   (dynamics/mass/src/mass_point_state.cc)
   (dynamics/dyn_manager/src/dyn_manager.cc)
   (dynamics/dyn_body/src/dyn_body.cc)
   (utils/message/src/message_handler.cc))



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
   succeeded(false),
   mass_parent(nullptr),
   dyn_parent(nullptr),
   ref_parent(nullptr)
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

   // Check for mass/dyn body parents first.
   if(!validate_body_inputs(dyn_parent, mass_parent, "parent", true) )
   {
       // Check for generic reference frame attach
       if(ref_parent == nullptr)
       {
           MessageHandler::fail (
              __FILE__, __LINE__, BodyActionMessages::null_pointer,
              "%s failed:\n"
              "The %s body/frame was not assigned",
              action_identifier.c_str(), "parent");
       }
   }

   return;
}

void BodyAttach::set_parent_body(MassBody &mass_body_in)
{
    mass_parent = &mass_body_in;
    dyn_parent = nullptr;
    ref_parent = nullptr;
}

void BodyAttach::set_parent_body(DynBody &dyn_body_in)
{
    dyn_parent = &dyn_body_in;
    mass_parent = nullptr;
    ref_parent = nullptr;
}

void BodyAttach::set_parent_frame(RefFrame & ref_parent_in)
{
    ref_parent = &ref_parent_in;
    dyn_parent = nullptr;
    mass_parent = nullptr;
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
   std::string subject_name("Unknown");
   std::string parent_name("Unknown");
	if (dyn_parent != nullptr) {
		parent_name = dyn_parent->name.get_name();
	} else {
		if (mass_parent != nullptr) {
			parent_name = mass_parent->name.get_name();
		} else if (ref_parent != nullptr) {
			parent_name = ref_parent->get_name();
		}
	}

	if (dyn_subject != nullptr) {
		subject_name = dyn_subject->name.get_name();
	} else {
		if (mass_subject != nullptr) {
			subject_name = mass_subject->name.get_name();
		}
	}

   // Attachment succeeded: Debug.
   if (succeeded) {
      MessageHandler::debug (
         __FILE__, __LINE__, BodyActionMessages::trace,
         "%s: %s attached to %s.",
         action_identifier.c_str(), subject_name.c_str(), parent_name.c_str());
   }

   // Attachment failed: Terminate the sim if terminate_on_error is set.
   else if (terminate_on_error) {
      MessageHandler::fail (
         __FILE__, __LINE__, BodyActionMessages::fatal_error,
         "'%s' failed to attach '%s' to '%s'. "
         "The terminate_on_failure flag set and an attachment error occurred.\n"
         "The attachment error described above is fatal per this setting.",
         action_identifier.c_str(), subject_name.c_str(), parent_name.c_str());
   }

   // Attachment failed, terminate_ not set: Tell the user about the problem.
   else {
      MessageHandler::error (
         __FILE__, __LINE__, BodyActionMessages::not_performed,
         "%s failed to attach %s to %s.",
         action_identifier.c_str(), subject_name.c_str(), parent_name.c_str());
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
