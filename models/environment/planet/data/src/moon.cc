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
   /* Transformation from Moon_PA to Moon_ME */
   double m[3][3]={
                   {0.99999987, -0.00032929, 0.00038087},
                   {0.00032929, 0.99999995, -0.00000145444094},
                   {-0.00038087, 0.00000157985579, 0.99999993}};
   

   Planet_ptr->name = "Moon";

   /* GSFC Moon fact sheet */
   Planet_ptr->flat_coeff = 0.00125;
   Planet_ptr->r_eq = 1000 * (1738.14);

   Planet_ptr->set_alt_pfix(m);
}
} // End JEOD namespace
