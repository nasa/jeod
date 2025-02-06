/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/src/dyn_body_init_rot_state.cc
 * Define methods for DynBodyInitRotState.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((dyn_body_init_rot_state.o)
   (body_action_messages.o)
   (dyn_body_init.o)
   (dynamics/mass/mass_point_state.o)
   (utils/message/message_handler.o)
   (utils/ref_frames/ref_frame.o)
   (utils/ref_frames/ref_frame_compute_relative_state.o))



*******************************************************************************/


// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "environment/ephemerides/ephem_interface/include/ephem_ref_frame.hh"
#include "utils/ref_frames/include/ref_frame.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/body_action_messages.hh"
#include "../include/dyn_body_init_rot_state.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a DynBodyInitRotState object.
 */
DynBodyInitRotState::DynBodyInitRotState (
   void)
:
   state_items(Both)
{
   return;
}


/**
 * Indicate what parts of the vehicle state this object initializes.
 * This is depends on the state specified by the user: Both=attitude and
 * rate, Attitude=attitude, Rate=rate.
 * @return States initialized
 */
RefFrameItems::Items
DynBodyInitRotState::initializes_what (
   void)
{
   // This class of initializers initializes attitude and/or rate,
   // per the state instance variable.
   switch (state_items) {
      case Both:
         return RefFrameItems::Att_Rate;

      case Attitude:
         return RefFrameItems::Att;

      case Rate:
         return RefFrameItems::Rate;

      default:
         return RefFrameItems::No_Items;
   }
}


/**
 * Indicate whether this initializer is ready to be applied.
 * The full rotational state of the reference reference frame must be
 * known to compute the subject reference frame's rotational state.
 * @return Is initializer ready?
 */
bool
DynBodyInitRotState::is_ready (
   void)
{
   RefFrameItems::Items required_items;
   BodyRefFrame * body_frame;

   body_frame = dynamic_cast<BodyRefFrame *> (reference_ref_frame);


   // The initializer is not ready if internal dependencies are not satisfied.
   if (! DynBodyInit::is_ready ()) {
      return false;
   }


   // Initializing from a non-body, non-ephemeris frame is potentially
   // problematic. The initialization will proceed, but ...
   if ((body_frame == nullptr) &&
       (dynamic_cast<EphemerisRefFrame *> (reference_ref_frame) == nullptr)) {
      MessageHandler::error (
         __FILE__, __LINE__, BodyActionMessages::invalid_object,
         "%s is suspect:\n"
         "Reference frame '%s' is of a unknown derived type.",
         action_identifier.c_str(), reference_ref_frame->get_name());
   }


   // Determine the items needed from the reference reference frame.
   // Attitude: T_A:C = T_B:C*T_A:B           Need reference (A->B) attitude
   // Rate:     w_A:C = W_B:C + T_B:C*w_A:B   Need reference rate
   // The required items are the same as the items to be initialized.
   required_items = initializes_what ();


   // The initializer is ready if the either the reference reference frame
   // is not a body frame or if reference body frame contains the
   // requisite required_items.
   return (body_frame == nullptr) ||
          body_frame->initialized_items.contains (required_items);
}


/**
 * Initialize aspects of this object and forward the initializer
 * to the immediate parent class.
 * This class needs no initialization per se.
 * \param[in,out] dyn_manager Dynamics manager
 */
void
DynBodyInitRotState::initialize (
   DynManager & dyn_manager)
{
   // Check that the state enum value is valid.
   if ((state_items != Both) &&
       (state_items != Attitude) &&
       (state_items != Rate)) {
      MessageHandler::fail (
         __FILE__, __LINE__, BodyActionMessages::illegal_value,
         "%s failed:\n"
         "State specification 'state_items' enum value is not valid.",
         action_identifier.c_str());
   }

   // Pass the message up the chain.
   DynBodyInit::initialize (dyn_manager);
}


/**
 * Apply the initializer.
 * \param[in,out] dyn_manager Dynamics manager
 */
void
DynBodyInitRotState::apply (
   DynManager & dyn_manager)
{

   // Compute the state relative to the parent given the user inputs
   // relative to the reference reference frame.
   apply_user_inputs ();

   // Pass the message up the chain. This will apply the state just calculated
   // to the vehicle.
   DynBodyInit::apply (dyn_manager);

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
