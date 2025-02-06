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

#include "../include/atmosphere_messages.hh"

#define PATH "environment/atmosphere/base_atmos"

//! Namespace jeod
namespace jeod
{

// Errors
const char * AtmosphereMessages::initialization_error = PATH "initialization_error";

const char * AtmosphereMessages::framework_error = PATH "framework_error";

const char * AtmosphereMessages::framework_warning = PATH "framework_warning";

const char * AtmosphereMessages::numerical_warning = PATH "numerical_warning";

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
