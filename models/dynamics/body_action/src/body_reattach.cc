/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/src/mass_body_reattach.cc
 * Define methods for the mass body initialization class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((body_reattach.o)
   (body_action.o)
   (body_action_messages.o)
   (dynamics/mass/mass_point_state.o)
   (dynamics/mass/mass_reattach.o)
   (utils/message/message_handler.o)
   (utils/orientation/orientation.o))



*******************************************************************************/


// System includes

// JEOD includes
#include "dynamics/mass/include/mass.hh"
#include "utils/math/include/vector3.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/body_reattach.hh"
#include "../include/body_action_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a MassBodyReattach.
 */
BodyReattach::BodyReattach (
   void)
:
   pstr_cstr()
{
   active = false;
   Vector3::initialize (offset_pstr_cstr_pstr);
}


/**
 * Initialize the core mass properties of the subject MassBody.
 * \param[in,out] dyn_manager Jeod manager
 */
void
BodyReattach::apply (
   DynManager & dyn_manager)
{
   bool succeeded = true;

   // Convert user-specified orientation to transformation matrix.
   pstr_cstr.compute_transform ();

   // Rettach the body.
   // FIXME, JEOD 2.1: reattach needs to return a status.
   subject->reattach (offset_pstr_cstr_pstr, pstr_cstr.trans);

   // Terminate the sim if requested to do so on failed attachments.
   if ((! succeeded) && terminate_on_error) {
      MessageHandler::fail (
         __FILE__, __LINE__, BodyActionMessages::fatal_error,
         "%s failed:\n"
         "terminate_on_failure flag set and attachment error occurred.\n"
         "The attachment error described above is fatal per this setting.",
         action_identifier);
   }

   // Debug.
   if (succeeded) {
      MessageHandler::debug (
            __FILE__, __LINE__, BodyActionMessages::trace,
            "%s: %s reattached.",
            action_identifier, subject->name.c_str());
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
