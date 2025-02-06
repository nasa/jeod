/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup RefFrames
 * @{
 *
 * @file models/utils/ref_frames/src/ref_frame_messages.cc
 * Implement the class RefFrameMessages.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((ref_frame_messages.cc))



*******************************************************************************/

// System includes

// JEOD includes
#include "utils/message/include/make_message_code.hh"

// JEOD includes
#include "../include/ref_frame_messages.hh"

//! Namespace jeod
namespace jeod
{

#define MAKE_REF_FRAME_MESSAGE_CODE(id) JEOD_MAKE_MESSAGE_CODE(RefFrameMessages, "utils/ref_frames/", id)

// Default RefFrameMessages static member data
MAKE_REF_FRAME_MESSAGE_CODE(attach_info);
MAKE_REF_FRAME_MESSAGE_CODE(duplicate_entry);
MAKE_REF_FRAME_MESSAGE_CODE(inconsistent_setup);
MAKE_REF_FRAME_MESSAGE_CODE(internal_error);
MAKE_REF_FRAME_MESSAGE_CODE(invalid_attach);
MAKE_REF_FRAME_MESSAGE_CODE(invalid_detach);
MAKE_REF_FRAME_MESSAGE_CODE(invalid_enum);
MAKE_REF_FRAME_MESSAGE_CODE(invalid_item);
MAKE_REF_FRAME_MESSAGE_CODE(invalid_name);
MAKE_REF_FRAME_MESSAGE_CODE(invalid_node);
MAKE_REF_FRAME_MESSAGE_CODE(null_pointer);
MAKE_REF_FRAME_MESSAGE_CODE(subscription_error);
MAKE_REF_FRAME_MESSAGE_CODE(removal_failed);

#undef MAKE_REF_FRAME_MESSAGE_CODE

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
