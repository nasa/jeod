/*******************************************************************************

Purpose:
  (Implement the class MessageHandlerVerifDriver.)

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((message_handler_verif_driver.cc)
  ((message_handler_verif_messages.cc))

 

*******************************************************************************/


// System includes

// JEOD includes
#include "../include/message_handler_verif_driver.hh"
#include "../include/message_handler_verif_messages.hh"
#include "utils/message/include/message_handler.hh"


//! Namespace jeod
namespace jeod {

/*******************************************************************************
Function: MessageHandlerVerifDriver
Purpose: (Construct a MessageHandlerVerifDriver)
*******************************************************************************/
MessageHandlerVerifDriver::MessageHandlerVerifDriver ( // Return: N/A
   void)
: message_number(0)
{
   ; // Empty
}


/*******************************************************************************
Function: integrate
Purpose: (Always return zero)
*******************************************************************************/
int
MessageHandlerVerifDriver::integrate (          // Return: -- Zero
   JEOD_TRICK_INTEGRATOR_ALIAS * integ __attribute__ ((unused)) ) // Inout:  -- Ignored
{
   return 0;
}


/*******************************************************************************
Function: generate_messages
Purpose: (Generate messages)
*******************************************************************************/
void
MessageHandlerVerifDriver::generate_messages () // Return: N/A
{
   switch (message_number) {
   case 0:
      break;

   case 1:
      MessageHandler::fail (
         __FILE__, __LINE__, MessageHandlerVerifMessages::error,
         "Test of %s",
         "MessageHandler::fail()");
      break;

   case 2:
      MessageHandler::error (
         __FILE__, __LINE__, MessageHandlerVerifMessages::error,
         "Test of %s",
         "MessageHandler::error()");
      break;

   case 3:
      MessageHandler::warn (
         __FILE__, __LINE__, MessageHandlerVerifMessages::warning,
         "Test of %s",
         "MessageHandler::warn()");
      break;

   case 4:
      MessageHandler::inform (
         __FILE__, __LINE__, MessageHandlerVerifMessages::inform,
         "Test of %s",
         "MessageHandler::inform()");
      break;

   case 5:
      MessageHandler::debug (
         __FILE__, __LINE__, MessageHandlerVerifMessages::debug,
         "Test of %s",
         "MessageHandler::debug()");
      break;

   case 6:
      MessageHandler::send_message (
         6, "Message",
         __FILE__, __LINE__, MessageHandlerVerifMessages::message,
         "Test of %s",
         "MessageHandler::send_message()");
      break;

   default:
      break;
   }

   message_number = 0;

   return;
}
} // End JEOD namespace
