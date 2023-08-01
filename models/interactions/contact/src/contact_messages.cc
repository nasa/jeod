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
namespace jeod {

char const * ContactMessages::initialization_error =
   PATH "initialization_error";

char const * ContactMessages::initialization_warns =
   PATH "initialization_warns";

char const * ContactMessages::runtime_error =
   PATH "runtime_error";

char const * ContactMessages::pre_initialization_error =
   PATH "pre_initialization_error";

char const * ContactMessages::runtime_warns =
   PATH "runtime_warns";

char const * ContactMessages::runtime_inform =
PATH "runtime_warns";

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
