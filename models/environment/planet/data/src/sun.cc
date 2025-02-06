/*******************************************************************************
Purpose:
   (Default data to model the Sun's planetary parameters.)

References:
   (((Seidelmann, P.K., ed.)(Explanatory Supplement to the Astronomical
      Almanac)(University Science Books)(1992))
    ((GSFC Sun Fact Sheet)
     (nssdc.gsfc.nasa.gov/planetary/factsheet/sunfact.html)
     (Retrieved December 22, 2010)))

Assumptions and Limitations:
   ((1. Vehicles are sufficiently far from the Sun))


*******************************************************************************/

class Planet;
#define JEOD_FRIEND_CLASS Planet_sun_default_data

// JEOD includes
#include "environment/planet/include/base_planet.hh"
#include "environment/planet/include/planet.hh"
#include "utils/named_item/include/named_item.hh"

// Unsupported includes
#include "../include/sun.hh"



//! Namespace jeod
namespace jeod {

void
Planet_sun_default_data::initialize (
   Planet * Planet_ptr)
{
   Planet_ptr->name = "Sun";

   /* Seidelmann pages 697, 700 */
   Planet_ptr->r_eq = 1000 * (696000.0);

   /* GSFC Sun fact sheet */
   Planet_ptr->flat_coeff = 5e-5;
}
} // End JEOD namespace
