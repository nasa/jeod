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
 * @file models/utils/integration/gauss_jackson/include/gauss_jackson_integrator_base_second.hh
 * Defines the template specializations of GaussJacksonIntegratorBase
 * for a first order ODE.
 */


/*
Purpose: ()
*/

#ifndef JEOD_GAUSS_JACKSON_INTEGRATOR_BASE_SECOND_HH
#define JEOD_GAUSS_JACKSON_INTEGRATOR_BASE_SECOND_HH

// Local includes
#include "gauss_jackson_integrator_base.hh"
#include "gauss_jackson_two_state.hh"

// ER7 utilities includes
#include "er7_utils/integration/core/include/second_order_ode_integrator.hh"
#include "er7_utils/integration/core/include/integrator_constructor.hh"
#include "er7_utils/interface/include/alloc.hh"

// System includes
#include <algorithm>
#include <cmath>


//! Namespace jeod 
namespace jeod {

#ifndef TRICK_ICG  // Grrr.
/**
 * Alias for a second order Gauss Jackson integrator.
 */
typedef GaussJacksonIntegratorBase <
      GaussJacksonTwoState,
      er7_utils::SecondOrderODEIntegrator>
      GaussJacksonIntegratorBaseSecond;
#else
#define GaussJacksonIntegratorBaseSecond \
      GaussJacksonIntegratorBase < \
      GaussJacksonTwoState, \
      er7_utils::SecondOrderODEIntegrator>
#endif


/**
 */
template<>
inline er7_utils::SecondOrderODEIntegrator*
GaussJacksonIntegratorBase <
GaussJacksonTwoState,
er7_utils::SecondOrderODEIntegrator>::create_primer (
   const er7_utils::IntegratorConstructor & priming_constructor,
   unsigned int size,
   er7_utils::IntegrationControls & priming_controls)
{
   return priming_constructor.create_second_order_ode_integrator (
             size, priming_controls);
}


/**
 */
template<>
inline er7_utils::SecondOrderODEIntegrator*
GaussJacksonIntegratorBase <
GaussJacksonTwoState,
er7_utils::SecondOrderODEIntegrator>::replicate_primer (
   const er7_utils::SecondOrderODEIntegrator * src)
{
   if (src == nullptr) {
      return nullptr;
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
GaussJacksonTwoState,
er7_utils::SecondOrderODEIntegrator>::integrate_primer (
   double dyn_dt,
   unsigned int target_stage,
   double const * deriv,
   GaussJacksonTwoState & state)
{
   return primer->integrate (dyn_dt, target_stage, deriv,
                             state.first, state.second);
}


/**
 * Save epoch data.
 */
template<>
inline void
GaussJacksonIntegratorBase <
GaussJacksonTwoState,
er7_utils::SecondOrderODEIntegrator>::save_epoch_data (
   const double* ER7_UTILS_RESTRICT acc,
   const GaussJacksonTwoState & state)
{
   er7_utils::integ_utils::two_state_copy_array (
      state.first, state.second, size, init_state.first, init_state.second);
   er7_utils::integ_utils::two_state_copy_array (
      state.second, acc, size, pos_hist[0], acc_hist[0]);
}

/**
 * Save comparison data.
 */
template<>
inline void
GaussJacksonIntegratorBase <
GaussJacksonTwoState,
er7_utils::SecondOrderODEIntegrator>::save_comparison_data (
   const GaussJacksonTwoState & state,
   double* ER7_UTILS_RESTRICT pos_hist_elem)
{
   er7_utils::integ_utils::copy_array (state.second, size, pos_hist_elem);
}

/**
 * Initialize the integration constants (i.e., delinv).
 */
template<>
inline void
GaussJacksonIntegratorBase <
GaussJacksonTwoState,
er7_utils::SecondOrderODEIntegrator>::initialize_edit_integration_constants (
   double dt)
{
   double dtsq = dt*dt;

   coeff->corrector[0].apply (size, order+1, acc_hist, delinv);

   for (unsigned int ii = 0; ii < size; ++ii) {
      delinv.first[ii]  = init_state.first[ii] / dt - delinv.first[ii];
      delinv.second[ii] = init_state.second[ii] / dtsq - delinv.second[ii];
   }
}

/**
 * Advance the integration constants by one cycle.
 */
template<>
inline void
GaussJacksonIntegratorBase <
GaussJacksonTwoState,
er7_utils::SecondOrderODEIntegrator>::advance_edit_integration_constants (
   unsigned int index)
{
   for (unsigned int ii = 0; ii < size; ++ii) {
      delinv.second[ii] += delinv.first[ii];
      delinv.first[ii]  += acc_hist[index][ii];
   }
}

/**
 * Initialize the integration constants (i.e., delinv).
 */
template<>
inline void
GaussJacksonIntegratorBase <
GaussJacksonTwoState,
er7_utils::SecondOrderODEIntegrator>::initialize_predictor_integration_constants (
   double dt)
{
   initialize_edit_integration_constants (dt);

   for (unsigned int ii = 1; ii < order; ++ii) {
      advance_edit_integration_constants (ii);
   }

   for (unsigned int ii = 0; ii < size; ++ii) {
      delinv.second[ii] += delinv.first[ii];
   }
}

/**
 * Advance the integration constants by one cycle.
 */
template<>
inline void
GaussJacksonIntegratorBase <
GaussJacksonTwoState,
er7_utils::SecondOrderODEIntegrator>::advance_predictor_integration_constants (
   unsigned int index)
{
   for (unsigned int ii = 0; ii < size; ++ii) {
      delinv.first[ii]  += acc_hist[index][ii];
      delinv.second[ii] += delinv.first[ii];
   }
}

/**
 * Apply a mid-corrector.
 */
template<>
inline void
GaussJacksonIntegratorBase <
GaussJacksonTwoState,
er7_utils::SecondOrderODEIntegrator>::mid_correct (
   unsigned int coeff_idx,
   double dt,
   GaussJacksonTwoState & state)
{
   double dtsq = dt*dt;

   coeff->corrector[coeff_idx].apply (size, order+1, acc_hist, state);

   for (unsigned int ii = 0; ii < size; ++ii) {
      state.first[ii] = dt * (delinv.first[ii] + state.first[ii]);
      state.second[ii] = dtsq * (delinv.second[ii] + state.second[ii]);
   }
}

/**
 * Apply the predictor.
 */
template<>
inline void
GaussJacksonIntegratorBase <
GaussJacksonTwoState,
er7_utils::SecondOrderODEIntegrator>::predict (
   double dt,
   double const * const * ER7_UTILS_RESTRICT ahist,
   GaussJacksonTwoState & state)
{
   double dtsq = dt*dt;
   double* ER7_UTILS_RESTRICT first_dinv = delinv.first;
   double* ER7_UTILS_RESTRICT second_dinv = delinv.second;
   double* ER7_UTILS_RESTRICT velocity = state.first;
   double* ER7_UTILS_RESTRICT position = state.second;

   coeff->predictor.apply (size, order+1, ahist, state);

   for (unsigned int ii = 0; ii < size; ++ii) {
      velocity[ii] = dt * (first_dinv[ii] + velocity[ii]);
      position[ii] = dtsq * (second_dinv[ii] + position[ii]);
   }
}

/**
 * Apply the corrector.
 */
template<>
inline void
GaussJacksonIntegratorBase <
GaussJacksonTwoState,
er7_utils::SecondOrderODEIntegrator>::correct (
   double dt,
   const double* ER7_UTILS_RESTRICT acc,
   GaussJacksonTwoState & state)
{
   double dtsq = dt*dt;
   double* ER7_UTILS_RESTRICT first_csum = corrector_sum.first;
   double* ER7_UTILS_RESTRICT second_csum = corrector_sum.second;
   double* ER7_UTILS_RESTRICT first_dinv = delinv.first;
   double* ER7_UTILS_RESTRICT second_dinv = delinv.second;
   double* ER7_UTILS_RESTRICT velocity = state.first;
   double* ER7_UTILS_RESTRICT position = state.second;
   double vfact = velocity_corrector;
   double pfact = position_corrector;

   for (unsigned int ii = 0; ii < size; ++ii) {

      double temp = first_csum[ii] + vfact*acc[ii];
      velocity[ii] = dt * (first_dinv[ii] + temp);

      temp = second_csum[ii] + pfact*acc[ii];
      position[ii] = dtsq * (second_dinv[ii] + temp);
   }
}

/**
 * Test for convergence.
 */
template<>
inline bool
GaussJacksonIntegratorBase <
GaussJacksonTwoState,
er7_utils::SecondOrderODEIntegrator>::test_for_convergence (
   const GaussJacksonTwoState & state,
   double* ER7_UTILS_RESTRICT hist_data)
{
   const double* ER7_UTILS_RESTRICT new_data = state.second;
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
GaussJacksonTwoState,
er7_utils::SecondOrderODEIntegrator>::swap_state (
   GaussJacksonTwoState & item,
   GaussJacksonTwoState & other_item)
{
   std::swap (item.first, other_item.first);
   std::swap (item.second, other_item.second);
}

/**
 * Replicate state data.
 */
template<>
inline void
GaussJacksonIntegratorBase <
GaussJacksonTwoState,
er7_utils::SecondOrderODEIntegrator>::replicate_state (
   GaussJacksonTwoState const & source,
   GaussJacksonTwoState & target)
{
   target.first = er7_utils::alloc::replicate_array (size, source.first);
   target.second = er7_utils::alloc::replicate_array (size, source.second);
}

/**
 * Allocate memory for a state item.
 */
template<>
inline void
GaussJacksonIntegratorBase <
GaussJacksonTwoState,
er7_utils::SecondOrderODEIntegrator>::allocate_state_contents (
   GaussJacksonTwoState & item)
{
   item.first = er7_utils::alloc::allocate_array<double> (size);
   item.second = er7_utils::alloc::allocate_array<double> (size);
}

/**
 * Deallocate state item memory.
 */
template<>
inline void
GaussJacksonIntegratorBase <
GaussJacksonTwoState,
er7_utils::SecondOrderODEIntegrator>::deallocate_state_contents (
   GaussJacksonTwoState & item)
{
   er7_utils::alloc::deallocate_array (item.first);
   er7_utils::alloc::deallocate_array (item.second);
}


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
