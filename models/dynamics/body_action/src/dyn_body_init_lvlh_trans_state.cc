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
  ((dyn_body_init_lvlh_trans_state.cc)
   (body_action_messages.cc)
   (dyn_body_init.cc)
   (dyn_body_init_lvlh_state.cc)
   (dyn_body_init_planet_derived.cc)
   (dyn_body_init_wrt_planet.cc)
   (dynamics/mass/src/mass_point_state.cc)
   (utils/message/src/message_handler.cc)
   (utils/ref_frames/src/ref_frame_items.cc))



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
namespace jeod
{

/**
 * DynBodyInitLvlhTransState default constructor.
 */
DynBodyInitLvlhTransState::DynBodyInitLvlhTransState()
{
    set_items = RefFrameItems::Pos_Vel;
}

/**
 * Initialize the initializer.
 * \param[in,out] dyn_manager Dynamics manager
 */
void DynBodyInitLvlhTransState::initialize(DynManager & dyn_manager)
{
    RefFrameItems test_items(set_items);

    // Check for an invalid user-override of the to-be-initialized states.
    // Warn (but do not die) if this is the case.
    if(test_items.contains(RefFrameItems::Att) || test_items.contains(RefFrameItems::Rate))
    {
        MessageHandler::warn(__FILE__,
                             __LINE__,
                             BodyActionMessages::illegal_value,
                             "%s warning:\n"
                             "set_items contains rotational aspects. Removing them.",
                             action_identifier.c_str());

        test_items.remove(RefFrameItems::Att_Rate);
        set_items = test_items.get();
    }

    // Pass the message up the chain. This will initialize the base
    // characteristics of the instance.
    DynBodyInitLvlhState::initialize(dyn_manager);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
