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
 * @addtogroup Integration
 * @{
 * @addtogroup GaussJackson
 * @{
 *
 * @file models/utils/integration/gauss_jackson/include/gauss_jackson_coeffs.hh
 * Defines the class GaussJacksonCoeffs, which contains the
 * Gauss-Jackson predictor and corrector coefficients.
 */


/*
Purpose: ()
Library dependencies:
  ((../src/gauss_jackson_coeffs.cc))
*/


#ifndef JEOD_GAUSS_JACKSON_COEFFS_HH
#define JEOD_GAUSS_JACKSON_COEFFS_HH

// Local includes
#include "gauss_jackson_coefficients_pair.hh"

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// System includes
#include <iosfwd>


//! Namespace jeod 
namespace jeod {

/**
 * Contains the Gauss-Jackson predictor and corrector coefficients.
 */
class GaussJacksonCoeffs {

  JEOD_MAKE_SIM_INTERFACES(GaussJacksonCoeffs)

public:

   // Member data.

   /**
    * Summed Adams and Gauss-Jackson predictor coefficients.
    */
   GaussJacksonCoefficientsPair predictor; //!< trick_units(--)

   /**
    * Summed Adams and Gauss-Jackson corrector coefficients.
    */
   GaussJacksonCoefficientsPair* corrector; //!< trick_units(--)

   /**
    * Maximum order; used for sizing.
    */
   unsigned int max_order; //!< trick_units(--)

   /**
    * Current order; dictates the coefficient values.
    */
   unsigned int order; //!< trick_units(--)


   // Member functions.

   /**
    * Default constructor.
    */
   GaussJacksonCoeffs ()
   :
      predictor(),
      corrector(nullptr),
      max_order(0),
      order(0)
   {}

   /**
    * Copy constructor.
    * Note that this doesn't copy; it recomputes.
    * The end result is as if a copy had been made.
    * @param  src  Object to be copied.
    */
   GaussJacksonCoeffs (const GaussJacksonCoeffs & src)
   {
      if (max_order != src.max_order) {
         configure (src.max_order);
         order = 0;
      }
      if (order != src.order) {
         compute_coeffs (src.order);
      }
   }

   /**
    * Destructor.
    */
   ~GaussJacksonCoeffs ();

   /**
    * Copy-and-swap assignment operator.
    * @param  src  Object to be copied.
    */
   GaussJacksonCoeffs& operator= (GaussJacksonCoeffs src)
   {
      swap (src);
      return *this;
   }

   /**
    * Non-throwing swap.
    * @param  src  Object to swap contents with.
    */
   void swap (GaussJacksonCoeffs & src);

   /**
    * Configure to enable coefficients up to the specified maximum order.
    * @param  max_order_in  The maximum order to be used.
    */
   void configure (unsigned int max_order_in);

   /**
    * Compute coefficients for the specified order.
    * @param  order_in  The current order.
    */
   void compute_coeffs (unsigned int order_in);

   /**
    * Print the coefficients.
    * @param  stream  The stream to be printed to.
    * @param  coeff   The coefficients to be printed.
    */
   friend std::ostream & operator<< (
      std::ostream& stream,
      const GaussJacksonCoeffs& coeff);
};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
