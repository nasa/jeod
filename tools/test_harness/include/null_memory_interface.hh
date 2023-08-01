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
 * @file tools/test_harness/include/null_memory_interface.hh
 * Define the class NullMemoryInterface.
 */

/*******************************************************************************

Purpose:
  ()

 

*******************************************************************************/


#ifndef JEOD_NULL_MEMORY_INTERFACE_HH
#define JEOD_NULL_MEMORY_INTERFACE_HH

// System includes
#include <cstddef>
#include <cstring>
#include <string>

// JEOD includes
#include "utils/message/include/message_handler.hh"
#include "utils/sim_interface/include/memory_attributes.hh"
#include "utils/sim_interface/include/memory_interface.hh"

// Model includes
#include "test_harness_messages.hh"



//! Namespace jeod
namespace jeod {

/**
 * A NullMemoryInterface extends the JeodMemoryInterface, providing the
 * required functionality of the parent class in a null manner.
 */
class NullMemoryInterface : public JeodMemoryInterface {

 // Member functions
 public:

   /**
    * Default constructor.
    */
   NullMemoryInterface () {}

   /**
    * Destructor.
    */
   ~NullMemoryInterface () {}


      /**
    * Checkpoint/restart is not supported here.
    * @return False
    */
   bool is_checkpoint_restart_supported (
      void)
   const
   {
      return false;
   }

      /**
    * Do-nothing get_name_at_address handler.
    * @return Empty string
    * \param[in] addr Ignored
    * \param[in] tdesc Ignored
    */
   const std::string get_name_at_address (
      const void * addr JEOD_UNUSED,
      const JeodMemoryTypeDescriptor * tdesc JEOD_UNUSED)

   const
   {
      return "";
   }

      /**
    * Do-nothing get_address_at_name handler.
    * @return Null pointer
    * \param[in] name Ignored
    */
   void * get_address_at_name (
      const std::string & name JEOD_UNUSED)
   const
   {
      return NULL;
   }

      /**
    * Do-nothing find_attributes handler.
    * @return NULL
    */
   const JEOD_ATTRIBUTES_POINTER_TYPE find_attributes (
      const std::string &)                              // In: -- Ignored
   const
   {
      return NULL;
   }

      /**
    * Do-nothing find_attributes handler.
    * @return NULL
    */
   const JEOD_ATTRIBUTES_POINTER_TYPE find_attributes (
      const std::type_info &)                          // In: -- Ignored
   const
   {
      return NULL;
   }

      /**
    * Do-nothing primitive_attributes handler.
    * @return Dummy
    */
   JEOD_ATTRIBUTES_TYPE primitive_attributes (
      const std::type_info &)                  // In: -- Ignored
   const
   {
#ifdef TRICK_VER
      ATTRIBUTES dummy;
      return dummy;
#else
      return 0;
#endif
   }

      /**
    * Do-nothing pointer_attributes handler.
    * @return Dummy
    */
   JEOD_ATTRIBUTES_TYPE pointer_attributes (
      const JEOD_ATTRIBUTES_TYPE &)          // In: -- Ignored
   const
   {
#ifdef TRICK_VER
      ATTRIBUTES dummy;
      return dummy;
#else
      return 0;
#endif
   }

      /**
    * Do-nothing pointer_attributes handler.
    * @return Dummy
    */
   JEOD_ATTRIBUTES_TYPE void_pointer_attributes (
      void)
   const
   {
#ifdef TRICK_VER
      ATTRIBUTES dummy;
      return dummy;
#else
      return 0;
#endif
   }

      /**
    * Do-nothing pointer_attributes handler.
    * @return Dummy
    */
   JEOD_ATTRIBUTES_TYPE structure_attributes (
      const JEOD_ATTRIBUTES_POINTER_TYPE,      // In: -- Ignored
      std::size_t)                             // In: -- Ignored
   const
   {
#ifdef TRICK_VER
      ATTRIBUTES dummy;
      return dummy;
#else
      return 0;
#endif
   }

      /**
    * Do-nothing register_allocation handler.
    * @return True
    * \param[in] addr Allocated memory
    * \param[in] item Description of the memory
    * \param[in] tdesc Description of the type
    * \param[in] file File containing JEOD_ALLOC
    * \param[in] line Line containing JEOD_ALLOC
    */
   virtual bool register_allocation (
      const void * addr,
      const JeodMemoryItem & item,
      const JeodMemoryTypeDescriptor & tdesc,
      const char * file,
      unsigned int line)
   {
      MessageHandler::debug (
         file, line, TestHarnessMessages::debug,
        "Memory at %p registered as a %s, size=%zd",
        addr, tdesc.type_spec(item).c_str(), tdesc.buffer_size(item));
      return true;
   }

      /**
    * Do-nothing deregister_allocation handler.
    * \param[in] addr Allocated memory
    * \param[in] item Description of the memory
    * \param[in] tdesc Description of the type
    * \param[in] file File containing JEOD_ALLOC
    * \param[in] line Line containing JEOD_ALLOC
    */
   virtual void deregister_allocation (
      const void * addr,
      const JeodMemoryItem & item,
      const JeodMemoryTypeDescriptor & tdesc,
      const char * file,
      unsigned int line)
   {
      MessageHandler::debug (
         file, line, TestHarnessMessages::debug,
        "Memory at %p deregistered as a %s, size=%zd",
        addr, tdesc.type_spec(item).c_str(), tdesc.buffer_size(item));
   }

      /**
    * Do-nothing register_container handler.
    * \param[in] container Ignored
    * \param[in] container_type Ignored
    * \param[in] elem_name Ignored
    * \param[in] checkpointable Ignored
    */
   void register_container (
      const void * container JEOD_UNUSED,
      const JeodMemoryTypeDescriptor & container_type JEOD_UNUSED,
      const char * elem_name JEOD_UNUSED,
      JeodCheckpointable & checkpointable JEOD_UNUSED)
   {
      ; // Empty
   }

      /**
    * Do-nothing deregister_container handler.
    * \param[in] container Ignored
    * \param[in] container_type Ignored
    * \param[in] elem_name Ignored
    * \param[in] checkpointable Ignored
    */
   void deregister_container (
      const void * container JEOD_UNUSED,
      const JeodMemoryTypeDescriptor & container_type JEOD_UNUSED,
      const char * elem_name JEOD_UNUSED,
      JeodCheckpointable & checkpointable JEOD_UNUSED)
   {
      ; // Empty
   }


 // The copy constructor and assignment operator are private/unimplemented.
 private:

   /**
    * Not implemented.
    */
   explicit NullMemoryInterface (const NullMemoryInterface &);

   /**
    * Not implemented.
    */
   NullMemoryInterface & operator= (const NullMemoryInterface &);
};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 */
