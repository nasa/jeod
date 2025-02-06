/*******************************************************************************

Purpose:
  (Default data to model Itokawa gravity.)

References:
   (((25143 Itokawa wikipedia)
   (https://en.wikipedia.org/wiki/25143_Itokawa)
   (Retrieved 02/11/2016)))

Assumptions and Limitations:
   ((nome))



*******************************************************************************/


class SphericalHarmonicsGravitySource;
#define JEOD_FRIEND_CLASS SphericalHarmonicsGravitySource_itokawa_spherical_default_data

// JEOD includes
#include "environment/gravity/include/gravity_source.hh"
#include "environment/gravity/include/spherical_harmonics_gravity_source.hh"
#include "utils/named_item/include/named_item.hh"

// Unsupported includes
#include "../include/itokawa_spherical.hh"



//! Namespace jeod
namespace jeod {

void
SphericalHarmonicsGravitySource_itokawa_spherical_default_data::initialize (
   SphericalHarmonicsGravitySource * SphericalHarmonicsGravitySource_ptr)
{
   SphericalHarmonicsGravitySource_ptr->name = "Itokawa";
   SphericalHarmonicsGravitySource_ptr->mu =2.34;
}
} // End JEOD namespace
