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
namespace jeod {

 // Static member data

char const * MessageHandlerVerifMessages::error   = PATH "Error";
char const * MessageHandlerVerifMessages::warning = PATH "Warning";
char const * MessageHandlerVerifMessages::inform  = PATH "Information";
char const * MessageHandlerVerifMessages::debug   = PATH "Debug";
char const * MessageHandlerVerifMessages::message = PATH "Message";

} // End JEOD namespace
