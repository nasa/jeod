/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Memory
 * @{
 *
 * @file models/utils/memory/src/memory_manager.cc
 * Implement the JeodMemoryManager class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((memory_manager.o)
   (memory_item.o)
   (memory_messages.o)
   (memory_type.o))

 
*******************************************************************************/


/**
 * \addtogroup classes
 * @{
 */

// System includes
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <map>
#include <sstream>
#include <typeinfo>
#include <pthread.h>
#include <stdint.h>

// JEOD includes
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/memory_manager.hh"
#include "../include/memory_item.hh"
#include "../include/memory_messages.hh"


#define MAGIC0 0x2203992c
#define MAGIC1 0x6c052d84


//! Namespace jeod
namespace jeod {

/**
 * Construct a MemoryManager object.
 * \param[in,out] interface The memory interface with the simulation engine
 */
JeodMemoryManager::JeodMemoryManager (
   JeodMemoryInterface & interface)
:
   sim_interface(interface),
   debug_level(Error_details),
   cur_data_size(0),
   max_data_size(0),
   max_table_size(0),
   allocation_number(0),
   alloc_table(),
   type_table(),
   string_table(),
   mode(JeodSimulationInterface::Construction),
   guard_enabled(true)
{
   // Nominal case: There is no master memory manager yet.
   // This object will become that master memory manager.
   // Complete construction.
   if (Master == NULL) {

      // This is the master memory manager.
      Master = this;


      // Populate the type table with commonly-used names for integer types.
      // This avoids someone overriding 'int' with 'int32_t' and such.
      // Note that the mutex is not yet initialized, so these operations must
      // be made through the primitive table operations.
      #define MAKE_DESCRIPTOR(type) \
         do { \
            JeodMemoryTypeDescriptorDerived<type> tdesc; \
            type_table.add (tdesc.get_typeid().name(), tdesc); \
         } while (0)
      MAKE_DESCRIPTOR (char);
      MAKE_DESCRIPTOR (unsigned char);
      MAKE_DESCRIPTOR (signed char);
      MAKE_DESCRIPTOR (short int);
      MAKE_DESCRIPTOR (unsigned short int);
      MAKE_DESCRIPTOR (int);
      MAKE_DESCRIPTOR (unsigned int);
      MAKE_DESCRIPTOR (long int);
      MAKE_DESCRIPTOR (unsigned long int);
      MAKE_DESCRIPTOR (long long int);
      MAKE_DESCRIPTOR (unsigned long long int);
      MAKE_DESCRIPTOR (bool);
      MAKE_DESCRIPTOR (float);
      MAKE_DESCRIPTOR (double);
      // MAKE_DESCRIPTOR (long double);  Not yet supported.
      MAKE_DESCRIPTOR (void *);
      #undef MAKE_DESCRIPTOR

      // Initialize the mutex.
      pthread_mutexattr_t mutex_attr;
      pthread_mutexattr_init (&mutex_attr);
      pthread_mutexattr_settype (&mutex_attr, PTHREAD_MUTEX_ERRORCHECK);
      pthread_mutex_init (&mutex, &mutex_attr);
   }

   // Error case: Raise an error (non-fatal).
   // Note that the above initializations did not occur, thereby making this
   // particular instance unusable.
   else {
      MessageHandler::error (
         __FILE__, __LINE__, MemoryMessages::singleton_error,
         "A master memory manager has already been established.");
   }

   // NOTE WELL: The memory manager is now available for use in multi-threaded
   // environments. All fragile operations between this point and the call to
   // the destructor must be performed with thread safety in mind.
   return;
}


/**
 * Destruct a MemoryManager object.
 *
 * \par Assumptions and Limitations
 *  - In a multi-threaded environment,
 *     - This destructor shall be called once and once only to destroy the
 *       singleton JeodMemoryManager object.
 *     - The thread that calls this destructor shall wait until all other threads
 *       that access JEOD memory have finished, either by default or by force.
 *     .
 *     Note that this is a constraint on the simulation engine, not on JEOD.
 */
JeodMemoryManager::~JeodMemoryManager (
   void)
{
   // Only shutdown if there is something to shutdown.
   if (Master == this) {

      // The master is gone.
      Master = NULL;

      // Destroy the mutex.
      // NOTE WELL: From this point onward it is assumed to be OK to perform
      // unsafe operations without thread protection.
      // (With the mutex destroyed, unsafe operations are the only option.)
      pthread_mutex_destroy (&mutex);

      // Report activity.
      generate_shutdown_report ();

      // Make leaks opaque to the simulation engine.
      // FUTURE_FEATURE: Garbage collect here?
      for (AllocTable::iterator it = alloc_table.begin();
           it != alloc_table.end();
           ++it) {
         const void * addr           = it->first;
         const JeodMemoryItem & item = it->second;
         const JeodMemoryTypeDescriptor & tdesc =
            get_type_descriptor_nolock (item);
         if (item.get_is_registered()) {
            sim_interface.deregister_allocation (
               addr, item, tdesc, __FILE__, __LINE__);
         }
      }

      // Delete the allocations.
      alloc_table.clear();
   }
   // No else; the error was already reported in the constructor.
}


/**
 * Generate a shutdown report.
 *
 * \par Assumptions and Limitations
 *  - This method is to be called by the destructor only.
 *     It freely accesses tabular data, the assumption being that the
 *     mutex and flags that protect that data are now gone.
 */
void
JeodMemoryManager::generate_shutdown_report (
   void)
{
   if (debug_level > 0) {

      // FUTURE_FEATURE: Do a better job of counting.
      // It would be nice to know the size of a red-black tree node.
      unsigned int telem_size = sizeof (AllocTable::value_type);
      unsigned int item_size  = sizeof (JeodMemoryItem);
      unsigned int  total_size = max_table_size *
                                 (telem_size + item_size);

      // Generate a summary report.
      MessageHandler::inform (
         __FILE__, __LINE__, MemoryMessages::debug,
         "JEOD memory table report:\n"
         "  Max # elements:       %7d\n"
         "  Table element size:   %7d\n"
         "  Item descriptor size: %7d\n"
         "  Memory overhead:      %7d\n"
         "  Allocated data size:  %7d",
         max_table_size, telem_size, item_size, total_size, max_data_size);

      // Report any memory that has not been freed.
      // Note: This reports only. Unfreed memory is a leak.
      if (alloc_table.empty()) {
         MessageHandler::inform (
            __FILE__, __LINE__, MemoryMessages::debug,
            "All JEOD-allocated memory has been freed!");
      }
      else {
         MessageHandler::warn (
            __FILE__, __LINE__, MemoryMessages::corrupted_memory,
            "Not all JEOD-allocated memory has been freed!");

         for (AllocTable::iterator it = alloc_table.begin();
              it != alloc_table.end();
              ++it) {
            const void * addr           = it->first;
            const JeodMemoryItem & item = it->second;
            const JeodMemoryTypeDescriptor & tdesc =
               get_type_descriptor_nolock (item);
            unsigned int alloc_idx = item.get_alloc_index();
            if (alloc_idx == 0) {
               MessageHandler::warn (
                  __FILE__, __LINE__, MemoryMessages::debug,
                  "Memory at %p was not freed\n"
                  "  Type=%s\n",
                  addr,
                  tdesc.type_spec(item).c_str());
            }
            else {
               MessageHandler::warn (
                  __FILE__, __LINE__, MemoryMessages::debug,
                  "Memory at %p was not freed\n"
                  "  Type=%s\n"
                  "  Allocated at %s",
                  addr,
                  tdesc.type_spec(item).c_str(),
                  string_table.get(alloc_idx)->c_str());
            }
         }
      }
   }
}


/******************************************************************************/


/**
 * Wipe out all allocated memory in anticipation of restoring the
 * memory in some previously recording checkpoint file.
 *
 * \par Assumptions and Limitations
 *  - If the restore doesn't work the sim will be knee deep in alligators.
 */


/*
Purpose:
  (Wipe out all allocated memory in anticipation of restoring the
   memory in some previously recording checkpoint file.)
Assumptions and limitations:
  ((If the restore doesn't work the sim will be knee deep in alligators.))
*/
void
JeodMemoryManager::restart_clear_memory (
   void)
{
   void * addr = NULL;
   JeodMemoryItem item;
   const JeodMemoryTypeDescriptor * type = NULL;

   // Just keep a deletin' and destroyin' 'til nothing's left.
   // NOTE: This only works if we are using placement new.
   for (delete_oldest_alloc_entry_atomic (addr, item, type);
        addr != NULL;
        delete_oldest_alloc_entry_atomic (addr, item, type)) {

      // De-register the item with the simulation engine.
      if (item.get_is_registered()) {
         sim_interface.deregister_allocation (
            addr, item, *type, __FILE__, __LINE__);
      }

      // Destruct and delete the item consistent with its creation.
      type->destroy_memory (
         item.get_placement_new(),
         item.get_is_array(),
         item.get_nelems(),
         addr);

      // Free memory that was allocated by this model for the item.
      if (item.get_placement_new()) {
         free_memory (addr,
                      type->buffer_size (item.get_nelems()),
                      item.get_is_guarded(),
                      item.get_alloc_index(),
                      __FILE__, __LINE__);
      }
   }
   cur_data_size = 0;
   max_data_size = 0;
   max_table_size = 0;
   allocation_number = 0;
}

/**
 * Restore one chunk of allocated memory per a checkpoint file entry.
 *
 * \par Assumptions and Limitations
 *  - This restores the allocation, but not the contents. The contents will soon
 *     be restored by the simulation engine.
 * \param[in] mangled_type_name Mangled type name
 * \param[in] unique_id Unique id
 * \param[in] nelements Number of elements
 * \param[in] is_array True => an array
 */
void
JeodMemoryManager::restart_reallocate (
   const std::string & mangled_type_name,
   uint32_t unique_id,
   uint32_t nelements,
   bool is_array)
{
   TypeEntry tentry =
      get_type_entry_atomic (Typeid_type_name, mangled_type_name);
   const JeodMemoryTypeDescriptor * type = tentry.tdesc;

   if (type == NULL) {
      MessageHandler::error (
         __FILE__, __LINE__, MemoryMessages::suspect_pointer,
         "Unable to find type '%s' for jeod_alloc_%06d.\n"
         "This object cannot be restored.",
         mangled_type_name.c_str(), unique_id);
      return;
   }

   std::size_t elem_size = type->get_size();
   void * addr;

   // Allocate and construct the object.
   addr = allocate_memory (nelements, elem_size, guard_enabled, 0);
   type->construct_array (nelements, addr);

   // Register with the simulation engine.
   register_memory_internal (
      addr, unique_id, true, is_array, nelements, tentry,
      __FILE__, __LINE__);
}


/******************************************************************************/


/**
 * Allocate memory for use with placement new and register that memory with
 * with the memory manager and with the simulation engine.
 *
 * \par Assumptions and Limitations
 *  - This method will be invoked via the JEOD memory allocation macros.
 *     Use in any other context is a case of caveat emptor.
 *  - The type descriptor index must index the type descriptor that describes the
 *     type to be created.
 *  - The memory is allocated but not constructed. Construction is the
 *     responsibility of the caller. The JEOD memory allocation macros construct
 *     the allocated memory via placement new.
 * @return Allocated memory
 * \param[in] is_array Memory constructed by new[] if set
 * \param[in] nelems Number of elements to be allocated
 * \param[in] fill Byte fill pattern
 * \param[in] tentry Type entry
 * \param[in] file Source file containing JEOD_ALLOC
 * \param[in] line Line number containing JEOD_ALLOC
 */


/*
Purpose:
  (Allocate memory for use with placement new and register that memory with
   with the memory manager and with the simulation engine.)
Assumptions and limitations:
  ((This method will be invoked via the JEOD memory allocation macros.
    Use in any other context is a case of caveat emptor.)
   (The type descriptor index must index the type descriptor that describes the
    type to be created.)
   (The memory is allocated but not constructed. Construction is the
    responsibility of the caller. The JEOD memory allocation macros construct
    the allocated memory via placement new.))
*/
void *
JeodMemoryManager::create_memory_internal (
   bool is_array,
   unsigned int nelems,
   int fill,
   const TypeEntry & tentry,
   const char * file,
   unsigned int line)
{
   std::size_t elem_size = tentry.tdesc->get_size();
   void * addr = allocate_memory (nelems, elem_size, guard_enabled, fill);

   register_memory_internal (
      addr, 0, true, is_array, nelems, tentry, file, line);

   return addr;
}


/**
 * Allocate memory if that was not already done by the caller and
 * register the memory with JEOD and with an external agent.
 *
 * \par Assumptions and Limitations
 *  - This method will be invoked via the JEOD memory allocation macros.
 *     Use in any other context is caveat emptor.
 *  - The corresponding delete macro will be used to delete the memory.
 *     Using the C free function or the C++ delete operator can cause *big*
 *     problems.
 *  - The delete macro will be expanded with the same placement new option
 *     as was used in the allocation macro that resulted in this call.
 *  - The memory is not constructed.
 *     That is the job of the expansion of the JEOD_ALLOC macro.
 * \param[in] addr Memory to be registered
 * \param[in] unique_id Unique id
 * \param[in] placement_new Was memory allocated by this model?
 * \param[in] is_array Was memory allocated as an array?
 * \param[in] nelems Array size
 * \param[in] tentry Type entry
 * \param[in] file Source file containing JEOD_ALLOC
 * \param[in] line Line number containing JEOD_ALLOC
 */
void
JeodMemoryManager::register_memory_internal (
   const void * addr,
   uint32_t unique_id,
   bool placement_new,
   bool is_array,
   unsigned int nelems,
   const TypeEntry & tentry,
   const char * file,
   unsigned int line)
{
   unsigned int alloc_idx = 0;
   uint32_t tidx = tentry.index;
   const JeodMemoryTypeDescriptor & tdesc = *(tentry.tdesc);

   // Sanity check: nelems and elem_size must be positive to continue.
   // (Note that c++ allows double foo = new double[0]. JEOD does not.)
   // Complain & return NULL; this will most likely raise havoc elsewhere.
   if (nelems == 0) {
      MessageHandler::error (
         __FILE__, __LINE__, MemoryMessages::invalid_size,
         "User allocation failed at %s:%d\n",
         "The number of elements must be positive.",
         file, line);
      return;
   }

   // Construct and save a string describing the place the memory was
   // allocated, but only if debugging levels are high enough.
   if ((file == NULL) || (debug_level <= 1)) {
      alloc_idx = 0;
   }
   else {
      std::ostringstream id;
      id << file << ":" << line;

      alloc_idx = add_string_atomic (id.str());
   }

   // Create the memory item that describes the allocated memory.
   JeodMemoryItem item (placement_new,
                        is_array,
                        true,
                        tdesc.is_structured(),
                        nelems,
                        tidx,
                        alloc_idx);

   // Provided unique_id is non-zero (called from restart_reallocate):
   // Use the provided number.
   if (unique_id > 0) {
      // Assign the given identifier to the item.
      item.set_unique_id (unique_id);

      // Update the allocation number.
      reset_alloc_id_atomic (unique_id);
   }

   // Unique id is zero (called from create_memory_internal):
   // Assign a unique id.
   else {
      // Assign a unique identifier to the item.
      item.set_unique_id (get_alloc_id_atomic (file, line));
   }

   // Register the item with the simulation engine.
   if (tdesc.get_register_instances () &&
       sim_interface.register_allocation (
          addr, item, tdesc, file, line)) {
      item.set_is_registered (true);
   }
   // No else: The sim interface is responsible for complaining.

   // Add the item to the memory table.
   add_allocation_atomic (addr, item, tdesc, file, line);

   // Report the allocation.
   if (debug_level > 2) {
      MessageHandler::debug (
         __FILE__, __LINE__, MemoryMessages::debug,
         "JEOD memory debug: Allocated %p at %s:%d\n"
         "  Type=%s\n"
         "  Size=%zu",
         addr, file, line,
         tdesc.type_spec(item).c_str(),
         tdesc.buffer_size(item));
   }

   return;
}


/**
 * Query whether some address was allocated by JEOD.
 * @return True if the address in question was allocated by JEOD
 * \param[in] addr Memory to be queried
 * \param[in] file Source file containing query
 * \param[in] line Line number containing query
 */
bool
JeodMemoryManager::is_allocated_internal (
   const void * addr,
   const char * file,
   unsigned int line)
{
   void * found_addr = NULL;
   JeodMemoryItem found_item;
   const JeodMemoryTypeDescriptor * found_type = NULL;

   // Silently indicate that a null pointer was not allocated by JEOD.
   if (addr == NULL) {
      return false;
   }

   // Find the matching allocation table entry for this address.
   find_alloc_entry_atomic (addr, false, file, line,
                            found_addr, found_item, found_type);

   // A non-null address means the input address is allocated by JEOD.
   return found_addr != NULL;
}


/**
 * Destroy a chunk of memory and knowledge about it.
 * This includes
 *  - De-registering the memory with JEOD and with an external agent.
 *  - Invoking the destructor in the case of a structured type.
 *  - Releasing the memory to the system.
 * \param[in,out] addr Memory to be destroyed
 * \param[in] delete_array DELETE_ARRAY (true) vs. DELETE_OBJECT
 * \param[in] file Source file containing delete
 * \param[in] line Line number containing delete
 */
void
JeodMemoryManager::destroy_memory_internal (
   void * addr,
   bool delete_array,
   const char * file,
   unsigned int line)
{
   void * found_addr = NULL;
   JeodMemoryItem found_item;
   const JeodMemoryTypeDescriptor * found_type = NULL;

   // Nothing to do if address is null.
   if (addr == NULL) {
      MessageHandler::warn (
         __FILE__, __LINE__, MemoryMessages::null_pointer,
         "Attempt to free a null pointer at %s:%d",
         file, line);
      return;
   }


   // Find and delete the matching allocation table entry for this address.
   find_alloc_entry_atomic (addr, true, file, line,
                            found_addr, found_item, found_type);


   // Item not found:
   // The most likely cause is trying to delete something not allocated by JEOD,
   // e.g., allocated by new, by the simulation engine, or from the stack.
   // The response here is to do nothing other than complain.
   if (found_addr == NULL) {
      MessageHandler::error (
         __FILE__, __LINE__, MemoryMessages::suspect_pointer,
         "JEOD memory error: Memory at %p was not created by JEOD.\n"
         "  Free occured at %s:%d",
         addr, file, line);
      return;
   }

   // Delete is incompatible with allocation:
   // Complain and continue.
   // destroy_memory() should be able to address the mismatch.
   if (delete_array != found_item.get_is_array()) {
      MessageHandler::warn (
         __FILE__, __LINE__, MemoryMessages::suspect_pointer,
         "JEOD memory warning: Deletion of %p is inconsistent with "
         "its creation.\n"
         "  Free occured at %s:%d",
         addr, file, line);
   }


   // De-register the item with the simulation engine.
   // Note that this must precede the destruction and free lest we subject the
   // simulation engine with the same problem we just punted above.
   if (found_item.get_is_registered()) {
      sim_interface.deregister_allocation (
         found_addr, found_item, *found_type, file, line);
   }

   // Destruct and delete the item consistent with its creation.
   found_type->destroy_memory (
      found_item.get_placement_new(),
      found_item.get_is_array(),
      found_item.get_nelems(),
      found_addr);

   // Free memory that was allocated by this model for the item.
   if (found_item.get_placement_new()) {
      free_memory (found_addr,
                   found_type->buffer_size (found_item.get_nelems()),
                   found_item.get_is_guarded(),
                   found_item.get_alloc_index(),
                   file, line);
   }


   // Print debugging info if enabled.
   if (debug_level > 2) {
      unsigned int alloc_idx = found_item.get_alloc_index();
      if (alloc_idx == 0) {
         MessageHandler::debug (
            __FILE__, __LINE__, MemoryMessages::debug,
            "JEOD memory debug: Deleted %p at %s:%d\n"
            "  Type=%s\n",
            found_addr, file, line,
            found_type->type_spec(found_item).c_str());
      }
      else {
         MessageHandler::debug (
            __FILE__, __LINE__, MemoryMessages::debug,
            "JEOD memory debug: Deleted %p at %s:%d\n"
            "  Type=%s\n"
            "  Allocated at %s",
            found_addr, file, line,
            found_type->type_spec(found_item).c_str(),
            get_string_atomic(alloc_idx).c_str());
      }
   }
}


/**
 * Set the mode and perform mode transitions.
 * \param[in] new_mode New mode
 */
void
JeodMemoryManager::set_mode_internal (
   JeodSimulationInterface::Mode new_mode)
{
   mode = new_mode;
}


/******************************************************************************/


/**
 * Allocate memory.
 *
 * \par Assumptions and Limitations
 *  - This is a low-level allocation function. It does not
 *      - Register the allocated memory with JEOD or with an external agent.
 *      - Construct the newly-allocated memory.
 *  - The returned address should not be released using the C free function
 *     or C++ delete operator.\n
 *     Failure to obey this restriction will result in big problems.
 * @return Allocated memory
 * \param[in] nelems Number of elements
 * \param[in] elem_size Size of each element
 * \param[in] guard Allocate guard bytes if set
 * \param[in] fill Fill pattern (ref. memset)
 */


/*
Purpose:
  (Allocate memory.)
Assumptions and limitations:
  ((This is a low-level allocation function. It does not
     - Register the allocated memory with JEOD or with an external agent.
     - Construct the newly-allocated memory.)
   (The returned address should not be released using the C free function
    or C++ delete operator.\n
    Failure to obey this restriction will result in big problems.))
*/
void *
JeodMemoryManager::allocate_memory (
   std::size_t nelems,
   std::size_t elem_size,
   bool guard,
   int fill)
const
{
   void * addr = NULL;
   char * buf = NULL;
   std::size_t length = nelems * elem_size;
   unsigned int start_offset;
   unsigned int end_offset;
   std::size_t buf_len;

   // Determine guard offsets.
   if (guard) {
      start_offset = 16;
      end_offset   =  8;
   }
   else {
      start_offset =  0;
      end_offset   =  0;
   }

   // Compute the allocation buffer size, including guards.
   buf_len = length + start_offset + end_offset;

   // Allocate the requisite amount of memory.
   buf = new char[buf_len];

   // Failure to allocate memory means either the program has consumed all
   // memory or the user asked for a huge amount.
   // *Try* to print a message.
   // Note to the user: If the program drops core in the message handler call
   // it means your sim has consumed all memory.
   if (buf == NULL) {
      MessageHandler::fail (
         __FILE__, __LINE__, MemoryMessages::out_of_memory,
         "Out of memory");
      return NULL;
   }

   // Insert guards.
   if (guard) {
      uint32_t * guard_lo;   // The four guard words at the start of the buffer
      uint32_t * guard_hi;   // The two guard words at the end of the buffer

      // Compute the address of the guards.
      guard_lo = reinterpret_cast<uint32_t*>(buf);
      guard_hi = reinterpret_cast<uint32_t*>(buf+buf_len-end_offset);

      // Set the guard words.
      guard_lo[0] = guard_lo[2] = guard_hi[0] = MAGIC0;
      guard_lo[1] = guard_lo[3] = guard_hi[1] = MAGIC1;
   }

   // Determine the address to be returned and fill with the fill pattern.
   addr = reinterpret_cast<void*> (buf+start_offset);
   std::memset (addr, fill, length);

   return addr;
}


/**
 * Release memory.
 *
 * \par Assumptions and Limitations
 *  - This is a low-level de-allocation function.
 *     It does not
 *      - De-register the memory with JEOD or with an external agent.
 *      - Destruct the memory.
 * \param[in,out] addr Memory to be freed
 * \param[in] length Buffer size
 * \param[in] guard Memory was guarded if set
 * \param[in] alloc_idx Allocation index
 * \param[in] file Source file containing delete
 * \param[in] line Line number containing delete
 */
void
JeodMemoryManager::free_memory (
   void * addr,
   std::size_t length,
   bool guard,
   unsigned int alloc_idx,
   const char * file,
   unsigned int line)
const
{
   char * buf;               // The address of the allocated buffer
   std::size_t buf_len;      // The length of the allocated buffer

   // Memory is guarded:
   // Free the allocated memory, which is offset from the input address.
   if (guard) {
      uint32_t * guard_lo;   // The four guard words at the start of the buffer
      uint32_t * guard_hi;   // The two guard words at the end of the buffer

      // Compute the addresses of the allocated memory and the guards
      buf      = reinterpret_cast<char*> (addr) - 16;
      guard_lo = reinterpret_cast<uint32_t*>(buf);
      guard_hi = reinterpret_cast<uint32_t*>(buf+length+16);
      buf_len  = length + 24;

      // Check whether the guard words have been overwritten.
      // Issue a complaint if this is the case.
      if ((guard_lo[0] != MAGIC0) ||
          (guard_lo[2] != MAGIC0) ||
          (guard_hi[0] != MAGIC0) ||
          (guard_lo[1] != MAGIC1) ||
          (guard_lo[3] != MAGIC1) ||
          (guard_hi[1] != MAGIC1)) {
         if (alloc_idx == 0) {
            MessageHandler::warn (
               __FILE__, __LINE__, MemoryMessages::corrupted_memory,
               "JEOD memory warning: Memory at %p is corrupted!\n"
               "  Freed at %s:%d",
               addr, file, line);
         }
         else {
            MessageHandler::warn (
               __FILE__, __LINE__, MemoryMessages::corrupted_memory,
               "JEOD memory warning: Memory at %p is corrupted!\n"
               "  Allocated at %s\n"
               "  Freed at %s:%d",
               addr, get_string_atomic(alloc_idx).c_str(), file, line);
         }
      }

   }

   // Memory is not guarded:
   // The input address is the address of allocated memory.
   else {
      buf      = reinterpret_cast<char*> (addr);
      buf_len  = length;
   }

   // Fill the buffer with garbage and free it.
   std::memset (buf, 0xa5, buf_len);
   delete[] buf;
}


} // End JEOD namespace

/**
 * @}
 */

/**
 * @}
 * @}
 * @}
 */
