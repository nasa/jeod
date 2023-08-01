/*******************************************************************************

 Purpose:
 (Create a terrain class for use with ground contact in JEOD example simulation
 that finds altitude and normal based on planetary radius.)

Reference:
  (GDAL.org)

Assumptions and limitations:
  ()

Library dependencies:
  ((terrain_radius.cc))


*******************************************************************************/

// jeod includes
#include "utils/math/include/vector3.hh"

// Local includes
#include "../include/terrain_radius.hh"

//! Namespace jeod
namespace jeod {

TerrainRadius::TerrainRadius (
	void)
{

}

TerrainRadius::~TerrainRadius( // Return: -- N/A
    void)
{

}

int
TerrainRadius::initialize ( // Return: -- status
  Planet * planet_in)
{
   planet = planet_in;

   return 0;
}

int
TerrainRadius::find_altitude ( // Return: -- status
   PlanetFixedPosition *point, // In: r latitude in radians
   double normal[3]) // In: r longitude in radiams
{

   point->ellip_coords.set_data(0.0, point->ellip_coords.latitude, point->ellip_coords.longitude);
   point->update_from_ellip(point->ellip_coords);
   Vector3::normalize(point->cart_coords, normal);

   return 1;
}

} // End JEOD namespace
