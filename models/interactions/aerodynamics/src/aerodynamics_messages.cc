/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup Aerodynamics
 * @{
 *
 * @file models/interactions/aerodynamics/src/aerodynamics_messages.cc
 * Implement aerodynamics_messages
 */

/*******************************************************************************

Purpose:
  ()

Reference:
   (((None)))

Assumptions and limitations:
   ((Outlined in the header file))

Class:
   (None)

Library dependencies:
  ((aerodynamics_messages.cc))



*******************************************************************************/

#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/aerodynamics_messages.hh"

//! Namespace jeod
namespace jeod
{

#define MAKE_AERODYNAMICS_MESSAGE_CODE(id)                                                                             \
    JEOD_MAKE_MESSAGE_CODE(AerodynamicsMessages, "interactions/aerodynamics/", id)

MAKE_AERODYNAMICS_MESSAGE_CODE(initialization_error);
MAKE_AERODYNAMICS_MESSAGE_CODE(runtime_error);
MAKE_AERODYNAMICS_MESSAGE_CODE(pre_initialization_error);
MAKE_AERODYNAMICS_MESSAGE_CODE(runtime_warns);

#undef MAKE_AERODYNAMICS_MESSAGE_CODE

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
