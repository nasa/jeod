/*******************************************************************************
Purpose:
   (Default data to model the Earth's planetary parameters.)

References:
   (((GSFC Earth Fact Sheet)
     (nssdc.gsfc.nasa.gov/planetary/factsheet/moonfact.html)
     (Retrieved February 10, 2012)))

Assumptions and Limitations:
   ((None))


*******************************************************************************/

class Planet;
#define JEOD_FRIEND_CLASS Planet_earth_default_data

// JEOD includes
#include "environment/planet/include/base_planet.hh"
#include "environment/planet/include/planet.hh"
#include "utils/named_item/include/named_item.hh"

// Unsupported includes
#include "../include/earth.hh"



//! Namespace jeod
namespace jeod {

void
Planet_earth_default_data::initialize (
   Planet * Planet_ptr)
{
   Planet_ptr->name = "Earth";

   /* GSFC Earth fact sheet */
   Planet_ptr->flat_inv = 298.257223563;
   Planet_ptr->r_eq = 1000 * (6378.137);
}
} // End JEOD namespace
