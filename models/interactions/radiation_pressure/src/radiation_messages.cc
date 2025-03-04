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
#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/radiation_messages.hh"

//! Namespace jeod
namespace jeod
{

#define MAKE_RADIATION_MESSAGE_CODE(id)                                                                                \
    JEOD_MAKE_MESSAGE_CODE(RadiationMessages, "interactions/radiation_pressure/", id)

// Static member data
// Errors and warnings
MAKE_RADIATION_MESSAGE_CODE(incomplete_setup_error);
MAKE_RADIATION_MESSAGE_CODE(invalid_setup_error);
MAKE_RADIATION_MESSAGE_CODE(operational_setup_error);
MAKE_RADIATION_MESSAGE_CODE(invalid_function_call);
MAKE_RADIATION_MESSAGE_CODE(unknown_numerical_error);

#undef MAKE_RADIATION_MESSAGE_CODE

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
