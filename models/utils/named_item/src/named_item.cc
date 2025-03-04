/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup NamedItem
 * @{
 *
 * @file models/utils/named_item/src/named_item.cc
 * Construct the name of a NamedItem object by conjoining the passed
 * parameters with a dot.
 */

/*******************************************************************************

Purpose:
  ()

Assumptions and limitations:
  ((These functions allocate memory and do not release it. Releasing the
    allocated memory is the responsibility of the calling function.))

Library Dependency:
  ((named_item.cc)
   (named_item_demangle.cc)
   (named_item_messages.cc)
   (utils/message/src/message_handler.cc))



*******************************************************************************/

// System includes
#include <cstdarg>
#include <cstddef>
#include <cstring>

// JEOD includes
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/named_item.hh"
#include "../include/named_item_messages.hh"

//! Namespace jeod
namespace jeod
{

// suffix: Find the part of the name that follows the prefix.
std::string NamedItem::suffix(const std::string & prefix, const std::string & name)
{
    std::size_t prefix_len = prefix.length();

    // Return the full name if
    // - the name is NULL (sanity check) or
    // - the name does not start with "prefix."
    if(name.empty() || (name.compare(0, prefix_len, prefix) != 0) || (name[prefix_len] != '.'))
    {
        return name;
    }

    // Name starts with "prefix.": return the suffix.
    else
    {
        return name.substr(prefix_len + 1);
    }
}

// validate_name: Check for illegal values (null pointer, empty string).
void NamedItem::validate_name(const char * file,
                              unsigned int line,
                              const std::string & variable_value,
                              const std::string & variable_type,
                              const std::string & variable_name)
{
    // Check for the empty string.
    if(variable_value.empty())
    {
        MessageHandler::fail(file,
                             line,
                             NamedItemMessages::invalid_name,
                             "%s %s is the empty string",
                             variable_type.c_str(),
                             variable_name.c_str());
    }
}

void NamedItem::verify_unfrozen_name() const
{
    if(is_frozen)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             NamedItemMessages::frozen_name,
                             "%s name is marked as frozen and cannot be changed",
                             name.c_str());
    }
}

} // namespace jeod

bool operator==(const jeod::NamedItem & lhs, const std::string & rhs)
{
    return (lhs.get_name() == rhs);
}

bool operator==(const std::string & lhs, const jeod::NamedItem & rhs)
{
    return (lhs == rhs.get_name());
}

/**
 * @}
 * @}
 * @}
 */
