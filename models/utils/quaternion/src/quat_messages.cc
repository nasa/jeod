/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Quaternion
 * @{
 *
 * @file models/utils/quaternion/src/quat_messages.cc
 * Implement the class QuatMessages.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((quat.cc))



*******************************************************************************/

// System includes

// JEOD includes
#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/quat_messages.hh"

//! Namespace jeod
namespace jeod
{

#define MAKE_QUAT_MESSAGE_CODE(id) JEOD_MAKE_MESSAGE_CODE(QuatMessages, "utils/quaternion/", id)

// Static member data
MAKE_QUAT_MESSAGE_CODE(undefined);
MAKE_QUAT_MESSAGE_CODE(invalid_entry);

#undef MAKE_QUAT_MESSAGE_CODE

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
