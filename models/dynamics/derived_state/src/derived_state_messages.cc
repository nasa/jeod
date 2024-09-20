/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DerivedState
 * @{
 *
 * @file models/dynamics/derived_state/src/derived_state_messages.cc
 * Implement the class DerivedStateMessages.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((derived_state_messages.cc))



*******************************************************************************/

// System includes

// JEOD includes
#include "../include/derived_state_messages.hh"

#define PATH "dynamics/derived_state/"

//! Namespace jeod
namespace jeod
{

// Static member data
const char * DerivedStateMessages::fatal_error = PATH "fatal_error";

const char * DerivedStateMessages::illegal_value = PATH "illegal_value";

const char * DerivedStateMessages::invalid_name = PATH "invalid_name";

const char * DerivedStateMessages::invalid_object = PATH "invalid_object";

const char * DerivedStateMessages::null_pointer = PATH "null_pointer";

const char * DerivedStateMessages::trace = PATH "trace";

const char * DerivedStateMessages::divide_by_zero = PATH "divide_by_zero";

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
