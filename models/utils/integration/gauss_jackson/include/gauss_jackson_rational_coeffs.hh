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
 * @file models/utils/integration/gauss_jackson/include/gauss_jackson_rational_coeffs.hh
 * Defines the class GaussJacksonRationalCoefficients, which contains
 * a set of Adams or Stormer-Cowell coefficients.
 */


/*
Purpose: ()
Library dependencies:
  ((gauss_jackson_rational_coeffs.o))
*/


#ifndef JEOD_GAUSS_JACKSON_RATIONAL_COEFFICIENTS_HH
#define JEOD_GAUSS_JACKSON_RATIONAL_COEFFICIENTS_HH

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Math utilities
#include "er7_utils/math/include/ratio128.hh"

// System includes
#include <vector>


// Forward declarations

/**
 * Namespace er7_utils contains the state integration models used by JEOD.
 */
namespace er7_utils {
   class NChooseM;
}


//! Namespace jeod 
namespace jeod {

/**
 * Contains a set of Adams or Stormer-Cowell coefficients.
 */
class GaussJacksonRationalCoefficients {

  JEOD_MAKE_SIM_INTERFACES(GaussJacksonRationalCoefficients)

public:

   /**
    * The coefficients.
    */
   std::vector<er7_utils::Ratio128> coefficients; //!< trick_units(--)


   /**
    * Default constructor.
    */
   GaussJacksonRationalCoefficients () {}


   /**
    * Configure the coefficients as an Adams corrector in difference form.
    * @param  nelem  The number of elements in the coefficients vector.
    */
   void configure_adams_corrector (
      unsigned int nelem);

   /**
    * Construct a GaussJacksonRationalCoefficients that contains the
    * Stormer-Cowell corrector coefficients. The coefficients are assumed
    * to be configured as Adams coefficients in difference form.
    *
    * @return
    * A GaussJacksonRationalCoefficients object with the coefficients
    * configured as Stormer-Cowell corrector coefficients.
    */
   GaussJacksonRationalCoefficients construct_stormer_cowell_corrector ()
   const;

   /**
    * Construct a GaussJacksonRationalCoefficients that contains a set of
    * predictor coefficients. The coefficients are assumed to be configured
    * as either Adams or Stormer-Cowell corrector coefficients.
    *
    * @return
    * A GaussJacksonRationalCoefficients object with the coefficients
    * configured as Adams or Stormer-Cowell predictor coefficients.
    */
   GaussJacksonRationalCoefficients construct_predictor ()
   const;

   /**
    * Convert the coefficients to ordinate form.
    * @param  n_choose_m  An NChooseM object that computes N choose M.
    * @param  result      The output ordinate form coefficients.
    */
   void convert_to_ordinate_form (
      er7_utils::NChooseM & n_choose_m, double * result)
   const;

   /**
    * Discard the specified number of terms from the front and back
    * of the coefficients array.
    * @param  nfront  The number of terms to be discarded from the front of
    *                 the coefficients vector.
    * @param  nback   The number of terms to be discarded from the back of
    *                 the coefficients vector.
    */
   void discard_extra_terms (unsigned int nfront, unsigned int nback);

   /**
    * Displace the corrector coefficients back one time step.
    */
   void displace_back ();
};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
