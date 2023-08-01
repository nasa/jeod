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
#include "../include/sim_interface_messages.hh"

#define PATH "utils/sim_interface/"
#define CLASS SimInterfaceMessages
#define MAKE_MESSAGE_CODE(id) char const * CLASS::id = PATH #id


//! Namespace jeod
namespace jeod {

// Static member data

MAKE_MESSAGE_CODE(singleton_error);
MAKE_MESSAGE_CODE(interface_error);
MAKE_MESSAGE_CODE(phasing_error);
MAKE_MESSAGE_CODE(integration_error);
MAKE_MESSAGE_CODE(implementation_error);

} // End JEOD namespace

#undef MAKE_MESSAGE_CODE
#undef CLASS
#undef PATH

/**
 * @}
 * @}
 * @}
 */
