/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Memory
 * @{
 *
 * @file models/utils/memory/src/memory_manager_protected.cc
 * Implement those JeodMemoryManager member functions that access data members
 * that need to be treated with care to make the memory manager thread safe.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((memory_manager.cc)
   (memory_item.cc)
   (memory_messages.cc)
   (memory_type.cc))



*******************************************************************************/

/**
 * \addtogroup classes
 * @{
 */

// System includes
#define __STDC_LIMIT_MACROS
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <map>
#include <pthread.h>
#include <sstream>
#include <typeinfo>

// JEOD includes
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/memory_item.hh"
#include "../include/memory_manager.hh"
#include "../include/memory_messages.hh"

//! Namespace jeod
namespace jeod
{

/*******************************************************************************
 * begin_atomic_block and end_atomic_block
 ******************************************************************************/

/**
 * Prepare for a set of operations that must be done atomically.
 *
 * \par Assumptions and Limitations
 *  - This method must be used in conjunction with end_atomic_block.@code
 *        try {
 *           begin_atomic_block ();
 *           operate_on_protected_members();
 *           end_atomic_block (false);
 *        }
 *        catch (...) {
 *           end_atomic_block (true);
 *           throw;
 *        }
 *     @endcode
 *  - See the class header for a detailed description.
 */

/**
 * Purpose:
 *   (Prepare for a set of operations that must be done atomically.)
 * Assumptions and limitations:
 *   This method must be used in conjunction with end_atomic_block.@code
 *        try {
 *           begin_atomic_block ();
 *           operate_on_protected_members();
 *           end_atomic_block (false);
 *        }
 *        catch (...) {
 *           end_atomic_block (true);
 *           throw;
 *        }
 *     @endcode
 *    (See the class header for a detailed description.))
 */
void JeodMemoryManager::begin_atomic_block() const
{
    // Try to lock the mutex.
    int mutex_lock_status = pthread_mutex_lock(&mutex);

    // Failure to obtain a lock is a fatal error.
    if(mutex_lock_status != 0)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             MemoryMessages::lock_error,
                             "pthread_mutex_lock() failed: %s",
                             std::strerror(mutex_lock_status));
    }
}

/**
 * End an atomic set of operations.
 * \param[in] ignore_errors Ignore errors from unlock?
 */
void JeodMemoryManager::end_atomic_block(bool ignore_errors) const
{
    // Try to unlock the mutex.
    int mutex_unlock_status = pthread_mutex_unlock(&mutex);

    // Failure to release a lock is a fatal error.
    if((!ignore_errors) && (mutex_unlock_status != 0))
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             MemoryMessages::lock_error,
                             "pthread_mutex_unlock() failed: %s",
                             std::strerror(mutex_unlock_status));
    }
}

/*******************************************************************************
 * string_table methods
 ******************************************************************************/

/**
 * Retrieve the specified string from the string table.
 *
 * \par Assumptions and Limitations
 *  - Operations on the map must be atomic.
 *     This method satisfies that requirement.
 * @return String table index
 * \param[in] idx Class index
 */

/*
Purpose:
  (Retrieve the specified string from the string table.)
Assumptions and limitations:
  ((Operations on the map must be atomic.
    This method satisfies that requirement.))
*/
const std::string & JeodMemoryManager::get_string_atomic(unsigned int idx) const
{
    if(idx == 0)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             MemoryMessages::internal_error,
                             "Attempt to retrieve string table entry for reserved index 0.");
    }

    // Perform lookup with table locked.
    try
    {
        begin_atomic_block();
        const std::string * str = string_table.get(idx);
        end_atomic_block(false);
        return *str;
    }
    catch(...)
    {
        end_atomic_block(true);
        throw;
    }
}

/**
 * Add a location identifier string to the string table.
 *
 * \par Assumptions and Limitations
 *  - Operations on the map must be atomic.
 *     This method satisfies that requirement.
 * @return String table index
 * \param[in] str String to add
 */
unsigned int JeodMemoryManager::add_string_atomic(const std::string & str)
{
    unsigned int idx = 0;

    // Insert/update the table with the table locked.
    // Note that table.add adds an entry to the table only if the table lacks an
    // entry for the key. Otherwise it returns the index for the existing entry.
    try
    {
        begin_atomic_block();
        idx = string_table.add(str);
        end_atomic_block(false);
    }
    catch(...)
    {
        end_atomic_block(true);
        throw;
    }

    // Return the found or added index.
    return idx;
}

/*******************************************************************************
 * type_table methods
 ******************************************************************************/

