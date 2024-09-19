/*******************************************************************************

 Purpose:
 (Create a terrain base class for use with ground contact in JEOD example simulation.)

Reference:
  (GDAL.org)

Assumptions and limitations:
  ()

Library dependencies:
  ((terrain.cc))


*******************************************************************************/

// Local includes
#include "../include/terrain.hh"

// jeod includes
#include "utils/math/include/vector3.hh"

//! Namespace jeod
namespace jeod
{

int Terrain::calculate_normal(double point1[3], double point2[3], double point3[3])
{
    double line1[3];
    double line2[3];

    Vector3::diff(point2, point1, line1);
    Vector3::diff(point3, point1, line2);

    Vector3::cross(line1, line2, point1);

    Vector3::normalize(point1);

    return 0;
}

} // namespace jeod
