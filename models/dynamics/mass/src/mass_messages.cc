/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup Mass
 * @{
 *
 * @file models/dynamics/mass/src/mass_messages.cc
 * Implement the class MassBodyMessages.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((mass_messages.cc))



*******************************************************************************/

// System includes

// JEOD includes
#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/mass_messages.hh"

//! Namespace jeod
namespace jeod
{

#define MAKE_MASSBODY_MESSAGE_CODE(id) JEOD_MAKE_MESSAGE_CODE(MassBodyMessages, "dynamics/mass/", id)

// Static member data
MAKE_MASSBODY_MESSAGE_CODE(attach_info);
MAKE_MASSBODY_MESSAGE_CODE(invalid_attach);
MAKE_MASSBODY_MESSAGE_CODE(invalid_detach);
MAKE_MASSBODY_MESSAGE_CODE(invalid_node);
MAKE_MASSBODY_MESSAGE_CODE(invalid_name);
MAKE_MASSBODY_MESSAGE_CODE(invalid_enum);
MAKE_MASSBODY_MESSAGE_CODE(io_error);
MAKE_MASSBODY_MESSAGE_CODE(internal_error);

#undef MAKE_MASSBODY_MESSAGE_CODE

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
