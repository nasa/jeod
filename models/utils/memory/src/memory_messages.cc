/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Memory
 * @{
 *
 * @file models/utils/memory/src/memory_messages.cc
 * Implement the class MemoryMessages.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((memory_messages.o))

 

*******************************************************************************/


/**
 * \addtogroup classes
 * @{
 */

// System includes

// JEOD includes
// JEOD includes
#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/memory_messages.hh"

#define MAKE_MEMORY_MESSAGE_CODE(id) \
   JEOD_MAKE_MESSAGE_CODE(MemoryMessages, "utils/memory/", id)


//! Namespace jeod
namespace jeod {

// Static member data

MAKE_MEMORY_MESSAGE_CODE (singleton_error);
MAKE_MEMORY_MESSAGE_CODE (out_of_memory);
MAKE_MEMORY_MESSAGE_CODE (lock_error);
MAKE_MEMORY_MESSAGE_CODE (null_pointer);
MAKE_MEMORY_MESSAGE_CODE (suspect_pointer);
MAKE_MEMORY_MESSAGE_CODE (invalid_size);
MAKE_MEMORY_MESSAGE_CODE (corrupted_memory);
MAKE_MEMORY_MESSAGE_CODE (registration_error);
MAKE_MEMORY_MESSAGE_CODE (internal_error);
MAKE_MEMORY_MESSAGE_CODE (debug);

} // End JEOD namespace

#undef MAKE_MEMORY_MESSAGE_CODE
#undef CLASS
#undef PATH

/**
 * @}
 */

/**
 * @}
 * @}
 * @}
 */
