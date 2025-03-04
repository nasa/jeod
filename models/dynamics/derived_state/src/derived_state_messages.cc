/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DerivedState
 * @{
 *
 * @file models/dynamics/derived_state/src/derived_state_messages.cc
 * Implement the class DerivedStateMessages.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((derived_state_messages.cc))



*******************************************************************************/

// System includes

// JEOD includes
#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/derived_state_messages.hh"

//! Namespace jeod
namespace jeod
{

#define MAKE_DERIVEDSTATE_MESSAGE_CODE(id) JEOD_MAKE_MESSAGE_CODE(DerivedStateMessages, "dynamics/derived_state/", id)

// Static member data
MAKE_DERIVEDSTATE_MESSAGE_CODE(fatal_error);
MAKE_DERIVEDSTATE_MESSAGE_CODE(illegal_value);
MAKE_DERIVEDSTATE_MESSAGE_CODE(invalid_name);
MAKE_DERIVEDSTATE_MESSAGE_CODE(invalid_object);
MAKE_DERIVEDSTATE_MESSAGE_CODE(null_pointer);
MAKE_DERIVEDSTATE_MESSAGE_CODE(trace);
MAKE_DERIVEDSTATE_MESSAGE_CODE(divide_by_zero);

#undef MAKE_DERIVEDSTATE_MESSAGE_CODE

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
