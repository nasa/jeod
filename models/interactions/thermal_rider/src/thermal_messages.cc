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
#include "../include/thermal_messages.hh"

#define PATH "utils/thermal_rider/"

//! Namespace jeod
namespace jeod
{

// Static member data
// Errors and warnings
const char * ThermalMessages::incomplete_setup_error = PATH "incomplete_setup_error";

const char * ThermalMessages::invalid_integration_operation = PATH "invalid_integration_operation";

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
