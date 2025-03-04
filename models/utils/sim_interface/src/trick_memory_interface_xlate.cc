/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup SimInterface
 * @{
 *
 * @file models/utils/sim_interface/src/trick_memory_interface_xlate.cc
 * Define JeodTrickMemoryInterface methods related to name translation.
 */

/*******************************************************************************

Purpose:
  ()

Library Dependency:
  ((trick_memory_interface_xlate.cc)
   (trick10_memory_interface.cc))



*******************************************************************************/

#ifdef TRICK_VER

// System includes
#include <cstddef>
#include <cstdlib>
#include <string>

// Trick includes
#include <iosfwd>
using std::ostream;
#include "sim_services/CheckPointAgent/include/ClassicCheckPointAgent.hh"
#include "sim_services/MemoryManager/include/MemoryManager.hh"
#include "sim_services/MemoryManager/include/attributes.h"
#include "sim_services/MemoryManager/include/memorymanager_c_intf.h"
extern Trick::MemoryManager * trick_MM;
#if((TRICK_VER > 13) || (TRICK_VER == 13 && TRICK_MINOR >= 3))
// New API as of Trick 13.3.
#include "sim_services/CheckPointRestart/include/CheckPointRestart_c_intf.hh"
#else
// Trick versions <= 13.2
#include "sim_services/MemoryManagerWrapper/include/MemoryManagerWrapper_c_intf.hh"
#endif

// JEOD includes
#include "utils/memory/include/memory_type.hh"

// Model includes
#include "../include/sim_interface_messages.hh"
#include "../include/simulation_interface.hh"
#include "../include/trick10_memory_interface.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Get the name of the current Trick checkpoint file.
 * @return Name of the current Trick checkpoint file.\n Units:
 * \param[in] checkpoint True for checkpoint, false for restart
 */
const std::string JeodTrick10MemoryInterface::get_trick_checkpoint_file(bool checkpoint)
{
    std::string result;
    if(checkpoint)
    {
        result = checkpoint_get_output_file();
    }
    else
    {
        result = checkpoint_get_load_file();
    }
    return result;
}

/**
 * Get the simulation name, if any, associated with the address.
 * @return Name of the address, if any
 * \param[in] addr Address of memory whose name is to be found
 * \param[in] tdesc How to interpret address
 */
const std::string JeodTrick10MemoryInterface::get_name_at_address(const void * addr,
                                                                  const JeodMemoryTypeDescriptor * tdesc) const
{
    const ATTRIBUTES & item_attr = tdesc->get_attr();
    std::string result;

    // A null pointer is a null pointer. Period.
    if(addr == nullptr)
    {
        result = "NULL";
    }

    else
    {
        // Interpret the address as a pointer to the specified type.
        result = translate_addr_to_name(addr, &item_attr);

        // Translating back to an address should yield the same address.
        // Return the empty string if it doesn't.
        if(translate_name_to_addr(result) != addr)
        {
            MessageHandler::error(__FILE__,
                                  __LINE__,
                                  SimInterfaceMessages::interface_error,
                                  "Translating the original address into a name and then\n"
                                  "translating the name back into an address yielded a different\n"
                                  "result than the original.\n");
            result = "";
        }
    }

    return result;
}

/**
 * Get the address, if any, that corresponds to the given name.
 * @return Name of the address, if any
 * \param[in] name of an address\n Units: Name
 */
void * JeodTrick10MemoryInterface::get_address_at_name(const std::string & name) const
{
    return translate_name_to_addr(name);
}

/*
 * NOTE: The remaining functions could have been folded into the above.
 * They were not in the hope that the functionality will eventually be
 * incorporated into Trick itself.
 */

/**
Translate the given address to an address specification string, with the
address interpreted in the context of the supplied attributes. It is the
attributes structure that resolves the A versus A.B versus A.B.C ambiguity.
@note The attributes structure must be that of a pointer type.
@param addr The address to be translated.
@param attr The context in which to interpret the address.
@return Address specification string, e.g., &foo.bar.baz[42]
*/
std::string JeodTrick10MemoryInterface::translate_addr_to_name(const void * addr, const ATTRIBUTES * attr) const
{
    ATTRIBUTES ptr_attr(pointer_attributes(*attr));
    int pointer_index = ptr_attr.num_index;

    // Find the index in the attributes at which the pointer business starts.
    // A size of zero for some index indicates that the index is a pointer.
    for(int ii = 0; ii < ptr_attr.num_index; ++ii)
    {
        if(ptr_attr.index[ii].size == 0)
        {
            pointer_index = ii;
            break;
        }
    }

#if 0
   // None found: The attributes don't describe a pointer.
   if (pointer_index == attr->num_index) {
      MessageHandler::error (
         __FILE__, __LINE__, SimInterfaceMessages::interface_error,
         "translate_addr_to_name called with non-pointer attributes.");
      return "NULL /* ERROR: Non-pointer attributes */";
   }
#endif

    // The attributes describe a pointer: Translate the address to a name.
    return trick_checkpoint_agent->ref_string_from_ptr(const_cast<void *>(addr), &ptr_attr, pointer_index);
}

/**
Translate the given address specification string to an address.
This is the inverse of translate_addr_to_name.
@param spec The address specification to be interpreted.
@return    Address corresponding to the address specification.
*/
void * JeodTrick10MemoryInterface::translate_name_to_addr(const std::string & spec) const
{
    std::string name(spec);
    void * result;
    std::size_t position;

    // Sometimes translate_addr_to_name returns strings like
    //    NULL /* ERROR Explanation of problem */
    // Strip the C-style comment off the end of the string.
    // Note: This assumes
    //   - A single slash indicates start of comment.
    //   - There's nothing of interest after the end of the comment.
    position = name.find_first_of('/');
    if(position < name.length())
    {
        name.erase(position);
        position = name.find_last_not_of(' ');
        if(position < name.length())
        {
            name.erase(position + 1);
        }
        // No else: We'll deal with an empty spec later.
    }

    // "NULL" means NULL, but ref_attributes doesn't grok "NULL".
    if(name.compare("NULL") == 0)
    {
        result = nullptr;
    }

    else
    {
        std::size_t offset = 0;

        // Deal with strings of the form "&foo.bar + 42":
        // Interpret and then get rid of the trailing offset.
        position = name.find_first_of('+');
        if(position < name.length())
        {
            offset = std::strtoul(name.substr(position + 1).c_str(), nullptr, 0);
            name.erase(position);
        }

        // Get rid of the leading ampersand if it is present.
        if(name[0] == '&')
        {
            name = name.substr(1);
        }

        // Get rid of trailing spaces and translate.
        position = name.find_last_not_of(' ');
        if(position < name.length())
        {
            name.erase(position + 1);

            // We now have a name spec that ref_attributes might understand.
            const char * name_string = name.c_str();
            REF2 * ref = ref_attributes(const_cast<char *>(name_string));

            // We have a good name spec: form the result.
            if(ref != nullptr)
            {
                result = static_cast<void *>(static_cast<char *>(ref->address) + offset);

                // Clean up the REFidue before returning the result.
                std::free(ref->reference);
                std::free(ref->units);
                std::free(ref);
            }

            // Oops. We don't have a name spec that ref_attributes understands.
            // For example, name.not.in.name.space.known.to.trick
            // Complain and return NULL.
            else
            {
                MessageHandler::error(__FILE__,
                                      __LINE__,
                                      SimInterfaceMessages::interface_error,
                                      "Name '%s' cannot be translated to an address.",
                                      spec.c_str());
                result = nullptr;
            }
        }

        // Name is all spaces or is empty: Complain and return NULL.
        else
        {
            MessageHandler::error(__FILE__,
                                  __LINE__,
                                  SimInterfaceMessages::interface_error,
                                  "Name '%s' is illegal.",
                                  spec.c_str());
            result = nullptr;
        }
    }

    return result;
}

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