/**
 * Retrieve the index for the specified type from the type table,
 * adding an entry if needed.
 *
 * \par Assumptions and Limitations
 *  - Operations on the type table must be atomic.
 *     This method *does not* satisfy that requirement.
 * @return True => table updated
 * \param[in] tdesc Descriptor
 * \param[out] idx Type descriptor index
 */

/*
Purpose:
  (Retrieve the index for the specified type from the type table,
   adding an entry if needed.)
Assumptions and limitations:
  ((Operations on the type table must be atomic.
    This method *does not* satisfy that requirement.))
*/
bool JeodMemoryManager::get_type_index_nolock(const JeodMemoryTypeDescriptor & tdesc, uint32_t * idx)
{
    const std::string key(tdesc.get_typeid().name());
    uint32_t index = 0;
    bool added = false;

    // Lookup the type in the table, keyed by the RTTI mangled name.
    index = type_table.find(key);

    // Found index is zero => Type was not found. Add type to the table.
    if(index == 0)
    {
        added = true;
        index = type_table.add(key, tdesc);
    }

    // Found index is not zero => Table does not need update.
    else
    {
        added = false;
    }

    *idx = index;
    return added;
}

/**
 * Return the type entry for the input type descriptor, adding the
 * descriptor to the type table if the type has not yet been registered.
 *
 * \par Assumptions and Limitations
 *  - The mangled name returned by the std::type_info name method is unique
 *     across all allocatable types and is invariant.
 *  - Operations on the map must be atomic.
 *     This method satisfies that requirement.
 * @return Type descriptor index
 * \param[in] tdesc Type pre-descriptor
 */
const JeodMemoryManager::TypeEntry JeodMemoryManager::get_type_entry_atomic(JeodMemoryTypePreDescriptor & tdesc)
{
    const std::type_info & typeid_info = tdesc.get_typeid();
    const std::string key(typeid_info.name());
    uint32_t index = 0;
    const JeodMemoryTypeDescriptor * table_tdesc = nullptr;
    bool added = false;

    // Insert/update the table with the table locked.
    try
    {
        begin_atomic_block();

        // Check if the table already has an entry for this type.
        index = type_table.find(key);

        // Add the type to the table if the above indicates it is absent.
        // Note that is at this point that a JeodMemoryTypeDescriptor is created.
        // A descriptor is not created if there is already an entry for the type.
        if(index == 0)
        {
            added = true;
            index = type_table.add(key, tdesc.get_descriptor());
        }

        // Get the type descriptor as it exists in the table,
        // which is now guaranteed.
        table_tdesc = type_table.get(index);

        end_atomic_block(false);
    }
    catch(...)
    {
        end_atomic_block(true);
        throw;
    }

    // Report the addition if it occurred.
    if(added && (debug_level > 1))
    {
        MessageHandler::debug(__FILE__,
                              __LINE__,
                              MemoryMessages::debug,
                              "JEOD memory debug: Added type %s to the type table",
                              table_tdesc->get_name().c_str());
    }

    // Return the found/added type index.
    return TypeEntry(index, table_tdesc);
}

/**
 * Retrieve the descriptor for the specified type from the type table.
 *
 * \par Assumptions and Limitations
 *  - Operations on the map must be atomic.
 *     This method satisfies that requirement.
 * @return Type descriptor
 * \param[in] typeid_info Type info
 */
const JeodMemoryTypeDescriptor * JeodMemoryManager::get_type_descriptor_atomic(const std::type_info & typeid_info) const
{
    const std::string key(typeid_info.name());
    const JeodMemoryTypeDescriptor * tdesc = nullptr;

    // Lookup the type descriptor with the table locked.
    try
    {
        begin_atomic_block();

        // Lookup the type in the table, keyed by the RTTI mangled name.
        uint32_t idx = type_table.find(key);

        // Get the descriptor if the above indicates the type is in the table.
        if(idx != 0)
        {
            tdesc = type_table.get(idx);
        }

        end_atomic_block(false);
    }
    catch(...)
    {
        end_atomic_block(true);
        throw;
    }

    return tdesc;
}

/**
 * Retrieve the descriptor for the specified type from the type table.
 *
 * \par Assumptions and Limitations
 *  - Operations on the map must be atomic.
 *     This method satisfies that requirement.
 * @return Type entry
 * \param[in] name_type Name type spec
 * \param[in] type_name Type name
 */
