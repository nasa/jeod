/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Math
 * @{
 *
 * @file models/utils/math/src/dm_invert_symm.cc
 * Define Matrix3x3::invert_symmetric.
 */

/*******************************************************************************
*                                                                              *
* Trick Simulation Environment Software                                        *
*                                                                              *
* Copyright (c) 1996,1997 LinCom Corporation, Houston, TX                      *
* All rights reserved.                                                         *
*                                                                              *
* Copyrighted by LinCom Corporation and proprietary to it. Any unauthorized    *
* use of Trick Software including source code, object code or executables is   *
* strictly prohibited and LinCom assumes no liability for such actions or      *
* results thereof.                                                             *
*                                                                              *
* Trick Software has been developed under NASA Government Contracts and        *
* access to it may be granted for Government work by the following contact:    *
*                                                                              *
* Contact: Charles Gott, Branch Chief                                          *
*          Simulation and Graphics Branch                                      *
*          Automation, Robotics, & Simulation Division                         *
*          NASA, Johnson Space Center, Houston, TX                             *
*                                                                              *
*******************************************************************************/


/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((dm_invert_symm.cc)
   (math_messages.cc))

 

*******************************************************************************/


// System includes
#include <cmath>

// JEOD includes
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/matrix3x3.hh"
#include "../include/math_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Compute the inverse of a symmetric 3x3 matrix.
 *
 * \par Assumptions and Limitations
 *  - Input and output matrices are distinct.
 *  - Input matrix is symmetric.
 *  - Determinate is non-zero.
 * @return 0=success, non-zero=singular
 * \param[in] matrix Symmetric matrix to invert
 * \param[out] inverse Inverse
 */
int
Matrix3x3::invert_symmetric (
   double const matrix[3][3],
   double inverse[3][3])
{
   double determinant;     // The determinant of the input matrix
   double det_inv;         // The inverse of the above

   // Calculate the determinant and test it
   // (Save the intermediate calculations in 'inverse' for later use)
   inverse[0][0] = matrix[1][1] * matrix[2][2] - matrix[2][1] * matrix[1][2];
   inverse[0][1] = matrix[2][1] * matrix[0][2] - matrix[0][1] * matrix[2][2];
   inverse[0][2] = matrix[0][1] * matrix[1][2] - matrix[1][1] * matrix[0][2];

   determinant = matrix[0][0] * inverse[0][0] +
                 matrix[1][0] * inverse[0][1] +
                 matrix[2][0] * inverse[0][2];

#if 0
   // Calculate the square of the Frobenius norm of the matrix
   double frob_sq;
   frob_sq = matrix[0][0] * matrix[0][0] +
             matrix[1][1] * matrix[1][1] +
             matrix[2][2] * matrix[2][2] +
             2.0*(matrix[0][1] * matrix[0][1] +
                  matrix[0][2] * matrix[0][2] +
                  matrix[1][2] * matrix[1][2]);
#endif

   // Test for an ill-conditioned matrix
   // FIXME: Maybe use a Frobenius norm condition test?
   if (std::fpclassify(determinant) == FP_ZERO) {
      MessageHandler::error (
         __FILE__, __LINE__, MathMessages::ill_conditioned,
         "Matrix is ill-conditioned in Matrix3x3::invert_symm.");
      return 1;
   }

   // Calculate the inverse matrix
   det_inv = 1.0 / determinant;

   inverse[0][0] *= det_inv;
   inverse[0][1] *= det_inv;
   inverse[0][2] *= det_inv;
   inverse[1][1]  = (matrix[0][0] * matrix[2][2] -
                     matrix[2][0] * matrix[0][2]) * det_inv;
   inverse[1][2]  = (matrix[1][0] * matrix[0][2] -
                     matrix[0][0] * matrix[1][2]) * det_inv;
   inverse[2][2]  = (matrix[0][0] * matrix[1][1] -
                     matrix[1][0] * matrix[0][1]) * det_inv;
   inverse[1][0]  = inverse[0][1];
   inverse[2][0]  = inverse[0][2];
   inverse[2][1]  = inverse[1][2];

   // Return success code (0).
   return 0;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
