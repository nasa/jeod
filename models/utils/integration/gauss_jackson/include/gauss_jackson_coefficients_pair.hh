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
 * @addtogroup Integration
 * @{
 * @addtogroup GaussJackson
 * @{
 *
 * @file models/utils/integration/gauss_jackson/include/gauss_jackson_coefficients_pair.hh
 * Defines the class GaussJacksonCoefficientsPair, which contains
 * summed Adams and Gauss-Jackson coefficient pair.
 */


/*
Purpose: ()
Library dependencies:
  ((gauss_jackson_coefficients_pair.o))
*/


#ifndef JEOD_GAUSS_JACKSON_COEFFICIENTS_PAIR_HH
#define JEOD_GAUSS_JACKSON_COEFFICIENTS_PAIR_HH

// Local includes
#include "gauss_jackson_one_state.hh"
#include "gauss_jackson_two_state.hh"

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// ER7 utilities includes
#include "er7_utils/interface/include/config.hh"

// System includes
#include <cstddef>
#include <iosfwd>



//! Namespace jeod 
namespace jeod {

/**
 * Contains a summed Adams and Gauss-Jackson coefficient pair.
 */
class GaussJacksonCoefficientsPair {

  JEOD_MAKE_SIM_INTERFACES(GaussJacksonCoefficientsPair)

public:

   // Member data.

   /**
    * Summed Adams coefficients, in ordinate form.
    */
   double* sa_coefs; //!< trick_units(--)

   /**
    * Gauss Jackson coefficients, in ordinate form.
    */
   double* gj_coefs; //!< trick_units(--)


   // Member functions.

   /**
    * Default constructor.
    */
   GaussJacksonCoefficientsPair ()
   :
      sa_coefs (NULL),
      gj_coefs (NULL)
   {}

   /**
    * Destructor.
    */
   ~GaussJacksonCoefficientsPair ()
   {
      deallocate_arrays ();
   }

   /**
    * Allocate (re-allocate) memory for the coefficients. Arrays are size
    & to contain max_order+1 elements.
    * @param  max_order  Maximum order that will be used.
    */
   void configure (int max_order)
   {
      deallocate_arrays ();
      allocate_arrays (max_order+1);
   }


   /**
    * Non-throwing swap.
    * @param  other  Coeffs pair with which contents are to be swapped.
    */
   void swap (GaussJacksonCoefficientsPair & other);


   /**
    * Allocate space for the coefficients.
    * @param  size  Array size.
    */
   void allocate_arrays (int size);


   /**
    * Release allocated memory.
    */
   void deallocate_arrays ();


   /**
    * Apply both sets of coefficients to the supplied history data.
    * The first element of the output state_sum is calculated as the inner
    * products of the acceleration history with the summed Adams coefficients.
    * The second element is calculated as the inner product with the
    * Gauss-Jackson coefficients.
    * (First = first integral; second = second integral.)
    * @param  nelem      Dimensionality of each acceleration history element
    * @param  ncoeff     Number of elements in the acceleration history
    * @param  acc_hist   Acceleration history
    * @param  state_sum  Output inner products
    */
   void apply (
      int nelem,
      int ncoeff,
      double const * ER7_UTILS_RESTRICT const * ER7_UTILS_RESTRICT acc_hist,
      GaussJacksonTwoState & state_sum) const ER7_UTILS_RESTRICT;

   /**
    * Apply just the Adams coefficients to the supplied history data.
    * @param  nelem      Dimensionality of each acceleration history element
    * @param  ncoeff     Number of elements in the acceleration history
    * @param  acc_hist   Acceleration history
    * @param  state_sum  Output inner products
    */
   void apply (
      int nelem,
      int ncoeff,
      double const * ER7_UTILS_RESTRICT const * ER7_UTILS_RESTRICT acc_hist,
      GaussJacksonOneState & state_sum) const ER7_UTILS_RESTRICT;


   /**
    * Print the coefficients.
    * @param  order   Coefficients order
    * @param  stream  Output stream
    */
   void print (int order, std::ostream & stream) const;


private:

   // The copy constructor and assignment operator are private/unimplemented.

   /**
    * Not implemented.
    */
   GaussJacksonCoefficientsPair (
      const GaussJacksonCoefficientsPair &);

   /**
    * Not implemented.
    */
   GaussJacksonCoefficientsPair& operator= (
      const GaussJacksonCoefficientsPair &);
};

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
