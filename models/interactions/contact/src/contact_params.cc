/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup Contact
 * @{
 *
 * @file models/interactions/contact/src/contact_params.cc
 * contact parameters for use in the surface model
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((None))

Library dependencies:
    ((contact_params.cc))


*******************************************************************************/

/* JEOD includes */
#include "utils/memory/include/jeod_alloc.hh"

/* Model includes */
#include "../include/contact_params.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Default Constructor
 */
ContactParams::ContactParams()
{
    JEOD_REGISTER_CLASS(ContactParams);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
