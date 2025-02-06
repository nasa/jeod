/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Quaternion
 * @{
 *
 * @file models/utils/quaternion/src/quat_to_mat.cc
 * Define Quaternion::left_quat_to_transformation, which computes the parent-
 * to-child transformation matrix from the parent-to-child left quaternion.
 */

/*******************************************************************************

Purpose:
  ()

Library dependency:
  ((quat_to_mat.cc))

 

*******************************************************************************/

/*------------------------------------------------------------------------------
 * Overview
 *
 * Nomenclature
 *   q        Left quaternion
 *   a        Transformation matrix based on q
 *   qs       The scalar part of the quaternion
 *   qv       The vector part of the quaternion
 *   qv_i     Element i of qv
 *   a_ij     Element i,j of a
 *   t        Rotation angle
 *   cost     Cosine of t
 *   sint     Sine of t
 *   u        Rotation axis unit vector
 *   eps_ijk  Permutation tensor
 *
 * Given a single axis rotation t from reference frame 1 to reference frame 2,
 * the transformation matrix from frame 1 to frame 2 is
 *   a_ii = 1 - (1-cost)*(1-u_i^2)
 *        = cost + (1-cost)*u_i^2
 *   a_ij = (1-cost) u_i u_j + sint eps_ijk u_k   i!=j
 * The left transformation quaternion from frame 1 to frame 2 is
 *   qs   = cos(t/2)
 *   qv_i = -sin(t/2)*u_i
 * The transformation matrix can be expressed in terms of the left
 * quaternion by using the half-angle formulae
 *   cost   = 2*cos^2(t/2) - 1
 *   sint   = 2*cos(t/2)*sin(t/2)
 *   1-cost = 2*sin^2(t/2)
 * With a little manipulation,
 *   cost = 2*qs^2 - 1
 *   a_ii = cost + 2*qv_i^2
 *   a_ij = 2*(qv_i*qv_j - eps_ijk*qs*qv_k)   i!=j
 *
 *----------------------------------------------------------------------------*/


// System includes

// JEOD includes
#include "utils/math/include/vector3.hh"

// Model includes
#include "../include/quat.hh"


//! Namespace jeod
namespace jeod {

/**
 * Compute the parent-to-child transformation matrix from the
 * parent-to-child left quaternion.
 *
 * \par Assumptions and Limitations
 *  - Quaternion is normalized.
 * \param[out] T Transformation matrix
 */
void
Quaternion::left_quat_to_transformation (
   double T[3][3])
const
{
   double cost;                 // Cosine of rotation angle
   double qvx2[3];              // qvx2_i = 2 qv_i (See discussion above)
   double qsqv2[3];             // qsqv2_i = 2 qs qv_i
   double qvqv2[3];             // qvqv2_i = 2 qv_j qv_k


   // Compute the cosine of the rotation angle.
   cost = 2.0 * scalar * scalar - 1.0;

   // Form intermediate vectors qvx2, qsqv2, qvqv2.
   Vector3::sum (vector, vector, qvx2);
   Vector3::scale (qvx2, scalar, qsqv2);
   qvqv2[0] = vector[1] * qvx2[2];
   qvqv2[1] = vector[2] * qvx2[0];
   qvqv2[2] = vector[0] * qvx2[1];

   // Construct the transformation matrix diagonal:
   //   T_ii = cost + 2 qv_i^2 = cost + qv_i*qvx2_i
   T[0][0] = cost + vector[0] * qvx2[0];
   T[1][1] = cost + vector[1] * qvx2[1];
   T[2][2] = cost + vector[2] * qvx2[2];


   // Contruct off-diagonal transformation matrix elements:
   //   T_ij = 2 (qv_i qv_j - eps_ijk qs qv_k)
   //        = qvqv2_k - eps_ijk qsqv2_k
   T[0][1] = qvqv2[2] - qsqv2[2];
   T[1][0] = qvqv2[2] + qsqv2[2];
   T[1][2] = qvqv2[0] - qsqv2[0];
   T[2][1] = qvqv2[0] + qsqv2[0];
   T[2][0] = qvqv2[1] - qsqv2[1];
   T[0][2] = qvqv2[1] + qsqv2[1];

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
