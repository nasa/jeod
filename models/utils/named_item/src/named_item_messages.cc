/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup NamedItem
 * @{
 *
 * @file models/utils/named_item/src/named_item_messages.cc
 * Implement the class NamedItemMessages.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((named_item_messages.cc))



*******************************************************************************/

// System includes

// JEOD includes
#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/named_item_messages.hh"

//! Namespace jeod
namespace jeod
{

#define MAKE_NAMEDITEM_MESSAGE_CODE(id) JEOD_MAKE_MESSAGE_CODE(NamedItemMessages, "utils/named_item/", id)

// Static member data
MAKE_NAMEDITEM_MESSAGE_CODE(bad_args);
MAKE_NAMEDITEM_MESSAGE_CODE(invalid_name);
MAKE_NAMEDITEM_MESSAGE_CODE(frozen_name);

#undef MAKE_NAMEDITEM_MESSAGE_CODE

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
