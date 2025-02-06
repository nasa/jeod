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
 * @file models/utils/math/include/vector3.hh
 * Vector math inline functions
 */

/*******************************************************************************
Purpose:
  ()


*******************************************************************************/

#ifndef JEOD_VECTOR3_H
#define JEOD_VECTOR3_H

//! Namespace jeod
namespace jeod
{

/**
 * Provides static methods for operations that involve 3-vectors.
 */
class Vector3
{
public:
    // Zero-fill vector :
    // vec[i] = 0.0
    static double * initialize(double vec[3]);

    // Construct unit vector :
    // vec[i] = delta_ij (delta_ij is the Kronecker delta)
    static double * unit(unsigned int index, double vec[3]);

    // Construct a vector from scalar :
    // vec[i] = scalar
    static double * fill(double scalar, double vec[3]);

    // Zero-out small components of a vector :
    // vec[i] = 0 if abs(vec[i]) < limit
    static double * zero_small(double limit, double vec[3]);

    // Copy vector contents :
    // copy[i] = vec[i]
    static double * copy(const double vec[3], double copy[3]);

    // Compute vector inner product :
    // result = sum_i vec1[i] * vec2[i]
    static double dot(const double vec2[3], const double vec1[3]);

    // Compute square of vector magnitude :
    // result = dot(vec,vec), but protects against underflow
    static double vmagsq(const double vec[3]);

    // Compute vector magnitude :
    // result = sqrt(vmagsq(vec))
    static double vmag(const double vec[3]);

    // Make vector a unit vector in-place :
    // vec = vec * 1/vmag(vec)
    static double * normalize(double vec[3]);

    // Construct unit vector :
    // unit_vec = vec * 1/vmag(vec)
    static double * normalize(const double vec[3], double unit_vec[3]);

    // Scale a vector in-place :
    // vec[i] = scalar
    static double * scale(double scalar, double vec[3]);

    // Scale a vector :
    // prod[i] = vec[i] * scalar
    static double * scale(const double vec[3], double scalar, double prod[3]);

    // Negate vector in-place :
    // vec[i] = -vec[i]
    static double * negate(double vec[3]);

    // Negate vector :
    // copy[i] = -vec[i]
    static double * negate(const double vec[3], double copy[3]);

    // Transform a column vector :
    // prod[i] = tmat[i][j]*vec[j]
    static double * transform(const double tmat[3][3], const double vec[3], double prod[3]);

    // Transform a column vector in-place :
    // vec[i] <- tmat[i][j]*vec[j]
    static double * transform(const double tmat[3][3], double vec[3]);

    // Transform a column vector with the transpose :
    // prod[i] = tmat[j][i]*vec[j]
    static double * transform_transpose(const double tmat[3][3], const double vec[3], double prod[3]);

    // Transform a column vector in-place with the transpose :
    // vec[i] <- tmat[j][i]*vec[j]
    static double * transform_transpose(const double tmat[3][3], double vec[3]);

    // Increment a vector :
    // vec[i] += addend[i]
    static double * incr(const double addend[3], double vec[3]);

    // Increment a vector :
    // vec[i] += addend1[i] + addend2[i]
    static double * incr(const double addend1[3], const double addend2[3], double vec[3]);

    // Decrement a vector :
    // vec[i] -= subtrahend[i]
    static double * decr(const double subtrahend[3], double vec[3]);

    // Decrement a vector :
    // vec[i] -= subtrahend1[i] + subtrahend2[i]
    static double * decr(const double subtrahend1[3], const double subtrahend2[3], double vec[3]);

    // Compute the sum of two vectors :
    // vec[i] = addend1[i] + addend2[i]
    static double * sum(const double addend1[3], const double addend2[3], double vec[3]);

    // Compute the sum of three vectors :
    // vec[i] = addend1[i] + addend2[i] + addend3[i]
    static double * sum(const double addend1[3], const double addend2[3], const double addend3[3], double vec[3]);

    // Compute the difference between two vectors :
    // diff[i] = minuend[i] - subtrehend[i]
    static double * diff(const double minuend[3], const double subtrahend[3], double vec[3]);

    // Compute the cross product between two vectors :
    // prod[i] = epsilon_ijk * vec_left[j] * vec_right[k]
    static double * cross(const double vec_left[3], const double vec_right[3], double prod[3]);

    // Increment a vector with a scaled vector :
    // prod[i] += scalar*vec[i]
    static double * scale_incr(const double vec[3], double scalar, double prod[3]);

    // Decrement a vector with a scaled vector :
    // prod[i] += scalar*vec[i]
    static double * scale_decr(const double vec[3], double scalar, double prod[3]);

    // Increment a vector with the the cross product between two vectors :
    // prod[i] += epsilon_ijk * vec_left[j] * vec_right[k]
    static double * cross_incr(const double vec_left[3], const double vec_right[3], double prod[3]);

    // Decrement a vector with the the cross product between two vectors :
    // prod[i] -= epsilon_ijk * vec_left[j] * vec_right[k]
    static double * cross_decr(const double vec_left[3], const double vec_right[3], double prod[3]);

    // Increment a vector with a transformed column vector :
    // prod[i] += tmat[i][j]*vec[j]
    static double * transform_incr(const double tmat[3][3], const double vec[3], double prod[3]);

    // Decrement a vector with a transformed column vector :
    // prod[i] -= tmat[i][j]*vec[j]
    static double * transform_decr(const double tmat[3][3], const double vec[3], double prod[3]);

    // Increment a vector with a transpose-transformed column vector :
    // prod[i] += tmat[j][i]*vec[j]
    static double * transform_transpose_incr(const double tmat[3][3], const double vec[3], double prod[3]);

    // Decrement a vector with a transpose-transformed column vector :
    // prod[i] -= tmat[j][i]*vec[j]
    static double * transform_transpose_decr(const double tmat[3][3], const double vec[3], double prod[3]);
};

} // namespace jeod

#include "vector3_inline.hh"

#endif

/**
 * @}
 * @}
 * @}
 */
