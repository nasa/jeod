/**
 * @addtogroup Tools
 * @{
 * @addtogroup TestHarness
 * @{
 *
 * @file tools/test_harness/src/test_harness_messages.cc
 * Implement the class TestHarnessMessages.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((test_harness_messages.cc))



*******************************************************************************/

// System includes

// JEOD includes
#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/test_harness_messages.hh"

#define MAKE_TEST_HARNESS_MESSAGE_CODE(id) JEOD_MAKE_MESSAGE_CODE(TestHarnessMessages, "tools/test_harness/", id)

//! Namespace jeod
namespace jeod
{

// Define TestHarnessMessages static member data
MAKE_TEST_HARNESS_MESSAGE_CODE(inconsistent_setup);
MAKE_TEST_HARNESS_MESSAGE_CODE(invalid_option);
MAKE_TEST_HARNESS_MESSAGE_CODE(test_failed);
MAKE_TEST_HARNESS_MESSAGE_CODE(test_message);
MAKE_TEST_HARNESS_MESSAGE_CODE(debug);

} // namespace jeod

#undef MAKE_TEST_HARNESS_MESSAGE_CODE

/**
 * @}
 * @}
 */
