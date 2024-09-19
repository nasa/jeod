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
namespace jeod
{

// Static member data
const char * PlanetMessages::name_error = PATH "name_error";
const char * PlanetMessages::registration_error = PATH "registration_error";
const char * PlanetMessages::domain_error = PATH "domain_error";

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
