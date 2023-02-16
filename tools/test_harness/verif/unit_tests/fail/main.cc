/******************************************************************************
 Purpose: (Define a simple unit test that intentionally fails.)
 *****************************************************************************/


// JEOD includes
#include "utils/message/include/message_handler.hh"

// Test harness includes
#include "test_harness/include/test_sim_interface.hh"
#include "test_harness/include/test_harness_messages.hh"

using namespace jeod;

int
main (
   int argc,
   char ** argv)
{

   // Test harness stuff
   TestSimInterface sim_interface;

   MessageHandler::set_suppression_level (MessageHandler::Notice);


   if ((argc != 2) || (argv[1][0] == '\0') || (argv[1][1] != '\0')) {
      MessageHandler::fail (
         __FILE__, __LINE__, TestHarnessMessages::inconsistent_setup,
        "Inconsistent test setup");
   }

   switch (argv[1][0]) {
   case '1':
      MessageHandler::inform (
         __FILE__, __LINE__, TestHarnessMessages::debug,
        "Marking '%s' as an allowed code.",
         TestHarnessMessages::test_message);

      sim_interface.add_allowed_code (TestHarnessMessages::test_message);

      MessageHandler::fail (
         __FILE__, __LINE__, TestHarnessMessages::test_message,
        "Testing the ability to mark a failure as successful.\n"
        "Test #1 should have an exit code of 0, making the failure a success.");
      return 42;
      break;

   case '2':
      MessageHandler::inform (
         __FILE__, __LINE__, TestHarnessMessages::debug,
        "Marking '%s' as an allowed code.",
         TestHarnessMessages::test_message);

      sim_interface.add_allowed_code (TestHarnessMessages::test_message);

      MessageHandler::inform (
         __FILE__, __LINE__, TestHarnessMessages::test_message,
        "Notices whose code match an allowed code should not count.");

      if (sim_interface.get_allowed_count() != 0) {
         MessageHandler::fail (
            __FILE__, __LINE__, TestHarnessMessages::test_failed,
           "Didn't work! Intercepted message count = %d",
           sim_interface.get_allowed_count());
         return 42;
      }
      else {
         MessageHandler::inform (
            __FILE__, __LINE__, TestHarnessMessages::debug,
           "That worked.");
      }

      MessageHandler::error (
         __FILE__, __LINE__, TestHarnessMessages::test_message,
        "Testing the ability to detect non-fatal but allowed errors.\n",
        "This should bump the count of allowed severe messages.");

      if (sim_interface.get_allowed_count() != 1) {
         MessageHandler::fail (
            __FILE__, __LINE__, TestHarnessMessages::test_failed,
           "Didn't work! Intercepted message count = %d",
           sim_interface.get_allowed_count());
         return 42;
      }
      else {
         MessageHandler::inform (
            __FILE__, __LINE__, TestHarnessMessages::debug,
           "That worked.");
      }

      MessageHandler::warn (
         __FILE__, __LINE__, TestHarnessMessages::test_message,
        "Warnings should also be intercepted.");

      if (sim_interface.get_allowed_count() != 2) {
         MessageHandler::fail (
            __FILE__, __LINE__, TestHarnessMessages::test_failed,
           "Didn't work! Intercepted message count = %d",
           sim_interface.get_allowed_count());
         return 42;
      }
      else {
         MessageHandler::inform (
            __FILE__, __LINE__, TestHarnessMessages::debug,
           "That worked.");
      }

      MessageHandler::warn (
         __FILE__, __LINE__, TestHarnessMessages::debug,
        "Warnings with a code that doesn't match should not count.");

      if (sim_interface.get_allowed_count() != 2) {
         MessageHandler::fail (
            __FILE__, __LINE__, TestHarnessMessages::test_failed,
           "Didn't work! Intercepted message count = %d",
           sim_interface.get_allowed_count());
         return 42;
      }
      else {
         MessageHandler::inform (
            __FILE__, __LINE__, TestHarnessMessages::debug,
           "That worked. Test passes.");
         return 0;
      }
      break;


   case '3':
      MessageHandler::inform (
         __FILE__, __LINE__, TestHarnessMessages::debug,
        "Marking '%s' as an allowed code.",
         TestHarnessMessages::inconsistent_setup);

      sim_interface.add_allowed_code (TestHarnessMessages::inconsistent_setup);

      MessageHandler::fail (
         __FILE__, __LINE__, TestHarnessMessages::test_message,
        "Testing that non-allowed fatal errors fail with non-zero status.\n"
        "This should register as a failed run.\n"
        "Use make -k to see even more failures that should be failures.");
      return 42;
      break;


   case '4':
      MessageHandler::inform (
         __FILE__, __LINE__, TestHarnessMessages::debug,
        "Marking '' as a verboten code.");

      sim_interface.add_verboten_code ("");

      MessageHandler::warn (
         __FILE__, __LINE__, TestHarnessMessages::test_message,
        "Testing whether verboten messages are caught.\n"
        "This test passes if the test harness marks the test as failed.\n"
        "Use make -k to see even more failures that should be failures.\n");

      MessageHandler::fail (
         __FILE__, __LINE__, TestHarnessMessages::test_failed,
        "Didn't work! This test truly did fail.");
      return 1;
      break;

   case '5':
      MessageHandler::inform (
         __FILE__, __LINE__, TestHarnessMessages::debug,
        "Marking 'tools/test_harness/test' as a verboten code.");

      sim_interface.add_verboten_code ("tools/test_harness/test");

      MessageHandler::warn (
         __FILE__, __LINE__, TestHarnessMessages::debug,
        "Testing whether verboten messages are caught.\n"
        "This message should not be caught.");

      MessageHandler::warn (
         __FILE__, __LINE__, TestHarnessMessages::test_message,
        "Testing whether verboten messages are caught.\n"
        "This message should be caught.\n"
        "This test passes if the test harness marks the test as failed.\n"
        "Use make -k to see even more failures that should be failures.\n");

      MessageHandler::fail (
         __FILE__, __LINE__, TestHarnessMessages::test_failed,
        "Didn't work! This test truly did fail.");
      return 1;
      break;
   }

   // Fortytwo is not the answer in this case.
   // This statement should be unreachable.
   return 42;
}
