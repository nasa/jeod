/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup Contact
 * @{
 *
 * @file models/interactions/contact/src/contact_messages.cc
 * Implement contact_messages
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
  ((contact_messages.cc))



*******************************************************************************/

#include "../include/contact_messages.hh"

#define PATH "interactions/contact"

//! Namespace jeod
namespace jeod
{

const char * ContactMessages::initialization_error = PATH "initialization_error";

const char * ContactMessages::initialization_warns = PATH "initialization_warns";

const char * ContactMessages::runtime_error = PATH "runtime_error";

const char * ContactMessages::pre_initialization_error = PATH "pre_initialization_error";

const char * ContactMessages::runtime_warns = PATH "runtime_warns";

const char * ContactMessages::runtime_inform = PATH "runtime_warns";

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