const JeodMemoryManager::TypeEntry JeodMemoryManager::get_type_entry_atomic(JeodMemoryManager::NameType name_type,
                                                                            const std::string & type_name) const
{
    uint32_t index = 0;
    const JeodMemoryTypeDescriptor * table_tdesc = nullptr;

    // Lookup the type descriptor with the table locked.
    try
    {
        begin_atomic_block();

        // Lookup is by typeid name:
        // The typeid name is the type table key, so this is a simple lookup.
        if(name_type == Typeid_type_name)
        {
            // Lookup the type in the table, keyed by the mangled name.
            index = type_table.find(type_name);

            // Get the descriptor if the above indicates the type is in the table.
            if(index != 0)
            {
                table_tdesc = type_table.get(index);
            }

            // Add an entry for the type to the table if the type isn't in it.
            else
            {
            }
        }

        // Lookup is by demangled name:
        // This is not keyed, so we need to do a search in this case.
        else
        {
            for(auto iter = type_table.begin(); iter != type_table.end(); ++iter)
            {
                const JeodMemoryTypeDescriptor * tdesc = *iter;

                // Name matches: This is the entry we're looking for.
                if(type_name.compare(tdesc->get_name()) == 0)
                {
                    index = iter - type_table.begin();
                    table_tdesc = tdesc;
                    break;
                }
            }
        }

        end_atomic_block(false);
    }
    catch(...)
    {
        end_atomic_block(true);
        throw;
    }

    return TypeEntry(index, table_tdesc);
}

/**
 * Retrieve the descriptor for the specified type from the type table.
 *
 * \par Assumptions and Limitations
 *  - The input index is non-zero. This assumption is enforced.
 *  - Operations on the map must be atomic.
 *     This method satisfies that requirement.
 * @return Type descriptor
 * \param[in] idx Type index
 */
const JeodMemoryTypeDescriptor & JeodMemoryManager::get_type_descriptor_atomic(unsigned int idx) const
{
    // An index of zero is reserved. The calling code should know that.
    if(idx == 0)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             MemoryMessages::internal_error,
                             "Attempt to retrieve type descriptor for reserved index 0.");
    }

    // Index is value: Get the corresponding descriptor with the table locked.
    try
    {
        begin_atomic_block();
        const JeodMemoryTypeDescriptor * tdesc = type_table.get(idx);
        end_atomic_block(false);
        return *tdesc;
    }
    catch(...)
    {
        end_atomic_block(true);
        throw;
    }
}

/*******************************************************************************
 * alloc_table methods
 ******************************************************************************/

/**
 * Create a unique identifier for an allocation.
 *
 * \par Assumptions and Limitations
 *  - Operations on the map must be atomic.
 *     This method satisfies that requirement.
 * @return Allocation ID
 * \param[in] file Source file containing JEOD_ALLOC
 * \param[in] line Line number containing JEOD_ALLOC
 */

/*
Purpose:
  (Create a unique identifier for an allocation.)
Assumptions and limitations:
  ((Operations on the map must be atomic.
    This method satisfies that requirement.))
*/
uint32_t JeodMemoryManager::get_alloc_id_atomic(const char * file, unsigned int line)
{
    uint32_t unique_id = 0;

    // Construct the unique identifier in protected mode.
    try
    {
        begin_atomic_block();

        // Bump the allocation number.
        ++allocation_number;

        // Check for overflow.
        if((allocation_number == UINT32_MAX) || (allocation_number == 0))
        {
            end_atomic_block(true);
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 MemoryMessages::corrupted_memory,
                                 "Memory allocation limit exceeded at %s:%d.",
                                 file,
                                 line);
            return unique_id;
        }

        // The incremented allocation number is the unique identifier.
        unique_id = allocation_number;

        end_atomic_block(false);
    }
    catch(...)
    {
        end_atomic_block(true);
        throw;
    }

    return unique_id;
}

/**
 * Reset the unique identifier for a restart.
 *
 * \par Assumptions and Limitations
 *  - Operations on the map must be atomic.
 *     This method satisfies that requirement.
 * \param[in] unique_id Unique id of a restored allocation
 */
void JeodMemoryManager::reset_alloc_id_atomic(uint32_t unique_id)
{
    // Update the allocation number in protected mode.
    try
    {
        begin_atomic_block();

        if(allocation_number < unique_id)
        {
            allocation_number = unique_id;
        }

        end_atomic_block(false);
    }
    catch(...)
    {
        end_atomic_block(true);
        throw;
    }
}

