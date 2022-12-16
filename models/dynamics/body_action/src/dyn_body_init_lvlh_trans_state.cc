/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/src/dyn_body_init_lvlh_trans_state.cc
 * Define methods for DynBodyInitLvlhTransState.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((dyn_body_init_lvlh_trans_state.o)
   (body_action_messages.o)
   (dyn_body_init.o)
   (dyn_body_init_lvlh_state.o)
   (dyn_body_init_planet_derived.o)
   (dyn_body_init_wrt_planet.o)
   (dynamics/mass/mass_point_state.o)
   (utils/message/message_handler.o)
   (utils/ref_frames/ref_frame_items.o))



*******************************************************************************/


// System includes
#include <cstddef>

// JEOD includes
#include "utils/message/include/message_handler.hh"
#include "utils/ref_frames/include/ref_frame_items.hh"

// Model includes
#include "../include/body_action_messages.hh"
#include "../include/dyn_body_init_lvlh_trans_state.hh"



//! Namespace jeod
namespace jeod {

/**
 * DynBodyInitLvlhTransState default constructor.
 */
DynBodyInitLvlhTransState::DynBodyInitLvlhTransState (
   void)
:
   DynBodyInitLvlhState()
{
   set_items = RefFrameItems::Pos_Vel;
   return;
}


/**
 * DynBodyInitLvlhTransState destructor.
 */
DynBodyInitLvlhTransState::~DynBodyInitLvlhTransState (
   void)
{
   return;
}


/**
 * Initialize the initializer.
 * \param[in,out] dyn_manager Dynamics manager
 */
void
DynBodyInitLvlhTransState::initialize (
   DynManager & dyn_manager)
{

   RefFrameItems test_items(set_items);

   // Check for an invalid user-override of the to-be-initialized states.
   // Warn (but do not die) if this is the case.
   if (test_items.contains (RefFrameItems::Att) ||
       test_items.contains (RefFrameItems::Rate)) {

      MessageHandler::warn (
         __FILE__, __LINE__, BodyActionMessages::illegal_value,
         "%s warning:\n"
         "set_items contains rotational aspects. Removing them.",
         action_identifier);

      test_items.remove (RefFrameItems::Att_Rate);
      set_items = test_items.get();
   }

   // Pass the message up the chain. This will initialize the base
   // characteristics of the instance.
   DynBodyInitLvlhState::initialize (dyn_manager);

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
