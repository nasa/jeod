/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup SimInterface
 * @{
 *
 * @file models/utils/sim_interface/src/sim_interface_messages.cc
 * Implement the class SimInterfaceMessages.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((sim_interface_messages.cc))



*******************************************************************************/

// System includes

// JEOD includes
#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/sim_interface_messages.hh"

//! Namespace jeod
namespace jeod
{

#define MAKE_SIMINTERFACE_MESSAGE_CODE(id) JEOD_MAKE_MESSAGE_CODE(SimInterfaceMessages, "utils/sim_interface/", id)

// Static member data

MAKE_SIMINTERFACE_MESSAGE_CODE(singleton_error);
MAKE_SIMINTERFACE_MESSAGE_CODE(interface_error);
MAKE_SIMINTERFACE_MESSAGE_CODE(phasing_error);
MAKE_SIMINTERFACE_MESSAGE_CODE(integration_error);
MAKE_SIMINTERFACE_MESSAGE_CODE(implementation_error);

#undef MAKE_SIMINTERFACE_MESSAGE_CODE

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
