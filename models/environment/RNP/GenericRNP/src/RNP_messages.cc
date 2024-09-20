/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup RNP
 * @{
 * @addtogroup GenericRNP
 * @{
 *
 * @file models/environment/RNP/GenericRNP/src/RNP_messages.cc
 * Implement RNP_messages
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
  ((RNP_messages.cc))



*******************************************************************************/

#include "../include/RNP_messages.hh"

#define PATH "environment/RNP/"

//! Namespace jeod
namespace jeod
{

// Errors
const char * RNPMessages::initialization_error = PATH "initialization_error";

const char * RNPMessages::fidelity_error = PATH "fidelity_error";

const char * RNPMessages::setup_error = PATH "setup_error";

// Warnings

const char * RNPMessages::polar_motion_table_warning = PATH "polar_motion_table_warning";

} // namespace jeod

/**
 * @}
 * @}
 * @}
 * @}
 */
