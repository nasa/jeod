/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup SimInterface
 * @{
 *
 * @file models/utils/sim_interface/src/trick_memory_interface_chkpnt.cc
 * Define JeodTrick10MemoryInterface methods related to checkpoint/restart.
 */

/*******************************************************************************

Purpose:
  ()

Library Dependency:
  ((trick_memory_interface_chkpnt.o)
   (trick10_memory_interface.o)
   (utils/container/primitive_serializer.o))

 

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
#include "utils/container/include/checkpointable.hh"
#include "utils/memory/include/memory_item.hh"
#include "utils/memory/include/memory_manager.hh"
#include "utils/memory/include/memory_type.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/sim_interface_messages.hh"
#include "../include/simulation_interface.hh"
#include "../include/trick10_memory_interface.hh"


//! Namespace jeod
namespace jeod {

/**
 * Register the checkpointable object with Trick.
 * This function is typically called at construction or initialization time
 * via JEOD_REGISTER_CHECKPOINTABLE.
 *
 * \par Assumptions and Limitations
 *  - The following unenforced assumptions are made:
 *     - Sim objects have been constructed and registered with Trick.
 *     - Checkpointable objects are unique.
 *     - Trick has been pre-initialized.
 *     - Not in shutdown mode.
 *     .
 *     Enforcement of the above is the responsibility the simulation developer,
 *     the JEOD memory manager, and the simulation interface.
 * \param[in] owner Owner of the container
 * \param[in] owner_type Owner type descriptor
 * \param[in] elem_name Container element
 * \param[in,out] container The container
 */
void
JeodTrick10MemoryInterface::register_container (
   const void * owner,
   const JeodMemoryTypeDescriptor & owner_type,
   const char * elem_name,
   JeodCheckpointable & container)
{

   // The owner and element name must be non-null.
   if (owner == NULL) {
      MessageHandler::error (
         __FILE__, __LINE__, SimInterfaceMessages::interface_error,
         "Invalid call to register_container: owner must be non-null.");
      return;
   }

   if (elem_name == NULL) {
      MessageHandler::error (
         __FILE__, __LINE__, SimInterfaceMessages::interface_error,
         "Invalid call to register_container: elem_name must be non-null.");
      return;
   }

   // Check for a duplicate entry.
   const void * derived = owner_type.most_derived_pointer (owner);
   for (ContainerList::iterator iter = container_list.begin();
        iter != container_list.end();
        ++iter) {
      ContainerListEntry & entry = *iter;
      if ((entry.owner_type.most_derived_pointer (entry.owner) == derived) &&
          (entry.elem_name == elem_name)) {
         MessageHandler::error (
            __FILE__, __LINE__, SimInterfaceMessages::interface_error,
            "Duplicate container entry %s.",
            get_container_id (entry).c_str());
         return;
      }
   }


   // Add the checkpointable object to the list of checkpointable objects.
   container_list.push_back (
      ContainerListEntry (owner, owner_type, elem_name, container));
}


/**
 * Revoke the registrations performed by register_container.
 * This function is typically called at destruction time
 * via JEOD_DEREGISTER_CHECKPOINTABLE.
 *
 * \par Assumptions and Limitations
 *  - The following unenforced assumptions are made:
 *     - A corresponding register_container was previously made.
 *     - Trick has been pre-initialized.
 *     .
 *     Enforcement of the above is the responsibility the simulation developer,
 *     the JEOD memory manager, and the simulation interface.
 * \param[in] owner Owner of the container
 * \param[in] owner_type Owner type descriptor
 * \param[in] elem_name Container element
 * \param[in,out] container The container
 */
void
JeodTrick10MemoryInterface::deregister_container (
   const void * owner,
   const JeodMemoryTypeDescriptor & owner_type,
   const char * elem_name,
   JeodCheckpointable & container JEOD_UNUSED)
{

   // The owner and element name must be non-null.
   if (owner == NULL) {
      MessageHandler::error (
         __FILE__, __LINE__, SimInterfaceMessages::interface_error,
         "Invalid call to deregister_container: owner must be non-null.");
      return;
   }

   if (elem_name == NULL) {
      MessageHandler::error (
         __FILE__, __LINE__, SimInterfaceMessages::interface_error,
         "Invalid call to deregister_container: elem_name must be non-null.");
      return;
   }

   // Find the entry.
   const void * derived = owner_type.most_derived_pointer (owner);
   ContainerList::iterator iter;
   for (iter = container_list.begin();
        iter != container_list.end();
        ++iter) {
      ContainerListEntry & entry = *iter;
      if ((entry.owner_type.most_derived_pointer (entry.owner) == derived) &&
          (entry.elem_name == elem_name)) {
         break;
      }
   }

   // Not found: Complain and exit.
   if (iter == container_list.end()) {
      MessageHandler::error (
         __FILE__, __LINE__, SimInterfaceMessages::interface_error,
         "Could not find entry '%s' for specified container.",
         elem_name);
         return;
   }

   // Remove the checkpointable object from the list of checkpointable objects.
   container_list.erase (iter);
}


/**
 * Construct the identifier for a checkpointable object.
 * @return Container ID
 * \param[in] entry Container list entry
 */
std::string
JeodTrick10MemoryInterface::get_container_id (
   const ContainerListEntry & entry)
const
{
   std::string ident;
   const void * owner = entry.owner;
   const JeodMemoryTypeDescriptor & owner_type = entry.owner_type;
   const ATTRIBUTES & owner_attr = owner_type.get_attr();


   // Get the name of the owner.
   ident = translate_addr_to_name (owner, &owner_attr);

   if ((ident.length() == 0) ||
       (translate_name_to_addr (ident) != owner)) {
      ident = "";
   }
   else {

      // Get rid of the leading '&'.
      if (ident[0] == '&') {
         ident.erase (0, 1);
      }

      // Get rid of the offset, if any (e.g., " + 42" in "foo.bar.baz + 42").
      std::size_t ppos = ident.find_first_of (" +");
      if (ppos < ident.size()) {
         ident.erase (ppos, ident.size()-ppos);
      }

      // Add the element name to the owner to construct the identifier.
      ident += '.';
      ident += entry.elem_name;
   }

   return ident;
}


/**
 * Dump the checkpointable objects to the checkpoint file.
 */
void
JeodTrick10MemoryInterface::checkpoint_containers (
   void)
{
   SectionedOutputStream writer (
      JeodSimulationInterface::get_checkpoint_writer ("JEOD_containers"));

   // Activate the writer.
   if (! writer.activate()) {
      MessageHandler::error (
         __FILE__, __LINE__, SimInterfaceMessages::interface_error,
         "Unable to create JEOD_containers checkpoint writer.");
      return;
   }

   // Checkpoint each of the containers.
   for (ContainerList::iterator iter = container_list.begin();
        iter != container_list.end();
        ++iter) {
      ContainerListEntry & entry = *iter;

      const std::string & identifier = get_container_id (entry);
      JeodCheckpointable & checkpointable = iter->container;

      // Add identifier.init_action() to the checkpoint section,
      // but only if the init_action is not the empty string.
      const std::string & init_action = checkpointable.get_init_name();
      if (! init_action.empty()) {
         const std::string & value  = checkpointable.get_init_value();
         writer << identifier << "." << init_action << "(" << value << ");\n";
      }

      // Walk over the checkpointable, writing entries of the form
      //    identifier.action(value)
      // to the checkpoint section until the checkpointable says it is done.
      for (checkpointable.start_checkpoint();
           !checkpointable.is_checkpoint_finished();
           checkpointable.advance_checkpoint()) {
         const std::string & action = checkpointable.get_item_name();
         const std::string & value  = checkpointable.get_item_value();
         writer << identifier << "." << action << "(" << value << ");\n";
      }

      // Add identifier.final_action() to the checkpoint section,
      // but only if the final_action is not the empty string.
      const std::string & final_action = checkpointable.get_final_name();
      if (! final_action.empty()) {
         const std::string & value  = checkpointable.get_final_value();
         writer << identifier << "." << final_action << "(" << value << ");\n";
      }
   }

   // Deactivate the writer.
   // This enables a post_checkpoint action to write its own checkpoint section.
   writer.deactivate ();

   // Perform the containers' post-checkpoint activities.
   for (ContainerList::iterator iter = container_list.begin();
        iter != container_list.end();
        ++iter) {
      JeodCheckpointable & checkpointable = iter->container;
      checkpointable.post_checkpoint();
   }
}


/**
 * Restore the checkpointable objects from the checkpoint file.
 */
void
JeodTrick10MemoryInterface::restore_containers (
   void)
{
   std::string line;
   std::size_t open_paren;
   std::size_t close_paren;
   std::size_t last_dot;
   std::string ident;
   std::string action;
   std::string value;

   typedef std::map <const std::string, JeodCheckpointable *> ContainerMap;
   ContainerMap container_map;

   // Create an entry in the container map for each valid entry in the
   // container list.
   for (ContainerList::iterator iter = container_list.begin();
        iter != container_list.end();
        ++iter) {
      ContainerListEntry & entry = *iter;

      std::string key = get_container_id (entry);

      if (key.length() > 0) {
         container_map.insert (
            ContainerMap::value_type (key, &entry.container));
      }
      // No else: Error condition already reported.
   }

   // Create a reader to read from the containers section of the
   // JEOD checkpoint file.
   SectionedInputStream reader (
      JeodSimulationInterface::get_checkpoint_reader ("JEOD_containers"));

   // Activate the reader.
   if (! reader.activate()) {
      MessageHandler::error (
         __FILE__, __LINE__, SimInterfaceMessages::interface_error,
         "Unable to create JEOD_containers checkpoint reader.");
      return;
   }

   // Read lines from the container checkpoint section.
   // Non-blank lines are of the form identifier.action (value).
   // Split each non-blank line into identifier, action, and value strings
   // and then direct the container corresponding to the identifier to perform
   // the indicated action.
   while (std::getline (reader, line)) {
      // Skip blank lines.
      if (line.length() == 0) {
         continue;
      }

      open_paren  = line.find_first_of ('(');
      close_paren = line.find_last_of  (')');
      last_dot    = line.find_last_of  ('.', open_paren);

      if ((open_paren == std::string::npos) ||
          (close_paren == std::string::npos) ||
          (last_dot == std::string::npos) ||
          (close_paren < open_paren)) {
         MessageHandler::error (
            __FILE__, __LINE__, SimInterfaceMessages::interface_error,
            "Badly formatted checkpoint file line '%s'\n"
            "Skipping processing of the line.",
            line.c_str());
         continue;
      }

      // Split the line into the identifier, action, and value.
      ident  = line.substr (0, last_dot);
      action = line.substr (last_dot + 1, open_paren - last_dot - 1);
      value  = line.substr (open_paren + 1, close_paren - open_paren - 1);

      // Find the entry for this identifier in the map.
      ContainerMap::iterator iter = container_map.find (ident);

      // Object not found: Error.
      if (iter == container_map.end()) {
         MessageHandler::error (
            __FILE__, __LINE__, SimInterfaceMessages::interface_error,
            "Unable to find container for checkpoint file line '%s'\n"
            "Skipping processing of the line.",
            line.c_str());
         continue;
      }

      // Object found: Have it perform the action.
      JeodCheckpointable * checkpointable = iter->second;
      int status = checkpointable->perform_restore_action (action, value);

      // Action failed: Report.
      if (status != 0) {
         MessageHandler::error (
            __FILE__, __LINE__, SimInterfaceMessages::interface_error,
            "Proccessing failed for checkpoint file line '%s'",
            line.c_str());
      }
   }

   // Deactivate the reader.
   // This enables a post_restart action to read its own restart section.
   reader.deactivate ();

   // Perform the containers' post-restart activities.
   for (ContainerList::iterator iter = container_list.begin();
        iter != container_list.end();
        ++iter) {
      JeodCheckpointable & checkpointable = iter->container;
      checkpointable.post_restart();
   }
}


/**
 * Dump the allocation information to the checkpoint file.
 */
void
JeodTrick10MemoryInterface::checkpoint_allocations (
   void)
{
   std::stringstream sstream;
   SectionedOutputStream writer (
      JeodSimulationInterface::get_checkpoint_writer ("JEOD_allocations"));

   // Activate the writer.
   if (! writer.activate()) {
      MessageHandler::error (
         __FILE__, __LINE__, SimInterfaceMessages::interface_error,
         "Unable to create JEOD_allocations checkpoint writer.");
      return;
   }


   // Perform the containers' pre-checkpoint activities.
   // Note that these calls may allocate memory; those allocations and the
   // contents of the allocated memory will be checkpointed,
   for (ContainerList::iterator iter = container_list.begin();
        iter != container_list.end();
        ++iter) {
      JeodCheckpointable & checkpointable = iter->container;
      checkpointable.pre_checkpoint();
   }


   // Record data allocations.
   for (AllocationMap::iterator iter = allocation_map.begin();
        iter != allocation_map.end();
        ++iter) {
      uint32_t unique_id = iter->first;
      const AllocationMapEntry & entry = iter->second;
      const std::type_info & typeid_info = entry.typeid_info;
      const JeodMemoryTypeDescriptor * tdesc =
          JeodMemoryManager::get_type_descriptor (typeid_info);

      // Record the allocation, but only if we have a valid type descriptor.
      if (tdesc != NULL) {
         std::string size_spec;
         if (entry.is_array) {
            sstream.clear ();
            sstream.str ("");
            sstream << "[" << entry.nelements << "]";
            size_spec = sstream.str();
         }
         // Output is of the form "mangled_type_name item_spec; comment"
         // item_spec is "id_num" for objects, "id_num[size]" for arrays.
         // comment contains the demangled name, the identifier, and size.
         writer << typeid_info.name()
                << " "
                << unique_id
                << size_spec
                << "; // "
                << tdesc->get_name()
                << " "
                << construct_identifier (unique_id)
                << size_spec
                << "\n";
      }
      // No else: Error has already been addressed.
   }
}


/**
 * Restore the allocated data per the checkpoint file.
 * \param[in,out] memory_manager JEOD memory manager
 */
void
JeodTrick10MemoryInterface::restore_allocations (
   JeodMemoryManager & memory_manager)
{
   std::string line;
   std::stringstream sstream;
   std::string mangled_type_name;
   std::string ident_and_size;
   std::size_t semicolon_pos;
   std::size_t end_unique_id_pos;
   uint32_t unique_id;
   bool is_array;
   uint32_t nelements;


   SectionedInputStream reader (
      JeodSimulationInterface::get_checkpoint_reader ("JEOD_allocations"));

   // Activate the reader.
   if (! reader.activate()) {
      MessageHandler::error (
         __FILE__, __LINE__, SimInterfaceMessages::interface_error,
         "Unable to create JEOD_allocations checkpoint reader.");
      return;
   }


   // Perform the containers' pre-restart activities.
   // Note that these calls may release memory and other resources.
   // The pre-restart method should not allocate memory, nor should they
   // access the checkpoint input file.
   for (ContainerList::iterator iter = container_list.begin();
        iter != container_list.end();
        ++iter) {
      JeodCheckpointable & checkpointable = iter->container;
      checkpointable.pre_restart();
   }

   // Clear all allocated memory.
   memory_manager.restart_clear_memory ();

   // Read the JEOD_allocations section of the input checkpoint file.
   // This section comprises lines of the form <type> <name>[size].
   while (std::getline (reader, line)) {
      // Skip blank lines.
      if (line.length() == 0) {
         continue;
      }

      // Split the line into two words, the mangled type name and the rest.
      sstream.clear ();
      sstream.str (line);
      sstream >> mangled_type_name >> ident_and_size;

      // ident_and_size should be comprise
      // - A sequence of digits
      // - An optional size spec of the form "[<digits>]"
      // - A terminating semicolon
      end_unique_id_pos = ident_and_size.find_first_not_of ("0123456789");
      semicolon_pos = ident_and_size.find (';');

      // Sanity check: Make sure we have something close to the above form.
      if ((end_unique_id_pos == 0) ||
          (semicolon_pos != ident_and_size.length() - 1)) {
         MessageHandler::error (
            __FILE__, __LINE__, SimInterfaceMessages::interface_error,
            "Badly formatted checkpoint file line '%s'\n"
            "Skipping processing of the line.",
            line.c_str());
         continue;
      }

      // Convert the identifier string to a number.
      sstream.clear ();
      sstream.str (ident_and_size);
      sstream >> unique_id;

      // String is all digits: We have an object rather than an array.
      if (end_unique_id_pos == semicolon_pos) {
         nelements = 1;
         is_array = false;
      }

      // Not all digits: The post-identifier should be of the form "[size]".
      else {
         if ((ident_and_size[end_unique_id_pos] != '[') ||
             (ident_and_size[ident_and_size.length()-2] != ']')) {
            MessageHandler::error (
               __FILE__, __LINE__, SimInterfaceMessages::interface_error,
               "Badly formatted checkpoint file line '%s'\n"
               "Skipping processing of the line.",
               line.c_str());
            continue;
         }

         // Form is good. Extract the array dimension.
         sstream.clear ();
         sstream.str (ident_and_size.substr(end_unique_id_pos+1));
         sstream >> nelements;
         is_array = true;
      }

      // Restore the allocation.
      memory_manager.restart_reallocate (
         mangled_type_name, unique_id, nelements, is_array);
   }
}


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
