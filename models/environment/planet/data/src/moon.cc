/*******************************************************************************
Purpose:
   (Default data to model the Moon's planetary parameters.)

References:
   (((GSFC Moon Fact Sheet)
     (nssdc.gsfc.nasa.gov/planetary/factsheet/moonfact.html)
     (Retrieved February 10, 2012)))

Assumptions and Limitations:
   ((None))


*******************************************************************************/

class Planet;
#define JEOD_FRIEND_CLASS Planet_moon_default_data

// JEOD includes
#include "environment/planet/include/base_planet.hh"
#include "environment/planet/include/planet.hh"
#include "utils/named_item/include/named_item.hh"

// Unsupported includes
#include "../include/moon.hh"



//! Namespace jeod
namespace jeod {

void
Planet_moon_default_data::initialize (
   Planet * Planet_ptr)
{
   Planet_ptr->name = "Moon";

   /* GSFC Moon fact sheet */
   Planet_ptr->flat_coeff = 0.00125;
   Planet_ptr->r_eq = 1000 * (1738.14);
}
} // End JEOD namespace
