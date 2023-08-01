/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynManager
 * @{
 *
 * @file models/dynamics/dyn_manager/src/dyn_manager_messages.cc
 * Implement the class DynManagerMessages.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((dyn_manager_messages.cc))



*******************************************************************************/


// System includes

// JEOD includes
#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/dyn_manager_messages.hh"


//! Namespace jeod
namespace jeod {

#define MAKE_DYNMANAGER_MESSAGE_CODE(id) \
   JEOD_MAKE_MESSAGE_CODE(DynManagerMessages, "dynamics/dyn_manager/", id)


// Define DynManagerMessages static member data

MAKE_DYNMANAGER_MESSAGE_CODE (null_pointer);
MAKE_DYNMANAGER_MESSAGE_CODE (duplicate_entry);
MAKE_DYNMANAGER_MESSAGE_CODE (invalid_name);
MAKE_DYNMANAGER_MESSAGE_CODE (invalid_frame);
MAKE_DYNMANAGER_MESSAGE_CODE (invalid_type);
MAKE_DYNMANAGER_MESSAGE_CODE (inconsistent_setup);
MAKE_DYNMANAGER_MESSAGE_CODE (singleton_error);
MAKE_DYNMANAGER_MESSAGE_CODE (internal_error);

#undef MAKE_DYNMANAGER_MESSAGE_CODE

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
