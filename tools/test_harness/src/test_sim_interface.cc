/**
 * @addtogroup Tools
 * @{
 * @addtogroup TestHarness
 * @{
 *
 * @file tools/test_harness/src/test_sim_interface.cc
 * Implement functions and methods declared in test_sim_interface.hh.
 */

/*******************************************************************************

Purpose:
  ()


*******************************************************************************/

// System includes
#include <sstream>
#include <string>

// Model includes
#include "../include/test_sim_interface.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Define storage for TestSimInterface::the_lock_enabled.
 */
bool TestSimInterface::the_lock_enabled = false;

/**
 * Define storage for TestSimInterface::the_printf_mutex.
 */
pthread_mutex_t * TestSimInterface::the_printf_mutex = nullptr;

/**
 * Construct a TestSimInterface object.
 */
TestSimInterface::TestSimInterface()
    : message_handler(*this)
{
    pthread_mutex_init(&data_mutex, nullptr);
    pthread_mutex_init(&printf_mutex, nullptr);
    the_printf_mutex = &printf_mutex;
    memory_manager = new JeodMemoryManager(memory_interface);
}

/**
 * Destruct a TestSimInterface object.
 */
TestSimInterface::~TestSimInterface()
{
    if(memory_manager != nullptr)
    {
        shutdown();
    }
    pthread_mutex_destroy(&printf_mutex);
    pthread_mutex_destroy(&data_mutex);
    the_printf_mutex = nullptr;
}

/**
 * Enable printf locking.
 */
void TestSimInterface::enable_lock()
{
    the_lock_enabled = true;
}

/**
 * Disable printf locking.
 */
void TestSimInterface::disable_lock()
{
    the_lock_enabled = false;
}

/**
 * Lock so we can do a printf without getting jumbled text.
 */
void TestSimInterface::lock_printf()
{
    if(the_lock_enabled && (the_printf_mutex != nullptr))
    {
        pthread_mutex_lock(the_printf_mutex);
    }
}

/**
 * Unlock so other threads can print, too.
 */
void TestSimInterface::unlock_printf()
{
    if(the_lock_enabled && (the_printf_mutex != nullptr))
    {
        pthread_mutex_unlock(the_printf_mutex);
    }
}

/**
 * Shutdown.
 */
void TestSimInterface::shutdown()
{
    if(memory_manager != nullptr)
    {
        delete memory_manager;
        memory_manager = nullptr;
    }
}

/**
 * Denote that error messages whose message code exactly matches the supplied
 * code is a result of an condition being tested in some unit test. The
 * registered 'allowed codes' are used two cases: fatal errors and non-fatal
 * but severe errors, the latter defined in terms of the fatal threshold.
 *
 * The default exit status for program termination resulting from a call to
 * MessageHandle::fail is one. When the message code matches one of the
 * allowed codes the exit status will be zero. Since the JEOD unit test driver
 * only looks at the program termination status, the allowed code mechanism
 * provides a means for failure into success. What this means to you as a
 * unit test developer is that you can now test the error tests in the mainline
 * code. It also means you need to be careful in how you use this capability;
 * maybe that successful failure is not as successful as you thought.
 *
 * Two things happen when a non-fatal but sufficiently serious MessageHandler
 * call is made whose the message code matches one of the allowed codes:
 * - The retrievable allowed_error_count is bumped. This provides a means
 *   for detecting in your unit test code that the an error condition was
 *   properly detected.
 * - The test for a verboten code is bypassed. The allowed codes trump the
 *   verboten codes in case of conflict.
 * \param[in] code The message code to be marked as allowed
 */
void TestSimInterface::add_allowed_code(const std::string & code)
{
    message_handler.add_allowed_code(code);
}

/**
 * Denote that an error message whose message code partially matches the
 * supplied code is bad news even if the error is not a fatal error.
 * The test is of the first N characters of the incoming message code versus
 * the verboten code, where N is the string length of the incoming code.
 * The empty string as a verboten code will match all messages.
 *
 * The test will be terminated with non-zero exit status when an incoming
 * non-fatal message
 *  - Has a severity level less than or equal to the failure_threshold,
 *  - Has a message code that does not match (exact match) any of the
 *    allowed codes, and
 *  - Has a message code that does match (initial match) one of the
 *    verboten codes.
 * \param[in] code The message code to be marked as verboten
 */
void TestSimInterface::add_verboten_code(const std::string & code)
{
    message_handler.add_verboten_code(code);
}

/**
 * Set the failure threshold used in determining whether a non-fatal error
 * needs to undergo accepted/verboten code examination.
 * \param[in] threshold New failure threshold value
 */
void TestSimInterface::set_failure_threshold(int threshold)
{
    message_handler.set_failure_threshold(threshold);
}

/**
 * Get the allowed_error_count.
 * @return Allowed error count
 */
unsigned int TestSimInterface::get_allowed_count()
{
    unsigned int count;
    pthread_mutex_lock(&data_mutex);
    count = allowed_error_count;
    pthread_mutex_unlock(&data_mutex);
    return count;
}

/**
 * Bump the allowed_error_count.
 */
void TestSimInterface::bump_allowed_count()
{
    pthread_mutex_lock(&data_mutex);
    ++allowed_error_count;
    pthread_mutex_unlock(&data_mutex);
}

} // namespace jeod

/**
 * @}
 * @}
 */
