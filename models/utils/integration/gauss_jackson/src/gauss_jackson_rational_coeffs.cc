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
 * @file models/utils/integration/gauss_jackson/src/gauss_jackson_rational_coeffs.cc
 * Defines member functions for the class GaussJacksonRationalCoefficients.
 */


/*
Purpose: ()
Library dependencies:
  ((gauss_jackson_rational_coeffs.o))
*/


// Local includes
#include "../include/gauss_jackson_rational_coeffs.hh"

// ER7 utilities
#include "er7_utils/math/include/n_choose_m.hh"
#include "er7_utils/math/include/ratio128.hh"

// System includes
#include <cassert>


//! Namespace jeod 
namespace jeod {

// Configure the coefficients as an Adams corrector in difference form.
void
GaussJacksonRationalCoefficients::configure_adams_corrector (
   unsigned int nelem)
{
   // Resize the coefficients array to accommodate nelem elements.
   coefficients.resize (nelem);

   // Calculate the coefficients using
   //   c_0 = 1
   //   c_n = -\sum_{i=0}^{n-1} \frac {c_i} {n+1-i}
   // See Berry eqn 2.37 and 2.38 for details.
   coefficients[0] = 1;
   for (unsigned int nn = 1; nn < nelem; ++nn) {
      er7_utils::Ratio128 sum = 0;
      for (unsigned int ii = 0; ii < nn; ++ii) {
         sum -= coefficients[ii] / er7_utils::Ratio128(nn+1-ii);
      }
      coefficients[nn] = sum;
   }
}


// Compute the Stormer-Cowell corrector coefficients.
GaussJacksonRationalCoefficients
GaussJacksonRationalCoefficients::construct_stormer_cowell_corrector ()
const
{
   // Create a set of unpopulated rational coefficients sized the same as this.
   GaussJacksonRationalCoefficients result;
   unsigned int nelem = coefficients.size();
   result.coefficients.resize (nelem);

   // Populate the result using
   //   q_i = \sum_{k=0}^i c_k c_{i-k}
   // See Berry eqn 2.55 for details.
   for (unsigned int ii = 0; ii < nelem; ++ii) {
      er7_utils::Ratio128 sum = 0;
      for (unsigned int kk = 0; kk <= ii; ++kk) {
         sum += coefficients[kk]*coefficients[ii-kk];
      }
      result.coefficients[ii] = sum;
   }
   return result;
}


// Compute predictor coefficients.
// Note: This calculation pertains to Adams and Stormer-Cowell coefficients.
GaussJacksonRationalCoefficients
GaussJacksonRationalCoefficients::construct_predictor ()
const
{
   // Create a set of unpopulated rational coefficients sized the same as this.
   GaussJacksonRationalCoefficients result;
   unsigned int nelem = coefficients.size();
   result.coefficients.resize (nelem);

   // Populate the result using
   //   \gamma_i = \sum_{k=0}^i c_k
   // See Berry eqn 2.43 for details.
   // Note: This calculation pertains to Adams and Stormer-Cowell coefficients.
   er7_utils::Ratio128 sum = 0;
   for (unsigned int ii = 0; ii < nelem; ++ii) {
      sum += coefficients[ii];
      result.coefficients[ii] = sum;
   }
   return result;
}


// Convert the coefficients to ordinate form.
// Note: This assumes that the correct number of leading terms have been
// discarded from the front of the coefficients vector.
void
GaussJacksonRationalCoefficients::convert_to_ordinate_form (
   er7_utils::NChooseM & n_choose_m,
   double * result)
const
{
   // Create a set of unpopulated rational coefficients sized the same as this.
   std::vector<er7_utils::Ratio128> rational_coefs;
   unsigned int nelem = coefficients.size();
   rational_coefs.resize (nelem);

   // Populate the result using
   //   z_{Nm} = (-1)^m \sum_{i=m}^N z'_i \binom i m
   // See Berry eqn 2.79 for details.
   for (unsigned int mm = 0; mm < nelem; ++mm) {
      er7_utils::Ratio128 sum = 0;
      for (unsigned int ii = mm; ii < nelem; ++ii) {
         sum += n_choose_m(ii, mm) * coefficients[ii];
      }
      if ((mm % 2) != 0) {
         sum = -sum;
      }

      // Store in reverse order to simplify calculations.
      rational_coefs[nelem-1-mm] = sum;
   }

   // Copy the computed coefficient to the result.
   // Note that copy converts the rational coefficients to doubles.
   std::copy (rational_coefs.begin(), rational_coefs.end(), result);
}


// Discard the specified number of coefficients from the front and back.
void
GaussJacksonRationalCoefficients::discard_extra_terms (
   unsigned int nfront,
   unsigned int nback)
{
   // We should be discarding two elements total, two from the front for
   // Stormer-Cowell, one each from the front and back for Adams.
   assert (nfront + nback == 2);

   coefficients.erase (coefficients.begin(), coefficients.begin()+nfront);
   coefficients.erase (coefficients.end()-nback, coefficients.end());
}


// Apply the operator (1-\nabla) to the coefficients.
// Note: This assumes that the correct number of leading terms have been
// discarded from the front of the coefficients vector.
void
GaussJacksonRationalCoefficients::displace_back ()
{
   // Create a set of unpopulated rational coefficients sized the same as this.
   std::vector<er7_utils::Ratio128> new_coefficients;
   unsigned int nelem = coefficients.size();
   new_coefficients.resize (nelem);

   // Apply (1-\nabla): c'_0 = c_0, c'_i = c_i - c_{i-1}
   new_coefficients[0] = coefficients[0];
   for (unsigned int ii = 1; ii < nelem; ++ii) {
      new_coefficients[ii] = coefficients[ii] - coefficients[ii-1];
   }

   // Replace the current coefficients with the new ones.
   coefficients = new_coefficients;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 * @}
 */
