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
#include "../include/planet_fixed_messages.hh"

#define PATH "environment/planet_fixed/"


//! Namespace jeod
namespace jeod {

// Static member data
// Errors

char const * PlanetFixedMessages::invalid_request = PATH "invalid_request";
char const * PlanetFixedMessages::domain_error    = PATH "domain_error";

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 * @}
 */
