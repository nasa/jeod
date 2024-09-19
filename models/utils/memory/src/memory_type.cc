/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Memory
 * @{
 *
 * @file models/utils/memory/src/memory_type.cc
 * Implement destructors for the classes for representing data types.
 */

/*******************************************************************************

Purpose:
   ()

Library dependencies:
   ((memory_type.cc)
    (utils/named_item/src/named_item_demangle.cc))


*******************************************************************************/

/**
 * \addtogroup classes
 * @{
 */

// System includes
#include <cstddef>
#include <sstream>
#include <string>

// JEOD includes
#include "utils/named_item/include/named_item.hh"
#include "utils/sim_interface/include/simulation_interface.hh"

// Model includes
#include "../include/memory_item.hh"
#include "../include/memory_manager.hh"
#include "../include/memory_type.hh"

//! Namespace jeod
namespace jeod
{

// Flag to enable registration_errors.
bool JeodMemoryTypeDescriptor::check_for_registration_errors = false;

/**
 * The jeod_alloc.hh macros insert a space between the type name and the
 * asterisks. Delete that space.
 * @return Name, as c++ string
 * \param[in] type_name Name, as C string
 */
std::string JeodMemoryTypeDescriptor::initialize_type_name(const std::string & type_name)
{
    std::string str(type_name);
    if(str[str.length() - 1] == '*')
    {
        std::size_t last_space = str.find_last_of(' ');
        if(last_space < str.length())
        {
            str.erase(last_space, 1);
        }
    }
    return str;
}

/**
 * Get the pointer dimensionality of the type.
 */
size_t JeodMemoryTypeDescriptor::pointer_dimension( // Return: -- Number asterisks
    const std::string & demangled_name)             // In:     -- Type name
{
    std::size_t naster = 0;
    std::size_t pos = demangled_name.find_last_of('>');

    if(pos == std::string::npos)
    {
        pos = 0;
    }

    for(pos = demangled_name.find('*', pos); pos != std::string::npos; pos = demangled_name.find('*', pos + 1))
    {
        ++naster;
    }

    return naster;
}

/**
 * Get the descriptor for the base (non-pointer) of some pointer type.
 * @note
 * Assumes GNU c++ name mangling, where 'const' is always preceded by a space.
 */
const JeodMemoryTypeDescriptor * JeodMemoryTypeDescriptor::base_type( // Return: -- Base type name
    const std::string & demangled_name)                               // In:     -- Type name
{
    std::string base_name(demangled_name);
    std::size_t len = base_name.length();
    std::size_t start_pos;
    std::size_t pos;

    // Skip past the template instantiation part of the type name.
    start_pos = base_name.find_last_of('>');
    if(start_pos == std::string::npos)
    {
        start_pos = 0;
    }
    else
    {
        ++start_pos;
    }

    // Get rid of consts in the type name.
    for(pos = base_name.find(" const", start_pos); pos < len; pos = base_name.find(" const", pos))
    {
        // Delete the " const" if the next character is
        // the null character, a space, an ampersand, or an asterisk.
        // Otherwise, false alarm (e.g. constable); skip past it.
        switch(base_name[pos + 6])
        {
            case '\0':
            case ' ':
            case '&':
            case '*':
                base_name.erase(pos, 6);
                len -= 6;
                break;

            default:
                pos += 6;
                break;
        }
    }

    // Get rid of asterisks in the type name.
    pos = base_name.find('*', start_pos);
    if(pos != std::string::npos)
    {
        base_name.erase(pos);
        len = base_name.length();
    }

    // Get rid of trailing blanks, if present.
    if(base_name[len - 1] == ' ')
    {
        pos = base_name.find_last_not_of(' ');
        base_name.erase(pos + 1);
    }

    // The base type for void* is void*.
    if(base_name.compare("void") == 0)
    {
        base_name += "*";
    }

    // Get the descriptor for this mangled-up demangled name.
    return JeodMemoryManager::get_type_descriptor(JeodMemoryManager::Demangled_type_name, base_name);
}

/**
 * Non-default constructor.
 * Note that construction is via a char* as that is what the C preprocessor
 * creates when it stringifies a token.
 * \param[in] obj_typeid Type ID for type
 * \param[in] type_attr Type attributes
 * \param[in] type_size Type size
 * \param[in] is_exportable Register instances?
 */
JeodMemoryTypeDescriptor::JeodMemoryTypeDescriptor(const std::type_info & obj_typeid,
                                                   const JEOD_ATTRIBUTES_TYPE & type_attr,
                                                   std::size_t type_size,
                                                   bool is_exportable)
    : obj_id(obj_typeid),
      name(NamedItem::demangle(obj_typeid)),
      attr(type_attr),
      size(type_size),
      register_instances(is_exportable)
{
}

/**
 * Construct a type specification string.
 * @return Type string
 * \param[in] item Item descriptor
 */
const std::string JeodMemoryTypeDescriptor::type_spec(const JeodMemoryItem & item) const
{
    std::ostringstream spec;

    spec << NamedItem::demangle(obj_id);
    if(item.get_is_array())
    {
        spec << "[";
        spec << item.get_nelems();
        spec << "]";
    }

    return spec.str();
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
