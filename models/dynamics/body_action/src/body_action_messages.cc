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
namespace jeod {


// Static member data
char const * BodyActionMessages::fatal_error =
    PATH "fatal_error";

char const * BodyActionMessages::illegal_value =
    PATH "illegal_value";

char const * BodyActionMessages::invalid_name =
    PATH "invalid_name";

char const * BodyActionMessages::invalid_object =
    PATH "invalid_object";

char const * BodyActionMessages::null_pointer =
    PATH "null_pointer";

char const * BodyActionMessages::not_performed =
    PATH "not_performed";

char const * BodyActionMessages::trace =
    PATH "trace";

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
