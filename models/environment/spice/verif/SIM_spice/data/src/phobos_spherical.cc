
/*******************************************************************************

Purpose:
  (Default data to model Phobos gravity.)

References:
   (((NSSDC Mars Fact Sheet)
   (http://nssdc.gsfc.nasa.gov/planetary/factsheet/marsfact.html)
   (Last updated 04/19/2016)))

Assumptions and Limitations:
(((none)))



*******************************************************************************/

class SphericalHarmonicsGravitySource;
#define JEOD_FRIEND_CLASS SphericalHarmonicsGravitySource_itokawa_spherical_default_data

// JEOD includes
#include "environment/gravity/include/gravity_source.hh"
#include "environment/gravity/include/spherical_harmonics_gravity_source.hh"
#include "utils/named_item/include/named_item.hh"

// Unsupported includes
#include "../include/phobos_spherical.hh"

//! Namespace jeod
namespace jeod
{

void SphericalHarmonicsGravitySource_phobos_spherical_default_data::initialize(
    SphericalHarmonicsGravitySource * SphericalHarmonicsGravitySource_ptr)
{
    SphericalHarmonicsGravitySource_ptr->name = "Phobos";
    SphericalHarmonicsGravitySource_ptr->mu = 7.07E+05;
}
} // namespace jeod
