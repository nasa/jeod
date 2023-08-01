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
 * @file models/utils/integration/lsode/src/lsode_first_order_ode_integrator__integrator.cc
 * Define member functions for the class LsodeFirstOrderODEIntegrator.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((dlsode.f)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((lsode_first_order_ode_integrator__manager.cc))

 

*******************************************************************************/

// System includes
#include <math.h> // pow
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
 * integrator_core provides the front-end to all of the integrator_*i
 * methods, which together perform one step of the integration of
 * an initial value problem for a system of ordinary differential
 * equations.
 *
 * Modified from DSTODE
 *
 * NOTES:
 * The entire integrator_* suite is independent of the value of the iteration
 * method indicator, corrector_method, when said is != 0, and hence is
 * independent of the type of chord method used, or the Jacobian structure.
 *
 * The value internal_state (JSTART) controls the direction that this method
 * takes.
 *
 * By commenting out substantial parts of the package that are not useful to the
 * ER7 / JEOD / Trick implementation, the only viable values for
 * internal_state are now 0 or 1.
 * internal_state = 0 : take the first step.
 * internal_state = 1 : take another step, continuing from the last.
 * internal_state = -1 was associated with externally driven changes to the input aprameters, something we do not allow.
 * internal_state = -2 is associated with the critical / singularity time avoidance, something we have not implemented.
 */
void
LsodeFirstOrderODEIntegrator::integrator_core()
{
   step_error = 0;
   data_stode.told = stage_target_time;
   data_stode.ncf = 0;
   iteration_matrix_singular = false;
   modified_iteration_matrix_singular = false;
   jacobian_current = false;
   convergence_jacobian_flag = 0;

   if (internal_state == 1) {  // 1 is only option >0
      integrator_predict(); // go to 200;
   }
   //   else if (internal_state == -2) {
   //      integrator_test_stepsize_change(); // go to 160
   //   }
   else if (internal_state == 0) {
      // On the first call, the order is set to 1, and other variables are
      // initialized.  The allowable step increase ratio,
      // max_step_increase_ratio is set to 10,000 to compensate for the small
      // initial step-size, but then is normally equal to 10.
      // If a failure occurs (in corrector convergence or error test),
      // this value is set to 2 for the next increase.

      max_history_size = max_order_internal;
      method_order_current = 1;
      num_nordsiek_cols = 2;
      order_select_para = 2;
      max_step_increase_ratio = 10000.0;
      rel_change_since_jacobian = 0.0;
      method_coeff_first = 1.0;
      convergence_rate = 0.7;
      prev_step_size = step_size;
      prev_integration_method = control_data.integration_method;
      step_at_last_jacobian_update = 0;
      update_jacobian =
                      !control_data.is_corrector_method_functional_iteration();
      data_stode.iret = 3;
      //go to 140

      // calculate_integration_coefficients (DCFODE) is called to get all the
      //   integration coefficients for the current integration method (METH).
      //   Then the EL vector and related constants are reset
      // whenever the order NQ is changed, or at the start of the problem.
      //-----------------------------------------------------------------------
      // 140
      calculate_integration_coefficients ();
      integrator_reset_method_coeffs();  // continues to 150
   }

   else if (internal_state == -1) { // go to 100
         //er7_utils::MessageHandler::fail (__FILE__, __LINE__,
         //   er7_utils::IntegrationMessages::internal_error,
         //   "internal state has taken the value -1.  This is typically\n"
         //   "associated with a user-specified change in the control\n"
         //   "parameters, which is not implemented.  Some other cause has\n"
         //   "driven internal_state < 0.\n");
      // The following block handles preliminaries needed when
      // internal_state = -1:
      // To force a matrix update, update_jacobian is set to true for all
      //   methods other than functional iteration
      // If an order increase is about to be considered
      //    (order_select_para = 1), order_select_para is reset to 2 to
      //    postpone consideration for one more step.
      // If the caller has changed integration method (METH),
      //    calculate_integration_coefficients (DCFODE) is called to reset
      //    the coefficients of the method.
      // If the caller has changed max_order (and propagated that change onto
      //    max_order_internal) to a value less than the current method
      //    order (method_order_current, NQ) then method_order_current is
      //    reduced to the new max_order_internal, and a new step size chosen
      //    accordingly.
      // If the step size is to be changed, the history array must be rescaled
      //    (integrator_reset_yh())
      // If the step size or the method is being changed, order_select_para is
      //    reset to method_order_current + 1 to prevent further changes in
      //    step size for that many steps.

      // 100
      update_jacobian =
                    !control_data.is_corrector_method_functional_iteration();
         //i.e. "No" if corrector_method is functional iteration
      max_history_size = max_order_internal + 1;
      if (order_select_para == 1) {
         order_select_para = 2;
      }
      if (control_data.integration_method != prev_integration_method) {
                                                           // inverse go to 110
         calculate_integration_coefficients ();
         prev_integration_method = control_data.integration_method;
         if (method_order_current <= max_order_internal) { // inverse go to 120
            order_select_para = num_nordsiek_cols;
            data_stode.iret = 1;
            integrator_reset_method_coeffs();  //go to 150
            return;
         }
         // else (i.e. method_order_current > max_order_internal),
         //       keep going (go to 120)
      }

      // 110
      // Get here if integration_method unchanged, or
      //          if method_order_current > max_order_internal
      if (method_order_current <= max_order_internal) { // go to 160;
         integrator_test_stepsize_change(); // go to line 160
         return;
      }

      // 120
      method_order_current = max_order_internal;
      num_nordsiek_cols = max_history_size;
      for (unsigned int ii = 0; ii < num_nordsiek_cols; ii++) { // do 125
         // 125
         method_coeffs_current[ii] =
                    method_coeffs_complete[ii][method_order_current-1];
      }
      num_predictor_elements = method_order_current*num_equations;
      rel_change_since_jacobian = rel_change_since_jacobian *
                                method_coeffs_current[0] / method_coeff_first;
      method_coeff_first = method_coeffs_current[0];
      convergence_factor = 0.5/(method_order_current+2);
      double ddn = magnitude_of_weighted_array (arrays.save) /
                          test_coeffs_complete[0][num_nordsiek_cols];
      double exdn = 1.0/num_nordsiek_cols;

      //FIXME **power and magic numbers
      double step_ratio_order_dec = 1.0/(1.3 * pow(ddn,exdn) + 0.0000013);

      data_stode.step_ratio = std::min(step_ratio_order_dec,1.0);
      data_stode.iredo = 3;
      if (Numerical::compare_exact(step_size,prev_step_size)) {// go to 170;
         // 170
         data_stode.step_ratio = std::max( data_stode.step_ratio ,
                             control_data.min_step_size / std::abs(step_size));
      }
      else {
         data_stode.step_ratio = std::min(data_stode.step_ratio,
                                  std::abs(step_size / prev_step_size));
         step_size = prev_step_size;
      }
      integrator_reset_yh(); //go to 175
      return;
   }

   else { //internal_state < -2
      er7_utils::MessageHandler::fail (__FILE__, __LINE__,
         er7_utils::IntegrationMessages::internal_error,
         "Invalid value for internal_state (%u) has been generated",
         internal_state);
   }
   return;
}


/**
 * Sets/resets the method_coeffs_current array
 */
void
LsodeFirstOrderODEIntegrator::integrator_reset_method_coeffs()
{
   // 150
   for (unsigned int ii = 0; ii < num_nordsiek_cols; ii++) { // do 155
      // 155
      method_coeffs_current[ii] =
                       method_coeffs_complete[ii][method_order_current-1];
   }
   num_predictor_elements = method_order_current*num_equations;
   rel_change_since_jacobian *= method_coeffs_current[0]/method_coeff_first;
   method_coeff_first = method_coeffs_current[0];
   convergence_factor = 0.5/(method_order_current+2);

   // branches here
   if (data_stode.iret == 1) {
      integrator_test_stepsize_change(); // go to 160
   }
   else if (data_stode.iret == 2) {
      // 170
      data_stode.step_ratio = std::max( data_stode.step_ratio,
                       (control_data.min_step_size / (std::abs(step_size)) ) );
      integrator_reset_yh();  // go to 175
   }
   else if (data_stode.iret ==3) {
      integrator_predict(); // go to 200
   }
   else {
      er7_utils::MessageHandler::fail (__FILE__, __LINE__,
         er7_utils::IntegrationMessages::internal_error,
         "Invalid value for internal variable iret in DSTODE has been "
         "generated.  iret = %u",data_stode.iret);
   }
   return;
}


/**
 * Tests h against old h
 */
void
LsodeFirstOrderODEIntegrator::integrator_test_stepsize_change()
{
   // If the step-size is not being changed, simply (re)do the prediction.
   //  Otherwise, the ratio of new to old step-size is recorded and the
   //  history array rescaled.

   // 160
   if (Numerical::compare_exact(step_size,prev_step_size)) {
      integrator_predict(); // go to 200;
   }
   else {
      data_stode.step_ratio = step_size/prev_step_size;
      step_size = prev_step_size;
      data_stode.iredo = 3;
      integrator_reset_yh();  //go to 175
   }
   return;
}


/**
 * Resets history arrays and time-step
 */
void
LsodeFirstOrderODEIntegrator::integrator_reset_yh()
{
   // 175
   // Note - first assignment is used to calculate values in second
   //        assignment.  Do not merge.
   data_stode.step_ratio = std::min(data_stode.step_ratio,max_step_increase_ratio);
   data_stode.step_ratio = data_stode.step_ratio /
       std::max( 1.0, std::abs(step_size) * max_step_size_inv * data_stode.step_ratio);

   double r = 1.0;
   for (unsigned int j = 1; j < num_nordsiek_cols; j++) { // do 180
      r *= data_stode.step_ratio;
      for (unsigned int i = 0; i < control_data.num_odes; i++) { // do 180
         // 180
         arrays.history[i][j] *= r;
      }
   }
   step_size *= data_stode.step_ratio;
   rel_change_since_jacobian *= data_stode.step_ratio;
   order_select_para = num_nordsiek_cols;
   if (data_stode.iredo == 0) {// go to 690;
      // 690
      max_step_increase_ratio = 10.0;
      integrator_wrapup();
   }
   else {
      integrator_predict();  // continue to 200
   }
   return;
}


/**
 * This section computes the predicted values by multiplying the
 * history array by the Pascal Triangle matrix.
 *
 * Extracted from DSTODE lines 200-215
 */
void
LsodeFirstOrderODEIntegrator::integrator_predict()
{
   // 200
   // Flag the Jacobian for needing an update if the following holds true:
   // 1. The corrector method is NOT Functional Iteration AND EITHER
   // 2a. The relative change since the last update exceeds the limit OR
   // 2b. The number of steps taken since the update exceeds the limit.
   //  To avoid running through the second two tests every time for
   //  functional iteration, do that test first.

   update_jacobian =
     ((!control_data.is_corrector_method_functional_iteration()) &&
      ((std::abs(rel_change_since_jacobian-1.0) >
                    max_rel_change_without_jacobian) ||
       (num_steps_taken >=
         step_at_last_jacobian_update + control_data.max_num_steps_jacobian)));

   stage_target_time = stage_target_time + step_size;

   // Multiply the history array by Pascal's Triangle.
   for (unsigned int i_iter = method_order_current; i_iter > 0; i_iter--) {
      for (unsigned int j_hist = i_iter-1; j_hist < method_order_current;
                                                                j_hist++) {
         for (unsigned int k_var = 0; k_var < num_equations; k_var++) {
            arrays.history[k_var][j_hist] += arrays.history[k_var][j_hist+1];
         }
      }
   }

   integrator_reset_iteration_loop_part1();
   return;
}


/**
 * This method resets the iteration loop to the values generated by the
 * integrator_predict method, which populated history[*][0].
 */
void
LsodeFirstOrderODEIntegrator::integrator_reset_iteration_loop_part1()
{
   // 220
   iteration_count = 0;
   for (unsigned int i = 0; i < control_data.num_odes; i++) { // do 230
      // 230
      y[i] = arrays.history[i][0];
   }
   re_entry_point = ResetIterLoop;
   return;  // exit to allow call to generate_derivatives.
}


/**
 * This code follows part 1 after the break-out to get to the
 * external calls, and completes the reset of the integration iteration loop.
 */
void
LsodeFirstOrderODEIntegrator::integrator_reset_iteration_loop_part2()
{

   // 250
   for (unsigned int i = 0; i < control_data.num_odes; i++) { // do 260
      // 260
      arrays.accum_correction[i] = 0.0;
   }

   iter_delta = 0.0;
   prev_iter_delta = 0.0;
   return;
}


/**
 * Keeps looping through the iterations until convergence or failure.
 */
void
LsodeFirstOrderODEIntegrator::integrator_corrector_iteration()
{
   // 270
   // When corrector_method is functional iteration, update y directly from
   // the result of the last function evaluation.
   if (control_data.is_corrector_method_functional_iteration()) {
                                                        // inverse go to 350;
      for (unsigned int ii = 0; ii < control_data.num_odes; ii++) { // do 290
         arrays.save[ii] = step_size*arrays.save[ii] - arrays.history[ii][1];
         // 290
         y[ii] = arrays.save[ii] - arrays.accum_correction[ii];
      }
      iter_delta = magnitude_of_weighted_array (y);
      for (unsigned int ii = 0; ii < control_data.num_odes; ii++) { // do 300
         y[ii] = arrays.history[ii][0] + method_coeffs_current[0] *
                                                               arrays.save[ii];
         // 300
         arrays.accum_correction[ii] = arrays.save[ii];
      }
   } // go to 400

   else {
      // In the other cases, compute the corrector error,
      //  and solve the linear system with that as right-hand side and
      //  lin_alg as coefficient matrix.
      // 350
      for (unsigned int ii = 0; ii < control_data.num_odes; ii++) { // do 360
         // 360
         y[ii] = step_size*arrays.save[ii] -
                       (arrays.history[ii][1] + arrays.accum_correction[ii]);
      }
      //linear_chord_iteration was SLVS: it is a linear solver.
      //Details are rather sketchy.
      linear_chord_iteration();

      // FIXME.  Alert that the original code may have been misinterpreted.
      // modified_iteration_matrix_singular represents IERSL.
      //  This can only take the values 0 or 1.  IERSL = 0 if no problems in
      //  DSOLSY.  IERSL = 1 if singular matrix found with MITER = 3.
      //  But not < 0.  There is no pathway to IERSL < 0.
      //
      //  In this implementation, modified_iteration_matrix_singular is
      //  a bool with value (IERSL==1).
      //if (modified_iteration_matrix_singular < 0) {
      //    integrator_corrector_failed_part2(); //go to 430;
      //    return;
      //}
      if (modified_iteration_matrix_singular) {
          integrator_corrector_failed_part1();  //go to 410;
          return;
      }
      // modified_iteration_matrix_singular = false.
      iter_delta = magnitude_of_weighted_array (y);
      for (unsigned int ii = 0; ii < control_data.num_odes; ii++) { // do 380
        arrays.accum_correction[ii] = arrays.accum_correction[ii] + y[ii];
         // 380
         y[ii] = arrays.history[ii][0] +
                 method_coeffs_current[0] * arrays.accum_correction[ii];
      }
   }

   // Test for convergence.  If iteration_count > 0, an estimate of the
   // convergence rate constant is stored in convergence_rate (was CRATE),
   // and this is used in the test.
   // 400
   if (iteration_count != 0) {
       convergence_rate = std::max( 0.2*convergence_rate,
                                    iter_delta/prev_iter_delta);
       // prev_iter_delta is previous value of iter_delta.
       // It starts at 0.0, but only when iteration_count=0.
       // Thereafter it gets set to the value of iter_delta.
   }
   double dcon = iter_delta * (std::min(1.0, 1.5*convergence_rate)) /
        (test_coeffs_complete[1][method_order_current-1] *
                                               convergence_factor);
   if (dcon <= 1.0) {
       integrator_corrector_converged();  //go to 450;
       return;
   }
   iteration_count ++;
   if ( (iteration_count == control_data.max_correction_iters) ||
        (iteration_count >= 2 && iter_delta > 2.0*prev_iter_delta)) {
       integrator_corrector_failed_part1(); //go to 410;  Iteration failed.
       return;
   }
   else { // Iterate again.
      prev_iter_delta = iter_delta;
      re_entry_point = IterationLoop;
   }
   return;
}


/**
 * The corrector iteration failed to converge.
 * If corrector_method != FunctionalIteration and the Jacobian is out of date,
 * exit so that the Jacobian method can be called (externally) for the next try.
 * Otherwise, try changing the step-size in part 2 of the failure recovery.
 *
 * Extracted from DSTODE lines 410-430
 */
void
LsodeFirstOrderODEIntegrator::integrator_corrector_failed_part1()
{
   //##-----------------------------------------------------------------------
   //##-----------------------------------------------------------------------
   // 410
   if (  control_data.is_corrector_method_functional_iteration() ||
         jacobian_current) {
       integrator_corrector_failed_part2(); // go to 430;
   }
   else {
      convergence_jacobian_flag = 1;
      // If corrector_method == FunctionalIteration, not going to be here.
      // Therefore if we are here, corrector_method != FunctionalIteration
      // and update_jacobian must be true.
      update_jacobian = true;
      integrator_reset_iteration_loop_part1(); // go to 220
   }
   return;
}


/**
 * Retracts the history array in the case that the correction iteration
 * failed to converge with either functional iteration or with an
 * up-to-date Jacobian.  If there are problems, the associated flags
 * are set.
 */
void
LsodeFirstOrderODEIntegrator::integrator_corrector_failed_part2()
{
   // 430
   convergence_jacobian_flag = 2;
   data_stode.ncf ++;
   max_step_increase_ratio = 2.0;
   stage_target_time = data_stode.told;
   for (unsigned int i_iter = method_order_current; i_iter > 0; i_iter--) {
      for (unsigned int j_hist = i_iter-1; j_hist < method_order_current;
                                                                j_hist++) {
         for (unsigned int k_var = 0; k_var < num_equations; k_var++) {
            // 440
            arrays.history[k_var][j_hist] -= arrays.history[k_var][j_hist+1];
         }
      }
   }
   // FIXME.  Alert that the original code may have been misinterpreted.
   // iteration_matrix_singular represents IERPJ, and
   // modified_iteration_matrix_singular represents IERSL.
   // These can only take the values 0 or 1, not < 0.
   // There is no pathway to IERPJ < 0.
   //if (iteration_matrix_singular < 0 ||
   //    modified_iteration_matrix_singular < 0) { //go to 680;
   //   step_error = -3;
   //   prev_step_size = step_size;
   //   internal_state = 1;
   //   return;
   //}
   if ( (std::abs(step_size) <= control_data.min_step_size*1.00001) ||
        (data_stode.ncf == control_data.max_num_conv_failure) ) { //go to 670;
      step_error = -2;
      integrator_terminate();
      return;
   }
   data_stode.step_ratio = 0.25;
   update_jacobian = !control_data.is_corrector_method_functional_iteration();
   data_stode.iredo = 1;
   integrator_reset_yh();     //go to 170
   return;
}


/**
 * Starts the processing of a converged iteration
 */
void
LsodeFirstOrderODEIntegrator::integrator_corrector_converged()
{
   // The iteration converged.
   // jacobian_current is set to false to signal that the Jacobian involved
   // may need updating later.
   // The local error test is made and control passes to statement 500
   // if it fails.
   // 450
   data_stode.dsm = 0.0;
   jacobian_current = false;
   if (iteration_count == 0) {
       data_stode.dsm = iter_delta /
                          test_coeffs_complete[1][method_order_current-1];
   }
   if (iteration_count > 0) {
      data_stode.dsm = magnitude_of_weighted_array (arrays.accum_correction) /
                     test_coeffs_complete[1][method_order_current-1];
   }
   if (data_stode.dsm > 1.0) {
       integrator_error_test_failed();  //go to 500;
       return;
   }

   // After a successful step, update the history array.
   // Consider changing step_size if order_select_para = 1 (order_select_para
   //   decrements with each successful step - it provides a countdown until
   //   the order (and with it the step size) MAY be changed.
   // If order_select_para reaches 1 save off the accumulated
   //   correction, which may be used to potentially increase the order
   //   (if current order < max order) at the next step.

   // If a change in step-size is considered, an increase or decrease of one
   //   place in the integrator order is considered also.
   // A change in step-size is made only if it is
   //   by a factor of at least 1.1.  If not, order_select_para is set to 3 to
   //   prevent testing for at least 2 additional steps.
   //-----------------------------------------------------------------------
   step_error = 0;
   data_stode.iredo = 0;
   num_steps_taken ++;
   prev_good_step_size = step_size;
   prev_method_order = method_order_current;
   for (unsigned int jj = 0; jj < num_nordsiek_cols; jj++) { // do 470
      for (unsigned int ii = 0; ii < control_data.num_odes; ii++) { // do 470
         // 470
         arrays.history[ii][jj] += method_coeffs_current[jj] *
                                        arrays.accum_correction[ii];
      }
   }
   order_select_para --;
   if (order_select_para == 0) {
      integrator_compute_new_order_prep();   //go to 520;
      return;
   }
   if ( (order_select_para > 1) || (num_nordsiek_cols == max_history_size) ){
      integrator_wrapup();   //go to 700;
      return;
   }
   for (unsigned int ii = 0; ii < control_data.num_odes; ii++) { // do 490
      // 490
      arrays.history[ii][max_history_size] = arrays.accum_correction[ii];
   }
   integrator_wrapup();   //go to 700;
   return;
}


/**
 * Restores the history array following the failure of the corrector for
 * exceeding local error bounds.
 */
void
LsodeFirstOrderODEIntegrator::integrator_error_test_failed()
{
   // step_error keeps track of multiple failures.
   // Restore stage_target_time and the history array to their previous values,
   // and prepare to try the step again.
   // Compute the optimum step size for this or one lower order.
   // After 2 or more failures, the step-size is forced to decrease by a
   // factor of 0.2 or less.
   // 500
   step_error = step_error - 1;
   stage_target_time = data_stode.told;
   for (unsigned int i_iter = method_order_current; i_iter > 0; i_iter--) {
      for (unsigned int j_hist = i_iter-1; j_hist < method_order_current;
                                                                j_hist++) {
         for (unsigned int k_var = 0; k_var < num_equations; k_var++) {
            // 510
            arrays.history[k_var][j_hist] -= arrays.history[k_var][j_hist+1];
         }
      }
   }
   max_step_increase_ratio = 2.0;
   if (std::abs(step_size) <= control_data.min_step_size*1.00001) {// go to 660
      step_error = -1;
      integrator_terminate();
      return;
   }
   if (step_error <= -3) {  // 3 such failures
       integrator_fail_reset_order_1_part1();    //go to 640;
       return;
   }
   data_stode.iredo = 2;
   data_stode.step_ratio_order_inc = 0.0;
   integrator_compute_new_order();    //  go to 540
   return;
}


/**
 * The first steps in computing whether the order of the integrator
 * should be changed.
 *
 * Regardless of the success or failure of the step, the step-ratio factors for
 *   an increase, decrease, or retention of the integrator order are computed.
 * In the case of failure, the increase ratio (data_stode.step_ratio_order_inc)
 *   has already been set to 0.0 to prevent an order increase.
 * The largest of these factors is determined and the new order chosen
 *   accordingly.  In the unusual case of equality, the priority is given to:
 *       1. retain the order
 *       2. increase the order (if inc = dec > same)
 *
 * If the order is to be increased, we compute one additional scaled derivative.
 *
 * This process is spread over four methods -
 *    integrator_compute_new_order_prep
 *    integrator_compute_new_order
 *    integrator_compute_new_order_check_step_error
 *    integrator_set_new_order
 *
 * extracted from lines 520-540
 */
void
LsodeFirstOrderODEIntegrator::integrator_compute_new_order_prep()
{
   // 520
   data_stode.step_ratio_order_inc = 0.0;
   if (num_nordsiek_cols != max_history_size) { // inverse go to 540;
      for (unsigned int ii = 0; ii < control_data.num_odes; ii++) { // do 530
         // 530
         arrays.save[ii] = arrays.accum_correction[ii] -
                                        arrays.history[ii][max_history_size];
      }
      double dup = magnitude_of_weighted_array (arrays.save) /
         test_coeffs_complete[2][method_order_current-1];
      double exup = 1.0/(num_nordsiek_cols+1);
      data_stode.step_ratio_order_inc = 1.0/(1.4 * pow(dup,exup) + 0.0000014);  // FIXME power fn, magic nums
   }
   integrator_compute_new_order();
   return;
}


/**
 * Computes the step-size scaling factors that will result once it is
 * determined what happens to the order at the next step.
 */
void
LsodeFirstOrderODEIntegrator::integrator_compute_new_order()
{
   // 540
   double exsm = 1.0/num_nordsiek_cols;
   double step_ratio_order_same = 1.0/(1.2 * pow(data_stode.dsm,exsm) +
                                                                    0.0000012);
   double step_ratio_order_dec = 0.0;
   if (method_order_current != 1) {   // inverse go to 560;
      double ddn = magnitude_of_weighted_array ( num_nordsiek_cols-1,
                                                 arrays.history       ) /
                         test_coeffs_complete[0][method_order_current-1];
      double exdn = 1.0/method_order_current;
      step_ratio_order_dec = 1.0/(1.3 * pow(ddn,exdn) + 0.0000013);
   }

   // 560
   if (step_ratio_order_same >= data_stode.step_ratio_order_inc) {//go to 570;
      // 570
      if (step_ratio_order_same >= step_ratio_order_dec) {// inverse go to 580;
         // Maintain current order.
         data_stode.new_method_order = method_order_current;
         data_stode.step_ratio = step_ratio_order_same;
         integrator_compute_new_order_check_step_error(); //go to 620
         return;
      }
   }

   else if (data_stode.step_ratio_order_inc > step_ratio_order_dec) {
      // 590
      // don't increase the order, but hold for at least 3 steps if increase
      // in step size is too small (< 10%) - realize that this is the
      // biggest of the three step ratios.
      data_stode.new_method_order = num_nordsiek_cols;
      data_stode.step_ratio = data_stode.step_ratio_order_inc;
      if (data_stode.step_ratio < 1.1) {  // go to 610;
         order_select_para = 3;
         integrator_wrapup();
         return;
      }
      // else
      double r = method_coeffs_current[num_nordsiek_cols-1]/num_nordsiek_cols;
      for (unsigned int ii = 0; ii < control_data.num_odes; ii++) { // do 600
         // 600
         arrays.history[ii][data_stode.new_method_order] =
                                               arrays.accum_correction[ii] * r;
      }
      // go to 630
      integrator_set_new_order();
      return;
   }

   // else (dec >= inc > same) OR (dec > inc >= same) OR (dec > same > inc)
   // Decrement the order
   // go to 580
   // 580
   data_stode.new_method_order = method_order_current - 1;
   data_stode.step_ratio = step_ratio_order_dec;
   if (step_error < 0 && data_stode.step_ratio > 1.0) {
       data_stode.step_ratio = 1.0;
   }
   integrator_compute_new_order_check_step_error();
   return;
}


/**
 *
 */
void
LsodeFirstOrderODEIntegrator::integrator_compute_new_order_check_step_error()
{

   // 620
   if ((step_error == 0) && (data_stode.step_ratio < 1.1)) {  // go to 610;
      //610 copy
      order_select_para = 3;
      integrator_wrapup();
   }
   else {
      if (step_error <= -2) {
          data_stode.step_ratio = std::min(data_stode.step_ratio, 0.2);
      }
      integrator_set_new_order();
   }
   return;
}


/**
 * Sets the new order and the step-ratio for the next step - or the
 * current step if redoing it
 */
void
LsodeFirstOrderODEIntegrator::integrator_set_new_order()
{
   if (data_stode.new_method_order == method_order_current) {  //go to 170;
      data_stode.step_ratio = std::max(data_stode.step_ratio,
                                       control_data.min_step_size /
                                                     std::abs(step_size));
      integrator_reset_yh();
   }
   else {
      // 630
      method_order_current = data_stode.new_method_order;
      num_nordsiek_cols = method_order_current + 1;
      data_stode.iret = 2;
      integrator_reset_method_coeffs(); //go to 150
   }
   return;
}


/**
 * Repopulates the y-array from history, recomputes the original first
 * derivatives, sets the order back to 1, and reduces the step size by
 * a factor of 10
 *
 * Called when 3 or more failures have occured.
 * It is assumed that the derivatives that have accumulated in the
 * history array have errors of the wrong order.  Hence the first
 * derivative is recomputed, and the order is set to 1.  Then
 * the step-size is reduced by a factor of 10, and the step is retried.
 * Repeat until successful, or the step reaches the minimum step-size.
 *
 * If 10 failures occur, exit with step_error = -1.
 *
 * This method is divided in two by a call to calculate the derivatives.
 * Part1 precedes that call, the execution exits from the integrator back to the
 * sim control engine; then on return to the integrator, execution immediately
 * proceeds with part2.
 *
 * extracted from lines 640-
 */
void
LsodeFirstOrderODEIntegrator::integrator_fail_reset_order_1_part1()
{
   // 640
   if (step_error == -10) { // go to 660;
      //660 copy
      step_error = -1;
      integrator_terminate();
      return;
   }
   data_stode.step_ratio = std::max(control_data.min_step_size /
                                            std::abs(step_size),
                                    0.1);
   step_size *= data_stode.step_ratio;
   for (unsigned int ii = 0; ii < control_data.num_odes; ii++) { // do 645
      // 645
      y[ii] = arrays.history[ii][0];
   }
   re_entry_point = DstodeResetStep;
   return;
}


/**
 * Continue reset, with derivatives now at hand. See
 * integrator_fail_reset_order_1_part1 for details.
 */
void
LsodeFirstOrderODEIntegrator::integrator_fail_reset_order_1_part2()
{
   for (unsigned int ii = 0; ii < control_data.num_odes; ii++) { // do 650
      // 650
      arrays.history[ii][1] = step_size*arrays.save[ii];
   }
   update_jacobian = !control_data.is_corrector_method_functional_iteration();
   order_select_para = 5;
   if (method_order_current == 1) {
      integrator_predict(); // go to 200;
      return;
   }
   method_order_current = 1;
   num_nordsiek_cols = 2;
   data_stode.iret = 3;
   integrator_reset_method_coeffs(); //go to 150
   return;
}


/**
 * Wraps up the completion of the integrator.
 */
void
LsodeFirstOrderODEIntegrator::integrator_wrapup()
{

   // 700
   double r = 1.0/test_coeffs_complete[1][prev_method_order-1];
   for (unsigned int ii = 0; ii < control_data.num_odes; ii++) { // do 710
      // 710
      arrays.accum_correction[ii] *= r;
   }
   integrator_terminate(); // 720
   return;
}


/**
 * this is the only succesful path back from integrator to manager.
 * All other returns from integrator_* back to manager_* are in response
 * to a need for new derivatives and carry with them a modified
 * re_entry_point to provide access back to the integrator on reentry.
 * All returns with re_entry_point = CycleStartFinish should go through
 * this method.
 */
void
LsodeFirstOrderODEIntegrator::integrator_terminate()
{
   // Store off the step-size to allow it to be used as the basis for
   // step-size changes on the next step.
   prev_step_size = step_size;
   internal_state = 1;
   return;
}

/**
 * @}
 * @}
 * @}
 * @}
 */
