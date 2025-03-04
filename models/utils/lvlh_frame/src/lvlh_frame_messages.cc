/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup LvlhFrame
 * @{
 *
 * @file models/utils/lvlh_frame/src/lvlh_frame_messages.cc
 * Implement the class LvlhFrameMessages.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((lvlh_frame_messages.cc))



*******************************************************************************/

// System includes

// JEOD includes
#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/lvlh_frame_messages.hh"

//! Namespace jeod
namespace jeod
{

#define MAKE_LVLHFRAME_MESSAGE_CODE(id) JEOD_MAKE_MESSAGE_CODE(LvlhFrameMessages, "utils/lvlh_frame/", id)

// Static member data
MAKE_LVLHFRAME_MESSAGE_CODE(fatal_error);
MAKE_LVLHFRAME_MESSAGE_CODE(illegal_value);
MAKE_LVLHFRAME_MESSAGE_CODE(invalid_name);
MAKE_LVLHFRAME_MESSAGE_CODE(invalid_configuration);
MAKE_LVLHFRAME_MESSAGE_CODE(invalid_object);
MAKE_LVLHFRAME_MESSAGE_CODE(null_pointer);
MAKE_LVLHFRAME_MESSAGE_CODE(trace);
MAKE_LVLHFRAME_MESSAGE_CODE(divide_by_zero);

#undef MAKE_LVLHFRAME_MESSAGE_CODE

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
