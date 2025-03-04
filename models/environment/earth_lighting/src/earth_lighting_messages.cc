/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup EarthLighting
 * @{
 *
 * @file models/environment/earth_lighting/src/earth_lighting_messages.cc
 * Implement earth_lighting_messages
 */

/*******************************************************************************

Purpose:
  ()

Reference:
   (((None)))

Assumptions and limitations:
   ((Outlined in the header file))

Class:
   (None)

Library dependencies:
  ((earth_lighting_messages.cc))



*******************************************************************************/

#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/earth_lighting_messages.hh"

//! Namespace jeod
namespace jeod
{

#define MAKE_EARTHLIGHTING_MESSAGE_CODE(id)                                                                            \
    JEOD_MAKE_MESSAGE_CODE(EarthLightingMessages, "environment/earth_lighting/", id)

// Errors
MAKE_EARTHLIGHTING_MESSAGE_CODE(initialization_error);

#undef MAKE_EARTHLIGHTING_MESSAGE_CODE

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
