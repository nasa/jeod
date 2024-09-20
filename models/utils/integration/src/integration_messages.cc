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
#include "../include/integration_messages.hh"

#define PATH "utils/integration/"
#define CLASS IntegrationMessages
#define MAKE_MESSAGE_CODE(id) char const * CLASS::id = PATH #id

//! Namespace jeod
namespace jeod
{

// Static member data

MAKE_MESSAGE_CODE(unsupported_option);
MAKE_MESSAGE_CODE(invalid_item);
MAKE_MESSAGE_CODE(internal_error);
MAKE_MESSAGE_CODE(invalid_request);
MAKE_MESSAGE_CODE(information);

} // namespace jeod

#undef MAKE_MESSAGE_CODE
#undef CLASS
#undef PATH

/**
 * @}
 * @}
 * @}
 */
