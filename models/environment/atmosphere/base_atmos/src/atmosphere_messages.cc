/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Atmosphere
 * @{
 *
 * @file models/environment/atmosphere/base_atmos/src/atmosphere_messages.cc
 * Implement atmosphere_messages
 */

/*******************************************************************************

Purpose:
  (Message roots)

Assumptions and limitations:
   ((Outlined in the header file))


  ()

*******************************************************************************/

#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/atmosphere_messages.hh"

//! Namespace jeod
namespace jeod
{

#define MAKE_ATMOSPHERE_MESSAGE_CODE(id)                                                                               \
    JEOD_MAKE_MESSAGE_CODE(AtmosphereMessages, "environment/atmosphere/base_atmos", id)

// Errors
MAKE_ATMOSPHERE_MESSAGE_CODE(initialization_error);
MAKE_ATMOSPHERE_MESSAGE_CODE(framework_error);
MAKE_ATMOSPHERE_MESSAGE_CODE(framework_warning);
MAKE_ATMOSPHERE_MESSAGE_CODE(numerical_warning);

#undef MAKE_ATMOSPHERE_MESSAGE_CODE

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
