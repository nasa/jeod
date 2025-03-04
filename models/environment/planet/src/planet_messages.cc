/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Planet
 * @{
 *
 * @file models/environment/planet/src/planet_messages.cc
 * Implement the class PlanetMessages.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((planet_messages.cc))



*******************************************************************************/

// System includes

// JEOD includes
#include "utils/message/include/make_message_code.hh"

// Model includes
#include "../include/planet_messages.hh"

//! Namespace jeod
namespace jeod
{

#define MAKE_PLANET_MESSAGE_CODE(id) JEOD_MAKE_MESSAGE_CODE(PlanetMessages, "environment/planet/", id)

// Static member data
MAKE_PLANET_MESSAGE_CODE(name_error);
MAKE_PLANET_MESSAGE_CODE(registration_error);
MAKE_PLANET_MESSAGE_CODE(domain_error);

#undef MAKE_PLANET_MESSAGE_CODE

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