/**
 * Find the allocation table entry that matches the input address,
 * and delete it if delete_entry is true.
 *
 * The matching is strict. A match occurs only if the input address is a key in
 * the allocation table. An error is reported if the input address is inside
 * the allocated space corresponding to one of the allocation table entries.
 *
 * Output values:
 *  - Entry not found:
 *    - The @a found_addr and @a found_type are set to NULL.
 *    - The @a found_item is not touched.
 *  - Entry found:
 *    - The @a found_addr is set to the key of the found entry.
 *    - The @a found_item is copied from the value of the found entry.
 *    - The @a found_type points to the type_descriptor entry for the
 *      found item's type.
 *
 * \par Assumptions and Limitations
 *  - Operations on the map must be atomic.
 *     This method satisfies that requirement.
 * \param[in] addr Address
 * \param[in] delete_entry Indicates entry is to be deleted
 * \param[in] file Source file containing JEOD_XXX
 * \param[in] line Line number containing JEOD_XXX
 * \param[out] found_addr Address found in table
 * \param[out] found_item Descriptor for above
 * \param[out] found_type Type descriptor
 */
void JeodMemoryManager::find_alloc_entry_atomic(const void * addr,
                                                bool delete_entry,
                                                const char * file,
                                                unsigned int line,
                                                void *& found_addr,
                                                JeodMemoryItem & found_item,
                                                const JeodMemoryTypeDescriptor *& found_type)
{
    bool address_inside = false;
    const void * test_addr = nullptr;
    const JeodMemoryItem * test_item = nullptr;
    const JeodMemoryTypeDescriptor * test_type = nullptr;

    // Set the output values to indicate the address was not found.
    found_addr = nullptr;
    found_type = nullptr;

    try
    {
        begin_atomic_block();

        // Find first entry whose address is strictly greater than
        // the subject address.
        auto iter = alloc_table.upper_bound(addr);

        // There is no match if the upper bound is the beginning of the table.
        // If the found iterator is not begin(), the entry before the found
        // entry *might* be a match. It is:
        // - A match if the input address and entry key are equal.
        // - Not a match if the input address is outside of the memory space
        //   represented by found entry.
        // - A mismatch if the input address is inside the memory space
        //   represented by found entry.
        if(iter != alloc_table.begin())
        {
            --iter;
            test_addr = iter->first;
            test_item = &(iter->second);
            test_type = &(get_type_descriptor_nolock(iter->second));

            // Test as described above.
            // Match: Set output values and delete if requested to do so.
            if(addr == test_addr)
            {
                // Set the outputs. Note that found_item is copied.
                found_addr = const_cast<void *>(test_addr);
                found_item = *test_item;
                found_type = test_type;

                // Delete if requested to do so.
                if(delete_entry)
                {
                    // Update the allocation statistics.
                    cur_data_size -= found_type->buffer_size(found_item);

                    // Delete the entry.
                    alloc_table.erase(iter);
                }
                // No else: Nothing to do here if delete_entry is false.
            }

            // Mismatch: Set error flag so condition can be reported.
            // (The error report is generated outside the atomic block.)
            else if(addr < test_type->buffer_end(test_addr, *test_item))
            {
                address_inside = true;
            }
            // No else: Output items set to indicate not_found at onset.
        }
        // No else: Output items set to indicate not_found at onset.

        end_atomic_block(false);
    }
    catch(...)
    {
        end_atomic_block(true);
        throw;
    }

    if(address_inside)
    {
        const char allocationStrings[2][18] = {"JEOD_IS_ALLOCATED", "JEOD_DELETE"};
        int index = 1;
        if(delete_entry)
        {
            index = 0;
        }
        MessageHandler::warn(__FILE__,
                             __LINE__,
                             MemoryMessages::suspect_pointer,
                             "Suspect use of %s at %s:%d\n",
                             "Pointer %p points inside allocated block of type %s.",
                             allocationStrings[index],
                             file,
                             line,
                             addr,
                             test_type->get_name().c_str());
    }
}

/**
 * Add the specified addr/item pair to the table.
 *
 * \par Assumptions and Limitations
 *  - Operations on the map must be atomic.
 *     This method satisfies that requirement.
 *  - The specified address must not already be in the table.
 * \param[in] addr Newly allocated memory
 * \param[in] item Description of that memory
 * \param[in] tdesc Description of the type
 * \param[in] file Source file containing JEOD_ALLOC
 * \param[in] line Line number containing JEOD_ALLOC
 */
