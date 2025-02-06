/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup SimInterface
 * @{
 *
 * @file models/utils/sim_interface/src/trick_memory_interface_alloc.cc
 * Define JeodTrickMemoryInterface methods related to allocation/deallocation.
 */

/*******************************************************************************

Purpose:
  ()

Library Dependency:
  ((trick_memory_interface_alloc.cc)
   (trick_memory_interface.cc))

 

*******************************************************************************/


#ifdef TRICK_VER

// System includes
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <dlfcn.h>
#include <iomanip>
#include <sstream>
#include <typeinfo>

// Trick includes
#include "sim_services/MemoryManager/include/attributes.h"
#include "sim_services/MemoryManager/include/ADefParseContext.hh"
#include "sim_services/MemoryManager/include/MemoryManager.hh"
extern Trick::MemoryManager * trick_MM;

// JEOD includes
#include "utils/memory/include/memory_item.hh"
#include "utils/memory/include/memory_manager.hh"
#include "utils/memory/include/memory_type.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/sim_interface_messages.hh"
#include "../include/simulation_interface.hh"
#include "../include/trick_memory_interface.hh"


//! Namespace jeod
namespace jeod {

/**
 * Register newly allocated memory with Trick.
 *
 * \par Assumptions and Limitations
 *  - Memory was indeed allocated.
 *  - The input address is not null.
 *  - The number of elements is positive.
 * @return True if registered
 * \param[in] addr Allocated memory
 * \param[in] item Description of the memory
 * \param[in] tdesc Description of the type
 * \param[in] file Source file containing JEOD_ALLOC
 * \param[in] line Line number containing JEOD_ALLOC
 */
bool
JeodTrickMemoryInterface::register_allocation (
   const void * addr,
   const JeodMemoryItem & item,
   const JeodMemoryTypeDescriptor & tdesc,
   const char * file,
   unsigned int line)
{
   unsigned int nelems = item.get_nelems();
   uint32_t unique_id = item.get_unique_id();

   // Register for checkpoint/restart in the allocation map.
   allocation_map.insert (
      AllocationMap::value_type (
         unique_id,
         AllocationMapEntry (
            tdesc.get_typeid(), nelems, item.get_is_array())));

   // Construct a string that emulates the declaration of the memory.
   std::stringstream sstream;
   sstream << tdesc.get_name()
           << " " << construct_identifier(unique_id)
           << "[" << nelems << "]";

   // Construct a context for parsing the above string.
   // Note: Requires Trick 10.2 or greater.
   Trick::ADefParseContext context(&sstream);

   // Parse the declaration context and register the memory.
   if ((ADEF_parse (&context) != 0) ||
       (trick_MM->declare_extern_var (
           const_cast <void *>(addr),
           context.type,
           context.user_type_name,
           context.n_stars,
           context.var_name,
           context.n_cdims,
           context.cdims) == nullptr)) {

      // Handle failed parse / failed registration.
      MessageHandler::error (
         __FILE__, __LINE__, SimInterfaceMessages::interface_error,
         "Memory registration failed with allocation at %s:%d",
         file, line);
      return false;
   }

   return true;
}


/**
 * Delete Trick information about some pointer -- but not the pointer itself.
 *
 * \par Assumptions and Limitations
 *  - Some other agent must freeing the memory at the input address itself.
 *     This function merely deletes Trick's knowledge of that pointer.
 * \param[in] addr Allocated memory
 * \param[in] item Description of the memory
 * \param[in] tdesc Description of the type
 * \param[in] file Source file containing JEOD_ALLOC
 * \param[in] line Line number containing JEOD_ALLOC
 */
void
JeodTrickMemoryInterface::deregister_allocation (
   const void * addr,
   const JeodMemoryItem & item,
   const JeodMemoryTypeDescriptor & tdesc JEOD_UNUSED,
   const char * file,
   unsigned int line)
{

   // Erase the allocation map entry for this item.
   allocation_map.erase (item.get_unique_id());

   // Revoke the registration of the allocated memory with Trick.
   int return_code = trick_MM->delete_extern_var (const_cast <void *>(addr));

   // Handle failed deregistration.
   if (return_code != 0) {
      MessageHandler::error (
         __FILE__, __LINE__, SimInterfaceMessages::interface_error,
         "Memory deregistration failed with free at %s:%d for address 0x%p",
         file, line, addr);
      return;
   }

   return;
}

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
