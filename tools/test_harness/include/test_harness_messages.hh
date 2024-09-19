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
 * @file tools/test_harness/include/test_harness_messages.hh
 * Define the class TestHarnessMessages, the class that specifies the message
 * IDs used in the JEOD test harness model.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/ephem_messages.cc))



*******************************************************************************/

#ifndef JEOD_TEST_HARNESS_MESSAGES_HH
#define JEOD_TEST_HARNESS_MESSAGES_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Specifies the message IDs used in the test harness model.
 */
class TestHarnessMessages
{
    JEOD_MAKE_SIM_INTERFACES(jeod, TestHarnessMessages)

    // Static member data
public:
    /**
     * Issued when the the programmer has done something wrong.
     */
    static const char * inconsistent_setup; //!< trick_units(--)

    /**
     * Issued when a something is wrong with the command line.
     */
    static const char * invalid_option; //!< trick_units(--)

    /**
     * Issued when a test has failed.
     */
    static const char * test_failed; //!< trick_units(--)

    /**
     * Used for testing purposes.
     */
    static const char * test_message; //!< trick_units(--)

    /**
     * Used for testing purposes.
     */
    static const char * debug; //!< trick_units(--)

    TestHarnessMessages() = delete;
    TestHarnessMessages(const TestHarnessMessages &) = delete;
    TestHarnessMessages & operator=(const TestHarnessMessages &) = delete;
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 */
