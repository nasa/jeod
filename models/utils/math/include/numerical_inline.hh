//=============================================================================
// Notices:
//
// Copyright © 2022 United States Government as represented by the Administrator
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
 * @file models/utils/math/include/numerical_inline.hh
 * Vector math inline functions
 */

/*******************************************************************************
Purpose:
  ()

 
*******************************************************************************/


#ifndef JEOD_NUMERICAL_INLINE_H
#define JEOD_NUMERICAL_INLINE_H

// JEOD includes
#include "numerical.hh"

// FIXME: This should be a constant.
#ifdef GSL_SQRT_DBL_MIN
#undef GSL_SQRT_DBL_MIN
#endif
#define GSL_SQRT_DBL_MIN 1.4916681462400413e-154


//! Namespace jeod
namespace jeod {

/**
 * Absolute value
 * @return Absolute value of x
 * \param[in] x x
 */
inline double
Numerical::fabs (
   double x)
{
   return (x < 0.0) ? -x : x;
}


/**
 * Compute the square of a number, protecting against undeflow
 * @return value^2 or zero if too small
 * \param[in] value Value
 */
inline double
Numerical::square (
   double value)
{

   if (fabs (value) > GSL_SQRT_DBL_MIN) {
      return value * value;
   }
   else {
      return 0.0;
   }
}


/**
 * Add number squared to accumulator, protecting against undeflow
 * @return Accumulated value
 * \param[in] value Value
 * \param[in,out] sum Accumulator
 */
inline double
Numerical::square_incr (
   double value,
   double & sum)
{
   if (fabs (value) > GSL_SQRT_DBL_MIN) {
      sum += value * value;
   }
   return sum;
}


/**
 * Compare two doubles for exact equality
 * @return whether inputs are exactly the same
 * \param[in] x Value1
 * \param[in] y Value2
 */
inline bool
Numerical::compare_exact (
   double x,
   double y)
{
   return *((long long *)&x)==*((long long *)&y);
}

} // End JEOD namespace


#endif

/**
 * @}
 * @}
 * @}
 */
