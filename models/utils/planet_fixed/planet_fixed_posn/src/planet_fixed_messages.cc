/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup PlanetFixed
 * @{
 * @addtogroup PlanetFixedPosn
 * @{
 *
 * @file models/utils/planet_fixed/planet_fixed_posn/src/planet_fixed_messages.cc
 * Implement the class PlanetFixedMessages.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((planet_fixed_messages.cc))


*******************************************************************************/

// System includes

// JEOD includes
#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/planet_fixed_messages.hh"

//! Namespace jeod
namespace jeod
{

#define MAKE_PLANETFIXED_MESSAGE_CODE(id) JEOD_MAKE_MESSAGE_CODE(PlanetFixedMessages, "environment/planet_fixed/", id)

// Static member data
// Errors
MAKE_PLANETFIXED_MESSAGE_CODE(invalid_request);
MAKE_PLANETFIXED_MESSAGE_CODE(domain_error);

#undef MAKE_PLANETFIXED_MESSAGE_CODE

} // namespace jeod

/**
 * @}
 * @}
 * @}
 * @}
 */
