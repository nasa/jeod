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
namespace jeod {

// Static member data
char const * MassBodyMessages::attach_info =
    PATH "attach_info";

char const * MassBodyMessages::invalid_attach =
    PATH "invalid_attach";

char const * MassBodyMessages::invalid_detach =
    PATH "invalid_detach";

char const * MassBodyMessages::invalid_node =
    PATH "invalid_node";

char const * MassBodyMessages::invalid_name =
    PATH "invalid_name";

char const * MassBodyMessages::invalid_enum =
    PATH "invalid_enum";

char const * MassBodyMessages::io_error =
    PATH "io_error";

char const * MassBodyMessages::internal_error =
    PATH "internal_error";

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
