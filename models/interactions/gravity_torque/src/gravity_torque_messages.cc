/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup GravityTorque
 * @{
 *
 * @file models/interactions/gravity_torque/src/gravity_torque_messages.cc
 * Implement the class GravityTorqueMessages.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((gravity_torque_messages.cc))



*******************************************************************************/

// System includes

// JEOD includes
#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/gravity_torque_messages.hh"

//! Namespace jeod
namespace jeod
{

#define MAKE_GRAVITYTORQUE_MESSAGE_CODE(id)                                                                            \
    JEOD_MAKE_MESSAGE_CODE(GravityTorqueMessages, "interactions/gravity_torque/", id)

// Static member data
MAKE_GRAVITYTORQUE_MESSAGE_CODE(initialization_error);

#undef MAKE_GRAVITYTORQUE_MESSAGE_CODE

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
