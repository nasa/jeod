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
  ((planet_rotation.cc))



*******************************************************************************/

// JEOD includes
#include "utils/math/include/matrix3x3.hh"

// Model includes
#include "../include/planet_rotation.hh"
#include "../include/planet_rotation_init.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Update the time that the next update_rotation call will use
 * \param[in] time Time. units and UTC/UT1/TAI etc are determined by the individual PlanetRotation model
 */
void PlanetRotation::update_time(double time)
{
    current_time = time;
}

/**
 * Copy the last calculated rotation to 'rot'
 * \param[out] rot Where the rotation matrix will be stored
 */
void PlanetRotation::get_rotation(double rot[3][3])
{
    Matrix3x3::copy(rotation, rot);
}

/**
 * Same as get_rotation, but returns the transpose
 * \param[out] rot Where the transpose matrix will be stored
 */
void PlanetRotation::get_rotation_transpose(double rot[3][3])
{
    Matrix3x3::transpose(rotation, rot);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 * @}
 */
