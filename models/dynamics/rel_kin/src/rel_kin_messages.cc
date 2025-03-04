/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup RelKin
 * @{
 *
 * @file models/dynamics/rel_kin/src/rel_kin_messages.cc
 * Implement the class RelKinMessages.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((rel_kin_messages.cc))



*******************************************************************************/

// System includes

// JEOD includes
#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/rel_kin_messages.hh"

//! Namespace jeod
namespace jeod
{

#define MAKE_RELKIN_MESSAGE_CODE(id) JEOD_MAKE_MESSAGE_CODE(RelKinMessages, "dynamics/rel_kin/", id)

// Static member data
MAKE_RELKIN_MESSAGE_CODE(duplicate_entry);
MAKE_RELKIN_MESSAGE_CODE(entry_not_found);
MAKE_RELKIN_MESSAGE_CODE(invalid_entry);

#undef MAKE_RELKIN_MESSAGE_CODE

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
