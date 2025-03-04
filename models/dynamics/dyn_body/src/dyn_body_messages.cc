/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynBody
 * @{
 *
 * @file models/dynamics/dyn_body/src/dyn_body_messages.cc
 * Implement the class De4xxMessages.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((dyn_body_messages.cc))



*******************************************************************************/

// System includes

// JEOD includes
#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/dyn_body_messages.hh"

//! Namespace jeod
namespace jeod
{

#define MAKE_DYNBODY_MESSAGE_CODE(id) JEOD_MAKE_MESSAGE_CODE(DynBodyMessages, "dynamics/dyn_body/", id)

// Static member data
MAKE_DYNBODY_MESSAGE_CODE(invalid_body);
MAKE_DYNBODY_MESSAGE_CODE(invalid_group);
MAKE_DYNBODY_MESSAGE_CODE(invalid_name);
MAKE_DYNBODY_MESSAGE_CODE(invalid_frame);
MAKE_DYNBODY_MESSAGE_CODE(invalid_attachment);
MAKE_DYNBODY_MESSAGE_CODE(invalid_technique);
MAKE_DYNBODY_MESSAGE_CODE(not_dyn_body);
MAKE_DYNBODY_MESSAGE_CODE(internal_error);

#undef MAKE_DYNBODY_MESSAGE_CODE

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
