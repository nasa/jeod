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
  ((earth_lighting_messages.o))



*******************************************************************************/

#include "../include/earth_lighting_messages.hh"

#define PATH "environment/earth_lighting/"

//! Namespace jeod
namespace jeod {

// Errors
char const * EarthLightingMessages::initialization_error =
   PATH "initialization_error";


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
