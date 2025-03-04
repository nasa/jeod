/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Integration
 * @{
 *
 * @file models/utils/integration/src/integration_messages.cc
 * Implement the class IntegrationMessages.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((integration_messages.cc))



*******************************************************************************/

// System includes

// JEOD includes
#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/integration_messages.hh"

//! Namespace jeod
namespace jeod
{

#define MAKE_INTEGRATION_MESSAGE_CODE(id) JEOD_MAKE_MESSAGE_CODE(IntegrationMessages, "utils/integration/", id)

// Static member data
MAKE_INTEGRATION_MESSAGE_CODE(unsupported_option);
MAKE_INTEGRATION_MESSAGE_CODE(invalid_item);
MAKE_INTEGRATION_MESSAGE_CODE(internal_error);
MAKE_INTEGRATION_MESSAGE_CODE(invalid_request);
MAKE_INTEGRATION_MESSAGE_CODE(information);

#undef MAKE_INTEGRATION_MESSAGE_CODE

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
