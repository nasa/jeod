/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup SimInterface
 * @{
 *
 * @file models/utils/sim_interface/src/trick_message_handler.cc
 * Define member functions for the class TrickMessageHandler.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((trick_message_handler.cc)
   (utils/message/src/message_handler.cc))



*******************************************************************************/

#ifdef TRICK_VER

// System includes
#include <cstdarg>
#include <cstdio>

// Trick includes
#include "sim_services/Executive/include/exec_proto.h"
#include "sim_services/Message/include/message_proto.h"

// not defined in Trick 10
#ifndef MAX_MSG_SIZE
#define MAX_MSG_SIZE 4096
#endif

// JEOD includes
#include "utils/memory/include/jeod_alloc.hh"

// Model includes
#include "../include/trick_message_handler.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Register the TrickMessageHandler's checkpointable contents.
 */
void TrickMessageHandler::register_contents()
{
    JEOD_REGISTER_CLASS(TrickMessageHandler);
    SuppressedCodeMessageHandler::register_contents();
}

/**
 * Handle a message.
 * All calls to the message-generating MessageHandler methods eventually result
 * in a call to thisTrickMessageHandler::process_message method. This method
 * uses the Trick function exec_terminate to process fatal errors.
 * The Trick function send_hs is used for all non-fatal messages, but only
 * if the message severity is at or below the message suppression level.
 * \param[in] severity Severity level
 * \param[in] prefix Message prefix (e.g., Error)
 * \param[in] file Typically __FILE__
 * \param[in] line Typically __LINE__
 * \param[in] msg_code Message code
 * \param[in] format sprintf format
 * \param[in] args Arguments
 */
void TrickMessageHandler::process_message(int severity,
                                          const char * prefix,
                                          const char * file,
                                          unsigned int line,
                                          const char * msg_code,
                                          const char * format,
                                          va_list args) const
{
    int buffer_length = MAX_MSG_SIZE - strlen(prefix) - strlen(msg_code);
    char buffer[buffer_length];

    // *Safely* generate the message proper.
    std::vsnprintf(buffer, sizeof(buffer), format, args); // flawfinder: ignore

    // Negative severity = error.
    // All errors are terminal. Handle with exec_terminate().
    if(severity < 0)
    {
        char message[MAX_MSG_SIZE];

        // Generate the termination message ...
        std::snprintf(message, sizeof(message), "%s %s:\n%s\n", prefix, msg_code, buffer);

        // ... and terminate the simulation.
        exec_terminate_with_return(1, file, line, message);
    }

    // Messages with non-negative severity are not fatal.
    // Low-level messages (high severity number) can be suppressed via the
    // user-controllable suppression_level or by the suppressed code mechanism.
    // Generate a message only if is not suppressed.
    else if((severity == 0) || ((suppressed_codes.find(msg_code) == suppressed_codes.end()) &&
                                (static_cast<unsigned int>(severity) <= suppression_level)))
    {
        // *Safely* generate the location information.
        char where[256];
        std::snprintf(where, sizeof(where), "%s line %u", file, line);

        // Message is to be generated. The user has control over the appearance
        // of the level via the suppress_id and suppress_location flags.
        // Handle all four combinations.

        // Suppress both: Just print the message.
        if(suppress_id && suppress_location)
        {
            send_hs(stderr, const_cast<char *>("\n%s\n"), buffer);
        }

        // Suppress ID but not location: Print location and message.
        else if(suppress_id && (!suppress_location))
        {
            send_hs(stderr, const_cast<char *>("\n%s\n%s\n"), where, buffer);
        }

        // Suppress location but not ID: Print ID and message.
        else if((!suppress_id) && suppress_location)
        {
            send_hs(stderr, const_cast<char *>("\n%s %s:\n%s\n"), prefix, msg_code, buffer);
        }

        // Suppress neither: Generate a full report.
        else
        {
            send_hs(stderr, const_cast<char *>("\n%s %s at %s:\n%s\n"), prefix, msg_code, where, buffer);
        }
    }

    // Empty else (message is suppressed).
    else
    {
    }
}

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
