/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup SurfaceModel
 * @{
 *
 * @file models/utils/surface_model/src/cylinder.cc
 * cylinders for use in the surface model
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
     (cylinder.cc))

 
*******************************************************************************/

#include "../include/cylinder.hh"
#include "utils/math/include/vector3.hh"


//! Namespace jeod
namespace jeod {

/**
 * Default Constructor
 */

Cylinder::Cylinder (
   void)
:  // Return: -- void
   length(0.0)
{

}

/**
 * Destructor
 */

Cylinder::~Cylinder (
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
