/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Orientation
 * @{
 *
 * @file models/utils/orientation/src/orientation_messages.cc
 * Implement the class OrientationMessages.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((orientation_messages.cc))

 

*******************************************************************************/


// System includes

// JEOD includes
#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/orientation_messages.hh"


//! Namespace jeod
namespace jeod {

#define MAKE_ORIENTATION_MESSAGE_CODE(id) \
   JEOD_MAKE_MESSAGE_CODE(OrientationMessages, "utils/orientation/", id)


// Define OrientationMessages static member data

MAKE_ORIENTATION_MESSAGE_CODE (invalid_enum);
MAKE_ORIENTATION_MESSAGE_CODE (invalid_data);
MAKE_ORIENTATION_MESSAGE_CODE (invalid_request);

#undef MAKE_ORIENTATION_MESSAGE_CODE

} // End JEOD namespace


/**
 * @}
 * @}
 * @}
 */
