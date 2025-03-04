/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup OrbitalElements
 * @{
 *
 * @file models/utils/orbital_elements/src/orbital_elements_messages.cc
 * Implement the class OrbitalElementsMessages.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((orbital_elements_messages.cc))



*******************************************************************************/

// System includes

// JEOD includes
#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/orbital_elements_messages.hh"

//! Namespace jeod
namespace jeod
{

#define MAKE_ORBITALELEMENTS_MESSAGE_CODE(id)                                                                          \
    JEOD_MAKE_MESSAGE_CODE(OrbitalElementsMessages, "utils/orbital_elements/", id)

// Static member data
MAKE_ORBITALELEMENTS_MESSAGE_CODE(domain_error);
MAKE_ORBITALELEMENTS_MESSAGE_CODE(convergence_error);

#undef MAKE_ORBITALELEMENTS_MESSAGE_CODE

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
