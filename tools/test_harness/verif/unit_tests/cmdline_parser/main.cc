/*******************************************************************************

Purpose:
  (Unit test for the CmdlineParser class. This test
    - Exercises the basic functionality of the model.
    - Demonstrates one way to to make a makefile run target communicate with the
      test article.)

 
*******************************************************************************/

// System includes
#include <cmath>
#include <iostream>

// JEOD includes
#include "utils/message/include/message_handler.hh"

// Test harness includes
#include "test_harness/include/cmdline_parser.hh"
#include "test_harness/include/test_harness_messages.hh"
#include "test_harness/include/test_sim_interface.hh"

using namespace jeod;

int
main (
   int argc,
   char ** argv)
{
   // Test harness stuff
   // Note well: The sim interface needs to be declared first.
   TestSimInterface sim_interface;
   CmdlineParser cmdline_parser;

   // Local variables.
   // Note that the initial values for the command line option variables
   // differ from the defaults.
   bool simple_switch = true;   // Associated with -switch
   bool toggle = false;         // Associated with -toggle
   int counter = 999;           // Associated with -counter
   const char * string = NULL;  // Associated with -string
   int integer = 999;           // Associated with -int
   double number = 999.9;       // Associated with -double
   int status;


   // Load the command line processor with the options to be parsed.
   cmdline_parser.add_switch  ("switch",        &simple_switch);
   cmdline_parser.add_toggle  ("toggle",  true, &toggle);
   cmdline_parser.add_counter ("counter",       &counter);
   cmdline_parser.add_string  ("string",   "",  &string);
   cmdline_parser.add_int     ("int",      0,   &integer);
   cmdline_parser.add_double  ("double",   0,   &number);

   // Parse the command line. It should return zero.
   status = cmdline_parser.parse (argc, argv);

   if (status != 0) {
      MessageHandler::fail (
         __FILE__, __LINE__, TestHarnessMessages::inconsistent_setup,
        "Command line parser returned non-zero parse status.");
   }


   // The command line parser eats the parsed command line options.
   // There should be two arguments left, the program name and the test number.
   // The last argument, the test number, should be a single character string
   // that contains ASCII '0' to '9'.
   if ((argc != 2) ||
       (std::strlen (argv[1]) != 1) ||
       (argv[1][0] < '0') ||
       (argv[1][0] > '9')) {
      MessageHandler::fail (
         __FILE__, __LINE__, TestHarnessMessages::inconsistent_setup,
        "Inconsistent test setup");
   }

   // Print the values retrieved by the command line parser.
   std::printf ("Test #%s retrieved command line option values:\n"
                "  switch   = %s\n"
                "  toggle   = %s\n"
                "  counter  = %d\n"
                "  string   = \"%s\"\n"
                "  integer  = %d\n"
                "  number   = %.15f\n",
                argv[1],
                (simple_switch ? "true" : "false"),
                (toggle ? "true" : "false"),
                counter, string, integer, number);


   // Determine if the received values match expectations.
   // Abort (MessageHandler::fail) if they don't.
   switch (argv[1][0]) {

	// Test 0: ./main.exe 0
   case '0':
      if ((simple_switch) ||
          (! toggle) ||
          (counter != 0) ||
          (string[0] != '\0') ||
          (integer != 0) ||
          (number != 0)) {
         MessageHandler::fail (
            __FILE__, __LINE__, TestHarnessMessages::invalid_option,
           "Unexpected results for test #%s", argv[1]);
      }
      break;

	// Test 1: ./main.exe -switch 1
   case '1':
      if ((! simple_switch) ||
          (! toggle) ||
          (counter != 0) ||
          (string[0] != '\0') ||
          (integer != 0) ||
          (number != 0)) {
         MessageHandler::fail (
            __FILE__, __LINE__, TestHarnessMessages::invalid_option,
           "Unexpected results for test #%s", argv[1]);
      }
      break;

   // Test 2: ./main.exe -toggle 2
   case '2':
      if ((simple_switch) ||
          (! toggle) ||
          (counter != 0) ||
          (string[0] != '\0') ||
          (integer != 0) ||
          (number != 0)) {
         MessageHandler::fail (
            __FILE__, __LINE__, TestHarnessMessages::invalid_option,
           "Unexpected results for test #%s", argv[1]);
      }
      break;

   // Test 3: ./main.exe -notoggle 3
   case '3':
      if ((simple_switch) ||
          (toggle) ||
          (counter != 0) ||
          (string[0] != '\0') ||
          (integer != 0) ||
          (number != 0)) {
         MessageHandler::fail (
            __FILE__, __LINE__, TestHarnessMessages::invalid_option,
           "Unexpected results for test #%s", argv[1]);
      }
      break;

   // Test 4: ./main.exe -counter 4
   case '4':
      if ((simple_switch) ||
          (! toggle) ||
          (counter != 1) ||
          (string[0] != '\0') ||
          (integer != 0) ||
          (number != 0)) {
         MessageHandler::fail (
            __FILE__, __LINE__, TestHarnessMessages::invalid_option,
           "Unexpected results for test #%s", argv[1]);
      }
      break;

   // Test 5: ./main.exe -counter -counter 5
   case '5':
      if ((simple_switch) ||
          (! toggle) ||
          (counter != 2) ||
          (string[0] != '\0') ||
          (integer != 0) ||
          (number != 0)) {
         MessageHandler::fail (
            __FILE__, __LINE__, TestHarnessMessages::invalid_option,
           "Unexpected results for test #%s", argv[1]);
      }
      break;

   // Test 6: ./main.exe -string hello 6
   case '6':
      if ((simple_switch) ||
          (! toggle) ||
          (counter != 0) ||
          (std::strcmp (string, "hello") != 0) ||
          (integer != 0) ||
          (number != 0)) {
         MessageHandler::fail (
            __FILE__, __LINE__, TestHarnessMessages::invalid_option,
           "Unexpected results for test #%s", argv[1]);
      }
      break;

   // Test 7: ./main.exe -int 42 7
   case '7':
      if ((simple_switch) ||
          (! toggle) ||
          (counter != 0) ||
          (string[0] != '\0') ||
          (integer != 42) ||
          (number != 0)) {
         MessageHandler::fail (
            __FILE__, __LINE__, TestHarnessMessages::invalid_option,
           "Unexpected results for test #%s", argv[1]);
      }
      break;

   // Test 8: ./main.exe -number 3.141592653589793 8
   case '8':
      if ((simple_switch) ||
          (! toggle) ||
          (counter != 0) ||
          (string[0] != '\0') ||
          (integer != 0) ||
          (std::abs (number-M_PI) > 1e-15)) {
         MessageHandler::fail (
            __FILE__, __LINE__, TestHarnessMessages::invalid_option,
           "Unexpected results for test #%s", argv[1]);
      }
      break;

   // Test 9: All options set
   case '9':
      if ((! simple_switch) ||
          (toggle) ||
          (counter != 2) ||
          (std::strcmp (string, "hello") != 0) ||
          (integer != 42) ||
          (std::abs (number-M_PI) > 1e-15)) {
         MessageHandler::fail (
            __FILE__, __LINE__, TestHarnessMessages::invalid_option,
           "Unexpected results for test #%s", argv[1]);
      }
      break;

   // The above should handle all of the cases. Just in case it doesn't, ...
   default:
      MessageHandler::fail (
         __FILE__, __LINE__, TestHarnessMessages::inconsistent_setup,
        "This can't happen.");
      break;
   }

   // Getting past all of the above hurdles means this test passed.
   std::printf ("Test #%s passes.\n\n", argv[1]);
   return 0;
}
