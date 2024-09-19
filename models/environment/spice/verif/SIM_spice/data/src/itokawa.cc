/*******************************************************************************
Purpose:
   (Default data to model the Itokawa planetary parameters.)

References:
   (((25143 Itokawa wikipedia)
   (https://en.wikipedia.org/wiki/25143_Itokawa)
   (Retrieved 02/11/2016)))

Assumptions and Limitations:
   ((None))



*******************************************************************************/

class Planet;
#define JEOD_FRIEND_CLASS Planet_itokawa_default_data

// JEOD includes
#include "environment/planet/include/base_planet.hh"
#include "environment/planet/include/planet.hh"
#include "utils/named_item/include/named_item.hh"

// Unsupported includes
#include "../include/itokawa.hh"

//! Namespace jeod
namespace jeod
{

void Planet_itokawa_default_data::initialize(Planet * Planet_ptr)
{
    Planet_ptr->name = "Itokawa";

    // Geometric mean of two longest dimensions
    Planet_ptr->r_eq = 396.6;
    Planet_ptr->r_pol = 209.0;
}
} // namespace jeod
