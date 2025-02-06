/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup SimInterface
 * @{
 *
 * @file models/utils/sim_interface/src/trick_memory_interface_attrib.cc
 * Define JeodTrickMemoryInterface methods related to attributes.
 */

/*******************************************************************************

Purpose:
  ()

Library Dependency:
  ((trick_memory_interface_attrib.o)
   (trick_memory_interface.o))

 
*******************************************************************************/


#ifdef TRICK_VER

// System includes
#include <cstddef>
#include <cstring>
#include <dlfcn.h>

// Trick includes
#include "sim_services/MemoryManager/include/attributes.h"

// JEOD includes
#include "utils/memory/include/memory_type.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/named_item/include/named_item.hh"

// Model includes
#include "../include/sim_interface_messages.hh"
#include "../include/trick_memory_interface.hh"


//! Namespace jeod
namespace jeod {

/**
 * Find the attributes for a class in the symbol table.
 * @return Found attributes
 * \param[in] type_name Demangled type name
 */
const JEOD_ATTRIBUTES_POINTER_TYPE
JeodTrickMemoryInterface::find_attributes (
   const std::string & type_name)
const
{
   std::string attr_name = "attr" + type_name;
   for (size_t idx = attr_name.find(':');
        idx < std::string::npos;
        idx = attr_name.find(':', idx)) {
      attr_name[idx] = '_';
   }

   void * symbol = dlsym (dlhandle, attr_name.c_str());

   if (symbol == NULL) {
      MessageHandler::inform (
         __FILE__, __LINE__, SimInterfaceMessages::interface_error,
         "Unable to find Trick attributes for type '%s'.\n"
         "Logging and checkpointing such objects will result in failure.",
         type_name.c_str());
   }

   return reinterpret_cast<ATTRIBUTES*> (symbol);
}


/**
 * Find the attributes for a class in the symbol table.
 * @return Found attributes
 * \param[in] data_type Data type descriptor
 */
const JEOD_ATTRIBUTES_POINTER_TYPE
JeodTrickMemoryInterface::find_attributes (
   const std::type_info & data_type)
const
{
   return find_attributes (NamedItem::demangle (data_type));
}


/**
 * Create an attributes structure that represents a primitive type.
 * @return Constructed attributes.
 * \param[in] data_type Data type descriptor
 */
JEOD_ATTRIBUTES_TYPE
JeodTrickMemoryInterface::primitive_attributes (
   const std::type_info & data_type)
const
{
   ATTRIBUTES attr;
   std::memset (&attr, 0, sizeof(attr));
   attr.io = 3;
   attr.language = Language_CPP;

   if (data_type == typeid(char)) {
      attr.type = TRICK_CHARACTER;
      attr.size = sizeof(char);
   }

   else if (data_type == typeid(signed char)) {
      attr.type = TRICK_CHARACTER;
      attr.size = sizeof(signed char);
   }

   else if (data_type == typeid(unsigned char)) {
      attr.type = TRICK_UNSIGNED_CHARACTER;
      attr.size = sizeof(unsigned char);
   }

   else if (data_type == typeid(short int)) {
      attr.type = TRICK_SHORT;
      attr.size = sizeof(short int);
   }

   else if (data_type == typeid(unsigned short int)) {
      attr.type = TRICK_UNSIGNED_SHORT;
      attr.size = sizeof(unsigned short int);
   }

   else if (data_type == typeid(int)) {
      attr.type = TRICK_INTEGER;
      attr.size = sizeof(int);
   }

   else if (data_type == typeid(unsigned int)) {
      attr.type = TRICK_UNSIGNED_INTEGER;
      attr.size = sizeof(unsigned int);
   }

   else if (data_type == typeid(long int)) {
      attr.type = TRICK_LONG;
      attr.size = sizeof(long int);
   }

   else if (data_type == typeid(unsigned long int)) {
      attr.type = TRICK_UNSIGNED_LONG;
      attr.size = sizeof(unsigned long int);
   }

   else if (data_type == typeid(long long int)) {
      attr.type = TRICK_LONG_LONG;
      attr.size = sizeof(long long int);
   }

   else if (data_type == typeid(unsigned long int)) {
      attr.type = TRICK_UNSIGNED_LONG_LONG;
      attr.size = sizeof(unsigned long int);
   }

   else if (data_type == typeid(unsigned long long int)) {
      attr.type = TRICK_UNSIGNED_LONG_LONG;
      attr.size = sizeof(unsigned long long);
   }

   else if (data_type == typeid(float)) {
      attr.type = TRICK_FLOAT;
      attr.size = sizeof(float);
   }

   else if (data_type == typeid(double)) {
      attr.type = TRICK_DOUBLE;
      attr.size = sizeof(double);
   }

   else if (data_type == typeid(bool)) {
      attr.type = TRICK_BOOLEAN;
      attr.size = sizeof(bool);
   }

   else if (data_type == typeid(wchar_t)) {
      attr.type = TRICK_WCHAR;
      attr.size = sizeof(wchar_t);
   }

   else {
      MessageHandler::error (
         __FILE__, __LINE__, SimInterfaceMessages::interface_error,
         "Type %s is not a primitive type",
         data_type.name());
   }

   return attr;
}


/**
 * Create an attributes structure that represents a pointer type.
 * @return Constructed pointer attributes.
 * \param[in] target_attr Pointed-to type attributes.
 */
JEOD_ATTRIBUTES_TYPE
JeodTrickMemoryInterface::pointer_attributes (
   const JEOD_ATTRIBUTES_TYPE & target_attr)
const
{
   ATTRIBUTES attr;
   int pointer_index = target_attr.num_index;

   // Check if the parent is an array.
   // A non-zero size for some index indicates that that element is an array.
   for (int ii = 0; ii < target_attr.num_index; ++ii) {
      if (target_attr.index[ii].size == 0) {
         pointer_index = ii;
         break;
      }
   }

   // Construct the output attributes.
   std::memset (&attr, 0, sizeof(attr));
   attr.language = Language_CPP;
   attr.io = 3;
   attr.size = sizeof(void *);
   attr.num_index = target_attr.num_index + 1;
   attr.type = target_attr.type;

   // Set the attribute's attributes for a structured type.
   if (target_attr.type == TRICK_STRUCTURED) {
      attr.attr = target_attr.attr;
   }

   // For non-structured types, attr.attr is the null pointer.
   else {
      attr.attr = NULL;
   }

   // Copy the parent array dimensions, if any.
   for (int ii = 0; ii < pointer_index; ++ii) {
      attr.index[ii] = target_attr.index[ii];
   }

   return attr;
}


/**
 * Create an attributes structure that represents a void* pointer.
 * @return Constructed pointer attributes.
 */
JEOD_ATTRIBUTES_TYPE
JeodTrickMemoryInterface::void_pointer_attributes (
   void)
const
{
   ATTRIBUTES attr;
   std::memset (&attr, 0, sizeof(attr));
   attr.language = Language_CPP;
   attr.io = 3;
   attr.size = sizeof(void *);

   attr.type = TRICK_STRUCTURED;
   attr.attr = NULL;
   attr.num_index = 1;

   return attr;
}


/**
 * Create an attributes structure that represents a structured type.
 * @return Constructed structure attributes.
 * \param[in] target_attr Return value from find_attributes.
 * \param[in] target_size Structure size.
 */
JEOD_ATTRIBUTES_TYPE
JeodTrickMemoryInterface::structure_attributes (
   const JEOD_ATTRIBUTES_POINTER_TYPE target_attr,
   std::size_t target_size)
const
{
   ATTRIBUTES attr;
   std::memset (&attr, 0, sizeof(attr));
   attr.language = Language_CPP;
   attr.io = 3;
   attr.size = target_size;
   attr.num_index = 0;
   attr.type = TRICK_STRUCTURED;
   attr.attr = reinterpret_cast<char*>(
                  const_cast<ATTRIBUTES*>(target_attr));

   return attr;
}


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
