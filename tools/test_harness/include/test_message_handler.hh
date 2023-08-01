//=============================================================================
// Notices:
//
// Copyright © 2023 United States Government as represented by the Administrator
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
 * @addtogroup Tools
 * @{
 * @addtogroup TestHarness
 * @{
 *
 * @file tools/test_harness/include/test_message_handler.hh
 * Define the class TestMessageHandler, the message handler designed for use
 * in unit tests.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/test_message_handler.cc))

 

*******************************************************************************/


#ifndef JEOD_TEST_MESSAGE_HANDLER_HH
#define JEOD_TEST_MESSAGE_HANDLER_HH

// System includes
#include <cstdarg>
#include <set>
#include <string>

// JEOD includes
#include "utils/message/include/message_handler.hh"


/**
 * \def JEOD_SEND_HS_LEVEL
 * Used to silence send_hs in tests where errors are expected.
 * If not defined, this is set to zero to let send_hs messages be reported.
 */
#ifndef JEOD_SEND_HS_LEVEL
#define JEOD_SEND_HS_LEVEL 0
#endif


// Some lazy unit test developers don't want to be bothered with messages.
extern "C" {

   // Variable arguments debug print function)
   void print_debug (
      unsigned int level,
      FILE * file,
      const char * format,
      ...);
}


//! Namespace jeod
namespace jeod {

class TestSimInterface;


/**
 * The MessageHandler class designed for use in unit tests.
 */
class TestMessageHandler: public MessageHandler {
JEOD_MAKE_SIM_INTERFACES(TestMessageHandler)

public:
   // Member functions

   // Non-default constructor.
   TestMessageHandler (TestSimInterface &);

   // Destructor.
   ~TestMessageHandler ();

   // Add a code to the allowed list.
   void add_allowed_code (const std::string &);

   // Add a code to the verboten list.
   void add_verboten_code (const std::string &);

   // Add a code to the suppressed list.
   virtual void process_add_suppressed_code (const char*);

   // Delete a code from the suppressed list.
   virtual void process_delete_suppressed_code (const char*);

   // Set the failure threshold.
   void set_failure_threshold (int level);

protected:

   // process_message() handles all messages.
   virtual void process_message (
      int severity,
      const char * prefix,
      const char * file,
      unsigned int line,
      const char * msg_code,
      const char * format,
      va_list args)
   const;

private:

   // Types

   /**
    * Maps message handler codes to an integer.
    */
   typedef std::set <std::string> CodeList;


   // Member data
   /**
    * process_message() calls sim_interface.set_failed() when a msg_code that is
    * not in the allowed_codes list (exact match) but is in the verboten_codes
    * list (initial match).
    */
   TestSimInterface & sim_interface; //!< trick_io(**) trick_units(--)

   /**
    * The list of message codes (e.g. utils/memory/debug) that partially
    * match one of the verboten_codes but are nonetheless acceptable.
    * Items are added to this list via TestMessageHandler::add_allowed_code.
    */
   CodeList allowed_codes; //!< trick_io(**) trick_units(--)

   /**
    * The list of message code prefixes that indicate a test has failed.
    * Items are added to this list via TestMessageHandler::add_verboten_code.
    *
    * An incoming message code to TestMessageHandler::process_message()
    * that matches (prefix match) one of the items in the verboten_codes
    * list and is not in the allowed_codes list will result in the simulation
    * interface being informed the test has failed.
    * For example, to make all message codes from the memory model except for
    * debug messages indicative of a failed test, add "/utils/memory" to the
    * verboten_codes list and "/utils/memory/debug" to the allowed_codes.
    */
   CodeList verboten_codes; //!< trick_io(**) trick_units(--)

   /**
    * The list of message code that are to be suppressed.
    */
   CodeList suppressed_codes; //!< trick_io(**) trick_units(--)

   /**
    * The message severity level that indicates a failed test.
    * The test is marked failed if a message with a verboten code and a
    * severity level at or below this threshold is generated.
    */
   int failure_threshold; //!< trick_units(--)


   // Deleted content

   // The copy constructor and assignment operator for this class are declared
   // private and are not implemented.

   /**
    * Not implemented.
    */
   TestMessageHandler (const TestMessageHandler &);

   /**
    * Not implemented.
    */
   TestMessageHandler & operator= (const TestMessageHandler &);
};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 */
