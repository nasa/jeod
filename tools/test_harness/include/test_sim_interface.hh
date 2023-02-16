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
 * @addtogroup Tools
 * @{
 * @addtogroup TestHarness
 * @{
 *
 * @file tools/test_harness/include/test_sim_interface.hh
 * Define functions and classes that provide functionality similar to that of
 * the JeodTrickSimInterface, but outside a Trick simulation environment.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((test_sim_interface.o))

 

*******************************************************************************/


#ifndef JEOD_TEST_HARNESS_SIM_INTERFACE_HH
#define JEOD_TEST_HARNESS_SIM_INTERFACE_HH

// System includes
#include <cstddef>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <pthread.h>

// JEOD includes
#include "utils/memory/include/memory_manager.hh"
#include "utils/memory/include/memory_messages.hh"
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/sim_interface/include/memory_interface.hh"
#include "utils/sim_interface/include/simulation_interface.hh"
#include "utils/sim_interface/include/trick_message_handler.hh"

// Model includes
#include "test_message_handler.hh"
#include "null_memory_interface.hh"
#include "test_harness_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * The TestSimInterface class is a JeodSimulationInterface that checks for
 * failures by means of message handler messages whose message code is deemed
 * indicative of a failure.
 */
class TestSimInterface : public JeodSimulationInterface {

JEOD_MAKE_SIM_INTERFACES (TestSimInterface)

public:

   // Static functions

   // Lock and unlock the printf mutex.
   static void enable_lock ();
   static void disable_lock ();
   static void lock_printf (void);
   static void unlock_printf (void);


   // Member functions

   // Default constructor and destructor.
   TestSimInterface ();
   ~TestSimInterface ();

   // Shut down the memory manager.
   void shutdown ();

   /**
    * Get the test failure status.
    * \return The value of the failed data member.
    */
   bool get_failed () const { return failed;}

   /**
    * Mark the test as failed.
    * This method is used by the TestMessageHandler when a message with a
    * verboten code is issued. Since the method has public visibility, the
    * external test mechanism can also use this method to mark the test
    * as failed. */
   void set_failed () {failed = true;}

   /**
    * Get the allowed_error_count.
    */
   unsigned int get_allowed_count ();

   /**
    * Bump the allowed_error_count.
    */
   void bump_allowed_count ();

   // Add a code to the allowed list.
   void add_allowed_code (const std::string &);

   // Add a code to the verboten list.
   void add_verboten_code (const std::string &);

   // Set the failure threshold.
   void set_failure_threshold (int level);


   // The following methods are pure virtual in the parent class.
   // Each returns dummy value.

   /**
    * Create an integrator interface.
    */
   JeodIntegratorInterface * create_integrator_internal (void)
   {
      return NULL;
   }

   /**
    * Create an integrator interface.
    */
   JeodIntegratorInterface * create_integrator_internal (void *)
   {
      return NULL;
   }

   /**
    * Get the cycle time of the currently-executing job.
    */
   virtual double get_job_cycle_internal (void)
   {
      return 1.0;
   }

   /**
    * Get the interface with the simulation memory manager.
    */
   virtual JeodMemoryInterface & get_memory_interface_internal (void)
   {
      return memory_interface;
   }

   /**
    * Get a checkpoint section reader.
    */
   virtual SectionedInputStream get_checkpoint_reader_internal (
      const std::string & section_id JEOD_UNUSED)
   {
      return SectionedInputStream();
   }

   /**
    * Get a checkpoint section writer.
    */
   virtual SectionedOutputStream get_checkpoint_writer_internal (
      const std::string & section_id JEOD_UNUSED)
   {
      return SectionedOutputStream();
   }


private:

   // Member data

   /**
    * Locking is enabled if set.
    */
   static bool the_lock_enabled; //!< trick_units(--)

   /**
    * A mutex for locking c-style output.
    */
   static pthread_mutex_t * the_printf_mutex; //!< trick_io(**) trick_units(--)

   /**
    * Has the test failed?
    * This member is accessible via TestSimInterface::get_failed() and
    * settable via TestSimInterface::set_failed(). Note that there is no
    * interface to clear the failure code.
    */
   bool failed; //!< trick_units(--)

   /**
    * Count of the number of times a severe but non-fatal error message
    * was generated whose message code matched one of the allowed codes.
    */
   unsigned int allowed_error_count; //!< trick_units(--)

   /**
    * A mutex for locking c-style output.
    */
   pthread_mutex_t printf_mutex; //!< trick_io(**) trick_units(--)

   /**
    * A mutex for locking data access.
    */
   pthread_mutex_t data_mutex; //!< trick_io(**) trick_units(--)

   /**
    * The message handler, which per JeodSimulationInterface rules must
    * be instantiated prior to instantiating the memory manager. This
    * message handler checks for test failures via messages whose message
    * code matches one of the verboten codes and does not match one of the
    * allowed codes.
    */
   TestMessageHandler message_handler; //!< trick_units(--)

   /**
    * The memory interface used by the memory manager.
    */
   NullMemoryInterface memory_interface; //!< trick_units(--)

   /**
    * The memory manager. This is a pointer rather than a instance so that
    * test failures that occur in destruction of the memory manager can
    * be detected.
    */
   JeodMemoryManager * memory_manager; //!< trick_units(--)


   // Deleted content
   // The copy constructor and assignment operator are private/unimplemented.

   /**
    * Not implemented.
    */
   explicit TestSimInterface (const TestSimInterface &);

   /**
    * Not implemented.
    */
   TestSimInterface & operator= (const TestSimInterface &);
};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 */
