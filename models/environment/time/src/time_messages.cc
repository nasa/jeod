/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/src/time_messages.cc
 * Implement the class TimeMessages.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((None)))

Assumptions and limitations:
  ((None))

Library dependencies:
  ((time_messages.cc))



*******************************************************************************/

// System includes

// JEOD includes
#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/time_messages.hh"

//! Namespace jeod
namespace jeod
{

#define MAKE_TIME_MESSAGE_CODE(id) JEOD_MAKE_MESSAGE_CODE(TimeMessages, "environment/time/", id)

// Static member data
// Errors and warnings
MAKE_TIME_MESSAGE_CODE(initialization_error);
MAKE_TIME_MESSAGE_CODE(memory_error);
MAKE_TIME_MESSAGE_CODE(invalid_setup_error);
MAKE_TIME_MESSAGE_CODE(invalid_data_error);
MAKE_TIME_MESSAGE_CODE(incomplete_setup_error);
MAKE_TIME_MESSAGE_CODE(redundancy_error);
MAKE_TIME_MESSAGE_CODE(duplicate_methods);
MAKE_TIME_MESSAGE_CODE(extension_error);
MAKE_TIME_MESSAGE_CODE(invalid_node);

#undef MAKE_TIME_MESSAGE_CODE

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
