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
    ((contact_params.o))


*******************************************************************************/

/* JEOD includes */
#include "utils/memory/include/jeod_alloc.hh"

/* Model includes */
#include "../include/contact_params.hh"

//! Namespace jeod
namespace jeod {

/**
 * Default Constructor
 */

ContactParams::ContactParams (
   void)
{
   JEOD_REGISTER_CLASS(ContactParams);
}


/**
 * Destructor
 */

ContactParams::~ContactParams (
   void)
{
   // empty for now
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
