//=============================================================================
// Notices:
//
// Copyright © 2022 United States Government as represented by the Administrator
// of the National Aeronautics and Space Administration.  All Rights Reserved.
//
//
// Disclaimers:
//
// No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF
// ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED
// TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO SPECIFICATIONS, ANY
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
// FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL BE ERROR
// FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO THE
// SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN
// ENDORSEMENT BY GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS,
// RESULTING DESIGNS, HARDWARE, SOFTWARE PRODUCTS OR ANY OTHER APPLICATIONS
// RESULTING FROM USE OF THE SUBJECT SOFTWARE.  FURTHER, GOVERNMENT AGENCY
// DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING THIRD-PARTY SOFTWARE,
// IF PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT "AS IS."
//
// Waiver and Indemnity:  RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS AGAINST THE
// UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
// PRIOR RECIPIENT.  IF RECIPIENT'S USE OF THE SUBJECT SOFTWARE RESULTS IN ANY
// LIABILITIES, DEMANDS, DAMAGES, EXPENSES OR LOSSES ARISING FROM SUCH USE,
// INCLUDING ANY DAMAGES FROM PRODUCTS BASED ON, OR RESULTING FROM, RECIPIENT'S
// USE OF THE SUBJECT SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE
// UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
// PRIOR RECIPIENT, TO THE EXTENT PERMITTED BY LAW.  RECIPIENT'S SOLE REMEDY FOR
// ANY SUCH MATTER SHALL BE THE IMMEDIATE, UNILATERAL TERMINATION OF THIS
// AGREEMENT.
//
//=============================================================================
//
//
/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Message
 * @{
 *
 * @file models/utils/message/include/message_handler.hh
 * Define the class MessageHandler, the base class for generating
 * messages.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((message_handler.o))

 

*******************************************************************************/


#ifndef JEOD_MESSAGE_HANDLER_HH
#define JEOD_MESSAGE_HANDLER_HH

// System includes
#include <cstdarg>

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "class_declarations.hh"



//! Namespace jeod
namespace jeod {

/**
 * The base class for generating JEOD messages.
 * This class provides:
 *  - A suite of public static message generation and message control functions.
 *    The message generation functions provide the mechanism to generate and
 *    present messages of various levels of severity to the simulation user.
 *    The message control functions provide the user the ability to control
 *    which messages will be presented and to control their presentation.
 *  - A set of defined constants that denote the severity levels used by JEOD.
 *    - Failure (-1): Any negative severity level value indicates an
 *      irrecoverable error. The simulation will terminate immediately.
 *    - Error (0): Errors almost certainly invalidate the simulation output.
 *      All severity zero messages are printed; they cannot be disabled.
 *      The difference between failures and errors is that there is some
 *      recovery from an error that lets the simulation limp on and find the
 *      next error.
 *    - Warning (9): Warnings represent conditions that a model deems to be
 *      suspect but not necessarily dangerous. Warning messages are printed
 *      by default, but they can be disabled.
 *    - Notice (99): Notices represent condiitions that a model deems to be
 *      suspicious, but not necessarily in error. Notifications are not
 *      printed by default, but they can be enabled.
 *    - Debug (999): Debug messages typically demonstrate progress of some
 *      sort. Enabling them may well result in spew.
 *  - A public default constructor and destructor.
 *    The constructor ensures that the created object is indeed a singleton.
 *  -
 */
class MessageHandler {
JEOD_MAKE_SIM_INTERFACES(MessageHandler)

public:
   // NOTE: The copy constructor and assignment operator are deleted.

   // Static member functions

   // The static member functions fail, error, ..., debug are the intended
   // public interface for generating messages.

   // fail() generates a message with severity MessageHandler::Failure.
   // Failures terminate the simulation.
   static void fail (
      const char * file,
      unsigned int line,
      const char * msg_code,
      const char * format,
      ...);

   // error() generates a message with severity MessageHandler::Error.
   // The intent is to identify severe but non-fatal errors.
   static void error (
      const char * file,
      unsigned int line,
      const char * msg_code,
      const char * format,
      ...);

   // warn() generates a message with severity MessageHandler::Warning.
   // The intent is to identify conditions that might be suspect.
   static void warn (
      const char * file,
      unsigned int line,
      const char * msg_code,
      const char * format,
      ...);

   // inform() generates a message with severity MessageHandler::Notice.
   // The intent is to identify conditions that might be worth reporting.
   static void inform (
      const char * file,
      unsigned int line,
      const char * msg_code,
      const char * format,
      ...);

   // debug() generates a message with severity MessageHandler::Debug.
   // The intent is to trace transactions at a verbose level.
   static void debug (
      const char * file,
      unsigned int line,
      const char * msg_code,
      const char * format,
      ...);


   // send_message() generates a message with the supplied severity and prefix.
   // This is the generic mechanism for generating a message when the
   // fail(), error(), ..., debug() methods just don't do the trick.
   static void send_message (
      int severity,
      const char * prefix,
      const char * file,
      unsigned int line,
      const char * msg_code,
      const char * format,
      ...);

#ifndef SWIG
   // va_send_message() is the equivalent of send message, except that the
   // argument list defined by <stdarg.h> replaces the variadic arguments.
   static void va_send_message (
      int severity,
      const char * prefix,
      const char * file,
      unsigned int line,
      const char * msg_code,
      const char * format,
      va_list args);
#endif


   // The next set of public interfaces provide the ability to suppress certain
   // messages and to control the appearance of displayed messages.

   // Set the suppression level in the global message handler.
   // Note: Fatal and serious errors (severity <= 0) cannot be suppressed.
   static void set_suppression_level (
      unsigned int suppression_level);

   static unsigned int get_suppression_level ();


   // Add a message code to the set of messages that are to be suppressed
   // for any message with a positive severity level.
   // Note: Fatal and serious errors (severity <= 0) cannot be suppressed.
   static void add_suppressed_code (
      const char * msg_code);

   // Delete a message code to the set of messages that are to be suppressed.
   static void delete_suppressed_code (
      const char * msg_code);

   // Clear the set of messages that are to be suppressed.
   static void clear_suppressed_codes ();

   // Set the suppress_id flag in the global message handler.
   static void set_suppress_id (
      bool suppress_id);

   static bool get_suppress_id ();

   // Set the suppress_location flag in the global message handler.
   static void set_suppress_location (
      bool suppress_location);

   static bool get_suppress_location ();


   // Member functions

   // Default constructor
   // Note: The first instantiated MessageHandler becomes *the* global message
   // handler.
   MessageHandler ();

   // Destructor
   virtual ~MessageHandler();

   /**
    * Register the checkpointable contents of the handler with the
    * simulation interface.
    * The base MessageHandler has not such content.
    */
   virtual void register_contents () {}

   /**
    * Deregister the checkpointable contents of the handler with the
    * simulation interface.
    * The base MessageHandler has not such content.
    */
   virtual void deregister_contents () {}


   // Static member data

   /**
    * The severity value passed by the static public MessageHandler::fail
    * method to the derived class process_message method.
    * This is set to -1 in the implementation, representing a fatal error.
    *
    * A valid implementation of the process_message method must treat negative
    * severity levels as fatal; they must not return to the calling procedure.
    * In other words, failures eventually result in a call to exit.
    */
   const static int Failure; //!< trick_io(*o) trick_units(--)

   /**
    * The severity value passed by the static public MessageHandler::error
    * method to the derived class process_message method.
    * This is set to 0 in the implementation, representing the most severe
    * non-fatal error.
    *
    * Non-negative severity levels indicate non-fatal conditions for which
    * messages might nonetheless need to be generated, depending on the
    * value of the user-settable suppression_level.
    */
   const static int Error; //!< trick_io(*o) trick_units(--)

   /**
    * The severity value passed by the static public MessageHandler::warn
    * method to the derived class process_message method.
    * This is set to 9 in the implementation. The intent is to indicate a
    * condition that might indicate that results are suspect.
    */
   const static int Warning; //!< trick_io(*o) trick_units(--)

   /**
    * The severity value passed by the static public MessageHandler::inform
    * method to the derived class process_message method.
    * This is set to 99 in the implementation. The intent is to indicate a
    * non-error condition that might be worthy of a user notification.
    */
   const static int Notice; //!< trick_io(*o) trick_units(--)

   /**
    * The severity value passed by the static public MessageHandler::debug
    * method to the derived class process_message method.
    * This is set to 999 in the implementation. The intent is to summarize to
    * the user of some event that the user requested did indeed transpire.
    * Ideally, JEOD code, particularly initialization code, will be peppered
    * with calls to MessageHandler::debug.
    */
   const static int Debug; //!< trick_io(*o) trick_units(--)


 protected:

   // Static member functions

   // no_handler_error() terminates the simulation for lack of a handler.
   static void no_handler_error (void);

   // Member functions

   /**
    * Generate the message.
    * All of the send_message() methods relay the message to the message
    * handler in the form of a call to process_message().
    *
    * An instantiable derived MessageHandler class must supply this
    * function.
    *
    * @param severity  Severity level
    * @param prefix    Message prefix (e.g., Error)
    * @param file      Typically __FILE__
    * @param line      Typically __LINE__
    * @param msg_code  Message code
    * @param format    sprintf format
    * @param args      Arguments
    */
   virtual void process_message (
      int severity,
      const char * prefix,
      const char * file,
      unsigned int line,
      const char * msg_code,
      const char * format,
      va_list args)
   const = 0;

   /**
    * Add a message code to the set of messages that are to be suppressed.
    * The method add_suppressed_code relays the call to the message
    * handler as a call to process_add_suppressed_code.
    *
    * The default behavior is a no-op. Suppressing messages by the message
    * code is an optional capability.
    *
    * @param msg_code  Message code to be suppressed
    */
   virtual void process_add_suppressed_code (
      const char * msg_code JEOD_UNUSED)
   {}


   /**
    * Delete a message code from the set of suppressed message codes.
    * The method delete_suppressed_code relays the call to the message
    * handler as a call to process_delete_suppressed_code.
    *
    * As with process_add_suppressed_code, the default for this function is
    * a no-op; suppressed codes are an optional capability.
    *
    * @param msg_code  Message code to be suppressed
    */
   virtual void process_delete_suppressed_code (
      const char * msg_code JEOD_UNUSED)
   {}


   /**
    * Clear the set of suppressed message codes.
    * The method clear_suppressed_codes relays the call to the message
    * handler as a call to process_clear_suppressed_codes.
    *
    * As with process_add_suppressed_code, the default for this function is
    * a no-op; suppressed codes are an optional capability.
    */
   virtual void process_clear_suppressed_codes ()
   {}


   // Static member data

   /**
    * The MessageHandler instance that generates messages.
    * The static MessageHandler functions invoked by various models pass the
    * message on to this instance in the form of a call to process_message.
    */
   static MessageHandler * handler;  //!< trick_io(*o) trick_units(--)


   // Member data

   /**
    * All messages have an associated severity level, with increasingly positive
    * values indicating warnings of decreasing severity. Fatal errors have a
    * negative severity level. Messages whose severity exceeds the value of
    * the global message handler's suppression_level are suppressed.
    * Note that fatal errors and severe errors cannot be suppressed.
    *
    * Default value: MessageHandler::Warning (warnings and non-fatal errors).
    */
   unsigned int suppression_level; //!< trick_units(--)

   /**
    * This flag indicates whether the message ID is printed for unsuppressed
    * messages. The ID is not printed if this flag is set to true.
    * The message ID is always printed for fatal errors.
    *
    * Default value: false.
    */
   bool suppress_id; //!< trick_units(--)

   /**
    * This flag indicates whether the message source file and line number
    * printed for unsuppressed messages. The location is not printed if this
    * flag is set to true.
    * The message location is always printed for fatal errors.
    *
    * Default value: false.
    */
   bool suppress_location; //!< trick_units(--)


 private:
   // The copy constructor and assignment operator for this class are declared
   // private and are not implemented.

   /**
    * Not implemented.
    */
   MessageHandler (const MessageHandler &);

   /**
    * Not implemented.
    */
   MessageHandler & operator= (const MessageHandler &);
};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
