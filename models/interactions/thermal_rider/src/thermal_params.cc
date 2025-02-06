/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup ThermalRider
 * @{
 *
 * @file models/interactions/thermal_rider/src/thermal_params.cc
 * Thermal Parameter definition
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((The thermal rider works with a surface interaction model.
        This is not a stand-alone interaction model))

Library dependencies:
   ((thermal_params.o)
    (utils/surface_model/interaction_surface.o))


*******************************************************************************/

/* System includes */

/*  JEOD includes */

/* Model structure includes */
#include "../include/thermal_params.hh"
#include "utils/surface_model/include/interaction_surface.hh"



//! Namespace jeod
namespace jeod {

/**
 * Constructor
 */
ThermalParams::ThermalParams (
   void)
{
   emissivity             = 0.0;
   heat_capacity_per_area = 1.0E+20;
   thermal_power_dump     = 0.0;
}

/**
 * Destructor
 */
ThermalParams::~ThermalParams (
   void)
{
}


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
