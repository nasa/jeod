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
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Memory
 * @{
 *
 * @file models/utils/memory/include/memory_messages.hh
 * Define the class MemoryMessages, the class that specifies the
 * message IDs used in the memory model.
 */

/*******************************************************************************

Purpose:
  ()

Assumptions and limitations:
  ((This is a complete catalog of all the messages sent by the memory model.)
   (This is not an exhaustive list of all the things that can go awry.))

Library dependencies:
  ((../src/memory_messages.cc))

 
*******************************************************************************/


#ifndef JEOD_MEMORY_MESSAGES_HH
#define JEOD_MEMORY_MESSAGES_HH

/**
 * \addtogroup classes
 * @{
 */

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"


//! Namespace jeod
namespace jeod {

/**
 * Declares messages associated with the integration test model.
 */
class MemoryMessages {


 JEOD_MAKE_SIM_INTERFACES(MemoryMessages)


 // Static member data
 public:

   /**
    * Error issued when multiple instance of a class that should be a singleton
    * are created or when no such instance exists (but should).
    */
   static char const * singleton_error; //!< trick_units(--)

   /**
    * Issued when malloc returns NULL.
    */
   static char const * out_of_memory; //!< trick_units(--)

   /**
    * Issued when problems arise with in protection for atomic operations.
    */
   static char const * lock_error; //!< trick_units(--)

   /**
    * Issued when the caller attempts to do something with a null pointer
    * such as registering or freeing.
    */
   static char const * null_pointer; //!< trick_units(--)

   /**
    * Issued when the caller attempts to register memory that overlaps with
    * previously recording allocations or attempts to destroy memory that
    * was not previously registered.
    */
   static char const * suspect_pointer; //!< trick_units(--)

   /**
    * Issued when the caller attempts to allocate zero bytes.
    */
   static char const * invalid_size; //!< trick_units(--)

   /**
    * Issued when guard bytes have been overwritten.
    */
   static char const * corrupted_memory; //!< trick_units(--)

   /**
    * Issued when a model programmer messed up.
    */
   static char const * registration_error; //!< trick_units(--)

   /**
    * Issued when the memory model programmer messed up.
    */
   static char const * internal_error; //!< trick_units(--)

   /**
    * Used to identify debug output.
    */
   static char const * debug; //!< trick_units(--)


 // Member functions
 // This class is not instantiable.
 // The constructors and assignment operator for this class are declared
 // private and are not implemented.
 private:
   /**
    * Not implemented.
    */
   MemoryMessages (void);

   /**
    * Not implemented.
    */
   MemoryMessages (const MemoryMessages &);

   /**
    * Not implemented.
    */
   MemoryMessages & operator= (const MemoryMessages &);

};

} // End JEOD namespace

/**
 * @}
 */

#endif

/**
 * @}
 * @}
 * @}
 */
