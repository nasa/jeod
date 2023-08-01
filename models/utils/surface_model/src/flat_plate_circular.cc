/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup SurfaceModel
 * @{
 *
 * @file models/utils/surface_model/src/flat_plate_circular.cc
 * circular flat plates for use in the surface model
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((None))

Library dependencies:
    ((flat_plate_circular.cc)
     (flat_plate.cc))

 
*******************************************************************************/

#include "../include/flat_plate_circular.hh"
#include "utils/math/include/vector3.hh"


//! Namespace jeod
namespace jeod {

/**
 * Default Constructor
 */

FlatPlateCircular::FlatPlateCircular (
   void)
:  // Return: -- void
   radius(0.0)
{

}

/**
 * Destructor
 */

FlatPlateCircular::~FlatPlateCircular (
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
