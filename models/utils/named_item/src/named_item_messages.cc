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
#include "../include/named_item_messages.hh"

#define PATH "utils/named_item/"

//! Namespace jeod
namespace jeod
{

// Static member data

const char * NamedItemMessages::bad_args = PATH "bad_args";
const char * NamedItemMessages::invalid_name = PATH "invalid_name";
const char * NamedItemMessages::frozen_name = PATH "frozen_name";

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
