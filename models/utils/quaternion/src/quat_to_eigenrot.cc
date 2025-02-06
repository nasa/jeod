/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Quaternion
 * @{
 *
 * @file models/utils/quaternion/src/quat_to_eigenrot.cc
 * Define Quaternion::left_quat_to_eigen_rotation, which computes the
 * eigen rotation corresponding to a quaternion.
 */

/*******************************************************************************

Purpose:
  ()

Library dependency:
  ((quat_to_eigenrot.cc))

 

*******************************************************************************/


// System includes
#include <cmath>

// JEOD includes
#include "utils/math/include/vector3.hh"

// Model includes
#include "../include/quat.hh"


//! Namespace jeod
namespace jeod {

/**
 * Compute the eigen rotation corresponding to a quaternion.
 *
 * \par Assumptions and Limitations
 *  - Quaternion is normalized.
 * \param[out] eigen_angle Eigen angle\n Units: r
 * \param[out] eigen_axis Eigen axis
 */
void
Quaternion::left_quat_to_eigen_rotation (
   double * eigen_angle,
   double eigen_axis[3])
const
{
   double qs, qv[3];
   double sinht;

    // The left quaternion is [cos(theta/2), -sin(theta/2)*uhat],
    // where theta and uhat represent the rotation from 0-0-0 attitude.

   // Extract the canonical scalar and vector parts from the quaternion.
   if (scalar >= 0.0) {
      qs = scalar;
      Vector3::copy (vector, qv);
   } else {
      qs = -scalar;
      Vector3::negate (vector, qv);
   }

   // Compute the quaternion vector magnitude = sin(theta/2).
   sinht = Vector3::vmag (qv);

   // Compute theta and uhat.
   // The rotation angle is zero and the unit vector is ill-defined
   // when sinht is identically zero.
   if (std::fpclassify(sinht) == FP_ZERO) {
      *eigen_angle = 0.0;
      Vector3::initialize (eigen_axis);
      eigen_axis[0] = 1.0;
   }

   // Otherwise (non-zero rotation), compute
   //  uhat  via -qv/sin(theta/2)
   //  theta via 2*asin(sin(theta/2)) or 2*acos(cos(theta/2)).
   else {
      Vector3::scale (qv, -1.0/sinht, eigen_axis);

      // Use the smaller of sinht and qs to determine theta.
      if (sinht < qs) {
         *eigen_angle = 2.0 * std::asin (sinht);
      } else {
         *eigen_angle = 2.0 * std::acos (qs);
      }
   }

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
