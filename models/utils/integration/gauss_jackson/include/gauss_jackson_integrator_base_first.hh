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
 * @file models/utils/integration/gauss_jackson/include/gauss_jackson_integrator_base_first.hh
 * Defines the template specializations of GaussJacksonIntegratorBase
 * for a first order ODE.
 */


/*
Purpose: ()
*/

#ifndef JEOD_GAUSS_JACKSON_INTEGRATOR_BASE_FIRST_HH
#define JEOD_GAUSS_JACKSON_INTEGRATOR_BASE_FIRST_HH

// Local includes
#include "gauss_jackson_integrator_base.hh"
#include "gauss_jackson_one_state.hh"

// ER7 utilities includes
#include "er7_utils/integration/core/include/first_order_ode_integrator.hh"
#include "er7_utils/integration/core/include/integrator_constructor.hh"
#include "er7_utils/interface/include/alloc.hh"

// System includes
#include <algorithm>
#include <cmath>


//! Namespace jeod 
namespace jeod {

#ifndef TRICK_ICG  // Grrr.
/**
 * Alias for a first order Gauss Jackson integrator.
 */
typedef GaussJacksonIntegratorBase <
      GaussJacksonOneState,
      er7_utils::FirstOrderODEIntegrator>
      GaussJacksonIntegratorBaseFirst;
#else
#define GaussJacksonIntegratorBaseFirst \
      GaussJacksonIntegratorBase < \
      GaussJacksonOneState, \
      er7_utils::FirstOrderODEIntegrator>
#endif


/**
 * Create the priming integrator.
 */
template<>
inline er7_utils::FirstOrderODEIntegrator*
GaussJacksonIntegratorBase <
GaussJacksonOneState,
er7_utils::FirstOrderODEIntegrator>::create_primer (
   const er7_utils::IntegratorConstructor & priming_constructor,
   unsigned int size,
   er7_utils::IntegrationControls & priming_controls)
{
   return priming_constructor.create_first_order_ode_integrator (
             size, priming_controls);
}


/**
 * Replicate the priming integrator.
 */
template<>
inline er7_utils::FirstOrderODEIntegrator*
GaussJacksonIntegratorBase <
GaussJacksonOneState,
er7_utils::FirstOrderODEIntegrator>::replicate_primer (
   const er7_utils::FirstOrderODEIntegrator * src)
{
   if (src == NULL) {
      return NULL;
   }
   else {
      return src->create_copy();
   }
}


/**
 * Integrate with the primer.
 */
template<>
inline er7_utils::IntegratorResult
GaussJacksonIntegratorBase <
GaussJacksonOneState,
er7_utils::FirstOrderODEIntegrator>::integrate_primer (
   double dyn_dt,
   unsigned int target_stage,
   double const * deriv,
   GaussJacksonOneState & state)
{
   return primer->integrate (dyn_dt, target_stage, deriv, state.first);
}


/**
 * Save epoch data.
 */
template<>
inline void
GaussJacksonIntegratorBase <
GaussJacksonOneState,
er7_utils::FirstOrderODEIntegrator>::save_epoch_data (
   const double * acc,
   const GaussJacksonOneState & state)
{
   er7_utils::integ_utils::copy_array (state.first, size, init_state.first);
   er7_utils::integ_utils::two_state_copy_array (
      state.first, acc, size, pos_hist[0], acc_hist[0]);
}


/**
 * Save comparison data.
 */
template<>
inline void
GaussJacksonIntegratorBase <
GaussJacksonOneState,
er7_utils::FirstOrderODEIntegrator>::save_comparison_data (
   const GaussJacksonOneState & state,
   double * pos_hist_elem)
{
   er7_utils::integ_utils::copy_array (state.first, size, pos_hist_elem);
}


/**
 * Initialize the integration constants (i.e., delinv).
 */
template<>
inline void
GaussJacksonIntegratorBase <
GaussJacksonOneState,
er7_utils::FirstOrderODEIntegrator>::initialize_edit_integration_constants (
   double dt)
{
   coeff->corrector[0].apply (size, order+1, acc_hist, delinv);

   for (unsigned int ii = 0; ii < size; ++ii) {
      delinv.first[ii]  = init_state.first[ii] / dt - delinv.first[ii];
   }
}


/**
 * Advance the integration constants by one cycle.
 */
template<>
inline void
GaussJacksonIntegratorBase <
GaussJacksonOneState,
er7_utils::FirstOrderODEIntegrator>::advance_edit_integration_constants (
   unsigned int index)
{
   double* ER7_UTILS_RESTRICT first_dinv = delinv.first;
   double* ER7_UTILS_RESTRICT ahist_idx = acc_hist[index];

   for (unsigned int ii = 0; ii < size; ++ii) {
      first_dinv[ii] += ahist_idx[ii];
   }
}


/**
 * Initialize the integration constants (i.e., delinv).
 */
template<>
inline void
GaussJacksonIntegratorBase <
GaussJacksonOneState,
er7_utils::FirstOrderODEIntegrator>::initialize_predictor_integration_constants (
   double dt)
{
   initialize_edit_integration_constants (dt);

   for (unsigned int ii = 1; ii < order; ++ii) {
      advance_edit_integration_constants (ii);
   }
}


/**
 * Advance the integration constants by one cycle.
 */
template<>
inline void
GaussJacksonIntegratorBase <
GaussJacksonOneState,
er7_utils::FirstOrderODEIntegrator>::advance_predictor_integration_constants (
   unsigned int index)
{
   double* ER7_UTILS_RESTRICT first_dinv = delinv.first;
   double* ER7_UTILS_RESTRICT ahist_idx = acc_hist[index];

   for (unsigned int ii = 0; ii < size; ++ii) {
      first_dinv[ii] += ahist_idx[ii];
   }
}


/**
 * Apply a mid-corrector.
 */
template<>
inline void
GaussJacksonIntegratorBase <
GaussJacksonOneState,
er7_utils::FirstOrderODEIntegrator>::mid_correct (
   unsigned int coeff_idx,
   double dt,
   GaussJacksonOneState & state)
{
   double* ER7_UTILS_RESTRICT first_dinv = delinv.first;
   double* ER7_UTILS_RESTRICT first_state = state.first;

   coeff->corrector[coeff_idx].apply (size, order+1, acc_hist, state);

   for (unsigned int ii = 0; ii < size; ++ii) {
      first_state[ii] = dt * (first_dinv[ii] + first_state[ii]);
   }
}


/**
 * Apply the predictor.
 */
template<>
inline void
GaussJacksonIntegratorBase <
GaussJacksonOneState,
er7_utils::FirstOrderODEIntegrator>::predict (
   double dt,
   double const * const * ahist,
   GaussJacksonOneState & state)
{
   double* ER7_UTILS_RESTRICT first_dinv = delinv.first;
   double* ER7_UTILS_RESTRICT first_state = state.first;

   coeff->predictor.apply (size, order+1, ahist, state);

   for (unsigned int ii = 0; ii < size; ++ii) {
      first_state[ii] = dt * (first_dinv[ii] + first_state[ii]);
   }
}


/**
 * Apply the corrector.
 */
template<>
inline void
GaussJacksonIntegratorBase <
GaussJacksonOneState,
er7_utils::FirstOrderODEIntegrator>::correct (
   double dt,
   const double* ER7_UTILS_RESTRICT acc,
   GaussJacksonOneState & state)
{
   double vfact = velocity_corrector;
   double* ER7_UTILS_RESTRICT first_csum = corrector_sum.first;
   double* ER7_UTILS_RESTRICT first_dinv = delinv.first;
   double* ER7_UTILS_RESTRICT first_state = state.first;

   for (unsigned int ii = 0; ii < size; ++ii) {
      double temp = first_csum[ii] + vfact*acc[ii];
      first_state[ii] = dt * (first_dinv[ii] + temp);
   }
}


/**
 * Test for convergence.
 */
template<>
inline bool
GaussJacksonIntegratorBase <
GaussJacksonOneState,
er7_utils::FirstOrderODEIntegrator>::test_for_convergence (
   const GaussJacksonOneState & state,
   double* ER7_UTILS_RESTRICT hist_data)
{
   const double* ER7_UTILS_RESTRICT new_data = state.first;

   bool passed = true;
   for (unsigned int ii = 0; ii < size; ++ii) {
      double error = std::abs(new_data[ii] - hist_data[ii]);
      if ((error > absolute_tolerance) &&
          (error > relative_tolerance*std::abs(new_data[ii]))) {
         passed = false;
      }
      hist_data[ii] = new_data[ii];
   }
   return passed;
}


/**
 * Swap state data with another of the same.
 */
template<>
inline void
GaussJacksonIntegratorBase <
GaussJacksonOneState,
er7_utils::FirstOrderODEIntegrator>::swap_state (
   GaussJacksonOneState & item,
   GaussJacksonOneState & other_item)
{
   std::swap (item.first, other_item.first);
}


/**
 * Replicate state data.
 */
template<>
inline void
GaussJacksonIntegratorBase <
GaussJacksonOneState,
er7_utils::FirstOrderODEIntegrator>::replicate_state (
   GaussJacksonOneState const & source,
   GaussJacksonOneState & target)
{
   target.first = er7_utils::alloc::replicate_array (size, source.first);
}


/**
 * Allocate memory for a state item.
 */
template<>
inline void
GaussJacksonIntegratorBase <
GaussJacksonOneState,
er7_utils::FirstOrderODEIntegrator>::allocate_state_contents (
   GaussJacksonOneState & item)
{
   item.first = er7_utils::alloc::allocate_array<double> (size);
}


/**
 * Deallocate state item memory.
 */
template<>
inline void
GaussJacksonIntegratorBase <
GaussJacksonOneState,
er7_utils::FirstOrderODEIntegrator>::deallocate_state_contents (
   GaussJacksonOneState & item)
{
   er7_utils::alloc::deallocate_array (item.first);
}


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
