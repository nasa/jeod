/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup Aerodynamics
 * @{
 *
 * @file models/interactions/aerodynamics/src/aerodynamics_messages.cc
 * Implement aerodynamics_messages
 */

/*******************************************************************************

Purpose:
  ()

Reference:
   (((None)))

Assumptions and limitations:
   ((Outlined in the header file))

Class:
   (None)

Library dependencies:
  ((aerodynamics_messages.cc))



*******************************************************************************/

#include "../include/aerodynamics_messages.hh"

#define PATH "interactions/aerodynamics/"

//! Namespace jeod
namespace jeod
{

const char * AerodynamicsMessages::initialization_error = PATH "initialization_error";

const char * AerodynamicsMessages::runtime_error = PATH "runtime_error";

const char * AerodynamicsMessages::pre_initialization_error = PATH "pre_initialization_error";

const char * AerodynamicsMessages::runtime_warns = PATH "runtime_warns";

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
