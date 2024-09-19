/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup RadiationPressure
 * @{
 *
 * @file models/interactions/radiation_pressure/src/radiation_messages.cc
 * Implement the class RadiationMessages.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((None)))

Assumptions and limitations:
  ((None))

Library dependencies:
  ((radiation_messages.cc))



*******************************************************************************/

// System includes

// JEOD includes
#include "../include/radiation_messages.hh"

#define PATH "interactions/radiation_pressure/"

//! Namespace jeod
namespace jeod
{

// Static member data
// Errors and warnings
const char * RadiationMessages::incomplete_setup_error = PATH "incomplete_setup_error";

const char * RadiationMessages::invalid_setup_error = PATH "invalid_setup_error";

const char * RadiationMessages::operational_setup_error = PATH "operational_setup_error";

const char * RadiationMessages::invalid_function_call = PATH "invalid_function_call";

const char * RadiationMessages::unknown_numerical_error = PATH "unknown_numerical_error";

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
