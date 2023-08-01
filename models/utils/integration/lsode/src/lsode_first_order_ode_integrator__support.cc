/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Integration
 * @{
 * @addtogroup Lsode
 * @{
 *
 * @file models/utils/integration/lsode/src/lsode_first_order_ode_integrator__support.cc
 * Define member functions for the class LsodeFirstOrderODEIntegrator.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((dlsode.f)))

Assumptions and limitations:
  (TBS)

Library dependencies:
  ((lsode_first_order_ode_integrator__manager.cc))



*******************************************************************************/

// System includes
#include <math.h>  //sqrt
#include <cmath> //std

// Interface includes
#include "er7_utils/interface/include/alloc.hh"

// Integration includes
#include "er7_utils/integration/core/include/integ_utils.hh"
#include "er7_utils/interface/include/message_handler.hh"
#include "er7_utils/integration/core/include/integration_messages.hh"
#include "er7_utils/integration/core/include/integration_controls.hh"

/* JEOD includes */
#include "utils/math/include/numerical.hh"

// Model includes
#include "../include/lsode_first_order_ode_integrator.hh"

using namespace jeod;

/**
 * Identify the smallest double precision value, epsilon, such that the
 * computer can distinguish (1+epsilon) from 1
 */
void
LsodeFirstOrderODEIntegrator::calculate_epsilon()
{
   epsilon = 1.0;
   //10
   while (!Numerical::compare_exact((1.0+epsilon),1.0)) {
      epsilon /= 2.0;
   }
   epsilon *= 2.0;
   sqrt_epsilon = std::sqrt(epsilon);
   return;
}


/**
 * Modified from original DCFODE subroutine.
 * calculate_integration_coefficients is called by dstode to set coefficients
 * needed there.
 * The coefficients for the current method, as given by the value of
 * integration_method, are set for all orders and saved.
 * The maximum order assumed here is 12 if integration_method =
 * ImplicitAdamsNonStiff and 5 if integration_method = ImplicitBackDiffStiff.
 *
 * NOTE - A smaller value of the maximum order is also allowed and may be set by
 * the user with the value control_data.max_order, which gets copied to the
 * protected value max_order_internal.
 *
 * calculate_integration_coefficients is called once at the beginning of the
 * problem, and again only if integration_method is changed.
 *
 * The coefficients are stored in two arrays:
 * method_coeffs_complete is a 13x12 array that contains a complete set of
 * coefficients for the method
 * test_coeffs_complete is a 3x12 array that contains the coefficients for local
 *                      error tests and selection of the step size and/or order.
 *               The 1st set of 12 coeffs is for order method_order_current - 1
 *               The 2nd set of 12 coeffs is for order method_order_current
 *               The 3rd set of 12 coeffs is for order method_order_current + 1
 *
 * The coefficients in method_coeffs_complete are computed by a genetrating
 * polynomial.  For a given order (note that order changes during the integration
 * process up to the maximum allowable, and is identified in the integrator as the
 * variable method_order_current), abbreviate
 *      method_coeffs_complete[i][order-1] to mcc[i]
 *
 * Then
 *   l(x) = mcc[0] + (mcc[1] * x) + (mcc[2] * x^2) + ... + mcc[order] * (x^order)
 * For the implicit Adams methods, l(x) is given by
 *     dl/dx = (x+1)*(x+2)*...*(x+order-1)/(order-1)!,    l(-1) = 0.;
 * For the BDF methods, l(x) is given by
 *     l(x) = (x+1)*(x+2)* ... *(x+order) / ((order)! * (1 + 1/2 + ... + 1/order))
 *
 * (! represents factorial)
 *
 * Note that while method_coeffs_compelte is a rectangular array for convenience,
 * it is effectively a triangular array since method_coeffs_complete[i][order-1]
 * has no meaning for i >= ( order + 2)
 * order=1:  method_coeffs_complete[0-1][0]
 * order=2:  method_coeffs_complete[0-2][1]
 * ...
 * order=12: method_coeffs_complete[0-12][11]
 * Hence a 13x12 array.
 */
void
LsodeFirstOrderODEIntegrator::calculate_integration_coefficients()
{
   // NOTE poly_coeff is only a 12-array in the original fortran
   //      However, since:
   //      1.  indices in Fortran are 1-12, not 0-11, and
   //      2.  the index is integral to the calculation, and
   //      3.  I don't have time to refactor and validate,
   //      I'm going with a 13 array so that I can still have indices 1-12,
   //      and throw away index 0.
   double poly_coeff[13];
   for (unsigned int ii = 0; ii < 13; ii++) {
      poly_coeff[ii] = 0;
   }
   double rqfac,rq1fac;

   switch (control_data.integration_method) {
   case LsodeControlDataInterface::ImplicitAdamsNonStiff:

      // 100
      //  Reducing index on m_c_c and t_c_c, but not on poly_coeff.
      method_coeffs_complete[0][0] = 1.0;
      method_coeffs_complete[1][0] = 1.0;
      test_coeffs_complete[0][0] = 0.0;
      test_coeffs_complete[1][0] = 2.0;
      test_coeffs_complete[0][1] = 1.0;
      test_coeffs_complete[2][11] = 0.0;
      poly_coeff[0] = 1.0;
      rqfac = 1.0;

      // NOTE - NQ is used here as the index, but it is NOT passed in,
      // and there is no common block in this method.  NQ is this context
      // does not appear to be associated with method_order_current.
      // Leaving it as nq.
      //
      for (int nq = 2; nq <= 12; nq++) { // do 140
         // The oly_coeff array will contain the coefficients of the polynomial
         //     p(x) = (x+1)*(x+2)*...*(x+nq-1).;
         // Initially, p(x) = 1.;
         rq1fac = rqfac;
         rqfac = rqfac/nq;

         // Form coefficients of p(x)*(x+nq-1). ------------------------
         poly_coeff[nq-1] = 0.0;
         // In the original for loop, i ran from ((nq+1)-1) = nq down to
         // ((nq+1)-(nq-1)) = 2.  We are reducing the index by 1, so nq-1
         // down to 1.
         for (int ii = nq-1; ii >= 1; ii--) { // do 110
            // 110
            poly_coeff[ii] = poly_coeff[ii-1] + (nq-1)*poly_coeff[ii];
         }
         poly_coeff[0] = (nq-1) * poly_coeff[0];

         // Compute integral, -1 to 0, of p(x) and x*p(x). ----------
         double pint = poly_coeff[0];
         double xpin = poly_coeff[0]/2.0;
         double tsign = 1.0;
         for (int ii = 2; ii <= nq; ii++) { // do 120
            tsign = -tsign;
            pint = pint + tsign*poly_coeff[ii-1]/ii;
            // 120
            xpin = xpin + tsign*poly_coeff[ii-1]/(ii+1);
         }
         // Store coefficients in method_coeffs_complete (ELCO) and
         //                       test_coeffs_complete (TESCO).
         method_coeffs_complete[0][nq-1] = pint*rq1fac;
         method_coeffs_complete[1][nq-1] = 1.0;
         for (int ii = 2; ii <= nq; ii++) { // do 130
            // 130
            method_coeffs_complete[ii][nq-1] = rq1fac*poly_coeff[ii-1]/ii;
         }
         double agamq = rqfac*xpin;
         double ragq = 1.0/agamq;
         test_coeffs_complete[1][nq-1] = ragq;
         if (nq < 12) {
           test_coeffs_complete[0][nq] = ragq*rqfac/(nq+1);
         }
         test_coeffs_complete[2][nq-2] = ragq;
       // 140
      }
      break;
   case LsodeControlDataInterface::ImplicitBackDiffStiff:
      // 200
      poly_coeff[0] = 1.0;
      rq1fac = 1.0;
      for (int nq = 1; nq <= 5; nq++) { // do 230
         int nqp1 = nq + 1;
         // Form coefficients of p(x)*(x+nq).
         poly_coeff[nq] = 0.0;

         for (int ii = nq; ii >= 1; ii--) { // do 210
            // 210
            poly_coeff[ii] = poly_coeff[ii-1] + nq*poly_coeff[ii];
         }
         poly_coeff[0] = nq*poly_coeff[1];
         // Store coefficients in method_coeffs_complete (ELCO) and
         //                       test_coeffs_complete (TESCO).
         for (int ii = 0; ii <= nq; ii++) { // do 220
            // 220
            method_coeffs_complete[ii][nq-1] = poly_coeff[ii]/poly_coeff[1];
         }
         method_coeffs_complete[1][nq-1] = 1.0;
         test_coeffs_complete[0][nq-1] = rq1fac;
         test_coeffs_complete[1][nq-1] = nqp1 /
                                               method_coeffs_complete[0][nq-1];
         test_coeffs_complete[2][nq-1] = (nq+2) /
                                               method_coeffs_complete[0][nq-1];
         rq1fac = rq1fac/nq;
      // 230
      }
      break;
   default:
      break;
   }
   return;
}




/**
 * Interpolates the zeroth-derivative of y
 * Adapted from subroutine DINTDY, which was a general method for interpolating
 * the K-th derivative of the dependent variable vector, y.
 *
 * Implementation notes -
 * DINTDY was called exclusively with three input values that always matched.
 *
 * 1.  Argument #1 T (time).  Calls to DINTDY always passed TOUT (now
 * cycle_target_time) in for T
 * 2.  Argument #2 K (order).  Calls to DINTDY always passed 0 in for K
 * 3.  Argument #5 DKY (value). DKY is the value that DINTDY interpolates.i
 * Calls to DINTDY always passed Y in for DKY.
 * This routine uses y directly in place of DKY, so can only interpolate the 0-th
 * derivative (since that is y), and always evaluates at cycle_target_time.
 * replaced accordingly.
 *
 * The computed values are gotten by interpolation using the Nordsieck history
 * array, arrays.history.
 * The formula for Y is:
 *
 *  Y[i]  =  sum {j=0 to method_order_current}  {
 *         (cycle_target_time - stage_target_time)^(j) *
 *         arrays.history[i-1][j] / h^j}
 */
void
LsodeFirstOrderODEIntegrator::interpolate_y()
{
   double tp =  100.0 * epsilon *
                (std::abs(stage_target_time) + std::abs(prev_good_step_size));
   if (prev_good_step_size > 0) {
      tp *= -1;
   }
   tp += stage_target_time - prev_good_step_size;

   if ( (cycle_target_time - tp) *
            (cycle_target_time - stage_target_time) > 0.0) {
      // 90
      er7_utils::MessageHandler::fail (__FILE__, __LINE__,
         er7_utils::IntegrationMessages::internal_error,
         "The cycle-target-time should be in the interval between \n"
         "(current-time - previous-good-step-size) and (current-time).\n"
         "It is not.\n"
         "cycle_target_time = %f \n"
         "stage_target_time = %f \n"
         "prev_good_step_size = %f \n"
         "tp = %f",cycle_target_time,stage_target_time,prev_good_step_size,tp);
       //FAILED.  NO CONTINUATION.
   }

   double s = (cycle_target_time - stage_target_time)/step_size;
   double c = 1.0;


   // 15
   for (unsigned int ii = 0; ii < control_data.num_odes; ii++) { // do 20
   // 20
      y[ii] = c * arrays.history[ii][num_nordsiek_cols-1];
   }
   if (method_order_current != 0) { // inverse go to 55 (K .EQ. NQ)
      for (unsigned int jb = 1; jb <= method_order_current; jb++) {
                                                                      // do 50
         int j = method_order_current - jb;
         // 35
         for (unsigned int ii = 0; ii < control_data.num_odes; ii++) { // do 40
            // 40
            y[ii] *= s;
            y[ii] += arrays.history[ii][j];
         }
         // 50
      }
      //return;
   }
   // 55 onwards is redundant for K=0.
   return;
}




/**
 * Modified from DPREPJ.
 * DPREPJ was called by DSTODE to compute and process the matrix
 * P = I - h*el(1)*J , where J is an approximation to the Jacobian.
 *
 * NOTES
 *
 * DPREPJ has been split into 3 parts, book-ended by the external-calls.
 * jacobian_prep_init contains the code that precededs the first external-call
 * jacobian_prep_loop contains the code that continues to loop according to the
 * limits as written is DPREPJ.
 * jacobian_prep_wrap_up contains the code that follows successful completion of
 * the looping section of DPREPJ.
 *
 * Note that the division in this implementation is not linear with that in the
 * original Fortran.  The external calls within the original fortran are embedded
 * within switch-blocks and for loops; the return points - to go to the next
 * routine will pick up from one of several locations in the Fortran code,
 * depending on the configuration at the time the external call was made.
 *
 * For the ER7_Utils / JEOD / Trick implementation, the derivative/jac calls must
 * be external to the integrate call, so we must fully back out and then reenter.
 * Rentry goes to jacobian_prep_loop.
 *
 * Some variables that were local have been moved to the class so that their value
 * is not lost in going from jacobian_prep_init to jacobian_prep_loop.
 * These are typically identified with ***_dprepj to indicate that their sole
 * purpose is within dprepj (the original name of the jacobian_prep_* routines).
 *
 * Here the jacobian is computed by the user-supplied routine JAC if
 * corrector_method = NewtonIterUserJac or NewtonIterUserBandJac, or by finite
 * differencing if corrector_method = NewtonIterInternalJac,
 * JacobiNewtonInternalJac, or NewtonIterInternalBandJac.
 *
 * If corrector_method = JacobiNewtonInternalJac, a diagonal approximation to the
 * Jacobian is used.
 * The Jacobian is stored in arrays lin_alg
 *
 * If corrector_method != JacobiNewtonInternalJac, P is subjected to
 * LU decomposition in preparation for later solution of linear systems with P as
 * coefficient matrix.
 * This is done by gauss_elim_factor (DGEFA) if
 *          corrector_method = NewtonIterUserJac or NewtonIterInternalJac,
 *       and by linear_solver (DGBFA) if
 *          corrector_method = NewtonIterUserBandJac or NewtonIterInternalBandJac.
 *
 * Note that the corrector_method using the banded Jacobians is not supported in
 * this release, so linear_solver is not used.
 *
 * FTEM and ACOR were effectively the same, now arrays.accum_correction.
 * SAVF is now arrays.save.
 * WM is now arrays.lin_alg
 */
void
LsodeFirstOrderODEIntegrator::jacobian_prep_init()
{
   double r;


   data_prepj.hl0 = step_size * method_coeff_first;
   data_prepj.index = 0;

   num_jacobian_evals ++;
   iteration_matrix_singular = false;
   jacobian_current = true;
   // Need to flag non-banded Jacobian methods from banded Jacobian methods,
   // i.e.  corrector_method = {NewtonIterUserJac,
   //                           NewtonIterInternalJac, and
   //                           JacobiNewtonInternalJac}
   //  separately from        {NewtonIterUserBandJac,
   //                          NewtonIterInternalBandJac}.

   switch (control_data.corrector_method) {
   case LsodeControlDataInterface::NewtonIterUserJac:
      // Prepare for calling the rest of jacobian_prep
      // 100
      // arrays.lin_alg is a  n*n matrix
      for (unsigned int ii = 0; ii < control_data.num_odes; ii++) { // do 110
         for (unsigned int jj = 0; jj < control_data.num_odes; jj++) { // do 110
            // 110
            arrays.lin_alg[ii][jj] = 0.0;
         }
      }
      break;  // return to exterior so that jac can be called.

   case LsodeControlDataInterface::NewtonIterInternalJac:
      // Need to make a series of calls to compute the derivatives in order
      // to approximate the Jacobian.  Set up the first call
      // 200
      // arrays.lin_alg is a  n*n matrix
      data_prepj.fac = magnitude_of_weighted_array ( arrays.save);
      data_prepj.r0 = 1000.0 * epsilon * std::abs(step_size) *
                                       control_data.num_odes * data_prepj.fac;
      if (std::fpclassify(data_prepj.r0) == FP_ZERO) {
          data_prepj.r0 = 1.0;
      }
      // SRUR = arrays.lin_alg_1, so just use arrays.lin_alg1;
      // Avoid the loop at this point, we are doing the first call only;
      // J=1
      data_prepj.index_max = control_data.num_odes;
      data_prepj.yj = y[0]; // jj = 0 this time around, this is first-time only.
      r = std::max( (arrays.lin_alg_1 * std::abs(data_prepj.yj)) ,
                    (data_prepj.r0 / arrays.error_weight[0]));
      y[0] += r;
      data_prepj.fac = -data_prepj.hl0/r;
      break;


   case LsodeControlDataInterface::JacobiNewtonInternalJac:
      // Construct a diagonal approximation to J and P. ---------;
      // 300
      // arrays.lin_alg is a  1*n matrix
      arrays.lin_alg_2 = data_prepj.hl0;
      r = method_coeff_first * 0.1; // FIXME magic number
      for (unsigned int ii = 0; ii < control_data.num_odes; ii++) { // do 310
         // 310
         y[ii] += r * (step_size * arrays.save[ii] - arrays.history[ii][2]);
      }
      break;

   case LsodeControlDataInterface::NewtonIterUserBandJac:
      // Option not currently supported.
      er7_utils::MessageHandler::fail (__FILE__, __LINE__,
         er7_utils::IntegrationMessages::invalid_request,
         "Corrector_method (MITER) 4 (Modified Newton iteration with"
         " user-supplied banded Jacobian) not supported.");
//      // 400
//      // arrays.lin_alg is a  (2*ml + mu *1)*n matrix
//      //jacobian_lower_half_bandwidth = iwm[1];
//      //jacobian_upper_half_bandwidth = iwm[2];
//      //jacobian_lower_half_bandwidth3 = jacobian_lower_half_bandwidth + 3;
//      mband = jacobian_lower_half_bandwidth + jacobian_upper_half_bandwidth + 1;
//      meband = mband + jacobian_lower_half_bandwidth;
//      for (int ii = 0; ii < meband; ii++) { // do 410
//         for (int jj = 0; jj < control_data.num_odes; jj++) { // do 410
//            // 410
//            arrays.lin_alg[ii][jj] = 0.0;
//         }
//      }
      break;

   case LsodeControlDataInterface::NewtonIterInternalBandJac:
      // Option not currently supported.
      er7_utils::MessageHandler::fail (__FILE__, __LINE__,
         er7_utils::IntegrationMessages::invalid_request,
         "Corrector_method (MITER) 5 (Modified Newton iteration with"
         " internally generated banded Jacobian) not supported.");
//      // 500
//      // arrays.lin_alg is a  (2*ml + mu + 1)*n matrix
//      //jacobian_lower_half_bandwidth = iwm[1];
//      //jacobian_upper_half_bandwidth = iwm[2];
//      mband = jacobian_lower_half_bandwidth + jacobian_upper_half_bandwidth + 1;
//
//      // mba = min (mband, control_data.num_odes):
//      mba = mband;
//      if (mba > control_data.num_odes) {
//         mba = control_data.num_odes;
//      }
//
//      meband = mband + jacobian_lower_half_bandwidth;
//      fac = magnitude_of_weighted_array (arrays.save);
//      r0 = 1000.0 * std::abs(step_size) * epsilon * control_data.num_odes * fac;
//      if (r0 == 0.0) {
//          r0 = 1.0;
//      }
//      data_prepj.r0=r0;
//      data_prepj.index_max = mba;
//      data_prepj.index = 0;
//
//      for (int j = 0; j < mba; j++) { // do 560
//      for (int ii = 0; ii < control_data.num_odes; ii+=mband) { // do 530
//         r = std::max( (arrays.lin_alg_1 * std::abs(y[ii])) ,
//                       (r0 / arrays.error_weight[ii]));
//         // 530
//         y[ii] = y[ii] + r;
//      }}
      break;
   case LsodeControlDataInterface::FunctionalIteration:
   default:
      break;
   } // end switch
   re_entry_point = JacobianPrep;
   return;
}


/***************************************************************************
jacobian_prep_loop
Purpose (Adapted from DPREPJ.  Gets called following the external function call
which follows the completion of dprej_init - formerly the first external call
from DPREPJ.  Continues to cycle back out to the exterior call as long as the
return value is false).
***************************************************************************/
bool
LsodeFirstOrderODEIntegrator::jacobian_prep_loop()
{
   int jj;

   switch (control_data.corrector_method) {

   case LsodeControlDataInterface::NewtonIterUserJac:
      // there is no loop in this case, go straight to wrap-up
      break;


   case LsodeControlDataInterface::NewtonIterInternalJac:
      jj = data_prepj.index;
      load_derivatives(arrays.accum_correction);
      for (unsigned int ii = 0; ii < control_data.num_odes; ii++) { // do 220
         // 220
         arrays.lin_alg[jj][ii] = (arrays.accum_correction[ii] -
                                            arrays.save[ii]) * data_prepj.fac;
      }
      y[jj] = data_prepj.yj;

      data_prepj.index++;
      if (data_prepj.index < data_prepj.index_max) { // refactor of DO 230
         // Prepare for next call to generate derivatives.
         jj = data_prepj.index;
         data_prepj.yj = y[jj];
         double r = std::max( (arrays.lin_alg_1 * std::abs(data_prepj.yj)) ,
                       (data_prepj.r0 / arrays.error_weight[jj]));
         y[jj] += r;
         data_prepj.fac = -data_prepj.hl0/r;
         return false; // re-cycle
      }
      break; // loop finished.

   case  LsodeControlDataInterface::JacobiNewtonInternalJac:
      // there is no loop in this case, go straight to wrap-up
      break;

//Unsupported cases:
//   case  LsodeControlDataInterface::NewtonIterUserBandJac:
//      // there is no loop in this case, go straight to wrap-up
//      break;

//
//
//   case   LsodeControlDataInterface::NewtonIterInternalBandJac:
//      load_derivatives(arrays.accum_correction);
//      for (int jj = data_prepj.index; jj < control_data.num_odes; jj+=mband) { // do 550
//         y[jj] = arrays.history[jj][0];
//         r = std::max( (arrays.lin_alg_1 * std::abs(y[jj])) ,
//                       (data_prepj.r0/arrays.error_weight[jj]));
//         fac = -data_prepj.hl0/r;
//         int i1 = std::max((jj+1)-jacobian_upper_half_bandwidth,1);
//
//         //int i2 = std::min((jj+1)+jacobian_lower_half_bandwidth,control_data.num_odes);
//         int i2 = (jj+1)+jacobian_lower_half_bandwidth;
//         if (i2 > control_data.num_odes) {
//            i2 = control_data.num_odes;
//         }
//
//         int ii = (jj+1)*(meband-1) - jacobian_lower_half_bandwidth + 2;
//         //FIXME  Reconstruct this from documentation.  it looks
//         //dodgy.  This is setting into arrays.lin_alg across data
//         //adresses that span the first index.
//         for (int i = i1-1; i < i2; i++) { // do 540
//            // 540
//            arrays.lin_alg[/*FIXME*/jj][i] = (arrays.accum_correction[i] - arrays.save[i]) * fac;
//         }
//      // 550
//      }
//
//      data_prepj.index++;
//      if (data_prepj.index < data_prepj.index_max) {
//         // Prepare for next call to generate derivatives.
//         for (int ii = data_prepj.index; ii < control_data.num_odes; ii+=mband) { // do 530
//            r = std::max( (arrays.lin_alg_1 * std::abs(y[ii])) ,
//                          (data_prepj.r0 / arrays.error_weight[ii]));
//            // 530
//            y[ii] = y[ii] + r;
//         }
//         return false; // re-cycle
//      }
//      break; // loop finished

   // 560
   case LsodeControlDataInterface::FunctionalIteration:
   case LsodeControlDataInterface::NewtonIterUserBandJac:
   case LsodeControlDataInterface::NewtonIterInternalBandJac:
   default:
      break;
   }
   return true; // loops finished.
}





/***************************************************************************
 * Wraps up the dprepj routine following completion of the loops.
***************************************************************************/
bool
LsodeFirstOrderODEIntegrator::jacobian_prep_wrap_up()
{
   switch (control_data.corrector_method) {
   case LsodeControlDataInterface::NewtonIterUserJac:
      for (unsigned int ii = 0; ii < control_data.num_odes; ii++) { // do 110
         for (unsigned int jj = 0; jj < control_data.num_odes; jj++) { // do 110
            // 120
            arrays.lin_alg[ii][jj] *= (-data_prepj.hl0);
         }
      }
      /* fall through */
   case LsodeControlDataInterface::NewtonIterInternalJac:
      //## Add identity matrix. ----------------------------------------------
      // 240
      for (unsigned int ii = 0; ii < control_data.num_odes; ii++) { // do 250
        arrays.lin_alg[ii][ii] += 1.0;
      }
   //## Do LU decomposition on P. --------------------------------------------
      if ( gauss_elim_factor() != 0) { // was DGEFA, returns IER,
          iteration_matrix_singular = true;
      }
      break;
   case LsodeControlDataInterface::JacobiNewtonInternalJac:
      load_derivatives(arrays.lin_alg[0]);
      for (unsigned int ii = 0; ii < control_data.num_odes; ii++) { // do 320
         double r0 = step_size*arrays.save[ii] - arrays.history[ii][2];
         double di = 0.1 * r0 - step_size*(arrays.lin_alg[0][ii] - arrays.save[ii]);
         arrays.lin_alg[0][ii] = 1.0;
         if (std::abs(r0) >= (epsilon / arrays.error_weight[ii])) {
                                                        // inverse go to 320;

            // CODE CHANGE!!!
            // this was a test of == 0.0, but it can get very close to 0 and
            // cause serious problems.
            //   di is computed as a difference of two values.  If those values
            //   agree to 1 part in 10^10 (arbitrary), we are going to call
            //   them equal, with the difference being a relative zero,
            //   which produces a near-singular matrix.
            if (std::abs(di) < 1E-11 * r0) {
               // 330
               iteration_matrix_singular = true;
               break; // out of for loop ... and thus out of case3
            }
            arrays.lin_alg[0][ii] = 0.1 * r0 / di;
         }
      // 320
      }
      break;

// Unsupported cases:
   case LsodeControlDataInterface::NewtonIterUserBandJac:
//      for (int ii = 0; ii < meband; ii++) { // do 420
//         for (int jj = 0; jj < control_data.num_odes; jj++) { // do 420
//            // 420
//            arrays.lin_alg[ii][jj] *= (-data_prepj.hl0);
//         }
//      }
//      // fall-through (go to 570)
//

   case LsodeControlDataInterface::NewtonIterInternalBandJac:
//      // 570
//      This code was so far from complete that it has been stripped.
//      If this option gets implemented at a later date, the code from
//      lines 570 - end must be re-implemented, including the DGBFA
//      method.
      er7_utils::MessageHandler::fail (__FILE__, __LINE__,
         er7_utils::IntegrationMessages::invalid_request,
         "Illegal corrector method requested, corrector_method = %d.",
         control_data.corrector_method);
      break;
   case LsodeControlDataInterface::FunctionalIteration:
        // No action, impossible case, FuncIter has no Jacobian.
      er7_utils::MessageHandler::error (__FILE__, __LINE__,
         er7_utils::IntegrationMessages::internal_error,
         "corrector method FunctionalIteration found in preparing the "
         "Jacobian.\nFuntional Iteration requires no Jacobian.\n");
      break;
   default:
      er7_utils::MessageHandler::fail (__FILE__, __LINE__,
         er7_utils::IntegrationMessages::invalid_request,
         "Unrecognized corrector method requested, corrector_method = %d.",
         control_data.corrector_method);
      break;
   }


   // This next code chunk comes from DSTODE, right after the call to
   // PJAC a little after line 230.  The only way to get to this little
   // nugget of code is to pass through the PJAC call, so it rightfully
   // belongs here.
   update_jacobian = false;
   rel_change_since_jacobian = 1.0;
   step_at_last_jacobian_update = num_steps_taken;
   convergence_rate = 0.7;
   if (iteration_matrix_singular) { // go to 430;
      integrator_corrector_failed_part2();
      return false; // Do not continue on to line 250
   }
   return true; // Continue on to line 250
}




/**
 * Modified from DSOLSY.  This routine manages the solution of the linear
 * system arising from a chord iteration.
 * It is called if corrector_method != FunctionalIteration.
 *
 * If corrector_method == NewtonIterUserJac || NewtonIterInternalJac,
 * it calls linear_solver (was DGESL).
 * If corrector_method = JacobiNewtonInternalJac it updates the coefficient
 *     hl0 = step_size * method_coeff_first (previously H*EL0) in the diagonal
 *     matrix, and then computes the solution.
 */
void
LsodeFirstOrderODEIntegrator::linear_chord_iteration()
{

   double hl0, phl0;

   modified_iteration_matrix_singular = false;
   switch (control_data.corrector_method) {
   case LsodeControlDataInterface::NewtonIterUserJac:
   case LsodeControlDataInterface::NewtonIterInternalJac:
      // 100
      linear_solver();
      break;
   case LsodeControlDataInterface::JacobiNewtonInternalJac:
      // 300
      phl0 = arrays.lin_alg_2;
      hl0 = step_size*method_coeff_first;
      arrays.lin_alg_2 = hl0;
      if (!Numerical::compare_exact(hl0,phl0)) {// go to 330;
         double r = hl0/phl0;
         for (unsigned int ii = 0; ii < control_data.num_odes; ii++) { // do 320
            double di = 1.0 - r*(1.0 - 1.0/arrays.lin_alg[0][ii]);
            if (std::fpclassify(di) == FP_ZERO) { // go to 390;
               // 390
               modified_iteration_matrix_singular = true;
               return;
            }
            // 320
            arrays.lin_alg[0][ii] = 1.0/di;
         }
      }
      // 330
      for (unsigned int ii = 0; ii < control_data.num_odes; ii++) { // do 340
         // 340
         y[ii] *= arrays.lin_alg[0][ii];
      }
      break;

// Not supported:
//   case LsodeControlDataInterface::NewtonIterUserBandJac:
//   case LsodeControlDataInterface::NewtonIterInternalBandJac:
//      // 400
//      //jacobian_lower_half_bandwidth = iwm[1];
//      //jacobian_upper_half_bandwidth = iwm[2];
//      //FIXME  Need to reconstruct to identify where the matrix starts
//      //and ends in wm in this case.
//      //meband = 2*jacobian_lower_half_bandwidth + jacobian_upper_half_bandwidth + 1;
//      dgbsl(); // THIS METHOD HAS NOT BEEN IMPLEMENTED.  IT MUST BE
//      IMPLEMENTED BEFORE THIS OPTION CAN BE PROVIDED.
//      return;
   case LsodeControlDataInterface::FunctionalIteration:
   case LsodeControlDataInterface::NewtonIterUserBandJac:
   case LsodeControlDataInterface::NewtonIterInternalBandJac:
   default:
      break;
   }
   return;
}



/**
 *
 */
void
LsodeFirstOrderODEIntegrator::load_ew_values()
{
   switch (control_data.error_control_indicator) {
   case LsodeControlDataInterface::CommonAbsCommonRel:
      for (unsigned int i = 0; i < control_data.num_odes; i++) { // do 15
         arrays.error_weight[i] = control_data.rel_tolerance_error_control[0] *
                                  std::abs(arrays.history[i][0]) +
                                  control_data.abs_tolerance_error_control[0];
      }
      break;
   case LsodeControlDataInterface::SpecificAbsCommonRel:
      for (unsigned int i = 0; i < control_data.num_odes; i++) { // do 25
         arrays.error_weight[i] = control_data.rel_tolerance_error_control[0] *
                                  std::abs(arrays.history[i][0]) +
                                  control_data.abs_tolerance_error_control[i];
      }
      break;
   case LsodeControlDataInterface::CommonAbsSpecificRel:
      for (unsigned int i = 0; i < control_data.num_odes; i++) { // do 35
         arrays.error_weight[i] = control_data.rel_tolerance_error_control[i] *
                                  std::abs(arrays.history[i][0]) +
                                  control_data.abs_tolerance_error_control[0];
      }
      break;
   case LsodeControlDataInterface::SpecificAbsSpecificRel:
      for (unsigned int i = 0; i < control_data.num_odes; i++) { // do 45
         arrays.error_weight[i] = control_data.rel_tolerance_error_control[i] *
                                  std::abs(arrays.history[i][0]) +
                                  control_data.abs_tolerance_error_control[i];
      }
      break;
   default:
      break;
   }
   return;
}


/**
 * @}
 * @}
 * @}
 * @}
 */
