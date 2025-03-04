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

#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/contact_messages.hh"

//! Namespace jeod
namespace jeod
{

#define MAKE_CONTACT_MESSAGE_CODE(id) JEOD_MAKE_MESSAGE_CODE(ContactMessages, "interactions/contact", id)

MAKE_CONTACT_MESSAGE_CODE(initialization_error);
MAKE_CONTACT_MESSAGE_CODE(initialization_warns);
MAKE_CONTACT_MESSAGE_CODE(runtime_error);
MAKE_CONTACT_MESSAGE_CODE(pre_initialization_error);
MAKE_CONTACT_MESSAGE_CODE(runtime_warns);
MAKE_CONTACT_MESSAGE_CODE(runtime_inform);

#undef MAKE_CONTACT_MESSAGE_CODE

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
