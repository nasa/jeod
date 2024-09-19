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
 * @file models/utils/math/include/matrix3x3.hh
 * Matrix math inline functions
 */

/*******************************************************************************
Purpose:
  ()

Library dependencies:
  ((../src/dm_invert.cc)
   (../src/dm_invert_symm.cc)
   (../src/math_messages.cc))


*******************************************************************************/

#ifndef JEOD_MATRIX3X3_H
#define JEOD_MATRIX3X3_H

//! Namespace jeod
namespace jeod
{

/**
 * Provides static methods for operations that involve 3x3 matrices.
 */
class Matrix3x3
{
public:
    // Zero-fill matrix :
    // mat[i][j] = 0.0
    static void initialize(double mat[3][3]);

    // Construct identity matrix :
    // mat[i][j] = delta_ij (delta_ij is the Kronecker delta)
    static void identity(double mat[3][3]);

    // Construct the skew symmetric cross product matrix :
    // mat[i][k] = epsilon_ijk vec[j] (epsilon_ijk is the Levi-Cevita symbol)
    static void cross_matrix(const double vec[3], double cross_mat[3][3]);

    // Construct the outer product of a pair of vectors :
    // mat[i][j] = vec_left[j] * vec_right[j]
    static void outer_product(const double vec_left[3], const double vec_right[3], double prod[3][3]);

    // Negated matrix in-place :
    // mat[i][j] = -mat[i][j]
    static void negate(double mat[3][3]);

    // Transpose matrix in-place :
    // mat[i][j] = mat[j][i]
    static void transpose(double mat[3][3]);

    // Scale matrix in-place :
    // mat[i][j] = scalar * mat[i][j]
    static void scale(double scalar, double mat[3][3]);

    // Increment matrix in-place :
    // mat[i][j] = mat[i][j] + addend[i][j]
    static void incr(const double addend[3][3], double mat[3][3]);

    // Decrement matrix in-place :
    // mat[i][j] = mat[i][j] - subtrahend[i][j]
    static void decr(const double subtrahend[3][3], double mat[3][3]);

    // Copy matrix :
    // copy[i][j] = mat[i][j]
    static void copy(const double input_mat[3][3], double copy[3][3]);

    // Negate matrix :
    // copy[i][j] = -mat[i][j]
    static void negate(const double input_mat[3][3], double copy[3][3]);

    // Transpose matrix :
    // copy[i][j] = mat[j][i]
    static void transpose(const double input_mat[3][3], double trans[3][3]);

    // Scale matrix :
    // copy[i][j] = scalar * mat[i][j]
    static void scale(const double mat[3][3], double scalar, double prod[3][3]);

    // Add matrices :
    // sum[i][j] = augend[i][j] + addend[i][j]
    static void add(const double augend[3][3], const double addend[3][3], double sum[3][3]);

    // Subtract matrices :
    // diff[i][j] = minuend[i][j] - subtrahend[i][j]
    static void subtract(const double minuend[3][3], const double subtrahend[3][3], double diff[3][3]);

    // Compute matrix product mat_left * mat_right :
    // prod[i][j] = mat_left[i][k] * mat_right[k][j]
    // Assumption: Matrices are distinct
    static void product(const double mat_left[3][3], const double mat_right[3][3], double prod[3][3]);

    // Compute matrix product mat_left^T * mat_right :
    // prod[i][j] = mat_left[k][i] * mat_right[k][j]
    // Assumption: Matrices are distinct
    static void product_left_transpose(const double mat_left[3][3], const double mat_right[3][3], double prod[3][3]);

    // Compute matrix product mat_left * mat_right^T :
    // prod[i][j] = mat_left[i][k] * mat_right[j][k]
    // Assumption: Matrices are distinct
    static void product_right_transpose(const double mat_left[3][3], const double mat_right[3][3], double prod[3][3]);

    // Compute matrix product mat_left^T * mat_right^T :
    // prod[i][j] = mat_left[k][i] * mat_right[j][k]
    // Assumption: Matrices are distinct
    static void product_transpose_transpose(const double mat_left[3][3],
                                            const double mat_right[3][3],
                                            double prod[3][3]);

    // Compute matrix product trans * mat * trans^T
    // prod[i][j] = trans[i][k] * mat[k][l] * trans[j][l]
    // Assumption: Matrices are distinct
    static void transform_matrix(const double trans[3][3], const double mat[3][3], double prod[3][3]);

    // Compute matrix product trans^T * mat * trans
    // prod[i][j] = trans[k][i] * mat[k][l] * trans[l][j]
    // Assumption: Matrices are distinct
    static void transpose_transform_matrix(const double trans[3][3], const double mat[3][3], double prod[3][3]);

    // Compute the inverse of a 3x3 matrix.
    // Assumption: Matrices are distinct
    static int invert(const double matrix[3][3], double inverse[3][3]);

    // Compute the inverse of a symmetric 3x3 matrix.
    // Assumption: Input matrix is symmetric, matrices are distinct
    static int invert_symmetric(const double matrix[3][3], double inverse[3][3]);

#ifndef SWIG
    // Print matrix to standard error
    static void print(const double mat[3][3]);
#endif
};

#define IDENTITY                                                                                                       \
    {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0},                                                                                  \
    {                                                                                                                  \
        0.0, 0.0, 1.0                                                                                                  \
    }

} // namespace jeod

#include "matrix3x3_inline.hh"

#endif

/**
 * @}
 * @}
 * @}
 */
