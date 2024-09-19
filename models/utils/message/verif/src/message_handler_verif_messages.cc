/*******************************************************************************

Purpose:
  (Implement the class MessageHandlerVerifMessages.)

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((message_handler_verif_messages.cc))



*******************************************************************************/

// System includes

// JEOD includes
#include "../include/message_handler_verif_messages.hh"

#define PATH "utils/message/verif/"

//! Namespace jeod
namespace jeod
{

// Static member data

const char * MessageHandlerVerifMessages::error = PATH "Error";
const char * MessageHandlerVerifMessages::warning = PATH "Warning";
const char * MessageHandlerVerifMessages::inform = PATH "Information";
const char * MessageHandlerVerifMessages::debug = PATH "Debug";
const char * MessageHandlerVerifMessages::message = PATH "Message";

} // namespace jeod
