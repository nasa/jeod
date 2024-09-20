/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/src/time_messages.cc
 * Implement the class TimeMessages.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((None)))

Assumptions and limitations:
  ((None))

Library dependencies:
  ((time_messages.cc))



*******************************************************************************/

// System includes

// JEOD includes
#include "../include/time_messages.hh"

#define PATH "environment/time/"

//! Namespace jeod
namespace jeod
{

// ENTRY POINT //

// Static member data
// Errors and warnings
const char * TimeMessages::initialization_error = PATH "initialization_error";

const char * TimeMessages::memory_error = PATH "memory_error";

const char * TimeMessages::invalid_setup_error = PATH "invalid_setup_error";

const char * TimeMessages::invalid_data_error = PATH "invalid_data_error";

const char * TimeMessages::incomplete_setup_error = PATH "incomplete_setup_error";

const char * TimeMessages::redundancy_error = PATH "redundancy_error";

const char * TimeMessages::duplicate_methods = PATH "duplicate_methods";

const char * TimeMessages::extension_error = PATH "extension_error";

const char * TimeMessages::invalid_node = PATH "invalid_node";

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
