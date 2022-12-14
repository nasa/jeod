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
  ((dyn_body_init_lvlh_rot_state.o)
   (body_action_messages.o)
   (dyn_body_init.o)
   (dyn_body_init_lvlh_state.o)
   (dyn_body_init_planet_derived.o)
   (dyn_body_init_wrt_planet.o)
   (dynamics/dyn_body/dyn_body.o)
   (dynamics/dyn_manager/dyn_manager.o)
   (dynamics/mass/mass.o)
   (dynamics/mass/mass_point_state.o)
   (utils/message/message_handler.o)
   (utils/ref_frames/ref_frame.o))



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
         action_identifier);

      test_items.remove (RefFrameItems::Pos_Vel);
      set_items = test_items.get();
   }

   // A null reference vehicle name means the reference vehicle is the
   // subject vehicle.
   if ( ( ref_body == nullptr ) && ( ref_body_name == nullptr ) )
   {
       if( dyn_subject != nullptr )
       {
           ref_body = dyn_subject;
       }
       else if( subject != nullptr )
       {
           dyn_subject = dyn_manager.find_dyn_body( subject->name.c_str() );
           ref_body = dyn_subject;
       }
       else
       {
           MessageHandler::fail (
                    __FILE__, __LINE__, BodyActionMessages::null_pointer,
                    "%s failed:\n"
                    "The subject body was not assigned",
                    action_identifier);
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
