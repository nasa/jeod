/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Quaternion
 * @{
 *
 * @file models/utils/quaternion/src/quat_messages.cc
 * Implement the class QuatMessages.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((quat.cc))



*******************************************************************************/

// System includes

// JEOD includes
#include "../include/quat_messages.hh"

#define PATH "utils/quaternion/"

//! Namespace jeod
namespace jeod
{

// Static member data

const char * QuatMessages::undefined = PATH "undefined";

const char * QuatMessages::invalid_entry = PATH "invalid_entry";

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
