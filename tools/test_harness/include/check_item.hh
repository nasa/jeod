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
 * @addtogroup Tools
 * @{
 * @addtogroup TestHarness
 * @{
 *
 * @file tools/test_harness/include/check_item.hh
 * Define the class CheckItem.
 */

/*******************************************************************************

Purpose:
  ()

 

*******************************************************************************/

#ifndef JEOD_TEST_CHECK_ITEM_HH
#define JEOD_TEST_CHECK_ITEM_HH

// System includes
#include <cmath>
#include <algorithm>

// JEOD includes
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"
#include "utils/orientation/include/orientation.hh"
#include "utils/quaternion/include/quat.hh"


//! Namespace jeod
namespace jeod {

/**
 * Provides several static methods for checking whether a computed value
 * agrees with an expected value to within some tolerance.
 * The methods are defined inline and are overloaded. The table that follows
 * describes these methods.
 * <table>
 * <tr>
 *  <td><b>Method</b></td>
 *  <td><b>Applies to</b></td>
 *  <td><b>Comments</b></td>
 * </tr>
 * <tr>
 *  <td>within_tolerance</td>
 *  <td>scalar, vector, matrix</td>
 *  <td>Two basic methods exist for each type.
 *      One tests that each computed elements is within an absolute tolerance
 *      of the corresponding expected element. The other uses an absolute
 *      tolerance for small expected values but a relative tolerance for
 *      larger values. For composite articles, each basic test comes in two
 *      flavors: A gross test and an item-by-item test.</td>
 * </tr>
 * <tr>
 *  <td>is_within</td>
 *  <td>scalar, vector, matrix</td>
 *  <td>Tests whether each computed element lies between the corresponding
 *      lower and upper bound elements.</td>
 * </tr>
 * <tr>
 *  <td>is_zero</td>
 *  <td>scalar, vector, matrix</td>
 *  <td>Tests whether each computed element is approximately zero.</td>
 * </tr>
 * <tr>
 *  <td>is_identity</td>
 *  <td>matrix, quaternion</td>
 *  <td>Tests whether the computed item is the identity matrix /
 *      real unit quaternion.</td>
 * </tr>
 * <tr>
 *  <td>is_unit</td>
 *  <td>scalar, vector, matrix, quaternion</td>
 *  <td>Tests whether the computed item is a 'unit'.
 *      For scalars, this means 1 (but not -1).
 *      For vectors and quaternions, this means the test item is approximately
 *      a unit vector/unit quaternion.
 *      For matrices, the test is for a right-handed orthogonal matrix.</td>
 * </tr>
 * <tr>
 *  <td>within_rss_tolerance</td>
 *  <td>vector, matrix</td>
 *  <td>Tests whether the root sum square of the vector/matrix is within
 *      the specified tolerance.</td>
 * </tr>
 * <tr>
 *  <td>within_eigen_tolerance</td>
 *  <td>orthogonal matrix, unit quaternion</td>
 *  <td>Tests whether the eigen rotation from expected to computed
 *      is within the specified tolerance.</td>
 * </tr>
 * </table>
 */
class CheckItem {
 public:

   // The within_tolerance methods apply to scalars, vectors, and matrices.
   // There are two basic methods for each class of test article:
   //   - Those that take a single tolerance argument.
   //     These are the absolute tolerance methods.
   //     The test is of the absolute error versus the tolerance.
   //   - Those that take a pair of tolerance arguments.
   //     These are the absolute+relative tolerance methods.
   //     An absolute error test is used for small values of expected but
   //     a relative error test for larger values. The transition point is
   //     the point at which the two tests are equivalent.
   // Another bifurcation exists for composite test articles. The pass/fail
   // status of each element is returned in the result vector/matrix in
   // those methods that take a result argument.

   // Scalar within tolerance, two versions, absolute versus absolute+relative.
   static bool within_tolerance (
      double computed, double expected, double abs_tol);

   static bool within_tolerance (
      double computed, double expected,
      double abs_tol, double rel_tol);


   // Vector within tolerance, four versions.
   static bool within_tolerance (
      const double computed[3], const double expected[3], double abs_tol);

   static bool within_tolerance (
      const double computed[3], const double expected[3], double abs_tol,
      bool result[3]);

   static bool within_tolerance (
      const double computed[3], const double expected[3],
      double abs_tol, double rel_tol);

   static bool within_tolerance (
      const double computed[3], const double expected[3],
      double abs_tol, double rel_tol,
      bool result[3]);


   // Matrix within tolerance, four versions.
   static bool within_tolerance (
      const double computed[3][3], const double expected[3][3], double abs_tol);

   static bool within_tolerance (
      const double computed[3][3], const double expected[3][3], double abs_tol,
      bool result[3][3]);

   static bool within_tolerance (
      const double computed[3][3], const double expected[3][3],
      double abs_tol, double rel_tol);

   static bool within_tolerance (
      const double computed[3][3], const double expected[3][3],
      double abs_tol, double rel_tol,
      bool result[3][3]);


   // The is_within methods apply to scalars, vectors, and matrices.
   static bool is_within (
      double computed, double lower_lim, double upper_lim);

   static bool is_within (
      const double computed[3],
      const double lower_lim[3], const double upper_lim[3]);

   static bool is_within (
      const double computed[3],
      const double lower_lim[3], const double upper_lim[3],
      bool result[3]);

   static bool is_within (
      const double computed[3][3],
      const double lower_lim[3][3], const double upper_lim[3][3]);

   static bool is_within (
      const double computed[3][3],
      const double lower_lim[3][3], const double upper_lim[3][3],
      bool result[3][3]);


   // The is_zero methods apply to scalars, vectors, and matrices.
   static bool is_zero (
      const double computed, double abs_tol);

   static bool is_zero (
      const double computed[3], double abs_tol);

   static bool is_zero (
      const double computed[3], double abs_tol, bool result[3]);

   static bool is_zero (
      const double computed[3][3], double abs_tol);

   static bool is_zero (
      const double computed[3][3], double abs_tol, bool result[3][3]);


   // The is_identity methods apply to matrices and quaternions.
   static bool is_identity (
      const double computed[3][3],
      double diag_tol, double off_diag_tol);

   static bool is_identity (
      const double computed[3][3],
      double diag_tol, double off_diag_tol, bool result[3][3]);

   static bool is_identity (
      const Quaternion & computed,
      double scalar_tol, double vector_tol);


   // The is_unit methods apply to scalars, vectors, matrices, and quaternions.
   // Note that because this test applies to the object as a whole there is
   // no test to identify the culprits on an element-by-element basis.
   static bool is_unit (
      const double computed, double abs_tol);

   static bool is_unit (
      const double computed[3], double abs_tol);

   static bool is_unit (
      const double computed[3][3], double unit_tol, double normal_tol);

   static bool is_unit (
      const Quaternion & computed, double abs_tol);


   // The within_rss_tolerance methods apply to vectors and matrices.
   static bool within_rss_tolerance (
      const double computed[3], const double expected[3],
      double rss_tolerance);

   static bool within_rss_tolerance (
      const double computed[3][3], const double expected[3][3],
      double rss_tolerance);


   // The within_eigen_tolerance methods apply to matrices and quaternions.
   static bool within_eigen_tolerance (
      const double computed[3][3], const double expected[3][3],
      double angular_tolerance);

   static bool within_eigen_tolerance (
      const Quaternion & computed, const Quaternion & expected,
      double angular_tolerance);
};


/*******************************************************************************
within_tolerance
*******************************************************************************/


/**
 * Test whether the computed value agrees with the expected value.
 * This method compares the absolute error to the provided absolute tolerance.
 *
 * \par Assumptions and Limitations
 *  - The absolute tolerance is assumed to be positive.
 *  - An error smaller than the absolute tolerance is assumed to be calculable.
 *     The expected value must be absolutely smaller than 5e15 times absolute
 *     tolerance lest the comparison devolve to equality. An absolute ratio of
 *     1e14 or smaller is preferable.
 * @return Is the computed value within tolerance?
 * \param[in] computed The computed value
 * \param[in] expected The expected value
 * \param[in] abs_tol Max absolute error
 */


/*
Purpose:
  (Test whether the computed value agrees with the expected value.
   This method compares the absolute error to the provided absolute tolerance.)
Assumptions and limitations:
  ((The absolute tolerance is assumed to be positive.)
   (An error smaller than the absolute tolerance is assumed to be calculable.
    The expected value must be absolutely smaller than 5e15 times absolute
    tolerance lest the comparison devolve to equality. An absolute ratio of
    1e14 or smaller is preferable.))
*/
inline bool
CheckItem::within_tolerance (
   double computed,
   double expected,
   double abs_tol)
{
   return std::abs(computed - expected) <= abs_tol;
}


/**
 * Test whether the computed value agrees with the expected value.
 * This method compares the absolute error to a computed tolerance.
 * This computed tolerance is the absolute tolerance for small values of the
 * expected value but the product of the relative tolerance and the expected
 * value for larger values of the expected value.
 *
 * \par Assumptions and Limitations
 *  - The provided tolerances are assumed to be positive.
 * @return Is the computed value within tolerance?
 * \param[in] computed The computed value
 * \param[in] expected The expected value
 * \param[in] abs_tol Max absolute error, used when expected is small
 * \param[in] rel_tol Max relative error, for not-so-small values
 */
inline bool
CheckItem::within_tolerance (
   double computed,
   double expected,
   double abs_tol,
   double rel_tol)
{
   return std::abs(computed - expected) <=
          std::max(abs_tol, rel_tol*std::abs(expected));
}


/**
 * Test whether each element of the computed vector agrees with the
 * corresponding element of the expected vector.
 * See the corresponding scalar version for the nature of the test.
 * @return Is the vector within tolerance?
 * \param[in] computed The computed value
 * \param[in] expected The expected value
 * \param[in] abs_tol Max absolute error
 */
inline bool
CheckItem::within_tolerance (
   const double computed[3],
   const double expected[3],
   double abs_tol)
{
   return within_tolerance (computed[0], expected[0], abs_tol) &&
          within_tolerance (computed[1], expected[1], abs_tol) &&
          within_tolerance (computed[2], expected[2], abs_tol);
}


/**
 * Test whether each element of the computed vector agrees with the
 * corresponding element of the expected vector.
 * See the corresponding scalar version for the nature of the test.
 * @return Is the vector within tolerance?
 * \param[in] computed The computed value
 * \param[in] expected The expected value
 * \param[in] abs_tol Max absolute error
 * \param[out] result Item-by-item status
 */
inline bool
CheckItem::within_tolerance (
   const double computed[3],
   const double expected[3],
   double abs_tol,
   bool result[3])
{
   result[0] = within_tolerance (computed[0], expected[0], abs_tol);
   result[1] = within_tolerance (computed[1], expected[1], abs_tol);
   result[2] = within_tolerance (computed[2], expected[2], abs_tol);

   return result[0] && result[1] && result[2];
}


/**
 * Test whether each element of the computed vector agrees with the
 * corresponding element of the expected vector.
 * See the corresponding scalar version for the nature of the test.
 * @return Is the vector within tolerance?
 * \param[in] computed The computed value
 * \param[in] expected The expected value
 * \param[in] abs_tol Max absolute error
 * \param[in] rel_tol Max relative error
 */
inline bool
CheckItem::within_tolerance (
   const double computed[3],
   const double expected[3],
   double abs_tol,
   double rel_tol)
{
   return within_tolerance (computed[0], expected[0], abs_tol, rel_tol) &&
          within_tolerance (computed[1], expected[1], abs_tol, rel_tol) &&
          within_tolerance (computed[2], expected[2], abs_tol, rel_tol);
}


/**
 * Test whether each element of the computed vector agrees with the
 * corresponding element of the expected vector.
 * See the corresponding scalar version for the nature of the test.
 * @return Is the vector within tolerance?
 * \param[in] computed The computed value
 * \param[in] expected The expected value
 * \param[in] abs_tol Max absolute error
 * \param[in] rel_tol Max relative error
 * \param[out] result Item-by-item status
 */
inline bool
CheckItem::within_tolerance (
   const double computed[3],
   const double expected[3],
   double abs_tol,
   double rel_tol,
   bool result[3])
{
   result[0] = within_tolerance (computed[0], expected[0], abs_tol, rel_tol);
   result[1] = within_tolerance (computed[1], expected[1], abs_tol, rel_tol);
   result[2] = within_tolerance (computed[2], expected[2], abs_tol, rel_tol);

   return result[0] && result[1] && result[2];
}


/**
 * Test whether each element of the computed matrix agrees with the
 * corresponding element of the expected matrix.
 * See the corresponding scalar version for the nature of the test.
 * @return Is the matrix within tolerance?
 * \param[in] computed The computed matrix
 * \param[in] expected The expected matrix
 * \param[in] abs_tol Max absolute error
 */
inline bool
CheckItem::within_tolerance (
   const double computed[3][3],
   const double expected[3][3],
   double abs_tol)
{
   return within_tolerance (computed[0], expected[0], abs_tol) &&
          within_tolerance (computed[1], expected[1], abs_tol) &&
          within_tolerance (computed[2], expected[2], abs_tol);
}


/**
 * Test whether each element of the computed matrix agrees with the
 * corresponding element of the expected matrix.
 * See the corresponding scalar version for the nature of the test.
 * @return Is the matrix within tolerance?
 * \param[in] computed The computed matrix
 * \param[in] expected The expected matrix
 * \param[in] abs_tol Max absolute error
 * \param[out] result Item-by-item status
 */
inline bool
CheckItem::within_tolerance (
   const double computed[3][3],
   const double expected[3][3],
   double abs_tol,
   bool result[3][3])
{
   bool vresult0, vresult1, vresult2;

   vresult0 = within_tolerance (computed[0], expected[0], abs_tol, result[0]);
   vresult1 = within_tolerance (computed[1], expected[1], abs_tol, result[1]);
   vresult2 = within_tolerance (computed[2], expected[2], abs_tol, result[2]);

   return vresult0 && vresult1 && vresult2;
}


/**
 * Test whether each element of the computed matrix agrees with the
 * corresponding element of the expected matrix.
 * See the corresponding scalar version for the nature of the test.
 * @return Is the matrix within tolerance?
 * \param[in] computed The computed matrix
 * \param[in] expected The expected matrix
 * \param[in] abs_tol Max absolute error
 * \param[in] rel_tol Max relative error
 */
inline bool
CheckItem::within_tolerance (
   const double computed[3][3],
   const double expected[3][3],
   double abs_tol,
   double rel_tol)
{
   return within_tolerance (computed[0], expected[0], abs_tol, rel_tol) &&
          within_tolerance (computed[1], expected[1], abs_tol, rel_tol) &&
          within_tolerance (computed[2], expected[2], abs_tol, rel_tol);
}


/**
 * Test whether each element of the computed matrix agrees with the
 * corresponding element of the expected matrix.
 * See the corresponding scalar version for the nature of the test.
 * @return Is the matrix within tolerance?
 * \param[in] computed The computed matrix
 * \param[in] expected The expected matrix
 * \param[in] abs_tol Max absolute error
 * \param[in] rel_tol Max relative error
 * \param[out] result Item-by-item status
 */
inline bool
CheckItem::within_tolerance (
   const double computed[3][3],
   const double expected[3][3],
   double abs_tol,
   double rel_tol,
   bool result[3][3])
{
   bool vresult0, vresult1, vresult2;

   vresult0 = within_tolerance (computed[0], expected[0], abs_tol, rel_tol,
                                result[0]);
   vresult1 = within_tolerance (computed[1], expected[1], abs_tol, rel_tol,
                                result[1]);
   vresult2 = within_tolerance (computed[2], expected[2], abs_tol, rel_tol,
                                result[2]);

   return vresult0 && vresult1 && vresult2;
}


/*******************************************************************************
is_within
*******************************************************************************/


/**
 * Test whether the computed value is between the specified bounds, inclusive.
 * @return Is the computed value within bounds?
 * \param[in] computed The computed value
 * \param[in] lower_lim The lower limit
 * \param[in] upper_lim The upper limit
 */


/*
Purpose:
  (Test whether the computed value is between the specified bounds, inclusive.)
*/
inline bool
CheckItem::is_within (
   double computed,
   double lower_lim,
   double upper_lim)
{
   return (computed >= lower_lim) && (computed <= upper_lim);
}


/**
 * Test whether each element of the computed vector is between the corresponding
 * elements of the lower and upper bound vectors, inclusive.
 * @return Is the computed value within bounds?
 * \param[in] computed The computed value
 * \param[in] lower_lim The lower limit
 * \param[in] upper_lim The upper limit
 */
inline bool
CheckItem::is_within (
   const double computed[3],
   const double lower_lim[3],
   const double upper_lim[3])
{
   return is_within (computed[0], lower_lim[0], upper_lim[0]) &&
          is_within (computed[1], lower_lim[1], upper_lim[1]) &&
          is_within (computed[2], lower_lim[2], upper_lim[2]);
}


/**
 * Test whether each element of the computed vector is between the corresponding
 * elements of the lower and upper bound vectors, inclusive.
 * @return Is the computed value within bounds?
 * \param[in] computed The computed value
 * \param[in] lower_lim The lower limit
 * \param[in] upper_lim The upper limit
 * \param[out] result Item-by-item status
 */
inline bool
CheckItem::is_within (
   const double computed[3],
   const double lower_lim[3],
   const double upper_lim[3],
   bool result[3])
{
   result[0] = is_within (computed[0], lower_lim[0], upper_lim[0]);
   result[1] = is_within (computed[1], lower_lim[1], upper_lim[1]);
   result[2] = is_within (computed[2], lower_lim[2], upper_lim[2]);

   return result[0] && result[1] && result[2];
}


/**
 * Test whether each element of the computed matrix is between the corresponding
 * elements of the lower and upper bound matrices, inclusive.
 * @return Is the computed value within bounds?
 * \param[in] computed The computed value
 * \param[in] lower_lim The lower limit
 * \param[in] upper_lim The upper limit
 */
inline bool
CheckItem::is_within (
   const double computed[3][3],
   const double lower_lim[3][3],
   const double upper_lim[3][3])
{
   return is_within (computed[0], lower_lim[0], upper_lim[0]) &&
          is_within (computed[1], lower_lim[1], upper_lim[1]) &&
          is_within (computed[2], lower_lim[2], upper_lim[2]);
}


/**
 * Test whether each element of the computed matrix is between the corresponding
 * elements of the lower and upper bound matrices, inclusive.
 * @return Is the computed value within bounds?
 * \param[in] computed The computed value
 * \param[in] lower_lim The lower limit
 * \param[in] upper_lim The upper limit
 * \param[out] result Item-by-item status
 */
inline bool
CheckItem::is_within (
   const double computed[3][3],
   const double lower_lim[3][3],
   const double upper_lim[3][3],
   bool result[3][3])
{
   bool vresult0, vresult1, vresult2;

   vresult0 = is_within (computed[0], lower_lim[0], upper_lim[0]);
   vresult1 = is_within (computed[1], lower_lim[1], upper_lim[1]);
   vresult1 = is_within (computed[2], lower_lim[2], upper_lim[2]);

   return vresult0 && vresult1 && vresult2;
}


/*******************************************************************************
is_zero
*******************************************************************************/


/**
 * Test whether the computed value is approximately equal to zero.
 * @return Is this zero?
 * \param[in] computed The computed value
 * \param[in] abs_tol Max absolute error
 */


/*
Purpose:
  (Test whether the computed value is approximately equal to zero.)
*/
inline bool
CheckItem::is_zero (
   double computed,
   double abs_tol)
{
   return std::abs(computed) <= abs_tol;
}


/**
 * Test whether each element of the computed vector is approximately zero.
 * @return Is this a zero vector?
 * \param[in] computed The computed vector
 * \param[in] abs_tol Max absolute error
 */
inline bool
CheckItem::is_zero (
   const double computed[3],
   double abs_tol)
{
   return is_zero (computed[0], abs_tol) &&
          is_zero (computed[1], abs_tol) &&
          is_zero (computed[2], abs_tol);
}


/**
 * Test whether each element of the computed vector is approximately zero.
 * @return Is this a zero vector?
 * \param[in] computed The computed vector
 * \param[in] abs_tol Max absolute error
 * \param[out] result Item-by-item status
 */
inline bool
CheckItem::is_zero (
   const double computed[3],
   double abs_tol,
   bool result[3])
{
   result[0] = is_zero (computed[0], abs_tol);
   result[1] = is_zero (computed[1], abs_tol);
   result[2] = is_zero (computed[2], abs_tol);

   return result[0] && result[1] && result[2];
}


/**
 * Test whether each element of the computed matrix is approximately zero.
 * @return Is this a zero matrix?
 * \param[in] computed The computed matrix
 * \param[in] abs_tol Max absolute error
 */
inline bool
CheckItem::is_zero (
   const double computed[3][3],
   double abs_tol)
{
   return is_zero (computed[0], abs_tol) &&
          is_zero (computed[1], abs_tol) &&
          is_zero (computed[2], abs_tol);
}


/**
 * Test whether each element of the computed matrix is approximately zero.
 * @return Is this a zero matrix?
 * \param[in] computed The computed matrix
 * \param[in] abs_tol Max absolute error
 * \param[out] result Item-by-item status
 */
inline bool
CheckItem::is_zero (
   const double computed[3][3],
   double abs_tol,
   bool result[3][3])
{
   bool vresult0, vresult1, vresult2;

   vresult0 = is_zero (computed[0], abs_tol, result[0]);
   vresult1 = is_zero (computed[1], abs_tol, result[1]);
   vresult2 = is_zero (computed[2], abs_tol, result[2]);

   return vresult0 && vresult1 && vresult2;
}


/*******************************************************************************
is_identity
*******************************************************************************/


/**
 * Test whether the computed matrix is more or less the identity matrix.
 * @return Is the matrix an identity matrix?
 * \param[in] computed The computed matrix
 * \param[in] diag_tol Max error, diagonal elements
 * \param[in] off_diag_tol Max error, off-diagonal elements
 */


/*
Purpose:
  (Test whether the computed matrix is more or less the identity matrix.)
*/
inline bool
CheckItem::is_identity (
   const double computed[3][3],
   double diag_tol,
   double off_diag_tol)
{
   return is_unit (computed[0][0], diag_tol) &&
          is_unit (computed[1][1], diag_tol) &&
          is_unit (computed[2][2], diag_tol) &&
          is_zero (computed[0][1], off_diag_tol) &&
          is_zero (computed[0][2], off_diag_tol) &&
          is_zero (computed[1][0], off_diag_tol) &&
          is_zero (computed[1][2], off_diag_tol) &&
          is_zero (computed[2][0], off_diag_tol) &&
          is_zero (computed[2][1], off_diag_tol);
}


/**
 * Test whether the computed matrix is more or less the identity matrix.
 * @return Is the matrix an identity matrix?
 * \param[in] computed The computed matrix
 * \param[in] diag_tol Max error, diagonal elements
 * \param[in] off_diag_tol Max error, off-diagonal elements
 * \param[out] result Item-by-item status
 */
inline bool
CheckItem::is_identity (
   const double computed[3][3],
   double diag_tol,
   double off_diag_tol,
   bool result[3][3])
{
   result[0][0] = is_unit (computed[0][0], diag_tol);
   result[1][1] = is_unit (computed[1][1], diag_tol);
   result[2][2] = is_unit (computed[2][2], diag_tol);
   result[0][1] = is_zero (computed[0][1], off_diag_tol);
   result[0][2] = is_zero (computed[0][2], off_diag_tol);
   result[1][0] = is_zero (computed[1][0], off_diag_tol);
   result[1][2] = is_zero (computed[1][2], off_diag_tol);
   result[2][0] = is_zero (computed[2][0], off_diag_tol);
   result[2][1] = is_zero (computed[2][1], off_diag_tol);

   return result[0][0] && result[0][1] && result[0][2] &&
          result[1][0] && result[1][1] && result[1][2] &&
          result[2][0] && result[2][1] && result[2][2];
}


/**
 * Test whether the computed quaternion is the real unit quaternion.
 * @return Is this the real unit quaternion?
 * \param[in] computed The computed quaternion
 * \param[in] scalar_tol Max error, scalar part
 * \param[in] vector_tol Max error, vector elements
 */
inline bool
CheckItem::is_identity (
   const Quaternion & computed,
   double scalar_tol,
   double vector_tol)
{
   return is_unit (computed.scalar, scalar_tol) &&
          is_zero (computed.vector, vector_tol);
}


/*******************************************************************************
is_unit
*******************************************************************************/


/**
 * Test whether the computed value is approximately equal to one.
 * @return Is this one?
 * \param[in] computed The computed value
 * \param[in] abs_tol Max absolute error
 */


/*
Purpose:
  (Test whether the computed value is approximately equal to one.)
*/
inline bool
CheckItem::is_unit (
   double computed,
   double abs_tol)
{
   return std::abs(computed - 1.0) <= abs_tol;
}


/**
 * Test whether the computed vector is more or less a unit vector.
 * @return Is this a unit vector?
 * \param[in] computed The computed vector
 * \param[in] abs_tol Max absolute error
 */
inline bool
CheckItem::is_unit (
   const double computed[3],
   double abs_tol)
{
   return is_within (Vector3::vmagsq(computed),
                     1.0 - abs_tol*(2.0-abs_tol),
                     1.0 + abs_tol*(2.0+abs_tol));
}


/**
 * Test whether the computed matrix is more or less a rotation matrix.
 * @return Is this an orthogonal matrix?
 * \param[in] computed The computed matrix
 * \param[in] unit_tol Unit vector tolerance
 * \param[in] normal_tol Normality tolerance
 */
inline bool
CheckItem::is_unit (
   const double computed[3][3],
   double unit_tol,
   double normal_tol)
{
   double e1_cross_e2[3];

   return is_unit (computed[0], unit_tol) &&
          is_unit (computed[1], unit_tol) &&
          is_unit (computed[2], unit_tol) &&

          is_zero (Vector3::dot (computed[0], computed[1]), normal_tol) &&
          is_zero (Vector3::dot (computed[0], computed[2]), normal_tol) &&
          is_zero (Vector3::dot (computed[1], computed[2]), normal_tol) &&

          (Vector3::dot (
              Vector3::cross (computed[0], computed[1], e1_cross_e2),
              computed[2]) > 0.0);
}


/**
 * Test whether the computed quaternion is more or less a unit quaternion.
 * @return Is this a unit quaternion?
 * \param[in] computed The computed quaternion
 * \param[in] abs_tol Max absolute error
 */
inline bool
CheckItem::is_unit (
   const Quaternion & computed,
   double abs_tol)
{
   return is_within (computed.norm_sq(),
                     1.0 - abs_tol*(2.0-abs_tol),
                     1.0 + abs_tol*(2.0+abs_tol));
}


/*******************************************************************************
within_rss_tolerance
*******************************************************************************/


/**
 * Test whether the computed and expected vectors are close to one another
 * in a root sum square (Euclidean norm) sense.
 * @return Is the vector within tolerance?
 * \param[in] computed The computed matrix
 * \param[in] expected The expected matrix
 * \param[in] rss_tolerance Allowed RSS error
 */


/*
Purpose:
  (Test whether the computed and expected vectors are close to one another
   in a root sum square (Euclidean norm) sense.)
*/
inline bool
CheckItem::within_rss_tolerance (
   const double computed[3],
   const double expected[3],
   double rss_tolerance)
{
   double diff[3];
   return is_zero (Vector3::vmagsq (Vector3::diff (computed, expected, diff)),
                   rss_tolerance*rss_tolerance);
}


/**
 * Test whether the computed and expected matrices are close to one another
 * in a root sum square (Frobenius norm) sense.
 * @return Is the matrix within tolerance?
 * \param[in] computed The computed matrix
 * \param[in] expected The expected matrix
 * \param[in] rss_tolerance Allowed RSS error
 */
inline bool
CheckItem::within_rss_tolerance (
   const double computed[3][3],
   const double expected[3][3],
   double rss_tolerance)
{
   double diff[3];
   double emagsq[3];

   emagsq[0] = Vector3::vmagsq (Vector3::diff (computed[0], expected[0], diff));
   emagsq[1] = Vector3::vmagsq (Vector3::diff (computed[1], expected[1], diff));
   emagsq[2] = Vector3::vmagsq (Vector3::diff (computed[2], expected[2], diff));

   return is_zero (emagsq[0] + emagsq[1] + emagsq[2],
                   rss_tolerance*rss_tolerance);
}


/*******************************************************************************
within_eigen_tolerance
*******************************************************************************/


/**
 * Test whether the computed and expected matrices are close to one another
 * in an eigen rotation sense. The test passes if the eigen rotation from
 * expected to computed is a small angle.
 * @return Is the matrix within tolerance?
 * \param[in] computed The computed transformation matrix
 * \param[in] expected The expected transformation matrix
 * \param[in] angular_tolerance Allowed angular error\n Units: r
 */


/*
Purpose:
  (Test whether the computed and expected matrices are close to one another
   in an eigen rotation sense. The test passes if the eigen rotation from
   expected to computed is a small angle.)
*/
inline bool
CheckItem::within_eigen_tolerance (
   const double computed[3][3],
   const double expected[3][3],
   double angular_tolerance)
{
   double error_matrix[3][3];
   double eigen_angle;
   double eigen_axis[3];

   Matrix3x3::product_left_transpose (computed, expected, error_matrix);
   Orientation::compute_eigen_rotation_from_matrix (
      error_matrix, &eigen_angle, eigen_axis);
   return std::abs(eigen_angle) <= angular_tolerance;
}


/**
 * Test whether the computed and expected quaternions are close to one another
 * in an eigen rotation sense. The test passes if the eigen rotation
 * from expected to computed is a small angle.
 * @return Is the quaternion within tolerance?
 * \param[in] computed The computed quaternion
 * \param[in] expected The expected value
 * \param[in] angular_tolerance Allowed angular error\n Units: r
 */
inline bool
CheckItem::within_eigen_tolerance (
   const Quaternion & computed,
   const Quaternion & expected,
   double angular_tolerance)
{
   double eigen_angle;
   double eigen_axis[3];

   computed.eigen_compare (expected, &eigen_angle, eigen_axis);
   return std::abs(eigen_angle) <= angular_tolerance;
}

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 */
