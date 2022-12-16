/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup NamedItem
 * @{
 *
 * @file models/utils/named_item/src/named_item_demangle.cc
 * Demangle a C++ name, isolated from other NamedItem methods because this has
 * the potential to get big and ugly if JEOD is ported to a number of different
 * systems.
 */

/*******************************************************************************

Purpose:
  ()

Assumptions and limitations:
  ((For now, GNU c++ only.))

Library Dependency:
  ((named_item_demangle.o))

 

*******************************************************************************/


// System includes
#ifndef __has_include
#define __has_include(x) 0
#endif
#if (defined __GNUC__) || __has_include(<cxxabi.h>)
#define HAVE_ABI
#endif

#include <cstdlib>
#include <string>
#include <typeinfo>
#ifdef HAVE_ABI
#include <cxxabi.h>
#endif

// Model includes
#include "../include/named_item.hh"
#include "../include/named_item_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Demangle a C++ name.
 * @return Demangled name
 * \param[in] info Typeinfo to be demangled
 */
const std::string
NamedItem::demangle (
   const std::type_info & info)
{
   std::string result;
   bool have_result = false;

#ifdef HAVE_ABI
   const char * type_name = NULL;
   int status;

   type_name = abi::__cxa_demangle (info.name(), NULL, NULL, &status);

   if (status == 0) {
      result = type_name;
      have_result = true;
   }
   else {
      have_result = false;
   }

   if (type_name != NULL) {
      std::free (const_cast<char *>(type_name));
      type_name = NULL;
   }
#endif

   // Punted by the above: Use the mangled name.
   if (! have_result) {
      result = info.name();
   }

   return result;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
