/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/src/body_action_messages.cc
 * Implement the class BodyActionMessages.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((body_action_messages.cc))



*******************************************************************************/

// System includes

// JEOD includes
#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/body_action_messages.hh"

//! Namespace jeod
namespace jeod
{

#define MAKE_BODYACTION_MESSAGE_CODE(id) JEOD_MAKE_MESSAGE_CODE(BodyActionMessages, "dynamics/body_action/", id)

// Static member data
MAKE_BODYACTION_MESSAGE_CODE(fatal_error);
MAKE_BODYACTION_MESSAGE_CODE(illegal_value);
MAKE_BODYACTION_MESSAGE_CODE(invalid_name);
MAKE_BODYACTION_MESSAGE_CODE(invalid_object);
MAKE_BODYACTION_MESSAGE_CODE(null_pointer);
MAKE_BODYACTION_MESSAGE_CODE(not_performed);
MAKE_BODYACTION_MESSAGE_CODE(trace);

#undef MAKE_BODYACTION_MESSAGE_CODE

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
