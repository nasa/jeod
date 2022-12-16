/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Orientation
 * @{
 *
 * @file models/utils/orientation/src/eigen_rotation.cc
 * Define Orientation methods related to computing single axis rotations.
 */

/******************************************************************************
*                                                                             *
* Trick Simulation Environment Software                                       *
*                                                                             *
* Copyright (c) 1996,1997 LinCom Corporation, Houston, TX                     *
* All rights reserved.                                                        *
*                                                                             *
* Copyrighted by LinCom Corporation and proprietary to it. Any unauthorized   *
* use of Trick Software including source code, object code or executables is  *
* strictly prohibited and LinCom assumes no liability for such actions or     *
* results thereof.                                                            *
*                                                                             *
* Trick Software has been developed under NASA Government Contracts and       *
* access to it may be granted for Government work by the following contact:   *
*                                                                             *
* Contact: Charles Gott, Branch Chief                                         *
*          Simulation and Graphics Branch                                     *
*          Automation, Robotics, & Simulation Division                        *
*          NASA, Johnson Space Center, Houston, TX                            *
*                                                                             *
******************************************************************************/

/*******************************************************************************

Purpose:
  ()

 

*******************************************************************************/

// System includes
#include <cmath>

// JEOD includes
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"

// Model includes
#include "../include/orientation.hh"


//! Namespace jeod
namespace jeod {

/**
 * Compute the transformation matrix from the eigen rotation.
 * Given a rotation by an angle \f$\phi\f$ about an axis
 * \f$\hat{\mathbf u}\f$,
 * the [i][j] element of the transformation matrix is given by
 * \f[
 *   T_{ij} =
 *      \cos\phi\,\delta_{ij} +
 *      (1-\cos\phi)\,\hat u_i \hat u_j +
 *      \epsilon_{ijk}\sin\phi\,\hat u_k
 * \f]
 * where
 *   - \f$\delta_{ij}\f$ is the Kronecker delta,
 *   -  @e k is \f$(i+j) \bmod 3\f$, and
 *   - \f$\epsilon_{ijk}\f$ is the Levi-Civita symbol taken with respect
 *      to (0,1,2).
 *
 * \par Assumptions and Limitations
 *  - The eigen axis is a unit vector.
 * \param[in] eigen_angle Rotation angle\n Units: r
 * \param[in] eigen_axis Rotation axis, unit vector
 * \param[out] trans Resultant transformation matrix
 */
void
Orientation::compute_matrix_from_eigen_rotation (
   double eigen_angle,
   const double eigen_axis[3],
   double trans[3][3])
{
   double cphi = std::cos (eigen_angle);
   double sphi = std::sin (eigen_angle);
   double omcp = 1.0 - cphi;
   double del;

   // Compute the diagonal elements of the matrix, which are of the form
   //   T[i][i] = cos(phi) + (1-cos(phi)) * uhat[i]*uhat[i]
   trans[0][0] = cphi + omcp * eigen_axis[0] * eigen_axis[0];
   trans[1][1] = cphi + omcp * eigen_axis[1] * eigen_axis[1];
   trans[2][2] = cphi + omcp * eigen_axis[2] * eigen_axis[2];

   // The off-diagonal elements are of the form
   //    T[i][j] = (1-cos(phi))*uhat[i]*uhat[j] + epsilon_ijk*sin(phi)*uhat[k]
   // where k is the omitted axis and epsilon_ijk is the Levi-Civita symbol.
   // epsilon_ijk is +1/-1 if (i,j,k) is an even/odd permutation of (0,1,2).
   // First compute the terms that don't involve epsilon_ijk.
   trans[0][1] =
   trans[1][0] = omcp * eigen_axis[0] * eigen_axis[1];
   trans[0][2] =
   trans[2][0] = omcp * eigen_axis[0] * eigen_axis[2];
   trans[1][2] =
   trans[2][1] = omcp * eigen_axis[1] * eigen_axis[2];

   // Now add/subtract the sin(phi)*uhat[k] term.

   // (i,j) = (0,1) and (1,0) => k=2. (0,1,2) is even, (1,0,2) is odd.
   del = sphi * eigen_axis[2];
   trans[0][1] += del;
   trans[1][0] -= del;

   // (i,j) = (2,0) and (0,2) => k=1. (2,0,1) is even, (0,2,1) is odd.
   del = sphi * eigen_axis[1];
   trans[2][0] += del;
   trans[0][2] -= del;

   // (i,j) = (1,2) and (2,1) => k=0. (1,2,0) is even, (2,1,0) is odd.
   del = sphi * eigen_axis[0];
   trans[1][2] += del;
   trans[2][1] -= del;
}


/**
 * Compute the eigen rotation from the transformation matrix.
 * There are several alternate expressions for computing the eigen rotation
 * from a matrix, all of which are equivalent in infinite precision arithmetic.
 * The use of finite precision arithmetic means that care must be taken
 * in choosing the algorithm to be used. The starting point is the
 * generic expression
 * \f[
 *   T_{ij} =
 *      \cos\phi\,\delta_{ij} +
 *      (1-\cos\phi)\,\hat u_i \hat u_j +
 *      \epsilon_{ijk}\sin\phi\,\hat u_k
 * \f]
 * From this, the trace of the matrix and the difference between and sum of
 * pairs of off-diagonal elements are
 * \f{eqnarray*}{
 *   \mathrm{tr}(T) &=& 2\cos\phi + 1 \\
 *    T_{ij}-T_{ji} &=& 2\epsilon_{ijk}\sin\phi\,\hat u_k \\
 *    T_{ij}+T_{ji} &=& 2(1-\cos\phi)\,\hat u_i \hat u_j
 * \f}
 *
 * <b>Method 1</b>\n
 * One approach to determining the eigen rotation involves the construction
 * of a vector of differences between pairs of off-diagonal elements of the
 * transformation matrix,
 * \f[
 *    d_k = T_{ij}-T_{ji} = 2 \sin\phi\,\hat u_k
 * \f]
 * where (i,j,k) is an even permutation of (0,1,2). With this,
 * \f{eqnarray*}{
 *    \phi &=& \arcsin\left(\frac{||\mathbf d||} 2\right) \\
 *    \hat{\mathbf u} &=& \frac{\mathbf d}{||\mathbf d||}
 * \f}
 * Note that the above of the inverse sine will restrict the rotation angle to
 * be between 0 and 90 degrees. Special processing is needed when the rotation
 * angle is between 90 and 180 degrees.
 * Note also that the symmetric difference vector will
 * be identically zero if the rotation angle is 0 or 180 degrees and will be
 * very small for rotation angles close to 0 or 180 degrees.
 * The precision loss for rotation angles near 0 and 180 degrees means the
 * individual components of the eigen axis will not be as precise with this
 * approach compared to alternatives.
 *
 * <b>Method 2</b>\n
 * The diagonal elements of the matrix yields another method for determining the
 * single axis rotation angle and the rotation axis:
 * \f{eqnarray*}{
 *    \phi &=& \arccos\left(\frac{\mathrm{tr}(T)-1} 2\right) \\
 *    |\hat u_i| &=& \sqrt{\frac{T_{ii} - \cos \phi}{1-\cos\phi}}
 * \f}
 * Note that this approach determines the magnitudes but not the signs of the
 * components of the eigen axis vector. Because this method is based on the
 * inverse cosine, the calculated phi angle will be less precise than that
 * obtained by method 1 for angles near 0 or 180 degrees. The unit vector
 * however will be more accurate than that obtained from method 1 for small
 * rotation angles.
 *
 * <b>Method 3</b>\n
 * Yet another alternative for computing components of the eigen axis is to
 * use the sum of pairs of off-diagonal elements of the transformation metrix,
 * \f{eqnarray*}{
 *    T_{ij}+T_{ji} &=& 2(1-\cos\phi)\,\hat u_i \hat u_j \\
 *    T_{ik}+T_{ki} &=& 2(1-\cos\phi)\,\hat u_i \hat u_k
 * \f}
 * This enables the calculation of two components of the unit vector. One
 * component needs to be computed by one of the two previous methods.
 *
 * \par Assumptions and Limitations
 *  - The matrix is a proper transformation matrix.
 * \param[in] trans Transformation matrix
 * \param[out] eigen_angle Resultant rotation angle\n Units: r
 * \param[out] eigen_axis Resultant rotation axis
 */
void
Orientation::compute_eigen_rotation_from_matrix (
   const double trans[3][3],
   double * eigen_angle,
   double eigen_axis[3])
{
   double cosphi;  // Cosing of the eigen angle
   double sinphi;  // Sine of the eigen angle
   double diff[3]; // Symmetric difference vector
   double dmag;    // Magnitude of the symmetric difference vector

   // Calculate the symmetric difference vector and its magnitude.
   diff[0] = trans[1][2] - trans[2][1];
   diff[1] = trans[2][0] - trans[0][2];
   diff[2] = trans[0][1] - trans[1][0];
   dmag = Vector3::vmag(diff);

   // Determine the sine and cosine of the rotation angle.
   sinphi = 0.5 * dmag;
   cosphi = 0.5 * (trans[0][0] + trans[1][1] + trans[2][2] - 1.0);


   // Now see which solution to use.
   // There are four cases.
   // Use the appropriate method and solve for the angle and the axis.


   // Case 1: The trivial case, rotation angle = 0.
   // This case is evidenced by sinphi = 0, cosphi = 1.
   // The rotation is ill-defined in this case.
   // Arbitrarily set the rotation vector to xhat.
   if ((std::fpclassify(dmag) == FP_ZERO) && (cosphi > 0.0)) {
     *eigen_angle  = 0.0;
     eigen_axis[0] = 1.0;
     eigen_axis[1] = 0.0;
     eigen_axis[2] = 0.0;
   }

   // Case 2: Rotation angle between 0 and 135 degrees (exclusive).
   // This case is evidenced by sinphi greater than -cosphi.
   // Here the symmetric difference (method 1) does the best job of determining
   // the eigen axis. Method 1 yields for best results for the rotation angle
   // for small rotation angles (0 to 45 degrees); method 2 is better for
   // larger values.
   else if (sinphi >= -cosphi) {
      // The rotation angle is given by
      //   phi = acos(cosphi)
      // or
      //   phi = asin(sinphi)
      // The inverse sine yields better results for angles less than 45 degrees
      // while the inverse cosine is more accurate for larger angles.
      if (sinphi < cosphi) {
         *eigen_angle = std::asin(sinphi);
      }
      else {
         *eigen_angle = std::acos(cosphi);
      }

      // Use method 1 to construct the eigen axis.
      Vector3::scale (diff, 1.0/dmag, eigen_axis);
   }

   // Case 3: Rotation angle between 135 and 180 degrees (inclusive).
   // Here the symmetric difference does the best job of determining the
   // rotation angle and the symmetric sum does the best job of determining
   // the eigen axis.
   else {
      unsigned int ii;             // Index of largest diagonal element
      double omcp = 1.0 - cosphi;  // 1 - cos(phi)

      // Use the trace to determine the eigen rotation angle,
      // taking care to place the angle in the correct quadrant.
      *eigen_angle = M_PI - std::asin(sinphi);

      // The symmetric sum can only determine two of the three components
      // of the eigen axis. The symmetric difference needs to be used for
      // one of the components. The best accuracy results from choosing the
      // the component correponding to the largest diagonal element.
      // Find the index of the largest diagonal element.
      ii = 0;
      if (trans[1][1] > trans[ii][ii]) {
         ii = 1;
      }
      if (trans[2][2] > trans[ii][ii]) {
         ii = 2;
      }

      // Construct the iith element of the eigen axis vector using
      // method 2 to determine the magnitude, method 1 to determine the sign.
      eigen_axis[ii] = std::sqrt ((trans[ii][ii] - cosphi) / omcp);
      if (diff[ii] < 0.0) {
         eigen_axis[ii] = -eigen_axis[ii];
      }

      // Determine the remaining elements of the eigen axis via method 3.
      double vscale = 1. / (2 * omcp * eigen_axis[ii]);
      for (unsigned int jj = 0; jj < 3; jj++) {
         if (jj != ii) {
            eigen_axis[jj] = (trans[ii][jj] + trans[jj][ii]) * vscale;
         }
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
