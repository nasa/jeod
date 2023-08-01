/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Planet
 * @{
 *
 * @file models/environment/planet/src/planet_messages.cc
 * Implement the class PlanetMessages.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((planet_messages.cc))



*******************************************************************************/


// System includes

// JEOD includes
#include "../include/planet_messages.hh"

#define PATH "environment/planet/"


//! Namespace jeod
namespace jeod {

// Static member data
char const * PlanetMessages::name_error         = PATH "name_error";
char const * PlanetMessages::registration_error = PATH "registration_error";
char const * PlanetMessages::domain_error       = PATH "domain_error";

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
