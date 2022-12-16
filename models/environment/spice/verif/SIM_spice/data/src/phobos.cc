/*******************************************************************************
Purpose:
   (Default data to model the Phobos planetary parameters.)

References:
   (((NSSDC Mars Fact Sheet)
   (http://nssdc.gsfc.nasa.gov/planetary/factsheet/marsfact.html)
   (Last updated 04/19/2016)))

Assumptions and Limitations:
   ((None))



*******************************************************************************/

class Planet;
#define JEOD_FRIEND_CLASS Planet_phobos_default_data

// JEOD includes
#include "environment/planet/include/base_planet.hh"
#include "environment/planet/include/planet.hh"
#include "utils/named_item/include/named_item.hh"

// Unsupported includes
#include "../include/phobos.hh"



//! Namespace jeod
namespace jeod {

void
Planet_phobos_default_data::initialize (
   Planet * Planet_ptr)
{
   Planet_ptr->name = "Phobos";

   // r_eq = geometric mean of two largest dimensions
   Planet_ptr->r_eq = 12173.74;
   Planet_ptr->r_pol =9100.0;
}
} // End JEOD namespace
