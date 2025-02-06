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
 * @file models/utils/integration/gauss_jackson/src/gauss_jackson_coeffs.cc
 * Defines member functions for the class GaussJacksonCoeffs.
 */


/*
Purpose: ()
Library dependencies:
  ((gauss_jackson_coeffs.o)
   (gauss_jackson_rational_coeffs.o))
*/


// Local includes
#include "../include/gauss_jackson_coeffs.hh"
#include "../include/gauss_jackson_coefficients_pair.hh"
#include "../include/gauss_jackson_rational_coeffs.hh"

// ER7 utilities includes
#include "er7_utils/math/include/n_choose_m.hh"
#include "er7_utils/interface/include/alloc.hh"

// System includes
#include <algorithm>
// #include <cmath>
#include <cassert>
// #include <cstdio>
#include <iostream>


//! Namespace jeod 
namespace jeod {

// Destructor.
GaussJacksonCoeffs::~GaussJacksonCoeffs ()
{
   er7_utils::alloc::deallocate_array (corrector);
}


// Swap.
void
GaussJacksonCoeffs::swap (
   GaussJacksonCoeffs & src)
{
   predictor.swap (src.predictor);
   std::swap (corrector, src.corrector);
   std::swap (max_order, src.max_order);
   std::swap (order, src.order);
}


// Size (but do not compute) the coefficients.
void
GaussJacksonCoeffs::configure (
   unsigned int max_order_in)
{
   max_order = max_order_in;
   order = 0;

   predictor.configure (max_order);

   er7_utils::alloc::deallocate_array (corrector);
   corrector = er7_utils::alloc::allocate_array<GaussJacksonCoefficientsPair> (
         max_order+1);

   for (unsigned int ii = 0; ii <= max_order; ++ii) {
      corrector[ii].configure (max_order);
   }
}


// Compute the coefficients.
void
GaussJacksonCoeffs::compute_coeffs (
   unsigned int order_in)
{
   assert (order_in <= max_order);
   order = order_in;

   // Compute the non-summed Adams corrector coefficients.
   // For now there's an extra coefficient at the end, needed for the
   // Stormer-Cowell coefficients.
   GaussJacksonRationalCoefficients ac;
   ac.configure_adams_corrector (order+3);

   // Compute the non-summed Stormer-Cowell corrector coefficients.
   GaussJacksonRationalCoefficients sc =
      ac.construct_stormer_cowell_corrector ();

   // Compute the non-summed predictor coefficients.
   GaussJacksonRationalCoefficients ap = ac.construct_predictor();
   GaussJacksonRationalCoefficients sp = sc.construct_predictor();

   // Get rid of the extra coefficients.
   // The Adams coefficients contain one extra on each side,
   // the Stormer-Cowell, two extra leading coefficients.
   ac.discard_extra_terms (1, 1);
   ap.discard_extra_terms (1, 1);
   sc.discard_extra_terms (2, 0);
   sp.discard_extra_terms (2, 0);


   // Convert the coefficients to ordinate form.
   er7_utils::NChooseM n_choose_m;

   // print_array (std::cout, ap.coefficients, "ap");
   ap.convert_to_ordinate_form (n_choose_m, predictor.sa_coefs);

   // print_array (std::cout, sp.coefficients, "sp");
   sp.convert_to_ordinate_form (n_choose_m, predictor.gj_coefs);

   // print_array (std::cout, ac.coefficients, "ac", order);
   ac.convert_to_ordinate_form (n_choose_m, corrector[order].sa_coefs);

   // print_array (std::cout, sc.coefficients, "sc", order);
   sc.convert_to_ordinate_form (n_choose_m, corrector[order].gj_coefs);

   for (unsigned int ii = 1; ii <= order; ++ii) {
      ac.displace_back ();
      // print_array (std::cout, ac.coefficients, "ac", ii);
      ac.convert_to_ordinate_form (n_choose_m, corrector[order-ii].sa_coefs);

      sc.displace_back ();
      // print_array (std::cout, sc.coefficients, "sc", ii);
      sc.convert_to_ordinate_form (n_choose_m, corrector[order-ii].gj_coefs);
   }
   // std::cout << *this << "\n";
}


// Print the coefficients.
std::ostream & operator<< (
   std::ostream& stream,
   const GaussJacksonCoeffs& coeff)
{
   stream << "predictor\n";
   coeff.predictor.print (coeff.order, stream);
   for (unsigned int ii = 0; ii <= coeff.order; ++ii) {
      stream << "corrector[" << ii << "]\n";
      coeff.corrector[ii].print (coeff.order, stream);
   }

   return stream;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 * @}
 */
