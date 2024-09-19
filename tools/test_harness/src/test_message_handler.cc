/**
 * @addtogroup Tools
 * @{
 * @addtogroup TestHarness
 * @{
 *
 * @file tools/test_harness/src/test_message_handler.cc
 * Implement functions and methods declared in test_sim_interface.hh.
 */

/*******************************************************************************

Purpose:
  ()



*******************************************************************************/

// System includes
#include <cstdlib>
#include <sstream>
#include <string>

// Model includes
#include "../include/test_message_handler.hh"
#include "../include/test_sim_interface.hh"

//! Namespace jeod
namespace jeod
{

extern "C"
{
#ifdef JEOD_MEMORY_DEBUG
    static const unsigned int Trick_stubs_debug_level = JEOD_MEMORY_DEBUG;
#else
    static const unsigned int Trick_stubs_debug_level = 0;
#endif

    /**
     * Variable arguments debug print function.
     * \param[in] level Print level
     * \param[in,out] file File
     * \param[in] format Format
     * \param[in] ... Items to be printed
     */
    void print_debug(unsigned int level, FILE * file, const char * format, ...)
    {
        va_list args;

        va_start(args, format);
        if(level <= Trick_stubs_debug_level)
        {
            vfprintf(file, format, args);
        }
        va_end(args);
    }

    /**
     * Variable arguments debug print function.
     * \param[in,out] file File
     * \param[in] level Print level
     * \param[in] format Format
     * \param[in] ... Items to be printed
     */
    static void print_nonfatal_message(FILE * file, unsigned int level, const char * format, ...)
    {
        va_list args;

        va_start(args, format);
        if(level <= Trick_stubs_debug_level)
        {
            vfprintf(file, format, args);
        }
        va_end(args);
    }

    /**
     * Emulates Trick's exec_terminate function.
     * \param[in,out] file File
     * \param[in] error Error message
     * \param[in] exit_status Exit status
     */
    static void print_message_and_exit(const char * file, const char * error, int exit_status)
    {
        fprintf(stdout, "TERMINATED IN %s:\n%s\n", file, error);
        exit(exit_status);
    }
}

/**
 * Non-default constructor.
 * \param[in,out] interface The simulation interface
 */
TestMessageHandler::TestMessageHandler(TestSimInterface & interface)
    : MessageHandler(),
      sim_interface(interface),
      allowed_codes(),
      verboten_codes(),
      suppressed_codes(),
      failure_threshold(MessageHandler::Warning)
{
}

/**
 * Destruct a TestMessageHandler.
 */
TestMessageHandler::~TestMessageHandler()
{
    allowed_codes.clear();
    verboten_codes.clear();
}

/**
 * Add a code to the allowed codes list;
 * see TestSimInterface::add_allowed_code for a detailed description.
 * \param[in] code The message code to be marked as allowed
 */
void TestMessageHandler::add_allowed_code(const std::string & code)
{
    allowed_codes.insert(code);
}

/**
 * Add a code to the verboten codes list;
 * see TestSimInterface::add_verboten_code for a detailed description.
 * \param[in] code The message code to be marked as verboten
 */
void TestMessageHandler::add_verboten_code(const std::string & code)
{
    verboten_codes.insert(code);
}

/**
 * Add a code to the suppressed codes list;
 * see MessageHandler::add_suppressed_code for a detailed description.
 * \param[in] msg_code The message code to be suppressed
 */
void TestMessageHandler::process_add_suppressed_code(const char * msg_code)
{
    suppressed_codes.insert(msg_code);
}

/**
 * Delete a code from the suppressed codes list.
 * \param[in] msg_code The message code to be unsuppressed
 */
void TestMessageHandler::process_delete_suppressed_code(const char * msg_code)
{
    suppressed_codes.erase(msg_code);
}

/**
 * Set the failure threshold used in determining whether a non-fatal error
 * needs to undergo accepted/verboten code examination.
 * \param[in] threshold New threshold
 */
void TestMessageHandler::set_failure_threshold(int threshold)
{
    failure_threshold = threshold;
}

/**
 * Handle a message.
 * All calls to the message-generating MessageHandler methods eventually result
 * in a call to this method. This method thus intercepts all message handler
 * calls. The message processing in this method is limited to test harness
 * issues; the bulk of the processing is done by the TrickMessageHandler.
 * \param[in] severity Severity level
 * \param[in] prefix Message prefix (e.g., Error)
 * \param[in] file Typically __FILE__
 * \param[in] line Typically __LINE__
 * \param[in] msg_code Message code
 * \param[in] format sprintf format
 * \param[in] args Arguments
 */
void TestMessageHandler::process_message(int severity,
                                         const char * prefix,
                                         const char * file,
                                         unsigned int line,
                                         const char * msg_code,
                                         const char * format,
                                         va_list args) const
{
    bool throw_failure = false;
    std::ostringstream why;

    char buffer[4096];
    char where[256];

    // *Safely* generate the message proper and location info.
    // C++ I/O is not safe at this point (it uses new).
    std::vsnprintf(buffer, sizeof(buffer), format, args);
    std::snprintf(where, sizeof(where), "%s line %d", file, line);

    // Call was to MessageHandler::fail:
    // Set the status to be returned from the program,
    // zero (success) for an allowed code, one (failure) for other codes.
    if(severity < 0)
    {
        int exit_status;

        if(allowed_codes.find(msg_code) != allowed_codes.end())
        {
            exit_status = 0;
        }
        else
        {
            exit_status = 1;
        }

        // Generate the termination message ...
        why << prefix << " " << msg_code << ":\n" << buffer;

        // ... and terminate the simulation.
        print_message_and_exit(where, why.str().c_str(), exit_status);

        // Not reached.
        return;
    }

    // Call was to some sufficiently serious but non-fatal MessageHandler method:
    // Process the message per the allowed and verboten codes.
    if(severity <= failure_threshold)
    {
        // Exact match with one of the allowed codes:
        // Note that we intercepted such a call.
        if(allowed_codes.find(msg_code) != allowed_codes.end())
        {
            sim_interface.bump_allowed_count();
        }

        // Not an allowed code:
        // Is it verboten? If denote that a failure is to be thrown.
        // The failure isn't thrown here so that the message can be printed.
        else
        {
            for(CodeList::const_iterator iter = verboten_codes.begin(); iter != verboten_codes.end(); ++iter)
            {
                const std::string & key = *iter;
                size_t key_len = key.length();
                int comp = key.compare(0, key_len, msg_code, key_len);
                if(comp == 0)
                {
                    throw_failure = true;
                    why << "The preceding error with code '" << msg_code << "'\n";
                    why << "matches the verboten message code '" << key << "'\n";
                    why << "Test marked as failed.\n";
                    break;
                }
                else if(comp > 0)
                {
                    break;
                }
            }
        }
    }

    // Handle non-fatal message per standard message handler rules.
    // Low-level messages (high severity number) can be suppressed via the
    // user-controllable suppression_level.
    // Generate a message only if the message is not suppressed.
    if(throw_failure || (severity == 0) ||
       ((suppressed_codes.empty() || (suppressed_codes.find(msg_code) == suppressed_codes.end())) &&
        (severity <= static_cast<int>(suppression_level))))
    {
        // Message is to be generated. The user has control over the appearance
        // of the level via the suppress_id and suppress_location flags.
        // Handle all four combinations.

        // Suppress both: Just print the message.
        if(suppress_id && suppress_location)
        {
            print_nonfatal_message(stderr, JEOD_SEND_HS_LEVEL, "\n%s\n", buffer);
        }

        // Suppress ID but not location: Print location and message.
        else if(suppress_id && (!suppress_location))
        {
            print_nonfatal_message(stderr, JEOD_SEND_HS_LEVEL, "\n%s\n%s\n", where, buffer);
        }

        // Suppress location but not ID: Print ID and message.
        else if((!suppress_id) && suppress_location)
        {
            print_nonfatal_message(stderr, JEOD_SEND_HS_LEVEL, "\n%s %s:\n%s\n", prefix, msg_code, buffer);
        }

        // Suppress neither: Generate a full report.
        else
        {
            print_nonfatal_message(stderr, JEOD_SEND_HS_LEVEL, "\n%s %s at %s:\n%s\n", prefix, msg_code, where, buffer);
        }
    }

    // Fail with non-zero exit status if this is a verboten code.
    if(throw_failure)
    {
        std::ostringstream at;
        at << __FILE__ << ":" << __LINE__ + 1;
        print_message_and_exit(at.str().c_str(), why.str().c_str(), 2);
    }
}

} // namespace jeod

/**
 * @}
 * @}
 */
