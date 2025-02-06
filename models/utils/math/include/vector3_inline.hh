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
 * @addtogroup Math
 * @{
 *
 * @file models/utils/math/include/vector3_inline.hh
 * Vector math inline functions
 */

/*******************************************************************************
Purpose:
  ()

 
*******************************************************************************/


#ifndef JEOD_VECTOR3_INLINE_H
#define JEOD_VECTOR3_INLINE_H

// System includes
#include <cmath>

// JEOD includes
#include "vector3.hh"
#include "numerical.hh"


//! Namespace jeod
namespace jeod {

/**
 * Zero-fill vector,
 * vec[i] = 0.0
 * @return Zero-filled vector
 * \param[out] vec Zero-filled vector
 */
inline double *
Vector3::initialize (
   double vec[3])
{
   vec[0] = vec[1] = vec[2] = 0.0;
   return vec;
}


/**
 * Construct unit vector,
 * vec[i] = delta_ij (delta_ij is the Kronecker delta)
 * @return Unit vector
 * \param[in] index Unit index: 0,1,2=x,y,z hat
 * \param[out] vec Unit vector
 */
inline double *
Vector3::unit (
   unsigned int index,
   double vec[3])
{
   vec[0]     = vec[1] = vec[2] = 0.0;
   vec[index] = 1.0;
   return vec;
}


/**
 * Construct a vector from scalar,
 * vec[i] = scalar
 * @return Filled vector
 * \param[in] scalar Scalar
 * \param[out] vec Filled vector
 */
inline double *
Vector3::fill (
   double scalar,
   double vec[3])
{
   vec[0] = vec[1] = vec[2] = scalar;
   return vec;
}


/**
 * Zero-out small components of a vector,
 * vec[i] = 0 if abs(vec[i]) < limit
 * @return Truncated vector
 * \param[in] limit Limit
 * \param[in,out] vec Truncated vector
 */
inline double *
Vector3::zero_small (
   double limit,
   double vec[3])
{

   if (Numerical::fabs (vec[0]) < limit) {
      vec[0] = 0.0;
   }

   if (Numerical::fabs (vec[1]) < limit) {
      vec[1] = 0.0;
   }

   if (Numerical::fabs (vec[2]) < limit) {
      vec[2] = 0.0;
   }

   return vec;
}


/**
 * Copy vector contents,
 * copy[i] = vec[i]
 * @return Copied vector
 * \param[in] vec Source vector
 * \param[out] copy Copied vector
 */
inline double *
Vector3::copy (
   double const vec[3],
   double copy[3])
{
   copy[0] = vec[0];
   copy[1] = vec[1];
   copy[2] = vec[2];

   return copy;
}


/**
 * Compute vector inner product,
 * result = sum_i vec1[i] * vec2[i]
 * @return Inner product
 * \param[in] vec2 Vector 2
 * \param[in] vec1 Vector 1
 */
inline double
Vector3::dot (
   double const vec2[3],
   double const vec1[3])
{
   return vec1[0] * vec2[0] +
          vec1[1] * vec2[1] +
          vec1[2] * vec2[2];
}


/**
 * Compute square of vector magnitude,
 * result = dot(vec,vec), but protects against underflow
 * @return Inner product
 * \param[in] vec Vector
 */
inline double
Vector3::vmagsq (
   double const vec[3])
{
    return vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2];
}


/**
 * Compute vector magnitude,
 * result = sqrt(vmagsq(vec))
 * @return Vector magnitude
 * \param[in] vec Vector
 */
inline double
Vector3::vmag (
   double const vec[3])
{
   return sqrt (vmagsq (vec));
}


/**
 * Make vector a unit vector in-place,
 * vec = vec * 1/vmag(vec)
 * @return Normalized vector
 * \param[in,out] vec Vector
 */
inline double *
Vector3::normalize (
   double vec[3])
{
   double mag = vmag (vec);

   if (mag > 0.0) {
      scale (1.0 / mag, vec);
   }
   else {
      initialize (vec);
   }

   return vec;
}


/**
 * Construct unit vector,
 * unit_vec = vec * 1/vmag(vec)
 * @return Unit vector
 * \param[in] vec Vector
 * \param[out] unit_vec Unit vector
 */
inline double *
Vector3::normalize (
   double const vec[3],
   double unit_vec[3])
{
   normalize (copy (vec, unit_vec));

   return unit_vec;
}


/**
 * Scale a vector in-place,
 * vec[i] = scalar
 * @return Scaled vector
 * \param[in] scalar Scalar
 * \param[in,out] vec Scaled vector
 */
inline double *
Vector3::scale (
   double scalar,
   double vec[3])
{
   vec[0] *= scalar;
   vec[1] *= scalar;
   vec[2] *= scalar;

   return vec;
}


/**
 * Scale a vector,
 * prod[i] = vec[i] * scalar
 * @return Scaled vector
 * \param[in] vec Source vector
 * \param[in] scalar Scalar
 * \param[out] prod Scaled vector
 */
inline double *
Vector3::scale (
   double const vec[3],
   double scalar,
   double prod[3])
{
   prod[0] = vec[0] * scalar;
   prod[1] = vec[1] * scalar;
   prod[2] = vec[2] * scalar;

   return prod;
}


/**
 * Negate vector in-place,
 * vec[i] = -vec[i]
 * @return Negated vector
 * \param[in,out] vec Vector
 */
inline double *
Vector3::negate (
   double vec[3])
{
   vec[0] = -vec[0];
   vec[1] = -vec[1];
   vec[2] = -vec[2];

   return vec;
}


/**
 * Negate vector,
 * copy[i] = -vec[i]
 * @return Negated vector
 * \param[in] vec Source vector
 * \param[out] copy Negated vector
 */
inline double *
Vector3::negate (
   double const vec[3],
   double copy[3])
{
   copy[0] = -vec[0];
   copy[1] = -vec[1];
   copy[2] = -vec[2];

   return copy;
}


/**
 * Transform a column vector,
 * prod[i] = tmat[i][j]*vec[j]
 * @return Transformed vector
 * \param[in] tmat Transformation matrix
 * \param[in] vec Source vector
 * \param[out] prod Transformed vector
 */
inline double *
Vector3::transform (
   double const tmat[3][3],
   double const vec[3],
   double prod[3])
{

   prod[0] = tmat[0][0] * vec[0] +
             tmat[0][1] * vec[1] +
             tmat[0][2] * vec[2];

   prod[1] = tmat[1][0] * vec[0] +
             tmat[1][1] * vec[1] +
             tmat[1][2] * vec[2];

   prod[2] = tmat[2][0] * vec[0] +
             tmat[2][1] * vec[1] +
             tmat[2][2] * vec[2];

   return prod;
}


/**
 * Transform a column vector in-place,
 * vec[i] <- tmat[i][j]*vec[j]
 * @return Transformed vector
 * \param[in] tmat Transformation matrix
 * \param[in,out] vec Transformed vector
 */
inline double *
Vector3::transform (
   double const tmat[3][3],
   double vec[3])
{
   double temp[3];

   transform (tmat, vec, temp);
   copy (temp, vec);

   return vec;
}


/**
 * Transform a column vector with the transpose,
 * prod[i] = tmat[j][i]*vec[j]
 * @return Transformed vector
 * \param[in] tmat Transformation matrix
 * \param[in] vec Source vector
 * \param[out] prod Transformed vector
 */
inline double *
Vector3::transform_transpose (
   double const tmat[3][3],
   double const vec[3],
   double prod[3])
{

   prod[0] = tmat[0][0] * vec[0] +
             tmat[1][0] * vec[1] +
             tmat[2][0] * vec[2];

   prod[1] = tmat[0][1] * vec[0] +
             tmat[1][1] * vec[1] +
             tmat[2][1] * vec[2];

   prod[2] = tmat[0][2] * vec[0] +
             tmat[1][2] * vec[1] +
             tmat[2][2] * vec[2];

   return prod;
}


/**
 * Transform a column vector in-place with the transpose,
 * vec[i] <- tmat[j][i]*vec[j]
 * @return Transformed vector
 * \param[in] tmat Transformation matrix
 * \param[in,out] vec Transformed vector
 */
inline double *
Vector3::transform_transpose (
   double const tmat[3][3],
   double vec[3])
{
   double temp[3];

   transform_transpose (tmat, vec, temp);
   copy (temp, vec);

   return vec;
}


/**
 * Increment a vector,
 * vec[i] += addend[i]
 * @return Incremented vector
 * \param[in] addend Increment
 * \param[in,out] vec Vector
 */
inline double *
Vector3::incr (
   double const addend[3],
   double vec[3])
{
   vec[0] += addend[0];
   vec[1] += addend[1];
   vec[2] += addend[2];

   return vec;
}


/**
 * Increment a vector,
 * vec[i] += addend1[i] + addend2[i]
 * @return Incremented vector
 * \param[in] addend1 Increment
 * \param[in] addend2 Increment
 * \param[in,out] vec Vector
 */
inline double *
Vector3::incr (
   double const addend1[3],
   double const addend2[3],
   double vec[3])
{
   vec[0] += addend1[0] + addend2[0];
   vec[1] += addend1[1] + addend2[1];
   vec[2] += addend1[2] + addend2[2];

   return vec;
}


/**
 * Decrement a vector,
 * vec[i] -= subtrahend[i]
 * @return Decremented vector
 * \param[in] subtrahend Decrement
 * \param[in,out] vec Vector
 */
inline double *
Vector3::decr (
   double const subtrahend[3],
   double vec[3])
{
   vec[0] -= subtrahend[0];
   vec[1] -= subtrahend[1];
   vec[2] -= subtrahend[2];

   return vec;
}


/**
 * Decrement a vector,
 * vec[i] -= subtrahend1[i] + subtrahend2[i]
 * @return Decremented vector
 * \param[in] subtrahend1 Decrement
 * \param[in] subtrahend2 Decrement
 * \param[in,out] vec Vector
 */
inline double *
Vector3::decr (
   double const subtrahend1[3],
   double const subtrahend2[3],
   double vec[3])
{
   vec[0] -= subtrahend1[0] + subtrahend2[0];
   vec[1] -= subtrahend1[1] + subtrahend2[1];
   vec[2] -= subtrahend1[2] + subtrahend2[2];

   return vec;
}


/**
 * Compute the sum of two vectors,
 * vec[i] = addend1[i] + addend2[i]
 * @return Sum vector
 * \param[in] addend1 Addend
 * \param[in] addend2 Addend
 * \param[out] vec Sum vector
 */
inline double *
Vector3::sum (
   double const addend1[3],
   double const addend2[3],
   double vec[3])
{
   vec[0] = addend1[0] + addend2[0];
   vec[1] = addend1[1] + addend2[1];
   vec[2] = addend1[2] + addend2[2];

   return vec;
}


/**
 * Compute the sum of three vectors,
 * vec[i] = addend1[i] + addend2[i] + addend3[i]
 * @return Sum vector
 * \param[in] addend1 Addend
 * \param[in] addend2 Addend
 * \param[in] addend3 Addend
 * \param[out] vec Sum vector
 */
inline double *
Vector3::sum (
   double const addend1[3],
   double const addend2[3],
   double const addend3[3],
   double vec[3])
{
   vec[0] = addend1[0] + addend2[0] + addend3[0];
   vec[1] = addend1[1] + addend2[1] + addend3[1];
   vec[2] = addend1[2] + addend2[2] + addend3[2];

   return vec;
}


/**
 * Compute the difference between two vectors,
 * diff[i] = minuend[i] - subtrehend[i]
 * @return Difference vector
 * \param[in] minuend Minuend
 * \param[in] subtrahend Subtrahend
 * \param[out] vec Difference vector
 */
inline double *
Vector3::diff (
   double const minuend[3],
   double const subtrahend[3],
   double vec[3])
{
   vec[0] = minuend[0] - subtrahend[0];
   vec[1] = minuend[1] - subtrahend[1];
   vec[2] = minuend[2] - subtrahend[2];

   return vec;
}


/**
 * Compute the cross product between two vectors,
 * prod[i] = epsilon_ijk * vec_left[j] * vec_right[k]
 * @return Cross product vector
 * \param[in] vec_left Left vector
 * \param[in] vec_right Right vector
 * \param[out] prod Cross product vector
 */
inline double *
Vector3::cross (
   double const vec_left[3],
   double const vec_right[3],
   double prod[3])
{
   prod[0] = vec_left[1] * vec_right[2] - vec_left[2] * vec_right[1];
   prod[1] = vec_left[2] * vec_right[0] - vec_left[0] * vec_right[2];
   prod[2] = vec_left[0] * vec_right[1] - vec_left[1] * vec_right[0];

   return prod;
}


/**
 * Increment a vector with a scaled vector,
 * prod[i] += scalar*vec[i]
 * @return Incremented vector
 * \param[in] vec Source vector
 * \param[in] scalar Scalar
 * \param[in,out] prod Incremented vector
 */
inline double *
Vector3::scale_incr (
   double const vec[3],
   double scalar,
   double prod[3])
{
   prod[0] += vec[0] * scalar;
   prod[1] += vec[1] * scalar;
   prod[2] += vec[2] * scalar;

   return prod;
}


/**
 * Decrement a vector with a scaled vector,
 * prod[i] += scalar*vec[i]
 * @return Decremented vector
 * \param[in] vec Source vector
 * \param[in] scalar Scalar
 * \param[in,out] prod Decremented vector
 */
inline double *
Vector3::scale_decr (
   double const vec[3],
   double scalar,
   double prod[3])
{
   prod[0] -= vec[0] * scalar;
   prod[1] -= vec[1] * scalar;
   prod[2] -= vec[2] * scalar;

   return prod;
}


/**
 * Increment a vector with the the cross product between two vectors,
 * prod[i] += epsilon_ijk * vec_left[j] * vec_right[k]
 * @return Cross product vector
 * \param[in] vec_left Left vector
 * \param[in] vec_right Right vector
 * \param[in,out] prod Cross product vector
 */
inline double *
Vector3::cross_incr (
   double const vec_left[3],
   double const vec_right[3],
   double prod[3])
{
   prod[0] += vec_left[1] * vec_right[2] - vec_left[2] * vec_right[1];
   prod[1] += vec_left[2] * vec_right[0] - vec_left[0] * vec_right[2];
   prod[2] += vec_left[0] * vec_right[1] - vec_left[1] * vec_right[0];

   return prod;
}


/**
 * Decrement a vector with the the cross product between two vectors,
 * prod[i] -= epsilon_ijk * vec_left[j] * vec_right[k]
 * @return Decremented vector
 * \param[in] vec_left Left vector
 * \param[in] vec_right Right vector
 * \param[in,out] prod Decremented vector
 */
inline double *
Vector3::cross_decr (
   double const vec_left[3],
   double const vec_right[3],
   double prod[3])
{
   prod[0] -= vec_left[1] * vec_right[2] - vec_left[2] * vec_right[1];
   prod[1] -= vec_left[2] * vec_right[0] - vec_left[0] * vec_right[2];
   prod[2] -= vec_left[0] * vec_right[1] - vec_left[1] * vec_right[0];

   return prod;
}


/**
 * Increment a vector with a transformed column vector,
 * prod[i] += tmat[i][j]*vec[j]
 * @return Incremented vector
 * \param[in] tmat Transformation matrix
 * \param[in] vec Source vector
 * \param[in,out] prod Incremented vector
 */
inline double *
Vector3::transform_incr (
   double const tmat[3][3],
   double const vec[3],
   double prod[3])
{

   prod[0] += tmat[0][0] * vec[0] +
              tmat[0][1] * vec[1] +
              tmat[0][2] * vec[2];

   prod[1] += tmat[1][0] * vec[0] +
              tmat[1][1] * vec[1] +
              tmat[1][2] * vec[2];

   prod[2] += tmat[2][0] * vec[0] +
              tmat[2][1] * vec[1] +
              tmat[2][2] * vec[2];

   return prod;
}


/**
 * Decrement a vector with a transformed column vector,
 * prod[i] += tmat[i][j]*vec[j]
 * @return Decremented vector
 * \param[in] tmat Transformation matrix
 * \param[in] vec Source vector
 * \param[in,out] prod Decremented vector
 */
inline double *
Vector3::transform_decr (
   double const tmat[3][3],
   double const vec[3],
   double prod[3])
{

   prod[0] -= tmat[0][0] * vec[0] +
              tmat[0][1] * vec[1] +
              tmat[0][2] * vec[2];

   prod[1] -= tmat[1][0] * vec[0] +
              tmat[1][1] * vec[1] +
              tmat[1][2] * vec[2];

   prod[2] -= tmat[2][0] * vec[0] +
              tmat[2][1] * vec[1] +
              tmat[2][2] * vec[2];

   return prod;
}


/**
 * Increment a vector with a transpose-transformed column vector,
 * prod[i] += tmat[j][i]*vec[j]
 * @return Incremented vector
 * \param[in] tmat Transformation matrix
 * \param[in] vec Source vector
 * \param[in,out] prod Incremented vector
 */
inline double *
Vector3::transform_transpose_incr (
   double const tmat[3][3],
   double const vec[3],
   double prod[3])
{

   prod[0] += tmat[0][0] * vec[0] +
              tmat[1][0] * vec[1] +
              tmat[2][0] * vec[2];

   prod[1] += tmat[0][1] * vec[0] +
              tmat[1][1] * vec[1] +
              tmat[2][1] * vec[2];

   prod[2] += tmat[0][2] * vec[0] +
              tmat[1][2] * vec[1] +
              tmat[2][2] * vec[2];

   return prod;
}


/**
 * decrement a vector with a transpose-transformed column vector,
 * prod[i] -= tmat[j][i]*vec[j]
 * @return Decremented vector
 * \param[in] tmat Transformation matrix
 * \param[in] vec Source vector
 * \param[in,out] prod Decremented vector
 */
inline double *
Vector3::transform_transpose_decr (
   double const tmat[3][3],
   double const vec[3],
   double prod[3])
{

   prod[0] -= tmat[0][0] * vec[0] +
              tmat[1][0] * vec[1] +
              tmat[2][0] * vec[2];

   prod[1] -= tmat[0][1] * vec[0] +
              tmat[1][1] * vec[1] +
              tmat[2][1] * vec[2];

   prod[2] -= tmat[0][2] * vec[0] +
              tmat[1][2] * vec[1] +
              tmat[2][2] * vec[2];

   return prod;
}


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
