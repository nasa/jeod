/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Message
 * @{
 *
 * @file models/utils/message/src/message_handler.cc
 * Define member functions for the class MessageHandler.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((message_handler.cc)
   (message_messages.cc))

 

*******************************************************************************/


// System includes
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>

// JEOD includes
#include "../include/message_handler.hh"
#include "../include/message_messages.hh"


//! Namespace jeod
namespace jeod {

/*
 Define message severity levels. See the header for documentation.
*/

const int MessageHandler::Failure =  -1;
const int MessageHandler::Error   =   0;
const int MessageHandler::Warning =   9;
const int MessageHandler::Notice  =  99;
const int MessageHandler::Debug   = 999;



/*
Purpose:
  (Define and initialize the global message handler.)
*/
MessageHandler * MessageHandler::handler = nullptr;



/**
 * Generate a message with negative severity, MessageHandler::Failure,
 * and terminate the simulation.
 * The intent of this method is to handle erroneous situations for which
 * no recovery path exists. If a recovery path does exist, even if very
 * suspect, callers of this method should consider calling
 * MessageHandler::error as an alternative.
 *
 * A conforming implementation of a class that derives MessageHandler
 * will force the simulation to terminate upon receipt of a
 * message with negative severity.
 * \param[in] file Typically __FILE__
 * \param[in] line Typically __LINE__
 * \param[in] msg_code Message code
 * \param[in] format sprintf format
 * \param[in] ... sprintf arguments
 */
void
MessageHandler::fail (
   const char * file,
   unsigned int line,
   const char * msg_code,
   const char * format,
   ...)
{
   // No handler: Exit.
   if (handler == nullptr) {
      no_handler_error ();
   }

   // Handler exists: Pass message to the handler.
   else {
      va_list args; // -- Varargs stack
      va_start (args, format);
      handler->process_message (
         MessageHandler::Failure, "Fatal Error", file, line, msg_code,
         format, args);
      va_end (args);
   }
}



/**
 * Generate a message with severity MessageHandler::Error.
 * An error represents a very serious problem.
 * The intent is to represent errors that invalidate simulation results
 * but for which a recovery path does exist. Using MessageHandler::error
 * rather than MessageHandler::fail enables the user to address
 * multiple errors at a time.
 *
 * A conforming implementation of a class that derives MessageHandler
 * will always report error messages. Errors should not be suppressed.
 * \param[in] file Typically __FILE__
 * \param[in] line Typically __LINE__
 * \param[in] msg_code Message code
 * \param[in] format sprintf format
 * \param[in] ... sprintf arguments
 */
void
MessageHandler::error (
   const char * file,
   unsigned int line,
   const char * msg_code,
   const char * format,
   ...)
{
   // No handler: Exit.
   if (handler == nullptr) {
      no_handler_error ();
   }

   // Handler exists: Pass message to the handler.
   else {
      va_list args; // -- Varargs stack
      va_start (args, format);
      handler->process_message (
         MessageHandler::Error  , "Error", file, line, msg_code, format, args);
      va_end (args);
   }
}



/**
 * Generate a message with severity MessageHandler::Warning.
 * Warnings represent situations where the model developer had to make
 * some assumptions to recover from what would otherwise be an erroneous
 * condition. The recovery based on those assumptions does not
 * necessarily invalidate invalidate the simulation results.
 * \param[in] file Typically __FILE__
 * \param[in] line Typically __LINE__
 * \param[in] msg_code Message code
 * \param[in] format sprintf format
 * \param[in] ... sprintf arguments
 */
void
MessageHandler::warn (
   const char * file,
   unsigned int line,
   const char * msg_code,
   const char * format,
   ...)
{
   // No handler: Exit.
   if (handler == nullptr) {
      no_handler_error ();
   }

   // Handler exists: Pass message to the handler.
   else {
      va_list args; // -- Varargs stack
      va_start (args, format);
      handler->process_message (
        MessageHandler::Warning, "Warning", file, line, msg_code, format, args);
      va_end (args);
   }
}


/**
 * Generates a message with severity MessageHandler::Notice.
 * Informational notices should not represent problems of any
 * significance as the default behavior is to suppress such messages.
 * \param[in] file Typically __FILE__
 * \param[in] line Typically __LINE__
 * \param[in] msg_code Message code
 * \param[in] format sprintf format
 * \param[in] ... sprintf arguments
 */
void
MessageHandler::inform (
   const char * file,
   unsigned int line,
   const char * msg_code,
   const char * format,
   ...)
{
   // No handler: Exit.
   if (handler == nullptr) {
      no_handler_error ();
   }

   // Handler exists: Pass message to the handler.
   else {
      va_list args; // -- Varargs stack
      va_start (args, format);
      handler->process_message (
         MessageHandler::Notice , "Notice", file, line, msg_code, format, args);
      va_end (args);
   }
}


/**
 * Generate a message with severity MessageHandler::Debug.
 * Debug messages should never be used for erroneous conditions.
 * They should instead be used for describing nominal behavior.
 * Note that debug messages are nominally suppressed.
 * \param[in] file Typically __FILE__
 * \param[in] line Typically __LINE__
 * \param[in] msg_code Message code
 * \param[in] format sprintf format
 * \param[in] ... sprintf arguments
 */
void
MessageHandler::debug (
   const char * file,
   unsigned int line,
   const char * msg_code,
   const char * format,
   ...)
{
   // No handler: Exit.
   if (handler == nullptr) {
      no_handler_error ();
   }

   // Handler exists: Pass message to the handler.
   else {
      va_list args; // -- Varargs stack
      va_start (args, format);
      handler->process_message (
         MessageHandler::Debug, "Debug", file, line, msg_code, format, args);
      va_end (args);
   }
}


/**
 * Generic variable arguments message interface.
 * This method gives the caller control over the severity level and over
 * the message prefix. These are automatically generated in the standard
 * set of MessageHandler interface methods.
 * \param[in] severity Severity level
 * \param[in] prefix Message prefix (e.g., Error)
 * \param[in] file Typically __FILE__
 * \param[in] line Typically __LINE__
 * \param[in] msg_code Message code
 * \param[in] format sprintf format
 * \param[in] ... sprintf arguments
 */
void
MessageHandler::send_message (
   int severity,
   const char * prefix,
   const char * file,
   unsigned int line,
   const char * msg_code,
   const char * format,
   ...)
{
   // No handler: Exit.
   if (handler == nullptr) {
      no_handler_error ();
   }

   // Handler exists: Pass message to the handler.
   else {
      va_list args; // -- Varargs stack
      va_start (args, format);
      handler->process_message (
         severity, prefix, file, line, msg_code, format, args);
      va_end (args);
   }
}


/**
 * Generic variable arguments message interface.
 * This method behaves similarly to MessageHandler::send_message except
 * that the caller has already captured the variable arguments in the
 * form of a va_list. Note that MessageHandler::va_send_message does not
 * call va_end macro.
 * \param[in] severity Severity level
 * \param[in] prefix Message prefix (e.g., Error)
 * \param[in] file Typically __FILE__
 * \param[in] line Typically __LINE__
 * \param[in] msg_code Message code
 * \param[in] format sprintf format
 * \param[in,out] args Varargs stack
 */
void
MessageHandler::va_send_message (
   int severity,
   const char * prefix,
   const char * file,
   unsigned int line,
   const char * msg_code,
   const char * format,
   va_list args)
{

   // No handler: Exit.
   if (handler == nullptr) {
      no_handler_error ();
   }

   // Handler exists: Pass message to the handler.
   else {
      handler->process_message (
         severity, prefix, file, line, msg_code, format, args);
   }
}



/**
 * Set the suppression level in the global message handler.
 * \param[in] suppression_level New suppression level
 */
void
MessageHandler::set_suppression_level (
   unsigned int suppression_level)
{

   // No handler: Exit.
   if (handler == nullptr) {
      no_handler_error ();
   }

   // Handler exists: Pass message to the handler.
   else {
      handler->suppression_level = suppression_level;
   }
}


/**
 * Get the suppress_id of the global massage handler.
 * @return Suppression level value
 */
unsigned int
MessageHandler::get_suppression_level (
   void)
{
   unsigned int result = -1;

   // No handler: Exit.
   if (handler == nullptr) {
      no_handler_error ();
   }

   // Handler exists: Get value from the handler.
   else {
      result = handler->suppression_level;
   }

   return result;
}


/**
 * Add a message code to the set of messages that are to be suppressed
 * for messages with positive severity level.
 * Note: Fatal errors and serious errors cannot be suppressed.
 * \param[in] msg_code Message code to be suppressed
 */

void
MessageHandler::add_suppressed_code (
   const char * msg_code)
{

   // No handler: Exit.
   if (handler == nullptr) {
      no_handler_error ();
   }

   // Handler exists: Pass message to the handler.
   else {
      handler->process_add_suppressed_code (msg_code);
   }
}


/**
 * Delete a message code from the set of suppressed message codes.
 * \param[in] msg_code Message code to be unsuppressed
 */

void
MessageHandler::delete_suppressed_code (
   const char * msg_code)
{

   // No handler: Exit.
   if (handler == nullptr) {
      no_handler_error ();
   }

   // Handler exists: Pass message to the handler.
   else {
      handler->process_delete_suppressed_code (msg_code);
   }
}


/**
 * Clear the set of suppressed message codes.
 */

void
MessageHandler::clear_suppressed_codes (
   void)
{

   // No handler: Exit.
   if (handler == nullptr) {
      no_handler_error ();
   }

   // Handler exists: Pass message to the handler.
   else {
      handler->process_clear_suppressed_codes ();
   }
}


/**
 * Set the suppress_id flag in the global message handler.
 * \param[in] suppress_id New suppress id value
 */
void
MessageHandler::set_suppress_id (
   bool suppress_id)
{

   // No handler: Exit.
   if (handler == nullptr) {
      no_handler_error ();
   }

   // Handler exists: Pass message to the handler.
   else {
      handler->suppress_id = suppress_id;
   }
}


/**
 * Get the suppress_id of the global massage handler.
 * @return ID value\n Units: Suppress
 */
bool
MessageHandler::get_suppress_id (
   void)
{
   bool result = false;

   // No handler: Exit.
   if (handler == nullptr) {
      no_handler_error ();
   }

   // Handler exists: Get value from the handler.
   else {
      result = handler->suppress_id;
   }

   return result;
}


/**
 * Set the suppress_location in the global message handler.
 * \param[in] suppress_location New suppress_loc value
 */
void
MessageHandler::set_suppress_location (
   bool suppress_location)
{

   // No handler: Exit.
   if (handler == nullptr) {
      no_handler_error ();
   }

   // Handler exists: Pass message to the handler.
   else {
      handler->suppress_location = suppress_location;
   }
}


/**
 * Get the suppress_location of the global massage handler.
 * @return Suppress location value
 */
bool
MessageHandler::get_suppress_location (
   void)
{
   bool result = false;

   // No handler: Exit.
   if (handler == nullptr) {
      no_handler_error ();
   }

   // Handler exists: Get value from the handler.
   else {
      result = handler->suppress_location;
   }

   return result;
}



/**
 * Handle the error condition where there is no global handler.
 * @note That this condition exists means the simulation is non-compliant.
 *
 * \par Assumptions and Limitations
 *  - All JEOD-based  simulations must have a message handler and memory handler
 *     that at instantiated prior to instantiating any other JEOD-based model
 *     and destroyed after all those other models have been destroyed.
 *  - That no message handler exists means the simulation is not a compliant
 *     with the above restrictions.
 *  - The handling of this condition is intentionally simplistic.
 *     An error message is printed and the simulation is terminated
 *     via a system call to exit.
 */
void
MessageHandler::no_handler_error (
   void)
{
   std::fprintf (stderr,
                 "Fatal error: No message handler exists.\n"
                 "Insert a JEOD simulation interface into your simulation.\n");
   std::exit (1);
}


/**
 * Construct a MessageHandler.
 * This default constructor sets the suppression level to
 * MessageHandler::Warning, which means that messages of that severity
 * and lower will be printed. The suppress_id and suppress_location
 * flags are set to false; auxiliary information is not suppressed.
 */
MessageHandler::MessageHandler (
   void)
:
   suppression_level(MessageHandler::Warning),
   suppress_id(false),
   suppress_location(false)
{

   // No message handler yet: This is the handler.
   if (handler == nullptr) {
      handler = this;
   }

   // Have a message handler: This is a singleton error.
   else {
      MessageHandler::error (
         __FILE__, __LINE__, MessageMessages::singleton_error,
         "A message handler already exists.\n"
         "The newly created handler will not be used.");
   }
}


/**
 * Destruct a MessageHandler.
 */
MessageHandler::~MessageHandler (
   void)
{
   // This can no longer serve as the global message handler.
   if (handler == this) {
      handler = nullptr;
   }
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
