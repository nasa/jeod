//=============================================================================
// Notices:
//
// Copyright © 2025 United States Government as represented by the Administrator
// of the National Aeronautics and Space Administration.  All Rights Reserved.
//
//
// Disclaimers:
//
// No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF
// ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED
// TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO SPECIFICATIONS, ANY
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
// FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL BE ERROR
// FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO THE
// SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN
// ENDORSEMENT BY GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS,
// RESULTING DESIGNS, HARDWARE, SOFTWARE PRODUCTS OR ANY OTHER APPLICATIONS
// RESULTING FROM USE OF THE SUBJECT SOFTWARE.  FURTHER, GOVERNMENT AGENCY
// DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING THIRD-PARTY SOFTWARE,
// IF PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT "AS IS."
//
// Waiver and Indemnity:  RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS AGAINST THE
// UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
// PRIOR RECIPIENT.  IF RECIPIENT'S USE OF THE SUBJECT SOFTWARE RESULTS IN ANY
// LIABILITIES, DEMANDS, DAMAGES, EXPENSES OR LOSSES ARISING FROM SUCH USE,
// INCLUDING ANY DAMAGES FROM PRODUCTS BASED ON, OR RESULTING FROM, RECIPIENT'S
// USE OF THE SUBJECT SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE
// UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
// PRIOR RECIPIENT, TO THE EXTENT PERMITTED BY LAW.  RECIPIENT'S SOLE REMEDY FOR
// ANY SUCH MATTER SHALL BE THE IMMEDIATE, UNILATERAL TERMINATION OF THIS
// AGREEMENT.
//
//=============================================================================
//
//
/*******************************************************************************

 Purpose:
 (Create a terrain base class for use with ground contact in JEOD example simulation.)

 Reference:
 (GDAL.org)

 Assumptions and limitations:
 ()

 Library dependencies:
 ((../src/terrain.cc)
  (utils/planet_fixed/planet_fixed_posn/src/planet_fixed_posn.cc))


 *******************************************************************************/
#ifndef JEOD_TERRAIN_HH_
#define JEOD_TERRAIN_HH_

// JEOD includes
#include "environment/planet/include/planet.hh"
#include "utils/planet_fixed/planet_fixed_posn/include/planet_fixed_posn.hh"

//! Namespace jeod
namespace jeod
{

class Terrain
{
public:
    Planet * planet{};
    double offset{}; /* trick_units(m) @n
       Offset to match sim to graphics. */

    Terrain() = default;
    virtual ~Terrain() = default;
    Terrain(const Terrain &) = delete;
    Terrain & operator=(const Terrain &) = delete;

    /*  initialize the terrain model */
    virtual int initialize(Planet * planet_in) = 0;

    /* find the altitude given a specific lat-long  */
    virtual int find_altitude(PlanetFixedPosition * point, double normal[3]) = 0;

    /* find the normal given three points in the planet frame */
    int calculate_normal(double point1[3], double point2[3], double point3[3]);
};

} // namespace jeod

#endif // end JEOD_TERRAIN_HH_ -- nothing after this line!
