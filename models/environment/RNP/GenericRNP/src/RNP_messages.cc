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

#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/RNP_messages.hh"

//! Namespace jeod
namespace jeod
{

#define MAKE_RNP_MESSAGE_CODE(id) JEOD_MAKE_MESSAGE_CODE(RNPMessages, "environment/RNP/", id)

// Errors
MAKE_RNP_MESSAGE_CODE(initialization_error);
MAKE_RNP_MESSAGE_CODE(fidelity_error);
MAKE_RNP_MESSAGE_CODE(setup_error);

// Warnings
MAKE_RNP_MESSAGE_CODE(polar_motion_table_warning);

#undef MAKE_RNP_MESSAGE_CODE

} // namespace jeod

/**
 * @}
 * @}
 * @}
 * @}
 */
