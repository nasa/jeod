/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/src/dyn_body_init_ned_rot_state.cc
 * Define methods for DynBodyInitNedRotState.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((dyn_body_init_ned_rot_state.cc)
   (body_action_messages.cc)
   (dyn_body_init.cc)
   (dyn_body_init_ned_state.cc)
   (dyn_body_init_planet_derived.cc)
   (dyn_body_init_wrt_planet.cc)
   (dynamics/mass/src/mass_point_state.cc)
   (utils/message/src/message_handler.cc)
   (utils/ref_frames/src/ref_frame.cc)
   (utils/ref_frames/src/ref_frame_compute_relative_state.cc))



*******************************************************************************/


// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/ref_frames/include/ref_frame_items.hh"

// Model includes
#include "../include/body_action_messages.hh"
#include "../include/dyn_body_init_ned_rot_state.hh"


//! Namespace jeod
namespace jeod {

/**
 * DynBodyInitNedRotState default constructor.
 */
DynBodyInitNedRotState::DynBodyInitNedRotState (
   void)
:
   DynBodyInitNedState()
{
   set_items = RefFrameItems::Att_Rate;

   return;
}


/**
 * DynBodyInitNedRotState destructor.
 */
DynBodyInitNedRotState::~DynBodyInitNedRotState (
   void)
{

   return;
}


/**
 * Initialize the initializer.
 * \param[in,out] dyn_manager Dynamics manager
 */
void
DynBodyInitNedRotState::initialize (
   DynManager & dyn_manager)
{
   RefFrameItems test_items(set_items);

   // Check for an invalid user-override of the to-be-initialized states.
   // Warn (but do not die) if this is the case.
   if (test_items.contains (RefFrameItems::Pos) ||
       test_items.contains (RefFrameItems::Vel)) {

      MessageHandler::warn (
         __FILE__, __LINE__, BodyActionMessages::illegal_value,
         "%s warning:\n"
         "set_items contains translational aspects. Removing them.",
         action_identifier.c_str());

      test_items.remove (RefFrameItems::Pos_Vel);
      set_items = test_items.get();
   }

   // Pass the message up the chain. This will initialize the base
   // characteristics of the instance.
   DynBodyInitNedState::initialize (dyn_manager);

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
