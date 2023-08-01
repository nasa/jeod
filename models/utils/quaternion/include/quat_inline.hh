//=============================================================================
// Notices:
//
// Copyright © 2023 United States Government as represented by the Administrator
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
/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Quaternion
 * @{
 *
 * @file models/utils/quaternion/include/quat_inline.hh
 * Define inline methods for the quaternion class.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

 

*******************************************************************************/

#ifndef JEOD_QUATERNION_INLINE_HH
#define JEOD_QUATERNION_INLINE_HH

// System includes
#include <cmath>

// Jeod includes
#include "quat.hh"
#include "utils/math/include/vector3.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct from a scalar and a vector.
 * \param[in] s Scalar part
 * \param[in] v Vector part
 */
inline
Quaternion::Quaternion (
   const double s,
   const double v[3])
:
   scalar (s)
{
   vector[0] = v[0];
   vector[1] = v[1];
   vector[2] = v[2];
}


/**
 * Construct from a double array.
 * \param[in] arr Quaternion source
 */
inline
Quaternion::Quaternion (
   const double arr[4])
{
   copy_from (arr);
}


/**
 * Set all components of the quaternion to zero.
 */
inline void
Quaternion::set_to_zero (
   void)
{
   scalar = 0.0;
   Vector3::initialize (vector);
}


/**
 * Make the quaternion represent an identity transform.
 */
inline void
Quaternion::make_identity (
   void)
{
   scalar = 1.0;
   Vector3::initialize (vector);
}


/**
 * Copy a quaternion to a four vector, with the scalar part copied
 * to arr[0] and the vector part to arr[1] to arr[3].
 * \param[out] arr Copy of quaternion
 */
inline void
Quaternion::copy_to (
   double arr[4])
const
{
   arr[0] = scalar;
   arr[1] = vector[0];
   arr[2] = vector[1];
   arr[3] = vector[2];
}


/**
 * Copy a quaternion from a four vector, with the scalar part of
 * the quaternion in arr[0] and the vector part in arr[1] to arr[3].
 * \param[in] arr Quaternion source
 */
inline void
Quaternion::copy_from (
   const double arr[4])
{
   scalar    = arr[0];
   vector[0] = arr[1];
   vector[1] = arr[2];
   vector[2] = arr[3];
}


/**
 * Construct the quaternion corresponding to an eigen rotation.
 * \param[in] eigen_angle Eigen angle\n Units: r
 * \param[in] eigen_axis Eigen axis
 */
inline void
Quaternion::left_quat_from_eigen_rotation (
   double eigen_angle,
   const double eigen_axis[3])
{
   double htheta;
   double cosht, sinht;

   htheta = 0.5*eigen_angle;
   cosht  = std::cos (htheta);
   sinht  = std::sin (htheta);

   scalar = cosht;
   Vector3::scale (eigen_axis, -sinht, vector);
}


/**
 * Scale the quaternion by a real.
 * \param[in] fact Scale factor
 */
inline void
Quaternion::scale (
   const double fact)
{
   scalar *= fact;
   Vector3::scale (fact, vector);
}


/**
 * Scale the quaternion by a real, leaving original intact.
 * \param[in] fact Scale factor
 * \param[out] quat Scaled quaternion
 */
inline void
Quaternion::scale (
   const double fact,
   Quaternion & quat)
const
{
   quat.scalar = scalar * fact;
   Vector3::scale (vector, fact, quat.vector);
}


/**
 * Compute the square of the norm of the quaternion.
 * @return Square of the norm of the quaternion
 */
inline double
Quaternion::norm_sq (
   void)
const
{
   return scalar * scalar + Vector3::vmagsq (vector);
}


/**
 * Form the normalized quaternion, leaving original intact.
 * \param[out] quat Normalized quaternion
 */
inline void
Quaternion::normalize (
   Quaternion & quat)
const
{
   quat = *this;
   quat.normalize ();
}


/**
 * Form the normalized quaternion, leaving original intact.
 * \param[out] quat Normalized quaternion
 */
inline void
Quaternion::normalize_integ (
   Quaternion & quat)
const
{
   quat = *this;
   quat.normalize_integ ();
}


/**
 * Replace the quaternion with its conjugate.
 */
inline void
Quaternion::conjugate (
   void)
{
   Vector3::negate (vector);
}


/**
 * Form the conjugate of a quaternion, leaving original intact.
 * \param[out] quat Conjugated quaternion
 */
inline void
Quaternion::conjugate (
   Quaternion & quat)
const
{
   quat.scalar = scalar;
   Vector3::negate (vector, quat.vector);
}


/**
 * Post-multiply this quaternion by another quaternion:
 * prod = this * quat.
 * \param[in] quat Right multiplicand
 * \param[out] prod Quaternion product
 */
inline void
Quaternion::multiply (
   const Quaternion & quat,
   Quaternion & prod)
const
{
   prod.scalar = scalar * quat.scalar - Vector3::dot (vector, quat.vector);
   Vector3::scale      (quat.vector, scalar, prod.vector);
   Vector3::scale_incr (vector, quat.scalar, prod.vector);
   Vector3::cross_incr (vector, quat.vector, prod.vector);
}


/**
 * Post-multiply this quaternion by another quaternion:
 * this = this * quat.
 * \param[in] quat Right multiplicand
 */
inline void
Quaternion::multiply (
   const Quaternion & quat)
{
   double v_dot_qv;
   double v_cross_qv[3];

   v_dot_qv = Vector3::dot (vector, quat.vector);
   Vector3::cross (vector, quat.vector, v_cross_qv);

   Vector3::scale      (quat.scalar,         vector);
   Vector3::scale_incr (quat.vector, scalar, vector);
   Vector3::incr       (v_cross_qv,          vector);

   scalar = scalar * quat.scalar - v_dot_qv;
}


/**
 * Post-multiply this quaternion's conjugate by another quaternion:
 * prod = conj(this) * quat.
 * \param[in] quat Right multiplicand
 * \param[out] prod Quaternion product
 */
inline void
Quaternion::conjugate_multiply (
   const Quaternion & quat,
   Quaternion & prod)
const
{
   prod.scalar = scalar * quat.scalar + Vector3::dot (vector, quat.vector);
   Vector3::scale      (quat.vector, scalar, prod.vector);
   Vector3::scale_decr (vector, quat.scalar, prod.vector);
   Vector3::cross_decr (vector, quat.vector, prod.vector);
}


/**
 * Post-multiply this quaternion's conjugate by another quaternion:
 * this = conj(this) * quat.
 * \param[in] quat Right multiplicand
 */
inline void
Quaternion::conjugate_multiply (
   const Quaternion & quat)
{
   double v_dot_qv;
   double v_cross_qv[3];

   v_dot_qv = Vector3::dot (vector, quat.vector);
   Vector3::cross (vector, quat.vector, v_cross_qv);

   Vector3::scale      (-quat.scalar,        vector);
   Vector3::scale_incr (quat.vector, scalar, vector);
   Vector3::decr       (v_cross_qv,          vector);

   scalar = scalar * quat.scalar + v_dot_qv;
}


/**
 * Post-multiply this quaternion by another's conjugate:
 * prod = this * conj(quat).
 * \param[in] quat Right multiplicand
 * \param[out] prod Quaternion product
 */
inline void
Quaternion::multiply_conjugate (
   const Quaternion & quat,
   Quaternion & prod)
const
{
   prod.scalar = scalar * quat.scalar + Vector3::dot (vector, quat.vector);
   Vector3::scale      (vector, quat.scalar, prod.vector);
   Vector3::scale_decr (quat.vector, scalar, prod.vector);
   Vector3::cross_decr (vector, quat.vector, prod.vector);
}


/**
 * Post-multiply this quaternion by another's conjugate:
 * this = this * conj(quat).
 * \param[in] quat Right multiplicand
 */
inline void
Quaternion::multiply_conjugate (
   const Quaternion & quat)
{
   double v_dot_qv;
   double v_cross_qv[3];

   v_dot_qv = Vector3::dot (vector, quat.vector);
   Vector3::cross (vector, quat.vector, v_cross_qv);

   Vector3::scale      (quat.scalar,         vector);
   Vector3::scale_decr (quat.vector, scalar, vector);
   Vector3::decr       (v_cross_qv,          vector);

   scalar = scalar * quat.scalar + v_dot_qv;
}


/**
 * Pre-multiply this quaternion by another quaternion:
 * prod = quat * this.
 * \param[in] quat Left multiplicand
 * \param[out] prod Quaternion product
 */
inline void
Quaternion::multiply_left (
   const Quaternion & quat,
   Quaternion & prod)
const
{
   prod.scalar = quat.scalar * scalar - Vector3::dot (quat.vector, vector);
   Vector3::scale      (quat.vector, scalar, prod.vector);
   Vector3::scale_incr (vector, quat.scalar, prod.vector);
   Vector3::cross_incr (quat.vector, vector, prod.vector);
}


/**
 * Pre-multiply this quaternion by another quaternion:
 * this = quat * this.
 * \param[in] quat Left multiplicand
 */
inline void
Quaternion::multiply_left (
   const Quaternion & quat)
{
   double qv_dot_v;
   double qv_cross_v[3];

   qv_dot_v = Vector3::dot (quat.vector, vector);
   Vector3::cross (quat.vector, vector, qv_cross_v);

   Vector3::scale      (quat.scalar,         vector);
   Vector3::scale_incr (quat.vector, scalar, vector);
   Vector3::incr       (qv_cross_v,          vector);

   scalar = scalar * quat.scalar - qv_dot_v;
}


/**
 * Pre-multiply this quaternion by another's conjugate:
 * prod = conj(quat) * this.
 * \param[in] quat Left multiplicand
 * \param[out] prod Quaternion product
 */
inline void
Quaternion::multiply_left_conjugate (
   const Quaternion & quat,
   Quaternion & prod)
const
{
   prod.scalar = quat.scalar * scalar + Vector3::dot (quat.vector, vector);
   Vector3::scale      (vector, quat.scalar, prod.vector);
   Vector3::scale_decr (quat.vector, scalar, prod.vector);
   Vector3::cross_decr (quat.vector, vector, prod.vector);
}


/**
 * Pre-multiply this quaternion by another's conjugate:
 * this = conj(quat) * this.
 * \param[in] quat Left multiplicand
 */
inline void
Quaternion::multiply_left_conjugate (
   const Quaternion & quat)
{
   double qv_dot_v;
   double qv_cross_v[3];

   qv_dot_v = Vector3::dot (quat.vector, vector);
   Vector3::cross (quat.vector, vector, qv_cross_v);

   Vector3::scale      (quat.scalar,         vector);
   Vector3::scale_decr (quat.vector, scalar, vector);
   Vector3::decr       (qv_cross_v,          vector);

   scalar = scalar * quat.scalar + qv_dot_v;
}


/**
 * Pre-multiply this quaternion by a pure imaginary
 * quaternion, the latter represented by a vector:
 * prod = [0, vec] * quat.
 * \param[in] vec Right multiplicand
 * \param[out] prod Quaternion product
 */
inline void
Quaternion::multiply_vector_left (
   const double vec[3],
   Quaternion & prod)
const
{
   prod.scalar = - Vector3::dot (vector, vec);
   Vector3::scale      (vec, scalar, prod.vector);
   Vector3::cross_incr (vec, vector, prod.vector);
}


/**
 * Post-multiply this quaternion by a pure imaginary
 * quaternion, the latter represented by a vector:
 * prod = quat * [0, vec].
 * \param[in] vec Right multiplicand
 * \param[out] prod Quaternion product
 */
inline void
Quaternion::multiply_vector_right (
   const double vec[3],
   Quaternion & prod)
const
{
   prod.scalar = - Vector3::dot (vector, vec);
   Vector3::scale      (vec, scalar, prod.vector);
   Vector3::cross_incr (vector, vec, prod.vector);
}


/**
 * Transform a vector.
 * \param[in] vec_in Vector to be transformed
 * \param[out] vec_out Transformed vector
 */
inline void
Quaternion::left_quat_transform (
   const double vec_in[3],
   double vec_out[3])
const
{
   double qv_cross_v[3];
   double qv_cross_qv_cross_v[3];

   Vector3::cross (vector, vec_in, qv_cross_v);
   Vector3::cross (vector, qv_cross_v, qv_cross_qv_cross_v);

   Vector3::scale (qv_cross_v, 2.0, vec_out);
   Vector3::scale_incr (vec_in, scalar, vec_out);
   Vector3::scale (scalar, vec_out);
   Vector3::scale_incr (vector, Vector3::dot (vector, vec_in), vec_out);
   Vector3::incr (qv_cross_qv_cross_v, vec_out);
}



/**
 * Compute eigen decomposition of this*conj(quat).
 * \param[in] quat Quaternion to compare to
 * \param[out] eigen_angle Eigen angle\n Units: r
 * \param[out] eigen_axis Eigen axis
 */
inline void
Quaternion::eigen_compare (
   const Quaternion & quat,
   double * eigen_angle,
   double eigen_axis[3])
const
{
   Quaternion prod;
   multiply_conjugate (quat, prod);
   prod.left_quat_to_eigen_rotation (eigen_angle, eigen_axis);
}


/**
 * Compute the time derivative of a left quaternion.
 * \param[in] ang_vel Angular velocity\n Units: r/s
 * \param[out] qdot Quaternion derivative
 */
inline void
Quaternion::compute_left_quat_deriv (
   const double ang_vel[3],
   Quaternion & qdot)
const
{
   double mhang_vel[3];                 // r/s -0.5 * ang_vel
   Vector3::scale (ang_vel, -0.5, mhang_vel);
   multiply_vector_left (mhang_vel, qdot);
}


/**
 * Compute the time derivative of a left quaternion.
 * \param[in] ang_vel Angular velocity\n Units: r/s
 * \param[in] ang_acc Angular acceleration\n Units: r/s2
 * \param[out] qddot Quaternion 2nd deriv
 */
inline void
Quaternion::compute_left_quat_second_deriv (
   const double ang_vel[3],
   const double ang_acc[3],
   Quaternion & qddot)
const
{
   double mhacc[3];                     // r/s -0.5 * ang_acc
   Vector3::scale (ang_acc, -0.5, mhacc);
   Quaternion left (-0.25*Vector3::vmagsq(ang_vel), mhacc);
   multiply_left (left, qddot);
}


/**
 * Compute the time derivative of a left quaternion.
 * \param[in] quat Quaternion as 4-vector
 * \param[in] ang_vel Angular velocity\n Units: r/s
 * \param[out] qdot Derivative as 4-vector
 */
inline void
Quaternion::compute_left_quat_deriv (
   const double quat[4],
   const double ang_vel[3],
   double qdot[4])
{
   double mhang_vel[3];               // r/s -0.5 * ang_vel
   Vector3::scale (ang_vel, -0.5, mhang_vel);
   qdot[0] = - Vector3::dot ((quat+1), mhang_vel);
   Vector3::scale      (mhang_vel, quat[0],  (qdot+1));
   Vector3::cross_incr (mhang_vel, (quat+1), (qdot+1));
}


/**
 * Compute the second time derivative of a left quaternion.
 * \param[in] quat Quaternion as 4-vector
 * \param[in] ang_vel Angular velocity\n Units: r/s
 * \param[in] ang_acc Angular acceleration\n Units: r/s2
 * \param[out] qddot 2nd derivative as 4-vector
 */
inline void
Quaternion::compute_left_quat_second_deriv (
   const double quat[4],
   const double ang_vel[3],
   const double ang_acc[3],
   double qddot[4])
{
   double mhwsq = -0.25 * Vector3::vmagsq(ang_vel);
   double mhacc[3];

   Vector3::scale (ang_acc, -0.5, mhacc);

   qddot[0] = mhwsq*quat[0] - Vector3::dot (mhacc, (quat+1));
   Vector3::scale      ((quat+1), mhwsq,    (qddot+1));
   Vector3::scale_incr (mhacc,    quat[0],  (qddot+1));
   Vector3::cross_incr (mhacc,    (quat+1), (qddot+1));
}


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
