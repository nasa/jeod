/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Quaternion
 * @{
 *
 * @file models/utils/quaternion/src/quat_norm.cc
 * Define quaternion normalization methods.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((IEEE 754 / IEC 60559:1989 double precision floating point standard.))

Library dependencies:
  ((quat_norm.o))

 

*******************************************************************************/

// System includes
#include <cmath>

// JEOD includes

// Model includes
#include "../include/quat.hh"


//! Namespace jeod
namespace jeod {

/**
 * Normalize the quaternion,
 * making the scalar part of the quaternion non-negative.
 */
void
Quaternion::normalize ()
{
   double qmagsq = norm_sq ();
   double diff1 = 1.0 - qmagsq;
   double fact;

   // Compute the normalization fact, nominally 1/sqrt(qmagsq).
   // Computational shortcut: Approximate as 2/(1+qmagsq)
   // To second order, the error in the approximation is diff1^2/8.
   // The approximation is valid if this error is smaller than numerical
   // precision. A double IEEE floating point number has a 53 bit mantissa plus
   // an implied 1 to the left of the binary point. The validity limit is thus
   // sqrt(8*2^(-54)) = 2.107342e-08, to the accuracy of the appoximation.
   if ((diff1 > -2.107342e-08) && (diff1 < 2.107342e-08)) {
      fact = 2.0 / (1.0 + qmagsq);
   } else {
      fact = 1.0 / std::sqrt (qmagsq);
   }

   // Negate the scale factor if needed to make the scalar part non-negative.
   if (scalar < 0.0) {
      fact = -fact;
   }

   // Scale the quaternion by the above normalization factor.
   scale (fact);
}


/**
 * Normalize the quaternion,
 * but do not make the scalar part non-negative.
 */
void
Quaternion::normalize_integ ()
{
   double qmagsq = norm_sq ();
   double diff1 = 1.0 - qmagsq;
   double fact;

   // Compute the normalization fact, see discussion in normalize().
   if ((diff1 > -2.107342e-08) && (diff1 < 2.107342e-08)) {
      fact = 2.0 / (1.0 + qmagsq);
   } else {
      fact = 1.0 / std::sqrt (qmagsq);
   }

   // Scale the quaternion by the above normalization factor.
   scale (fact);
}


/**
 * Normalize the quaternion,
 * but do not make the scalar part non-negative.
 * @param quat  Quaternion to be normalized.
 */
void
Quaternion::normalize_integ (
   double quat[4])
{
   double qmagsq = Numerical::square(quat[0]) + Vector3::vmagsq(quat+1);
   double diff1 = 1.0 - qmagsq;
   double fact;

   // Compute the normalization fact, see discussion in normalize().
   if ((diff1 > -2.107342e-08) && (diff1 < 2.107342e-08)) {
      fact = 2.0 / (1.0 + qmagsq);
   } else {
      fact = 1.0 / std::sqrt(qmagsq);
   }

   // Scale the quaternion by the above normalization factor.
   for (int ii = 0; ii < 4; ++ii) {
      quat[ii] *= fact;
   }
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
