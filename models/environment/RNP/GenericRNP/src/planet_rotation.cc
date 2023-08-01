/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup RNP
 * @{
 * @addtogroup GenericRNP
 * @{
 *
 * @file models/environment/RNP/GenericRNP/src/planet_rotation.cc
 * Implemenation for PlanetRotation
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((None)))

Assumptions and limitations:
  ((Rotation outputted can only depend on time
   (pure virtual class, all pure virtual functions must be implemented
    in an inheriting class, this class can't be instantiated)))

Library dependencies:
  ((planet_rotation.cc)
   (planet_rotation_init.cc))

 

*******************************************************************************/

// System includes
#include <cmath>

// JEOD includes
#include "utils/math/include/matrix3x3.hh"

// Model includes
#include "../include/planet_rotation.hh"
#include "../include/planet_rotation_init.hh"


//! Namespace jeod
namespace jeod {

/**
 * Constructor, initialize base level data
 */
PlanetRotation::PlanetRotation (
   void)
:
   current_time(0.0)
{
   Matrix3x3::identity(rotation);
   // We are removing this PI in favor of M_PI. Keeping the
   // old value present just in case we need to roll back
   // (if numerical errors show up because of the change)
   // PI              = 3.14159265358979323846264338328;
   RADTODEG        = 180.0 / M_PI;
   DEGTORAD        = M_PI / 180.0;
   DEGTOSEC        = 60.0 * 60.0;
   SECTODEG        = 1.0 / (60.0 * 60.0);
   JULIANDAYTOSEC  = 86400.0;
   SECTOJULIANDAY  = 1.0 / 86400.0;
   JULIANCENTTODAY = 36525.0;
   DAYTOJULIANCENT = 1.0 / 36525.0;
}

/**
 * Destructor
 */
PlanetRotation::~PlanetRotation (
   void)
{// empty
};

/**
 * Update the time that the next update_rotation call will use
 * \param[in] time Time. units and UTC/UT1/TAI etc are determined by the individual PlanetRotation model
 */
void
PlanetRotation::update_time (
   double time)
{
   current_time = time;
   return;
}

/**
 * Copy the last calculated rotation to 'rot'
 * \param[out] rot Where the rotation matrix will be stored
 */
void
PlanetRotation::get_rotation (
   double rot[3][3])
{
   Matrix3x3::copy (rotation, rot);
   return;
}

/**
 * Same as get_rotation, but returns the transpose
 * \param[out] rot Where the transpose matrix will be stored
 */
void
PlanetRotation::get_rotation_transpose (
   double rot[3][3])
{
   Matrix3x3::transpose (rotation, rot);
   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 * @}
 */
