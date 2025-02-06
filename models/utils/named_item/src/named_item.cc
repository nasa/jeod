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
  ((named_item.o)
   (named_item_demangle.o)
   (named_item_messages.o)
   (utils/message/message_handler.o))

 

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


#define MAX_NAME_ITEMS 8

//! Namespace jeod
namespace jeod {

// vconstruct_name: Construct a name as a dot-conjoined string.
char *
NamedItem::vconstruct_name (
   const char * name_item,
   ...)
{
   char * name;                 // -- The constructed name
   va_list args;                // -- Varargs stack

   // Initialize the argument stack.
   va_start (args, name_item);

   // Construct the name.
   name = va_construct_name (name_item, args);

   // Return the constructed name.
   return (name); // cppcheck-suppress va_end_missing
}


// va_construct_name: Construct a name as a dot-conjoined string.
char *
NamedItem::va_construct_name (
   const char * name_item,
   va_list args)
{
   const char * item;                  // -- A single argument
   std::size_t num_items;              // -- Number of arguments
   bool terminated;                    // -- Arguments terminated with NULL?

   // Protect against a null first argument.
   if (name_item == NULL) {
      va_end (args);
      return NULL;
   }

   std::string name(name_item);        // -- The constructed name

   // Initialize the argument stack.
   num_items = 0;
   terminated  = false;

   // Extract items from the argument stack, ending at the null pointer
   // or when too many arguments have been pulled off
   // (The latter probably means the caller forgot to end with a null).
   while (1) {
      item = va_arg (args, const char *);

      // NULL terminates the argument list.
      if (item == NULL) {
         terminated = true;
         break;
      }

      // Construct the name via join ('.', @arguments)
      name += ".";
      name += item;

      // Abort if too many arguments were passed (usually non NULL at end).
      if (num_items >= MAX_NAME_ITEMS) {
         break;
      }

      num_items++;
   }

   // Close the argument stack. This is needed here rather in the calling
   // function in case an error occurs. The call to MessageHandler::fail will
   // be fouled up if the argument stack is not closed.
   va_end (args);


   // Die if the null pointer was not found.
   if (! terminated) {
      MessageHandler::fail (
         __FILE__, __LINE__, NamedItemMessages::bad_args,
         "Too many arguments to NamedItem::construct_name");
      return NULL;
   }


   // Successfully extracted num_items arguments.
   char * new_name = JEOD_ALLOC_PRIM_ARRAY (name.length()+1, char);
   snprintf(new_name, name.length()+1, "%s", name.c_str());

   return new_name;
}


// suffix: Find the part of the name that follows the prefix.
const char *
NamedItem::suffix (
   const char * prefix,
   const char * name)
{
   std::string nprefix(prefix);
   std::size_t prefix_len = nprefix.length();

   // Return the full name if
   // - the name is NULL (sanity check) or
   // - the name does not start with "prefix."
   if ((name == NULL) ||
       (std::strncmp (name, prefix, prefix_len) != 0) ||
       (name[prefix_len] != '.')) {
      return name;
   }

   // Name starts with "prefix.": return the suffix.
   else {
      return &name[prefix_len+1];
   }

}


// validate_name: Check for illegal values (null pointer, empty string).
void
NamedItem::validate_name (
   const char * file,
   unsigned int line,
   const char * variable_value,
   const char * variable_type,
   const char * variable_name)
{

   // Check for a null value.
   if (variable_value == NULL) {
      MessageHandler::fail (
         file, line, NamedItemMessages::invalid_name,
         "%s %s value is NULL",
         variable_type, variable_name);
   }

   // Check for the empty string.
   if (*variable_value == '\0') {
      MessageHandler::fail (
         file, line, NamedItemMessages::invalid_name,
         "%s %s is the empty string",
         variable_type, variable_name);
   }
}


void
NamedItem::verify_unfrozen_name () const
{
   if (is_frozen) {
      MessageHandler::fail (
         __FILE__, __LINE__, NamedItemMessages::frozen_name,
         "%s name is marked as frozen and cannot be changed",
         name.c_str());
   }
}

} // End JEOD namespace

bool operator== (const jeod::NamedItem& lhs, const std::string& rhs)
{
   return (lhs.get_name() == rhs);
}

bool operator== (const std::string& lhs, const jeod::NamedItem& rhs)
{
   return (lhs == rhs.get_name());
}

/**
 * @}
 * @}
 * @}
 */
