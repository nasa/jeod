/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Math
 * @{
 *
 * @file models/utils/math/src/math_messages.cc
 * Implement the class MathMessages.
 */

/*******************************************************************************

Purpose:
  ()


Library dependencies:
  ((math_messages.cc))



*******************************************************************************/

// System includes

// JEOD includes
#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/math_messages.hh"

//! Namespace jeod
namespace jeod
{

#define MAKE_MATH_MESSAGE_CODE(id) JEOD_MAKE_MESSAGE_CODE(MathMessages, "utils/math/", id)

// Define MathMessages static member data
MAKE_MATH_MESSAGE_CODE(ill_conditioned);

#undef MAKE_MATH_MESSAGE_CODE

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