void JeodMemoryManager::add_allocation_atomic(const void * addr,
                                              const JeodMemoryItem & item,
                                              const JeodMemoryTypeDescriptor & tdesc,
                                              const char * file,
                                              unsigned int line)
{
    // Insert/update the table with the table locked.
    try
    {
        begin_atomic_block();

        // Sanity check: The new buffer should not overlap with registered memory.
        // First find the table entries whose addresses are:
        //   beg_iter -- strictly greater than the start of the buffer
        //   end_iter -- greater than or equal to the end of the buffer
        auto beg_iter(alloc_table.upper_bound(addr));
        auto end_iter(alloc_table.lower_bound(tdesc.buffer_end(addr, item.get_nelems())));

        // Now bump the beg_iter down a notch if its predecessor contains
        // the start of the buffer.
        if(beg_iter != alloc_table.begin())
        {
            --beg_iter;
            // Oops. Undo the above if there is no overlap with the predecessor.
            if(addr >= get_type_descriptor_nolock(beg_iter->second).buffer_end(beg_iter->first, beg_iter->second))
            {
                ++beg_iter;
            }
        }

        // We have an overlap if the end_iter is not equal to the beg_iter.
        // Possible causes:
        //  1. C++ delete was used to delete the overlapping memory.
        //  2. The memory manager was called outside the jeod_alloc.hh context.
        //  3. Something is terribly fouled up.
        // Not knowing which is which, the prudent thing to do is to fail the sim.
        if(beg_iter != end_iter)
        {
            end_atomic_block(true);
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 MemoryMessages::corrupted_memory,
                                 "The memory manager is corrupted:\n"
                                 "Memory allocated at %s:%d overlaps with registered memory.",
                                 file,
                                 line);
            return;
        }

        // No overlap: Insert the item in the table.
        std::pair<AllocTable::iterator, bool> ret = alloc_table.insert(AllocTable::value_type(addr, item));

        // The above *had* to work. The overlap check ensures no overlap.
        // Just in case it didn't, ...
        if(!ret.second)
        {
            end_atomic_block(true);
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 MemoryMessages::corrupted_memory,
                                 "Impossible condition!\n"
                                 "Memory allocated at %s:%d has already been registered.",
                                 file,
                                 line);
            return;
        }

        // Update stats on allocated memory and the tables.
        cur_data_size += tdesc.buffer_size(item);

        if(max_data_size < cur_data_size)
        {
            max_data_size = cur_data_size;
        }

        if(max_table_size < alloc_table.size())
        {
            max_table_size = alloc_table.size();
        }

        end_atomic_block(false);
    }
    catch(...)
    {
        end_atomic_block(true);
        throw;
    }
}

/**
 * Find and delete the alloc table entry with the smallest unique id,
 * setting the provided references with info about the deleted item.
 * The addr and type are set to NULL if the table is empty.
 *
 * \par Assumptions and Limitations
 *  - Operations on the map must be atomic.
 *     This method satisfies that requirement.
 *  - If the restore doesn't work the sim will be knee deep in alligators.
 * \param[out] addr Address found in table
 * \param[out] item Descriptor for above
 * \param[out] type Type descriptor
 */
void JeodMemoryManager::delete_oldest_alloc_entry_atomic(void *& addr,
                                                         JeodMemoryItem & item,
                                                         const JeodMemoryTypeDescriptor *& type)
{
    // Insert/update the table with the table locked.
    try
    {
        begin_atomic_block();

        // We are done when the table is finally empty.
        if(alloc_table.empty())
        {
            allocation_number = 0;
            addr = nullptr;
            type = nullptr;
        }

        // Not done. Find the oldest element, set outputs, and delete.
        else
        {
            AllocTable::iterator target_iter;
            uint32_t target_id = UINT32_MAX;

            // Find the oldest element -- the one with the smallest unique id.
            for(auto iter = alloc_table.begin(); iter != alloc_table.end(); ++iter)
            {
                uint32_t iter_id = iter->second.get_unique_id();
                if(iter_id < target_id)
                {
                    target_iter = iter;
                    target_id = iter_id;
                }
            }

            // Save info about this found element in the function args.
            addr = const_cast<void *>(target_iter->first);
            item = target_iter->second;
            type = &(get_type_descriptor_nolock(item));

            // Update the allocation statistics.
            cur_data_size -= type->buffer_size(item);

            // Delete the entry.
            alloc_table.erase(target_iter);
        }

        end_atomic_block(false);
    }
    catch(...)
    {
        end_atomic_block(true);
        throw;
    }
}

} // namespace jeod

/**
 * @}
 */

/**
 * @}
 * @}
 * @}
 */
