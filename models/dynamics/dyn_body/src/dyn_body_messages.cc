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
#include "../include/dyn_body_messages.hh"

#define PATH "dynamics/dyn_body/"

//! Namespace jeod
namespace jeod
{

// Static member data
const char * DynBodyMessages::invalid_body = PATH "invalid_body";

const char * DynBodyMessages::invalid_group = PATH "invalid_group";

const char * DynBodyMessages::invalid_name = PATH "invalid_name";

const char * DynBodyMessages::invalid_frame = PATH "invalid_frame";

const char * DynBodyMessages::invalid_attachment = PATH "invalid_attachment";

const char * DynBodyMessages::invalid_technique = PATH "invalid_technique";

const char * DynBodyMessages::not_dyn_body = PATH "not_dyn_body";

const char * DynBodyMessages::internal_error = PATH "internal_error";

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
