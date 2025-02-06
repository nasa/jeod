/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/src/body_action_messages.cc
 * Implement the class BodyActionMessages.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((body_action_messages.cc))



*******************************************************************************/

// System includes

// JEOD includes
#include "../include/body_action_messages.hh"

#define PATH "dynamics/body_action/"

//! Namespace jeod
namespace jeod
{

// Static member data
const char * BodyActionMessages::fatal_error = PATH "fatal_error";

const char * BodyActionMessages::illegal_value = PATH "illegal_value";

const char * BodyActionMessages::invalid_name = PATH "invalid_name";

const char * BodyActionMessages::invalid_object = PATH "invalid_object";

const char * BodyActionMessages::null_pointer = PATH "null_pointer";

const char * BodyActionMessages::not_performed = PATH "not_performed";

const char * BodyActionMessages::trace = PATH "trace";

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
