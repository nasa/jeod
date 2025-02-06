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
  ((time_messages.o))

 

*******************************************************************************/


// System includes

// JEOD includes
#include "../include/time_messages.hh"

#define PATH "environment/time/"


//! Namespace jeod
namespace jeod {

// ENTRY POINT //

// Static member data
// Errors and warnings
char const * TimeMessages::initialization_error =
    PATH "initialization_error";

char const * TimeMessages::memory_error =
    PATH "memory_error";

char const * TimeMessages::invalid_setup_error =
    PATH "invalid_setup_error";

char const * TimeMessages::invalid_data_error =
    PATH "invalid_data_error";

char const * TimeMessages::incomplete_setup_error =
    PATH "incomplete_setup_error";

char const * TimeMessages::redundancy_error =
    PATH "redundancy_error";

char const * TimeMessages::duplicate_methods =
    PATH "duplicate_methods";

char const * TimeMessages::extension_error =
    PATH "extension_error";

char const * TimeMessages::invalid_node =
    PATH "invalid_node";


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
