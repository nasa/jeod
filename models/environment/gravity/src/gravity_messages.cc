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
namespace jeod
{

// Static member data

const char * GravityMessages::duplicate_entry = PATH "duplicate_entry";
const char * GravityMessages::missing_entry = PATH "missing_entry";
const char * GravityMessages::invalid_name = PATH "invalid_name";
const char * GravityMessages::invalid_object = PATH "invalid_object";

const char * GravityMessages::invalid_limit = PATH "invalid_limit";
const char * GravityMessages::domain_error = PATH "domain_error";
const char * GravityMessages::null_pointer = PATH "null_pointer";

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
