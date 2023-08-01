/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Message
 * @{
 *
 * @file models/utils/message/src/suppressed_code_message_handler.cc
 * Define member functions for the class SuppressedCodeMessageHandler.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((suppressed_code_message_handler.cc)
   (message_handler.cc)
   (message_messages.cc))

 

*******************************************************************************/


// System includes
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>

// JEOD includes
#include "utils/memory/include/jeod_alloc.hh"

// Model includes
#include "../include/suppressed_code_message_handler.hh"


//! Namespace jeod
namespace jeod {

/**
 * Register the MessageHandler's checkpointable contents.
 */
void
SuppressedCodeMessageHandler::register_contents (
   void)
{
   JEOD_REGISTER_CLASS (SuppressedCodeMessageHandler);
   JEOD_REGISTER_CHECKPOINTABLE (this, suppressed_codes);
}


/**
 * Deregister the MessageHandler's checkpointable contents.
 */
void
SuppressedCodeMessageHandler::deregister_contents (
   void)
{
   JEOD_DEREGISTER_CHECKPOINTABLE (this, suppressed_codes);
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
