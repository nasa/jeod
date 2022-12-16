/*******************************************************************************
Purpose:
   (Default data to model Jupiter's planetary parameters.)

References:
   (((GSFC Jupiter Fact Sheet)
     (nssdc.gsfc.nasa.gov/planetary/factsheet/jupiterfact.html)
     (Retrieved February 10, 2012)))

Assumptions and Limitations:
   ((None))


*******************************************************************************/

class Planet;
#define JEOD_FRIEND_CLASS Planet_jupiter_default_data

// JEOD includes
#include "environment/planet/include/base_planet.hh"
#include "environment/planet/include/planet.hh"
#include "utils/named_item/include/named_item.hh"

// Unsupported includes
#include "../include/jupiter.hh"



//! Namespace jeod
namespace jeod {

void
Planet_jupiter_default_data::initialize (
   Planet * Planet_ptr)
{
   Planet_ptr->name = "Jupiter";

   /* GSFC Jupiter fact sheet */
   Planet_ptr->flat_coeff = 0.06487;
   Planet_ptr->r_eq = 1000 * (71492);
}
} // End JEOD namespace
