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
#include "../include/rel_kin_messages.hh"

#define PATH "dynamics/rel_kin/"


//! Namespace jeod
namespace jeod {

// Static member data

char const * RelKinMessages::duplicate_entry =
   PATH "duplicate_entry";

char const * RelKinMessages::entry_not_found =
   PATH "entry_not_found";

char const * RelKinMessages::invalid_entry =
   PATH "invalid_entry";

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
