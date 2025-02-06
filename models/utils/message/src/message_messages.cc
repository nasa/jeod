/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Message
 * @{
 *
 * @file models/utils/message/src/message_messages.cc
 * Implement the class MessageMessages.
 */

/*******************************************************************************

Purpose:
  ()


Library dependencies:
  ((message_messages.o))

 

*******************************************************************************/


// System includes

// JEOD includes
#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/message_messages.hh"


//! Namespace jeod
namespace jeod {

#define MAKE_MESSAGE_MESSAGE_CODE(id) \
   JEOD_MAKE_MESSAGE_CODE(MessageMessages, "utils/message/", id)


// Define MessageMessages static member data

MAKE_MESSAGE_MESSAGE_CODE (singleton_error);

#undef MAKE_MESSAGE_MESSAGE_CODE


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
