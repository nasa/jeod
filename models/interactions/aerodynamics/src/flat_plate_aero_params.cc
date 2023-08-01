/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup Aerodynamics
 * @{
 *
 * @file models/interactions/aerodynamics/src/flat_plate_aero_params.cc
 * Flat plates aerodynamic parameters for use in the surface model
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((None))

Library dependencies:
    ((flat_plate_aero_params.cc)
     (aero_params.cc)
     (aero_drag.cc))


*******************************************************************************/

#include "../include/flat_plate_aero_params.hh"

//! Namespace jeod
namespace jeod {

/**
 * Default Constructor
 */

FlatPlateAeroParams::FlatPlateAeroParams (
   void)
: // Return: -- None
   coef_method(AeroDragEnum::Specular),
   calculate_drag_coef(false),
   epsilon(0.0),
   temp_reflect(0.0),
   drag_coef_norm(0.0),
   drag_coef_tang(0.0),
   drag_coef_spec(0.0),
   drag_coef_diff(0.0)
{
   // empty for now
}

/**
 * Destructor
 */

FlatPlateAeroParams::~FlatPlateAeroParams (
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
