/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Gravity
 * @{
 *
 * @file models/environment/gravity/src/gravity_messages.cc
 * Implement the class GravityMessages.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((gravity_messages.cc))



*******************************************************************************/

// System includes

// JEOD includes
#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/gravity_messages.hh"

//! Namespace jeod
namespace jeod
{

#define MAKE_GRAVITY_MESSAGE_CODE(id) JEOD_MAKE_MESSAGE_CODE(GravityMessages, "environment/gravity/", id)

// Static member data

MAKE_GRAVITY_MESSAGE_CODE(duplicate_entry);
MAKE_GRAVITY_MESSAGE_CODE(missing_entry);
MAKE_GRAVITY_MESSAGE_CODE(invalid_name);
MAKE_GRAVITY_MESSAGE_CODE(invalid_object);
MAKE_GRAVITY_MESSAGE_CODE(invalid_limit);
MAKE_GRAVITY_MESSAGE_CODE(domain_error);
MAKE_GRAVITY_MESSAGE_CODE(null_pointer);

#undef MAKE_GRAVITY_MESSAGE_CODE

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
