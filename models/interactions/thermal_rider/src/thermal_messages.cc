/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup ThermalRider
 * @{
 *
 * @file models/interactions/thermal_rider/src/thermal_messages.cc
 * Implement the class ThermalMessages.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((None)))

Assumptions and limitations:
  ((None))

Library dependencies:
  ((thermal_messages.cc))


*******************************************************************************/

// System includes

// JEOD includes
#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/thermal_messages.hh"

//! Namespace jeod
namespace jeod
{

#define MAKE_THERMAL_MESSAGE_CODE(id) JEOD_MAKE_MESSAGE_CODE(ThermalMessages, "utils/thermal_rider/", id)

// Static member data
// Errors and warnings
MAKE_THERMAL_MESSAGE_CODE(incomplete_setup_error);
MAKE_THERMAL_MESSAGE_CODE(invalid_integration_operation);

#undef MAKE_THERMAL_MESSAGE_CODE

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
