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
  ((derived_state_messages.o))



*******************************************************************************/


// System includes

// JEOD includes
#include "../include/derived_state_messages.hh"

#define PATH "dynamics/derived_state/"


//! Namespace jeod
namespace jeod {

// Static member data
char const * DerivedStateMessages::fatal_error =
    PATH "fatal_error";

char const * DerivedStateMessages::illegal_value =
    PATH "illegal_value";

char const * DerivedStateMessages::invalid_name =
    PATH "invalid_name";

char const * DerivedStateMessages::invalid_object =
    PATH "invalid_object";

char const * DerivedStateMessages::null_pointer =
    PATH "null_pointer";

char const * DerivedStateMessages::trace =
    PATH "trace";

char const * DerivedStateMessages::divide_by_zero =
   PATH "divide_by_zero";

 } // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
