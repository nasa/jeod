/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Quaternion
 * @{
 *
 * @file models/utils/quaternion/src/quat_from_mat.cc
 * Define left_quat_from_transformation (), which computes the parent-to-child
 * left quaternion from the input transformation matrix.
 */

/*******************************************************************************

Purpose:
  ()

Library dependency:
  ((quat_from_mat.o))

 

*******************************************************************************/

/*------------------------------------------------------------------------------
 * Overview
 * The goal is to find a unitary quaternion 'q' such that 'q' yields the
 * same
 * vector transformations as does the transformation matrix 'a'. The
 * quaternion
 * is unique save for an ambiguity in the sign of all the elements. The sign
 * will be chosen such that the scalar part of the quaternion is
 * non-negative.
 *
 * Nomenclature
 *   a        Transformation matrix
 *   q        Left quaternion based on a
 *   qs       The scalar part of the quaternion
 *   qv       The vector part of the quaternion
 *   qv_i     Element i of qv
 *   a_ij     Element i,j of a
 *   tr       Trace of the transformation matrix: a_00 + a_11 + a_22
 *   t_i      a_ii - (a_jj + a_kk), k!=j!=i
 *   d_i      a_kj - a_jk, eps_ijk = 1
 *   s_ij     a_ij + a_ji
 *   t        Rotation angle
 *   cost     Cosine of t
 *   sint     Sine of t
 *   u        Rotation axis unit vector
 *   eps_ijk  Permutation tensor
 *
 * Given a single axis rotation t from reference frame 1 to reference frame
 * 2,
 * the transformation matrix from frame 1 to frame 2 is
 *   a_ii = 1 - (1-cost)(1-u_i^2)
 *        = cost + (1-cost) u_i^2
 *   a_ij = (1-cost) u_i u_j + sint eps_ijk u_k   i!=j
 * The left transformation quaternion from frame 1 to frame 2 is
 *   qs   = cos(t/2)
 *   qv_i = -sin(t/2) u_i
 * The left quaternion can be expressed in terms of the transformation
 * matrix
 * by using the half-angle formulae
 *   cost   = 2 cos^2(t/2) - 1
 *   sint   = 2 cos(t/2) sin(t/2)
 *   1-cost = 2 sin^2(t/2)
 * With a little manipulation,
 *   qs   = sqrt(1+tr)/2
 *        = d_i/sqrt(1+t_i)/2
 *   qv_i = sqrt(1+t_i)/2
 *        = s_ij/sqrt(1+t_j)/2
 *        = s_ik/sqrt(1+t_k)/2
 * The above equations are subject to precision loss / divide-by-zero
 * problems.
 * To avoid precision loss, the forms best suited to the given matrix will
 * be selected. Denote
 * Method -1:
 *   qix2  = sqrt(1+tr)
 *   fact  = 0.5 / qix2
 *   qs    = 0.5 * qsx2
 *   qv_i = d_i * fact
 * Methods 0-2:
 *   qix2 = sqrt(1+t_i)
 *   fact = 0.5 / qix2
 *   s    = sign(d_i)
 *   qs   = s d_i fact
 *   qv_i = s qix2 / 2
 * Method -1 is selected if the trace dominates the diagonal elements of
 * the matrix. Method 0 is selected if the a_00 dominates the trace and the
 * other two elements. Methods 1 and 2 are selected when a_11 or a_22
 * dominates.
 *----------------------------------------------------------------------------*/


// System includes
#include <cmath>

// JEOD includes

// Model includes
#include "../include/quat.hh"



//! Namespace jeod
namespace jeod {

/**
 * Compute the parent-to-child left quaternion from the input
 * transformation matrix.
 *
 * \par Assumptions and Limitations
 *  - Matrix is orthonormal.
 * \param[in] T Transformation matrix
 */
void
Quaternion::left_quat_from_transformation (
   const double T[3][3])
{
   double tr;                   /* Trace of input transformation matrix */
   double tmax;                 /* Max of tr, diagonal elements */
   double qix2;                 /* sqrt(1+max(tr,t_i)) */
   double di;                   /* a_kj - a_jk */
   double fact;                 /* 0.5/qix2 */
   int meth;                    /* Index of tmax in t (-1 if trace dominates) */
   int ii, jj, kk;


    /* Compute the trace of the matrix. */
   tr = T[0][0] + T[1][1] + T[2][2];

    /* Find the largest of the trace (meth = -1) and the three diagonal
     * elements of 'a' (meth = 0, 1, or 2). */
   meth = -1;
   tmax = tr;
   for (ii = 0; ii < 3; ii++) {
      if (T[ii][ii] > tmax) {
         meth = ii;
         tmax = T[ii][ii];
      }
   }

    /* Use method -1 when no diagonal element dominates the trace. */
   if (meth == -1) {

      qix2   = std::sqrt (1.0 + tr);
      fact   = 0.5 / qix2;
      scalar = 0.5 * qix2;
      vector[0] = fact * (T[2][1] - T[1][2]);
      vector[1] = fact * (T[0][2] - T[2][0]);
      vector[2] = fact * (T[1][0] - T[0][1]);

    /* Use method 0,1, or 2 based on the dominant diagonal element. */
   } else {

      ii = meth;
      jj = (ii+1)%3;
      kk = (jj+1)%3;

      di = T[kk][jj] - T[jj][kk];
      qix2 = sqrt (1.0 + T[ii][ii] - (T[jj][jj] + T[kk][kk]));
      if (di < 0.0) {
         qix2 = -qix2;
      }
      fact = 0.5 / qix2;
      vector[ii] = 0.5 * qix2;
      vector[jj] = fact * (T[ii][jj] + T[jj][ii]);
      vector[kk] = fact * (T[ii][kk] + T[kk][ii]);
      scalar = fact * di;
   }
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
