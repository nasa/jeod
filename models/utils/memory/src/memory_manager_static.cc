/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Memory
 * @{
 *
 * @file models/utils/memory/src/memory_manager_static.cc
 * Implement the static methods of the JeodMemoryManager class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((memory_manager_static.o)
   (memory_manager.o)
   (memory_item.o)
   (memory_messages.o)
   (memory_type.o))

 
*******************************************************************************/


/**
 * \addtogroup classes
 * @{
 */

// System includes
#include <string>

// JEOD includes
#include "utils/message/include/message_handler.hh"
#include "utils/named_item/include/named_item.hh"

// Model includes
#include "../include/memory_manager.hh"
#include "../include/memory_messages.hh"


//! Namespace jeod
namespace jeod {

// Linkage for JeodMemoryManager::Master
JeodMemoryManager * JeodMemoryManager::Master = NULL;


/**
 * Many of the static methods are a pass-through to a private non-static method,
 * with the static method testing that the pass-through is valid. This method
 * performs that test and handles the failure response.
 * @return True if Master is not null
 * \param[in] error_is_fatal True => call fail
 * \param[in] line __LINE__
 */
bool
JeodMemoryManager::check_master (
   bool error_is_fatal,
   int line)
{
   if (Master == NULL) {
      const char * msg = "The master memory manager has not been established.";
      if (error_is_fatal) {
         MessageHandler::fail (
            __FILE__, line, MemoryMessages::singleton_error, msg);
      }
      else {
         MessageHandler::error (
            __FILE__, line, MemoryMessages::singleton_error, msg);
      }
      return false;
   }
   else {
      return true;
   }
}


/**
 * Set the debug level.
 * \param[in] level New debug level
 */
void
JeodMemoryManager::set_debug_level (
   DebugLevel level)
{

   // Throw a non-fatal error if the singleton memory manager is not available.
   if (check_master (false, __LINE__)) {

      // Master exists: Set the manager's debug level.
      Master->debug_level = level;
   }
}


/**
 * Set the debug level.
 * \param[in] level New debug level
 */
void
JeodMemoryManager::set_debug_level (
   unsigned int level)
{
   if (level > Full_details) {
      set_debug_level (Full_details);
   }
   else {
      set_debug_level (static_cast<DebugLevel>(level));
   }
}


/**
 * Set the guard_enabled flag.
 * \param[in] value New value
 */
void
JeodMemoryManager::set_guard_enabled (
   bool value)
{

   // Throw a non-fatal error if the singleton memory manager is not available.
   if (check_master (false, __LINE__)) {

      // Set the manager's guard_enabled flag.
      Master->guard_enabled = value;
   }
}


/**
 * Query whether all allocated memory has been freed.
 *
 * \par Assumptions and Limitations
 *  - Intended for testing use only.
 *     This method does not use a thread-safe query.
 * @return Has all memory been freed?
 */
bool
JeodMemoryManager::is_table_empty (
   void)
{
   bool is_empty = false;

   // Throw a non-fatal error if the singleton memory manager is not available.
   if (check_master (false, __LINE__)) {

      // See if the table is empty.
      is_empty = Master->alloc_table.empty();
   }

   return is_empty;
}


/**
 * Register a class with the memory manager.
 *
 * \par Assumptions and Limitations
 *  - This method must not be called before the singleton memory manager has
 *     been created or after it has been destroyed.
 *     A fatal error results when this is not true.
 *  - Access to this method is through the JEOD memory allocation macros.
 *     Use in any other context is caveat emptor.
 * @return Type entry for the class
 * \param[in] tdesc Type pre-descriptor
 */
const JeodMemoryManager::TypeEntry
JeodMemoryManager::register_class (
   JeodMemoryTypePreDescriptor & tdesc)
{

   // Throw a fatal error if the singleton memory manager is not available.
   if (check_master (true, __LINE__)) {

      // Return a copy of the entry for the type, creating an entry if needed.
      return Master->get_type_entry_atomic (tdesc);
   }

   // Not reached.
   else {
      return JeodMemoryManager::TypeEntry (0, NULL);
   }
}


/**
 * Get a type descriptor from the memory manager's type table.
 *
 * \par Assumptions and Limitations
 *  - This method must not be called before the singleton memory manager has
 *     been created or after it has been destroyed.
 *     A fatal error results when this is not true.
 * @return Type descriptor
 * \param[in] typeid_info C++ type descriptor
 */
const JeodMemoryTypeDescriptor *
JeodMemoryManager::get_type_descriptor (
   const std::type_info & typeid_info)
{
   const JeodMemoryTypeDescriptor * result = NULL;

   // Throw a fatal error if the singleton memory manager is not available.
   if (check_master (true, __LINE__)) {

      // Get the descriptor from the master memory manager.
      result = Master->get_type_descriptor_atomic (typeid_info);
   }

   return result;
}


/**
 * Get a type descriptor from the memory manager's type table.
 *
 * \par Assumptions and Limitations
 *  - This method must not be called before the singleton memory manager has
 *     been created or after it has been destroyed.
 *     A fatal error results when this is not true.
 * @return Type descriptor
 * \param[in] name_type Typeid or demangled name
 * \param[in] type_name Type name
 */
const JeodMemoryTypeDescriptor *
JeodMemoryManager::get_type_descriptor (
   JeodMemoryManager::NameType name_type,
   const std::string & type_name)
{
   const JeodMemoryTypeDescriptor * result = NULL;

   // Throw a fatal error if the singleton memory manager is not available.
   if (check_master (true, __LINE__)) {

      // Get the descriptor from the master memory manager.
      result = Master->get_type_entry_atomic(name_type, type_name).tdesc;
   }

   return result;
}


/**
 * Allocate memory and register the allocated memory with JEOD.
 *
 * \par Assumptions and Limitations
 *  - This method must not be called before the singleton memory manager has
 *     been created or after it has been destroyed.
 *     A fatal error results when this is not true.
 *  - The allocated memory is not constructed by this method.
 *     The calling routine should initialize the memory with placement new.
 *  - Access to this method is through the JEOD memory allocation macros.
 *     Use in any other context is caveat emptor.
 * @return Allocated memory
 * \param[in] is_array Memory constructed by new[] if set
 * \param[in] nelems Number of elements to be allocated
 * \param[in] fill Byte fill pattern
 * \param[in] tentry Type entry
 * \param[in] file Source file containing JEOD_ALLOC
 * \param[in] line Line number containing JEOD_ALLOC
 */
void *
JeodMemoryManager::create_memory (
   bool is_array,
   unsigned int nelems,
   int fill,
   const TypeEntry & tentry,
   const char * file,
   unsigned int line)
{
   void * addr = NULL;       // -- Allocated memory

   // Throw a fatal error if the singleton memory manager is not available.
   if (check_master (true, __LINE__)) {

      // Pass the call on to the singular memory manager.
      addr = Master->create_memory_internal (
                is_array, nelems, fill, tentry, file, line);
   }

   return addr;
}


/**
 * Query whether some address was allocated by JEOD.
 *
 * \par Assumptions and Limitations
 *  - This method must not be called before the singleton memory manager has
 *     been created or after it has been destroyed.
 *     A fatal error results when this is not true.
 * @return True if allocated by JEOD
 * \param[in] addr Memory to be queried
 * \param[in] file Source file containing query
 * \param[in] line Line number containing query
 */
bool
JeodMemoryManager::is_allocated (
   const void * addr,
   const char * file,
   unsigned int line)
{
   bool allocated = false;

   // Throw a non-fatal error if the singleton memory manager is not available.
   if (check_master (false, __LINE__)) {

      // Pass the call on to the singular memory manager.
      allocated = Master->is_allocated_internal (addr, file, line);
   }

   return allocated;
}


/**
 * Destroy memory previously registered with JEOD.
 *
 * \par Assumptions and Limitations
 *  - This method must not be called before the singleton memory manager has
 *     been created or after it has been destroyed.
 *     A fatal error results when this is not true.
 *  - The provided memory shall not be used in any way after calling this method.
 *     This method destructs and frees that memory.
 *  - Access to this method is through the JEOD memory allocation macros.
 *     Use in any other context is caveat emptor.
 * \param[in,out] addr Memory to be destroyed
 * \param[in] delete_array DELETE_ARRAY (true) vs. DELETE_OBJECT
 * \param[in] file Source file containing delete
 * \param[in] line Line number containing delete
 */
void
JeodMemoryManager::destroy_memory (
   void * addr,
   bool delete_array,
   const char * file,
   unsigned int line)
{

   // Throw a non-fatal error if the singleton memory manager is not available.
   if (check_master (false, __LINE__)) {

      // Pass the call on to the singular memory manager.
      Master->destroy_memory_internal (
         addr, delete_array, file, line);
   }
}


/**
 * Register a checkpointable object with the memory manager.
 *
 * \par Assumptions and Limitations
 *  - This method must not be called before the singleton memory manager has
 *     been created or after it has been destroyed.
 *     A fatal error results when this is not true.
 * \param[in] container Object container
 * \param[in] container_type Container type info
 * \param[in] elem_name Element name
 * \param[in,out] checkpointable Checkpointable object
 */
void
JeodMemoryManager::register_container (
   const void * container,
   const std::type_info & container_type,
   const char * elem_name,
   JeodCheckpointable & checkpointable)
{

   // Throw a fatal error if the singleton memory manager is not available.
   if (check_master (true, __LINE__)) {

      const JeodMemoryTypeDescriptor * tdesc (
         Master->get_type_descriptor_atomic (container_type));

      // Protect against A null type descriptor (otherwise get core dump)
      if (tdesc == NULL) {
         std::string type_name(NamedItem::demangle(container_type));
         MessageHandler::fail (
            __FILE__, __LINE__, MemoryMessages::null_pointer,
            "Illegal attempt to register container %s:%s contained in %p\n"
            "Class %s has not been registered with the memory manager.",
            type_name.c_str(), elem_name, container, type_name.c_str());
      }

      // Register the checkpointable object with the sim interface.
      Master->sim_interface.register_container (
         container, *tdesc, elem_name, checkpointable);

      // Tell the checkpointable object to register / store type info.
      checkpointable.initialize_checkpointable (
         container, container_type, elem_name);
   }
}


/**
 * Deregister all checkpointable object contained within some object.
 *
 * \par Assumptions and Limitations
 *  - This method must not be called before the singleton memory manager has
 *     been created or after it has been destroyed.
 *     A fatal error results when this is not true.
 * \param[in] container Object container
 * \param[in] container_type Container type info
 * \param[in] elem_name Element name
 * \param[in,out] checkpointable Checkpointable object
 */
void
JeodMemoryManager::deregister_container (
   const void * container,
   const std::type_info & container_type,
   const char * elem_name,
   JeodCheckpointable & checkpointable)
{

   // Throw a fatal error if the singleton memory manager is not available.
   if (check_master (true, __LINE__)) {

      const JeodMemoryTypeDescriptor * tdesc (
         Master->get_type_descriptor_atomic (container_type));

      // Protect against A null type descriptor (otherwise get core dump)
      if (tdesc == NULL) {
         std::string type_name(NamedItem::demangle(container_type));
         MessageHandler::fail (
            __FILE__, __LINE__, MemoryMessages::null_pointer,
            "Illegal attempt to register container %s:%s contained in %p\n"
            "Class %s has not been registered with the memory manager.",
            type_name.c_str(), elem_name, container, type_name.c_str());
      }

      // De-register the checkpointable object with the sim interface.
      Master->sim_interface.deregister_container (
         container, *tdesc, elem_name, checkpointable);

      // Undo the external actions performed by initialize_checkpointable.
      checkpointable.undo_initialize_checkpointable (
         container, container_type, elem_name);
   }
}


/**
 * Set the memory manager's simulation interface mode.
 *
 * \par Assumptions and Limitations
 *  - This method must not be called before the singleton memory manager has
 *     been created or after it has been destroyed.
 *     A fatal error results when this is not true.
 * \param[in] new_mode New mode
 */
void
JeodMemoryManager::set_mode (
   JeodSimulationInterface::Mode new_mode)
{

   // Throw a fatal error if the singleton memory manager is not available.
   if (check_master (true, __LINE__)) {

      // Tell the master memory manager about the new mode.
      Master->set_mode_internal (new_mode);
   }
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
