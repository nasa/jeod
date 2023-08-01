/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Gravity
 * @{
 *
 * @file models/environment/gravity/src/gravity_messages.cc
 * Implement the class GravityMessages.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((gravity_messages.cc))



*******************************************************************************/


// System includes

// JEOD includes
#include "../include/gravity_messages.hh"

#define PATH "environment/gravity/"


//! Namespace jeod
namespace jeod {

// Static member data

char const * GravityMessages::duplicate_entry = PATH "duplicate_entry";
char const * GravityMessages::missing_entry   = PATH "missing_entry";
char const * GravityMessages::invalid_name    = PATH "invalid_name";
char const * GravityMessages::invalid_object  = PATH "invalid_object";

char const * GravityMessages::invalid_limit   = PATH "invalid_limit";
char const * GravityMessages::domain_error    = PATH "domain_error";
char const * GravityMessages::null_pointer    = PATH "null_pointer";

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
