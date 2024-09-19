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
#include "../include/mass_messages.hh"

#define PATH "dynamics/mass/"

//! Namespace jeod
namespace jeod
{

// Static member data
const char * MassBodyMessages::attach_info = PATH "attach_info";

const char * MassBodyMessages::invalid_attach = PATH "invalid_attach";

const char * MassBodyMessages::invalid_detach = PATH "invalid_detach";

const char * MassBodyMessages::invalid_node = PATH "invalid_node";

const char * MassBodyMessages::invalid_name = PATH "invalid_name";

const char * MassBodyMessages::invalid_enum = PATH "invalid_enum";

const char * MassBodyMessages::io_error = PATH "io_error";

const char * MassBodyMessages::internal_error = PATH "internal_error";

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
