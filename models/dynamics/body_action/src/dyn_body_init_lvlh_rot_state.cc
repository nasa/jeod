/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/src/dyn_body_init_lvlh_rot_state.cc
 * Define methods for DynBodyInitLvlhRotState.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((dyn_body_init_lvlh_rot_state.cc)
   (body_action_messages.cc)
   (dyn_body_init.cc)
   (dyn_body_init_lvlh_state.cc)
   (dyn_body_init_planet_derived.cc)
   (dyn_body_init_wrt_planet.cc)
   (dynamics/dyn_body/src/dyn_body.cc)
   (dynamics/dyn_manager/src/dyn_manager.cc)
   (dynamics/mass/src/mass.cc)
   (dynamics/mass/src/mass_point_state.cc)
   (utils/message/src/message_handler.cc)
   (utils/ref_frames/src/ref_frame.cc))



*******************************************************************************/


// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/ref_frames/include/ref_frame_items.hh"

// Model includes
#include "../include/body_action_messages.hh"
#include "../include/dyn_body_init_lvlh_rot_state.hh"




//! Namespace jeod
namespace jeod {

/**
 * DynBodyInitLvlhRotState default constructor.
 */
DynBodyInitLvlhRotState::DynBodyInitLvlhRotState (
   void)
:
   DynBodyInitLvlhState()
{
   set_items = RefFrameItems::Att_Rate;

   return;
}


/**
 * DynBodyInitLvlhRotState destructor.
 */
DynBodyInitLvlhRotState::~DynBodyInitLvlhRotState (
   void)
{

   return;
}


/**
 * Initialize the initializer.
 * \param[in,out] dyn_manager Dynamics manager
 */
void
DynBodyInitLvlhRotState::initialize (
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

   // A null reference vehicle name means the reference vehicle is the
   // subject vehicle.
   if ( ( ref_body == nullptr ) && ( ref_body_name.empty() ) )
   {
       ref_body = get_subject_dyn_body();

       if( ref_body == nullptr )
       {
           MessageHandler::fail (
                    __FILE__, __LINE__, BodyActionMessages::null_pointer,
                    "%s failed:\n"
                    "The subject body was not assigned",
                    action_identifier.c_str());
       }
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
