/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup SurfaceModel
 * @{
 *
 * @file models/utils/surface_model/src/flat_plate_thermal.cc
 * Flat plates for use in the surface model, with the thermal rider
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((None))

Library dependencies:
    ((flat_plate_thermal.o)
     (flat_plate.o)
     (interactions/thermal_rider/thermal_facet_rider.o))

 
*******************************************************************************/

#include "../include/flat_plate_thermal.hh"


//! Namespace jeod
namespace jeod {

/**
 * DefaultConstructor
 */

FlatPlateThermal::FlatPlateThermal (
   void)
{
   // empty

}

/**
 * Destructor
 */

FlatPlateThermal::~FlatPlateThermal (
   void)
{

   // empty

}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
