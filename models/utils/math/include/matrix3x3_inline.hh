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
/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Math
 * @{
 *
 * @file models/utils/math/include/matrix3x3_inline.hh
 * Matrix math inline functions
 */

/*******************************************************************************
Purpose:
  ()


*******************************************************************************/

#ifndef JEOD_MATRIX_INLINE_HH
#define JEOD_MATRIX_INLINE_HH

// System includes
#include <cstdio>

// JEOD includes
#include "matrix3x3.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Zero-fill matrix: mat[i][j] = 0.0
 * \param[out] mat Zero-filled matrix
 */
inline void Matrix3x3::initialize(double mat[3][3])
{
    mat[0][0] = mat[1][1] = mat[2][2] = 0.0;

    mat[0][1] = mat[1][0] = 0.0;
    mat[1][2] = mat[2][1] = 0.0;
    mat[2][0] = mat[0][2] = 0.0;
}

/**
 * Construct identity matrix: mat[i][j] = delta_ij
 * \param[out] mat Identity matrix
 */
inline void Matrix3x3::identity(double mat[3][3])
{
    mat[0][0] = mat[1][1] = mat[2][2] = 1.0;

    mat[0][1] = mat[1][0] = 0.0;
    mat[1][2] = mat[2][1] = 0.0;
    mat[2][0] = mat[0][2] = 0.0;
}

/**
 * Construct the skew symmetric cross product matrix:
 * mat[i][k] = epsilon_ijk vec[j], epsilon_ijk is the Levi-Cevita symbol
 * \param[in] vec Vector
 * \param[out] cross_mat Cross product matrix
 */
inline void Matrix3x3::cross_matrix(const double vec[3], double cross_mat[3][3])
{
    cross_mat[0][0] = cross_mat[1][1] = cross_mat[2][2] = 0.0;

    // ijk = 012, 210
    cross_mat[0][2] = vec[1];
    cross_mat[2][0] = -vec[1];

    // ijk = 120, 021
    cross_mat[1][0] = vec[2];
    cross_mat[0][1] = -vec[2];

    // ijk = 201, 102
    cross_mat[2][1] = vec[0];
    cross_mat[1][2] = -vec[0];
}

/**
 * Construct the outer product of two vectors:
 * mat[i][j] = vec_left[i] * vec_right[j]
 * \param[in] vec_left Vector
 * \param[in] vec_right Vector
 * \param[out] prod Outer product matrix
 */
inline void Matrix3x3::outer_product(const double vec_left[3], const double vec_right[3], double prod[3][3])
{
    prod[0][0] = vec_left[0] * vec_right[0];
    prod[0][1] = vec_left[0] * vec_right[1];
    prod[0][2] = vec_left[0] * vec_right[2];
    prod[1][0] = vec_left[1] * vec_right[0];
    prod[1][1] = vec_left[1] * vec_right[1];
    prod[1][2] = vec_left[1] * vec_right[2];
    prod[2][0] = vec_left[2] * vec_right[0];
    prod[2][1] = vec_left[2] * vec_right[1];
    prod[2][2] = vec_left[2] * vec_right[2];
}

/**
 * Negated matrix in-place: mat[i][j] = -mat[i][j]
 * \param[in,out] mat Negated matrix
 */
inline void Matrix3x3::negate(double mat[3][3])
{
    mat[0][0] = -mat[0][0];
    mat[0][1] = -mat[0][1];
    mat[0][2] = -mat[0][2];

    mat[1][0] = -mat[1][0];
    mat[1][1] = -mat[1][1];
    mat[1][2] = -mat[1][2];

    mat[2][0] = -mat[2][0];
    mat[2][1] = -mat[2][1];
    mat[2][2] = -mat[2][2];
}

/**
 * Transpose matrix in-place: mat[i][j] = mat[j][i]
 * \param[in,out] mat Transposed matrix
 */
inline void Matrix3x3::transpose(double mat[3][3])
{
    double temp;

    temp = mat[0][1];
    mat[0][1] = mat[1][0];
    mat[1][0] = temp;

    temp = mat[0][2];
    mat[0][2] = mat[2][0];
    mat[2][0] = temp;

    temp = mat[1][2];
    mat[1][2] = mat[2][1];
    mat[2][1] = temp;
}

/**
 * Scale matrix in-place, mat[i][j] = scalar * mat[i][j]
 * \param[in] scalar Scalar
 * \param[in,out] mat Scaled matrix
 */
inline void Matrix3x3::scale(double scalar, double mat[3][3])
{
    mat[0][0] *= scalar;
    mat[0][1] *= scalar;
    mat[0][2] *= scalar;
    mat[1][0] *= scalar;
    mat[1][1] *= scalar;
    mat[1][2] *= scalar;
    mat[2][0] *= scalar;
    mat[2][1] *= scalar;
    mat[2][2] *= scalar;
}

/**
 * Increment matrix in-place: mat[i][j] = mat[i][j] + addend[i][j]
 * \param[in] addend Increment
 * \param[in,out] mat Incremented matrix
 */
inline void Matrix3x3::incr(const double addend[3][3], double mat[3][3])
{
    mat[0][0] += addend[0][0];
    mat[0][1] += addend[0][1];
    mat[0][2] += addend[0][2];
    mat[1][0] += addend[1][0];
    mat[1][1] += addend[1][1];
    mat[1][2] += addend[1][2];
    mat[2][0] += addend[2][0];
    mat[2][1] += addend[2][1];
    mat[2][2] += addend[2][2];
}

/**
 * Decrement matrix in-place: mat[i][j] = mat[i][j] - subtrahend[i][j]
 * \param[in] subtrahend Decrement
 * \param[in,out] mat Decremented matrix
 */
inline void Matrix3x3::decr(const double subtrahend[3][3], double mat[3][3])
{
    mat[0][0] -= subtrahend[0][0];
    mat[0][1] -= subtrahend[0][1];
    mat[0][2] -= subtrahend[0][2];
    mat[1][0] -= subtrahend[1][0];
    mat[1][1] -= subtrahend[1][1];
    mat[1][2] -= subtrahend[1][2];
    mat[2][0] -= subtrahend[2][0];
    mat[2][1] -= subtrahend[2][1];
    mat[2][2] -= subtrahend[2][2];
}

/**
 * Copy matrix: copy[i][j] = mat[i][j]
 * \param[in] input_mat Source matrix
 * \param[out] copy Matrix copy
 */
inline void Matrix3x3::copy(const double input_mat[3][3], double copy[3][3])
{
    copy[0][0] = input_mat[0][0];
    copy[0][1] = input_mat[0][1];
    copy[0][2] = input_mat[0][2];

    copy[1][0] = input_mat[1][0];
    copy[1][1] = input_mat[1][1];
    copy[1][2] = input_mat[1][2];

    copy[2][0] = input_mat[2][0];
    copy[2][1] = input_mat[2][1];
    copy[2][2] = input_mat[2][2];
}

/**
 * Negate matrix: copy[i][j] = -mat[i][j]
 *
 * \par Assumptions and Limitations
 *  - Input and output matrices are distinct.
 * \param[in] input_mat Source matrix
 * \param[out] copy Negated matrix
 */
inline void Matrix3x3::negate(const double input_mat[3][3], double copy[3][3])
{
    copy[0][0] = -input_mat[0][0];
    copy[0][1] = -input_mat[0][1];
    copy[0][2] = -input_mat[0][2];

    copy[1][0] = -input_mat[1][0];
    copy[1][1] = -input_mat[1][1];
    copy[1][2] = -input_mat[1][2];

    copy[2][0] = -input_mat[2][0];
    copy[2][1] = -input_mat[2][1];
    copy[2][2] = -input_mat[2][2];
}

/**
 * Transpose matrix: copy[i][j] = mat[j][i]
 *
 * \par Assumptions and Limitations
 *  - Input and output matrices are distinct.
 * \param[in] input_mat Source matrix
 * \param[out] trans Matrix transpose
 */
inline void Matrix3x3::transpose(const double input_mat[3][3], double trans[3][3])
{
    if(input_mat != nullptr)
    {
        trans[0][0] = input_mat[0][0];
        trans[0][1] = input_mat[1][0];
        trans[0][2] = input_mat[2][0];

        trans[1][0] = input_mat[0][1];
        trans[1][1] = input_mat[1][1];
        trans[1][2] = input_mat[2][1];

        trans[2][0] = input_mat[0][2];
        trans[2][1] = input_mat[1][2];
        trans[2][2] = input_mat[2][2];
    }
}

/**
 * Scale matrix: copy[i][j] = scalar * mat[i][j]
 * \param[in] mat Matrix
 * \param[in] scalar Scalar
 * \param[out] prod Product
 */
inline void Matrix3x3::scale(const double mat[3][3], double scalar, double prod[3][3])
{
    prod[0][0] = mat[0][0] * scalar;
    prod[0][1] = mat[0][1] * scalar;
    prod[0][2] = mat[0][2] * scalar;
    prod[1][0] = mat[1][0] * scalar;
    prod[1][1] = mat[1][1] * scalar;
    prod[1][2] = mat[1][2] * scalar;
    prod[2][0] = mat[2][0] * scalar;
    prod[2][1] = mat[2][1] * scalar;
    prod[2][2] = mat[2][2] * scalar;
}

/**
 * Add matrices: sum[i][j] = augend[i][j] + addend[i][j]
 * \param[in] augend Matrix
 * \param[in] addend Matrix
 * \param[out] sum Sum
 */
inline void Matrix3x3::add(const double augend[3][3], const double addend[3][3], double sum[3][3])
{
    sum[0][0] = augend[0][0] + addend[0][0];
    sum[0][1] = augend[0][1] + addend[0][1];
    sum[0][2] = augend[0][2] + addend[0][2];
    sum[1][0] = augend[1][0] + addend[1][0];
    sum[1][1] = augend[1][1] + addend[1][1];
    sum[1][2] = augend[1][2] + addend[1][2];
    sum[2][0] = augend[2][0] + addend[2][0];
    sum[2][1] = augend[2][1] + addend[2][1];
    sum[2][2] = augend[2][2] + addend[2][2];
}

/**
 * Subtract matrices: diff[i][j] = minuend[i][j] - subtrahend[i][j]
 * \param[in] minuend Matrix
 * \param[in] subtrahend Matrix
 * \param[out] diff Difference
 */
inline void Matrix3x3::subtract(const double minuend[3][3], const double subtrahend[3][3], double diff[3][3])
{
    diff[0][0] = minuend[0][0] - subtrahend[0][0];
    diff[0][1] = minuend[0][1] - subtrahend[0][1];
    diff[0][2] = minuend[0][2] - subtrahend[0][2];
    diff[1][0] = minuend[1][0] - subtrahend[1][0];
    diff[1][1] = minuend[1][1] - subtrahend[1][1];
    diff[1][2] = minuend[1][2] - subtrahend[1][2];
    diff[2][0] = minuend[2][0] - subtrahend[2][0];
    diff[2][1] = minuend[2][1] - subtrahend[2][1];
    diff[2][2] = minuend[2][2] - subtrahend[2][2];
}

/**
 * Compute the matrix product mat_left * mat_right:
 * prod[i][j] = mat_left[i][k] * mat_right[k][j]
 *
 * \par Assumptions and Limitations
 *  - Input and output matrices are distinct.
 * \param[in] mat_left Multiplier
 * \param[in] mat_right Multiplicand
 * \param[out] prod Product
 */
inline void Matrix3x3::product(const double mat_left[3][3], const double mat_right[3][3], double prod[3][3])
{
    prod[0][0] = mat_left[0][0] * mat_right[0][0] + mat_left[0][1] * mat_right[1][0] + mat_left[0][2] * mat_right[2][0];

    prod[0][1] = mat_left[0][0] * mat_right[0][1] + mat_left[0][1] * mat_right[1][1] + mat_left[0][2] * mat_right[2][1];

    prod[0][2] = mat_left[0][0] * mat_right[0][2] + mat_left[0][1] * mat_right[1][2] + mat_left[0][2] * mat_right[2][2];

    prod[1][0] = mat_left[1][0] * mat_right[0][0] + mat_left[1][1] * mat_right[1][0] + mat_left[1][2] * mat_right[2][0];

    prod[1][1] = mat_left[1][0] * mat_right[0][1] + mat_left[1][1] * mat_right[1][1] + mat_left[1][2] * mat_right[2][1];

    prod[1][2] = mat_left[1][0] * mat_right[0][2] + mat_left[1][1] * mat_right[1][2] + mat_left[1][2] * mat_right[2][2];

    prod[2][0] = mat_left[2][0] * mat_right[0][0] + mat_left[2][1] * mat_right[1][0] + mat_left[2][2] * mat_right[2][0];

    prod[2][1] = mat_left[2][0] * mat_right[0][1] + mat_left[2][1] * mat_right[1][1] + mat_left[2][2] * mat_right[2][1];

    prod[2][2] = mat_left[2][0] * mat_right[0][2] + mat_left[2][1] * mat_right[1][2] + mat_left[2][2] * mat_right[2][2];
}

/**
 * Compute the matrix product mat_left^T * mat_right:
 * prod[i][j] = mat_left[k][i] * mat_right[k][j]
 *
 * \par Assumptions and Limitations
 *  - Input and output matrices are distinct.
 * \param[in] mat_left Multiplier
 * \param[in] mat_right Multiplicand
 * \param[out] prod Product
 */
inline void Matrix3x3::product_left_transpose(const double mat_left[3][3],
                                              const double mat_right[3][3],
                                              double prod[3][3])
{
    prod[0][0] = mat_left[0][0] * mat_right[0][0] + mat_left[1][0] * mat_right[1][0] + mat_left[2][0] * mat_right[2][0];

    prod[0][1] = mat_left[0][0] * mat_right[0][1] + mat_left[1][0] * mat_right[1][1] + mat_left[2][0] * mat_right[2][1];

    prod[0][2] = mat_left[0][0] * mat_right[0][2] + mat_left[1][0] * mat_right[1][2] + mat_left[2][0] * mat_right[2][2];

    prod[1][0] = mat_left[0][1] * mat_right[0][0] + mat_left[1][1] * mat_right[1][0] + mat_left[2][1] * mat_right[2][0];

    prod[1][1] = mat_left[0][1] * mat_right[0][1] + mat_left[1][1] * mat_right[1][1] + mat_left[2][1] * mat_right[2][1];

    prod[1][2] = mat_left[0][1] * mat_right[0][2] + mat_left[1][1] * mat_right[1][2] + mat_left[2][1] * mat_right[2][2];

    prod[2][0] = mat_left[0][2] * mat_right[0][0] + mat_left[1][2] * mat_right[1][0] + mat_left[2][2] * mat_right[2][0];

    prod[2][1] = mat_left[0][2] * mat_right[0][1] + mat_left[1][2] * mat_right[1][1] + mat_left[2][2] * mat_right[2][1];

    prod[2][2] = mat_left[0][2] * mat_right[0][2] + mat_left[1][2] * mat_right[1][2] + mat_left[2][2] * mat_right[2][2];
}

/**
 * Compute the matrix product mat_left * mat_right^T:
 * prod[i][j] = sum_k mat_left[i][k] * mat_right[j][k]
 *
 * \par Assumptions and Limitations
 *  - Input and output matrices are distinct.
 * \param[in] mat_left Multiplier
 * \param[in] mat_right Multiplicand
 * \param[out] prod Product
 */
inline void Matrix3x3::product_right_transpose(const double mat_left[3][3],
                                               const double mat_right[3][3],
                                               double prod[3][3])
{
    prod[0][0] = mat_left[0][0] * mat_right[0][0] + mat_left[0][1] * mat_right[0][1] + mat_left[0][2] * mat_right[0][2];

    prod[0][1] = mat_left[0][0] * mat_right[1][0] + mat_left[0][1] * mat_right[1][1] + mat_left[0][2] * mat_right[1][2];

    prod[0][2] = mat_left[0][0] * mat_right[2][0] + mat_left[0][1] * mat_right[2][1] + mat_left[0][2] * mat_right[2][2];

    prod[1][0] = mat_left[1][0] * mat_right[0][0] + mat_left[1][1] * mat_right[0][1] + mat_left[1][2] * mat_right[0][2];

    prod[1][1] = mat_left[1][0] * mat_right[1][0] + mat_left[1][1] * mat_right[1][1] + mat_left[1][2] * mat_right[1][2];

    prod[1][2] = mat_left[1][0] * mat_right[2][0] + mat_left[1][1] * mat_right[2][1] + mat_left[1][2] * mat_right[2][2];

    prod[2][0] = mat_left[2][0] * mat_right[0][0] + mat_left[2][1] * mat_right[0][1] + mat_left[2][2] * mat_right[0][2];

    prod[2][1] = mat_left[2][0] * mat_right[1][0] + mat_left[2][1] * mat_right[1][1] + mat_left[2][2] * mat_right[1][2];

    prod[2][2] = mat_left[2][0] * mat_right[2][0] + mat_left[2][1] * mat_right[2][1] + mat_left[2][2] * mat_right[2][2];
}

/**
 * Compute the matrix product mat_left^T * mat_right^T:
 * prod[i][j] = sum_k mat_left[k][i] * mat_right[j][k]
 *
 * \par Assumptions and Limitations
 *  - Input and output matrices are distinct.
 * \param[in] mat_left Multiplier
 * \param[in] mat_right Multiplicand
 * \param[out] prod Product
 */
inline void Matrix3x3::product_transpose_transpose(const double mat_left[3][3],
                                                   const double mat_right[3][3],
                                                   double prod[3][3])
{
    if(mat_left != nullptr && mat_right != nullptr)
    {
        prod[0][0] = mat_left[0][0] * mat_right[0][0] + mat_left[1][0] * mat_right[0][1] +
                     mat_left[2][0] * mat_right[0][2];

        prod[0][1] = mat_left[0][0] * mat_right[1][0] + mat_left[1][0] * mat_right[1][1] +
                     mat_left[2][0] * mat_right[1][2];

        prod[0][2] = mat_left[0][0] * mat_right[2][0] + mat_left[1][0] * mat_right[2][1] +
                     mat_left[2][0] * mat_right[2][2];

        prod[1][0] = mat_left[0][1] * mat_right[0][0] + mat_left[1][1] * mat_right[0][1] +
                     mat_left[2][1] * mat_right[0][2];

        prod[1][1] = mat_left[0][1] * mat_right[1][0] + mat_left[1][1] * mat_right[1][1] +
                     mat_left[2][1] * mat_right[1][2];

        prod[1][2] = mat_left[0][1] * mat_right[2][0] + mat_left[1][1] * mat_right[2][1] +
                     mat_left[2][1] * mat_right[2][2];

        prod[2][0] = mat_left[0][2] * mat_right[0][0] + mat_left[1][2] * mat_right[0][1] +
                     mat_left[2][2] * mat_right[0][2];

        prod[2][1] = mat_left[0][2] * mat_right[1][0] + mat_left[1][2] * mat_right[1][1] +
                     mat_left[2][2] * mat_right[1][2];

        prod[2][2] = mat_left[0][2] * mat_right[2][0] + mat_left[1][2] * mat_right[2][1] +
                     mat_left[2][2] * mat_right[2][2];
    }
}

/**
 * Compute the matrix product trans * mat * trans^T
 * prod[i][j] = trans[i][k] * mat[k][l] * trans[j][l]
 *
 * \par Assumptions and Limitations
 *  - Input and output matrices are distinct.
 * \param[in] trans Transformation matrix
 * \param[in] mat Matrix to transform
 * \param[out] prod Product
 */
inline void Matrix3x3::transform_matrix(const double trans[3][3], const double mat[3][3], double prod[3][3])
{
    double temp[3][3];

    product(trans, mat, temp);
    product_right_transpose(temp, trans, prod);
}

/**
 * Compute the matrix product trans^T * mat * trans
 * prod[i][j] = trans[k][i] * mat[k][l] * trans[l][j]
 *
 * \par Assumptions and Limitations
 *  - Input and output matrices are distinct.
 * \param[in] trans Transformation matrix
 * \param[in] mat Matrix to transform
 * \param[out] prod Product
 */
inline void Matrix3x3::transpose_transform_matrix(const double trans[3][3], const double mat[3][3], double prod[3][3])
{
    double temp[3][3];

    product_left_transpose(trans, mat, temp);
    product(temp, trans, prod);
}

/**
 * Print matrix to standard error
 * \param[in] mat Matrix to print
 */
inline void Matrix3x3::print(const double mat[3][3])
{
    std::fprintf(stderr, "\n%f %f %f\n", mat[0][0], mat[0][1], mat[0][2]);
    std::fprintf(stderr, "%f %f %f\n", mat[1][0], mat[1][1], mat[1][2]);
    std::fprintf(stderr, "%f %f %f\n", mat[2][0], mat[2][1], mat[2][2]);
}

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
