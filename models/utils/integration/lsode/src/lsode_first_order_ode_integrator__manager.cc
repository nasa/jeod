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
 * @file models/utils/integration/lsode/src/lsode_first_order_ode_integrator__manager.cc
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
  ((lsode_first_order_ode_integrator__integrator.cc)
   (lsode_first_order_ode_integrator__manager.cc)
   (lsode_first_order_ode_integrator__support.cc)
   (lsode_first_order_ode_integrator__utility.cc)
   (lsode_data_classes.cc))



*******************************************************************************/

// System includes
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
 * Propagate state via the LSODE method.
 */
er7_utils::IntegratorResult
LsodeFirstOrderODEIntegrator::integrate (
   double dyn_dt,
   unsigned int target_stage JEOD_UNUSED,
   double const * y_dot_in,
   double * y_in)
{
   // Implementation-Notes
   //  1. We do not need the ability to change integration parameters mid-sim,
   //     so calculation_phase (ISTATE) =3 is redundant.  Therefore, it can only
   //     have values:
   //       1 (first time only),
   //       2 (all other times), or
   //       <0 (various failure modes).
   //     See also note #3 for values < 0.
   //     Changing this from an integer to a flag, first_time, to distinguish
   //     between values 1 and 2.
   //  2. We are not interested in critical time operations, so all code relating
   //     to calculation_task (ITASK) = 4 or 5 is redundant.  Similarly, ITASK = 2
   //     (single step and return) and ITASK =3 (return from first mesh point
   //     after TOUT) are also unnecessary for our purposes and are not
   //     implemented.
   //  3. Error messages that returned values of calculation_phase < 0 simply
   //     fail.  We cannot tweak the integrator parameters mid-sim, so any failure
   //     results in a total failure.  The error messages are retained,
   //     with the identification of the error included in the error
   //     message making the additional identification with a ISTATE value
   //     redundant.
   //     Additionally, it appears that the old implementation was also largely -
   //     if not entirely - unable to recover from a negative value making
   //     it redundant it redundant there also.

   er7_utils::IntegratorResult return_val;  // Don't move this line.
                                            // First entry for optimization
// dyn_dt comes from LsodeIntegrationControls
// target_stage is always 1
   y_dot = y_dot_in;
   y=y_in;

   switch (re_entry_point) {
   case CycleStartFinish: // default, regular call to integrate
      // Subtract off the previous cycle-target-time to identify by how
      // much the last known data overshot its target.  Then reset the
      // new cycle_target_time.  New code associated with JEOD's timekeeping.
      // NOTE - stage_target_time here is the time to which the integrator has
      //        advanced, while cycle_target_time is the last requested data
      //        point.  In order for re_entry_point to have the value
      //        CycleStartFinish, s_t_t >= c_t_t.
      // EXAMPLE - last request was to intergate to 10s.  Integrator actually
      //           reached 12s and interpolated back to 10.  The state that is
      //           stored by the integrator is that at 12s, 2s into the new
      //           cycle.
      stage_target_time -= cycle_target_time;
      cycle_target_time = dyn_dt;

      process_entry_point_cycle_start();
      break;

   case InitCalc:
      cycle_target_time = dyn_dt;
      re_entry_point = CycleStartFinish;

      manager_initialize_calculation_part2();
      // Includes loading the derivatives into the history array.

      manager_integration_loop_part2(); // go to 270
      // part 3 called automatically if DSTODE (called from part 2)
      // does not go through a "CALL F".
      break;

   case JacobianPrep: // from jacobian_prep_init and jacobian_prep_loop
      // derivatives are loaded into places dependent on
      // corrector_method, so load_derivatives is called from
      // jacobian_prep_loop.

      // The re_entry_point is not changed until the looping iteration
      // is finished and the jacobian_wrap_up finished.  Otherwise, we
      // will keep coming back to this point.

      if (jacobian_prep_loop()) {
         if (jacobian_prep_wrap_up()) {
            // Moving on to line 250 in DSTODE.

            re_entry_point = CycleStartFinish;
            integrator_reset_iteration_loop_part2();
            integrator_corrector_iteration();
            // Following the return from any part of the integrator
            // (DSTODE), the rest of the manager (DLSODE) must be called
            // UNLESS the integrator returned prematurely, in need of a
            // recompute of the derivatives.  If it did, the
            // re_entry_point will be set accordingly.  If
            // re_entry_point has not been reset, wrap up the manger
            // functionality.
            // Comment applies to all calls to integrator_* from all
            // re_entry_point cases.
            if (re_entry_point == CycleStartFinish) {
               manager_integration_loop_part3();
            }
         }
         else if (step_error == -2) {
            er7_utils::MessageHandler::fail (__FILE__, __LINE__,
               er7_utils::IntegrationMessages::internal_error,
               "Infinite loop encountered in computing the Jacobian.\n"
               "Repeated attempts keep producing a singular matrix.\n");
         }
      }
      // else, the loop did not converge and need to return to it after
      // recomputing derivatives.
      break;

   case ResetIterLoop:
      load_derivatives(arrays.save);

      // Go back into DSTODE

      if (update_jacobian) { // inverse go to 250;

         // Was a call to PJAC; the Fortran code is non-obvious here,
         // with the call to PJAC really going to DPREPJ (DPREPJ is
         // passed in as an argument where it is received as PJAC).
         // PJAC and DPREPJ are equivalent.  There are no other instances
         // of DSTODE being called so it is safe to say that PJAC is NOT a
         // generic form, but that there is only one PJAC, and it is
         // DPREPJ.
         //
         // In this code, the replacement is jacobian_prep_*, a
         // multi-installment representation of DPREPJ.
         //
         // The first part, jacobian_prep_init, gets called here; this
         // method sets the re_entry_point to JacobianPrep, which
         // handles the other two parts.
         jacobian_prep_init(); // sets re_entry_point to JacobianPrep
      }
      else {
         // If the Jacobian does not require an update continue on to
         // line 250
         re_entry_point = CycleStartFinish;
         integrator_reset_iteration_loop_part2();
         integrator_corrector_iteration();
         // See comment at end of case JacobianPrep
         if (re_entry_point == CycleStartFinish) {
            manager_integration_loop_part3();
         }
      }
      break;

   case IterationLoop:
      // F called from integrator_corrector_iteration ~line 400+ in fortran code.
      load_derivatives(arrays.save);
      re_entry_point = CycleStartFinish;
      integrator_corrector_iteration();
      // See comment at end of case JacobianPrep
      if (re_entry_point == CycleStartFinish) {
         manager_integration_loop_part3();
      }
      break;

   case DstodeResetStep:
      // F called from out of integrator_fail_reset_order_1_part1
      load_derivatives(arrays.save);
      re_entry_point = CycleStartFinish;
      integrator_fail_reset_order_1_part2();
      // See comment at end of case JacobianPrep
      if (re_entry_point == CycleStartFinish) {
         manager_integration_loop_part3();
      }
      break;
   default:
      break;
   }

   if (re_entry_point == CycleStartFinish) {
      return_val.set_passed();
      return_val.set_time_scale(1.0);
   }
   else {
      return_val.set_failed();
      return_val.set_time_scale(stage_target_time/cycle_target_time);
   }
   return return_val;
}

// Lines 70-95+ were for reconfiguring the integrator following a change to
// parameters mid-integration, i.e. when calculation_phase (ISTATE) = 3.
// This is no longer valid in this implementation. Deleting these lines.


/**
 * The code block from the main integrate routine for
 * re_entry_point=CycleStartFinish
 */
// parameters mid-integration, i.e. when calculation_phase (ISTATE) = 3.
// This is no longer valid in this implementation. Deleting these lines.


/*****************************************************************************
process_entry_point_cycle_start
Purpose:(The code block from the main integrate routine for
         re_entry_point=CycleStartFinish)
Taken from DLSODE subroutine, lines 0-20, and direction up to 250.
******************************************************************************/
void
LsodeFirstOrderODEIntegrator::process_entry_point_cycle_start()
{
   if (first_pass) { // Was ISTATE == 1, go to 10
      num_equations = control_data.num_odes;
      stage_target_time = 0.0;
      // 10
      initialized = false;
      if (std::fpclassify(cycle_target_time) != FP_ZERO){// Otherwise nothing to do, time is current
         if ((cycle_target_time) * control_data.initial_step_size < 0.0) {
            er7_utils::MessageHandler::fail (__FILE__, __LINE__,
               er7_utils::IntegrationMessages::invalid_request,
               "The initial step size (%f) has been specified and "
               "indicates that integration is to "
               "proceed in the wrong direction;\n"
               "cycle_target_time (%f) has a different sign to the "
                "initial_step_size.",
               control_data.initial_step_size,cycle_target_time);
         }
         manager_initialize_calculation_part1(); // 100-190
         first_pass = false;
      }
      return;
   }
   // else, not in first-pass mode but in another cycle-start

   // ISTATE==2 or ISTATE==3
   if (!initialized) {  // IF(INIT.EQ.0) Go TO 603
      er7_utils::MessageHandler::fail (__FILE__, __LINE__,
         er7_utils::IntegrationMessages::internal_error,
         "first_pass flag has been switched off but lsode does not indicate "
         "initialized");
   }


   // orig code has:
   //    if (ISTATE.EQ.2) GO TO 200 (manager_check_stop_conditions)
   //    otherwise (i.e. calculation_phase = 3), do stuff before
   //    going to 200 from line 95.
   // Note that the code from 20 to 70 has been pulled out into
   // lsode_control_data_interface, or has become unnecessary.
   // This implementation does not provide capabilities associated
   // with ISTATE == 3, (See Implementation-Note #1), so go straight
   // to line 200.

   int temporary_branch_value = manager_check_stop_conditions(); // go to 200

   // manager_check_stop_conditions could set temporary_branch_value to
   // 0, 1, or 2, although the ability to set it to 2 is associated with the
   // critical / singularity time features which we do not implement
   // (see Implementation note #2).
   // So the return is effectively a bi-state, and this architecture is
   // overkill.  Leaving it though in case it ever is
   // necessary to add in the critical time code again.
   // Returning with a 0 means that the cycle has completed and no further
   // action is needed.
   if (temporary_branch_value == 1) {
      manager_integration_loop_part1();  // Go to 250
   }
//   else if (temporary_branch_value == 2) {
//      manager_set_calculation_phase_eq_2_reload();
//   }
   return;
}



/**
 * Sets the values for the case with calculation_phase = 1
 */
void
LsodeFirstOrderODEIntegrator::manager_initialize_calculation_part1()
{
   arrays.allocate_arrays(control_data.num_odes,
                          control_data.corrector_method);
   control_data.allocate_arrays();
//##-----------------------------------------------------------------------
//## Block C.
//##-----------------------------------------------------------------------


// Unnecessary - see Implementation-Note #2
//   // Check for a critical time (singularity) in the integration interval
//   // or in the initial integration step (if defined):
//   if (calculation_task == NormalWithSingularity ||
//       calculation_task == OneStepWithSingularity) {
//      if ((t_crit - cycle_target_time) * (cycle_target_time - current_time)
//                                                                   < 0.0) {
//         er7_utils::MessageHandler::fail (__FILE__, __LINE__,
//            er7_utils::IntegrationMessages::internal_error,
//               "Calculation task = 4 or 5 and critical time (t_crit = %f) is "
//               "behind\n cycle target time (cycle_target_time = %f)",
//               t_crit, cycle_target_time);
//      }
//      if ( initial_step_size != 0.0 &&
//           (current_time + initial_step_size - t_crit) * initial_step_size
//                                                                   > 0.0) {
//         initial_step_size = t_crit - current_time;
//      }
//   }


   //110
   internal_state = 0;
   if(!control_data.is_corrector_method_functional_iteration()) {
      arrays.lin_alg_1 = sqrt_epsilon;
   }
   //## Initial call to F.  (LF0 points to YH[*,2].) -------------------------
   //   lf0 = lyh + nyh;
   // This function call appears to load the derivatives into rwork,
   // beginning with index lf0, which should have the value
   // 21+num_equations_to_solve.
   // Get out of here back to the simulation engine to recalcualte the
   // derivatives.  Come back at entry-point InitCalc
   re_entry_point = InitCalc;
   return;
}



/***************************************************************************
Part 2.  From lines 115-190
***************************************************************************/
void
LsodeFirstOrderODEIntegrator::manager_initialize_calculation_part2()
{

   // Load the initial value vector in YH. ---------------------------------
   for (unsigned int ii = 0; ii<control_data.num_odes; ii++) { //do 115
      arrays.history[ii][0] = y[ii];
      arrays.history[ii][1] = y_dot[ii];
   }

   // Load and invert the error-weight array.
   // step-size is temporarily set to 1.0.
   method_order_current = 1;
   step_size = 1.0;
   load_ew_values(); // was DEWSET

   // Check for legal values and invert.
   for (unsigned int ii = 0; ii<control_data.num_odes; ii++) { // do 120
      if (arrays.error_weight[ii] <= 0.0) {
         er7_utils::MessageHandler::fail (__FILE__, __LINE__,
            er7_utils::IntegrationMessages::internal_error,
            "Working value of arrays.error_weight at index %u fell to <= 0.\n"
            "arrays.error_weight[%u] = %f",
            ii, ii, arrays.error_weight[ii]);
         return;
      }
      //120
      arrays.error_weight[ii] = 1.0/arrays.error_weight[ii];
   }

   // Only calculate the initial step size it has not been set by the user.
   // Compare against the default initialized value of 0.0 to identify
   // change; since comparing against zero set at construction, comparing
   // double against set value  should be ok.
   if (std::fpclassify(control_data.initial_step_size) == FP_ZERO) {   // inverse go to 180


      // Make sure that the target time and current time are
      // distinguishable.  current_time in this implementation is
      // identically 0.0.
      double baseline_time = std::abs(cycle_target_time);
      if (baseline_time < 2.0 *epsilon) {
         er7_utils::MessageHandler::fail (__FILE__, __LINE__,
            er7_utils::IntegrationMessages::internal_error,
            "cycle_target_time (%f) too close to current_time to "
            "start integration.\n",
            cycle_target_time);
         return;
      }
      // Find the largest relative tolerance.  Start with the first, test
      // the others only if not using under a common factor.
      double rel_tolerance = control_data.rel_tolerance_error_control[0];
      if (control_data.error_control_indicator ==
                         LsodeControlDataInterface::CommonAbsSpecificRel ||
          control_data.error_control_indicator ==
                         LsodeControlDataInterface::SpecificAbsSpecificRel) {
         for (unsigned int ii = 1; ii<control_data.num_odes; ii++) {
            //130
            if (rel_tolerance < control_data.rel_tolerance_error_control[ii]) {
               rel_tolerance = control_data.rel_tolerance_error_control[ii];
            }
         }
      }
      //140
      // If there is no relative tolerance, use the absolute tolerance to
      // generate one.
      if (rel_tolerance <= 0.0) {  // inverse go to 160
         // Note - have already checked for < 0 and failed if not satisfied,
         // so this is really test of ==0
         double abs_tolerance = control_data.abs_tolerance_error_control[0];
         for (unsigned int ii = 1; ii<control_data.num_odes; ii++) { // do 150
            if (control_data.error_control_indicator ==
                         LsodeControlDataInterface::SpecificAbsCommonRel ||
                control_data.error_control_indicator ==
                         LsodeControlDataInterface::SpecificAbsSpecificRel) {
               abs_tolerance = control_data.abs_tolerance_error_control[ii];
            }
            // otherwise, abs_tolerance = abs_tolerance_error_control[0]

            if (std::fpclassify(y[ii]) != FP_ZERO) {
               rel_tolerance = std::max( rel_tolerance,
                                         abs_tolerance/(std::abs(y[ii])));
            }
         }
      }
      //160
      // Bound the tolerance
      // These magic numbers come from the LSODE code.
      rel_tolerance = std::max( rel_tolerance, 100.0*epsilon);
      rel_tolerance = std::min( rel_tolerance, 0.001);

      //sum over (arrays.history(*,1) * arrays.error_weight(*))**2
      double sum_square = magnitude_of_weighted_array(1, arrays.history);
      double sum = (1.0 / (rel_tolerance * baseline_time * baseline_time)) +
                   (rel_tolerance*sum_square*sum_square);

      control_data.initial_step_size = 1.0 / sqrt(sum);
      control_data.initial_step_size = std::min(control_data.initial_step_size,
                                                baseline_time);
      if (cycle_target_time < 0.0) { // sign of cycle_target_time-t is -ve
         control_data.initial_step_size *= -1;
      }
   }

   // Now have an initial step size, whether it is user-specified or
   // auto-generated.
   //
   // Adjust this initial_step_size if necessary to meet max_step_size bound.
   // Awkward implementation, but initial_step_size could be negative.
   //180
   double step_size_ratio = std::abs(control_data.initial_step_size)*max_step_size_inv;
   if (step_size_ratio > 1.0) {
      control_data.initial_step_size /= step_size_ratio;
   }

   // Load initial_step_size into step_size and scale the first-derivativei
   // component of the history array by this initial step size (converts
   // initial velocity into initial estimate of delta-x.)
   step_size = control_data.initial_step_size;
   for (unsigned int ii = 0; ii < control_data.num_odes; ii++) {
      arrays.history[ii][1] *= control_data.initial_step_size;
   }
   // Moving GO TO 270 (call to integration_loop) back to integrate()
   // to follow on to this method.
   return;
}




/**
 * verifies whether the convergence conditions have been met to end
 * the cycle.
 */
int
LsodeFirstOrderODEIntegrator::manager_check_stop_conditions()
{
   // Taken from DLSODE lines 200-250.
   // Instead of calling from out of this method, it returns a value to
   // process_entry_point_cycle_start() according to the following options:
   //
   // 0: go to 420 (cycle complete) OR
   //    return from error condition.  In either case, no action necessary.
   // 1: go to 250 (manager_integration_loop_part1())
   // 2: go to 400 (manager_set_calculation_phase_eq_2_reload())
   //
   // Additional note - return 2 is now disabled because it is only associated with
   //                   cases that we do not support.  This method could become a
   //                   bool instead, but keeping it as an integer method in case
   //                   those cases are needed at a later date.


//##-----------------------------------------------------------------------
//## Block D...
//##-----------------------------------------------------------------------
   // 200
   prior_num_steps = num_steps_taken;
   switch (calculation_task) {
   case Normal: // Normal
      // if have not overshot target, integrate it.  note -
      // stage_target_time is from previous stage.
      if ((stage_target_time - cycle_target_time)*step_size < 0.0) {
          return 1;  // go to 250
      }
      // else interpolate
      interpolate_y();

      // return a 0 to indicate stop conditions met.
      // re_entry_point stays at CycleStartFinish, and cycle is complete.
      return 0;

// See Implementation-Note #2.  These options are not implemented.
   case OneStep: // Take one step and return.
//      return 1;
//
   case CompleteCycle: // Stop at first mesh point at or beyond
                       //  cycle_target_time
//      //220
//      if ((stage_target_time - prev_good_step_size*(1.0 + 100.0*epsilon) -
//                  cycle_target_time) * step_size > 0.0) {
//         er7_utils::MessageHandler::fail (__FILE__, __LINE__,
//            er7_utils::IntegrationMessages::internal_error,
//            "Calculation task (%u) incompatible with cycle-target-time (%f)"
//            "being behind \n(current-time (%f)-previous good step size (%f))"
//            ".\n", calculation_task, cycle_target_time, stage_target_time,
//            prev_good_step_size);
//         return 0;
//      }
//      if ((stage_target_time - cycle_target_time)*step_size < 0.0) {
//          return 1;
//      }
//      return 2; //manager_set_calculation_phase_eq_2_reload();
//   case NormalWithSingularity: // Normal computation, with safeguard
//                               // on singularity time, t_crit
//      if ((stage_target_time - t_crit)*step_size > 0.0) {
//         er7_utils::MessageHandler::fail (__FILE__, __LINE__,
//            er7_utils::IntegrationMessages::internal_error,
//            "Calculation task = 4 and critical time (t_crit = %f) is behind\n"
//            "current time (stage_target_time = %f)",
//            t_crit, stage_target_time);
//         return 0;
//      }
//      if ((t_crit - cycle_target_time)*step_size < 0.0) {
//         er7_utils::MessageHandler::fail (__FILE__, __LINE__,
//            er7_utils::IntegrationMessages::internal_error,
//               "Calculation task = 4 or 5 and critical time (t_crit = %f) is "
//               "behind\n cycle target time (cycle_target_time = %f)",
//               t_crit, cycle_target_time);
//         return 0;
//      }
//      if ((stage_target_time - cycle_target_time)*step_size < 0.0) {
//         break;  // Do not return, merge with case 5 at line 245
//      }
//      interpolate_y();
//      current_time = cycle_target_time;
//      // go to 420
//      return 0;
//
   case OneStepWithSingularity:
//      // 240
//      if ((stage_target_time - t_crit)*step_size > 0.0) {
//         er7_utils::MessageHandler::fail (__FILE__, __LINE__,
//            er7_utils::IntegrationMessages::internal_error,
//            "Calculation task = 5 and critical time (t_crit = %f) is behind\n"
//            "current time (stage_target_time = %f)",
//            t_crit, stage_target_time);
//         return 0;
//      }
   case NormalWithSingularity:
   default:
      return 0;
   }
//   //245 - merge cases 4 and 5 that still require work.
//   t_crit_hit = ( std::abs(stage_target_time - t_crit) <=
//                     100.0 * epsilon *
//                       (std::abs(stage_target_time) + std::abs(step_size)));
//   if( t_crit_hit) {
//       return 2;
//   }
//   if ((stage_target_time + step_size*(1.0 + 4.0*epsilon) - t_crit) *
//                                                       step_size > 0.0) {
//      step_size = (t_crit - stage_target_time)*(1.0 - 4.0*epsilon);
//      internal_state = -2;
//   }
   return 1;
}





/**
 * The iteration loop for the integration process.
 */
void
LsodeFirstOrderODEIntegrator::manager_integration_loop_part1()
{
   // The integration loop code block can be accessed from multiple external points,
   // so it is divided into 3 parts.  Part 1 automatically flows into part 2 and
   // part 2 may flow into part 3.  All 3 are called directly from other parts of
   // the code.
   //
   // The first part is a configuration; the second part continues the configuration,
   // ending with a call to the core integrator (core_integrator, DSTODE).  The third
   // part follows the return from the integrator, assuming that the integrator did
   // not need to exit prematurely to recalculate derivatives.
   //
   // Taken from code lines 250-350

   //##-----------------------------------------------------------------------
   //## Block E.
   //##-----------------------------------------------------------------------

   //250
   // Check for too many steps being taken
   if ((num_steps_taken-prior_num_steps) >= control_data.max_num_steps) {
      // 500
      er7_utils::MessageHandler::fail (__FILE__, __LINE__,
         er7_utils::IntegrationMessages::internal_error,
         "Number of steps taken reached the maximum allowed value (%u)"
         "at time %f.\n"
         "The intended cycle target time has not been reached.\n",
         control_data.max_num_steps, stage_target_time);
      return;
   }
   // update the error weights
   load_ew_values();
   for (unsigned int ii = 0; ii<control_data.num_odes; ii++) {
      if (arrays.error_weight[ii] <= 0.0)  {
         // 510
         er7_utils::MessageHandler::fail (__FILE__, __LINE__,
            er7_utils::IntegrationMessages::internal_error,
            "arrays.error_weight[%u] has become <= 0.0 at time %f",
            ii, stage_target_time);
         return;
      }
      arrays.error_weight[ii] = 1.0/arrays.error_weight[ii];
   }
   manager_integration_loop_part2(); // continue to 270
   return;
}

/****************************************************************************/
// Part 2
/****************************************************************************/
void
LsodeFirstOrderODEIntegrator::manager_integration_loop_part2()
{
   // Check for too much accuracy being requested, and
   //## check for H below the roundoff level in T.
   //270
   if ( magnitude_of_weighted_array ( 0, arrays.history) > 1/epsilon) {
   // Without the calculation_phase (ISTATE = -2 vs ISTATE = -3), the possible
   // paths from here (GO TO 520, GO TO 626) are essentially identical.
   // Whether or not this is the first step (NST.EQ.0) is irrelevant.  Too
   // much accuracy was requested.  The doubling of TOLSF is completely
   // redundant.  Fail.
      // 520
      er7_utils::MessageHandler::fail (__FILE__, __LINE__,
         er7_utils::IntegrationMessages::invalid_request,
         "Requested accuracy is too high for machine.  At time %f,\n"
         "allowable tolerance is %E, producing a magnitude of weighted \n"
         "history of %E.  The limit (1/epsilon) is %E (tolerance =%E).\n",
         stage_target_time, 1/magnitude_of_weighted_array ( 0, arrays.history),
         magnitude_of_weighted_array ( 0, arrays.history),
         1/epsilon, epsilon );
      return;
   }
   //280
   if (Numerical::compare_exact((stage_target_time + step_size),stage_target_time)) {
      num_small_step_warnings ++;
      if (num_small_step_warnings <= control_data.max_num_small_step_warnings){
         er7_utils::MessageHandler::warn (__FILE__, __LINE__,
            er7_utils::IntegrationMessages::internal_error,
            "Internal time, (t=%f),  and time step, (dt=%f),  are such that "
            "adding the timestep to the time yields the time.\n"
            "                t + dt   =   t\n"
            "Solver will continue anyway.\n",
            stage_target_time, step_size);
      }
      if (num_small_step_warnings == control_data.max_num_small_step_warnings){
         er7_utils::MessageHandler::warn (__FILE__, __LINE__,
            er7_utils::IntegrationMessages::internal_error,
            "Warning has been issued %d times.\n"
            "It will not be issued again.\n",
            num_small_step_warnings);
      }
   }
   //290
   integrator_core();
   if (re_entry_point == CycleStartFinish) {
      manager_integration_loop_part3(); // integrator_core never needed to
                                        // call the derivative function F, so
                                        // no need to exit.
   }
   return;
}



/****************************************************************************/
//  Part 3
/****************************************************************************/
void
LsodeFirstOrderODEIntegrator::manager_integration_loop_part3()
{

// This method gets called following a return from some component of what was
// DSTODE, such as integrator_core, integrator_corrector_iteration, etc.
// In the Fortran code, the first thing on return from DSTODE is to check
// KFLAG (now step_error).  So the first thing to do in the new function that
// immediately follows the return from DSTODE is to do likewise.
   // branch to 530, 540.
   if (step_error == -1) {
      // 530
      er7_utils::MessageHandler::fail (__FILE__, __LINE__,
         er7_utils::IntegrationMessages::internal_error,
         "Error test failed repeatedly at time %f with step size %f",
         stage_target_time, step_size);
      return;
   }
   else if (step_error == -2) {
      // 540
      er7_utils::MessageHandler::fail (__FILE__, __LINE__,
         er7_utils::IntegrationMessages::internal_error,
         "Corrector convergence failed repeatedly at time %f and with step "
         "size %f"
         "or with step size = minimum step size (%f).",
         stage_target_time, step_size, control_data.min_step_size);

      return;
   }
   // else:
   // Block F.
   // The following block handles the case of a successful return from the
   // core integrator (step_error = 0).  Test for stop conditions.;

   // 300
   initialized = true;
   switch(calculation_task) {

   //## ITASK = 1.  If TOUT has been reached, interpolate. -------------------;
   // 310
   case Normal:
      //
      //if ((stage_target_time - cycle_target_time)*step_size < 0.0),
      //stay in loop, otherwise perform actions and quit out.
      if ((stage_target_time - cycle_target_time)*step_size < 0.0) {
         //go to 250, stay in loop
         manager_integration_loop_part1();
      }
      else {
         interpolate_y ();
         // return with re_entry_point = CycleStartFinish still -
         //    indicates that cycle is complete.
      }
      break;

// See Implementation-Notes #2.
   case OneStep:
//      manager_set_calculation_phase_eq_2_reload(); // go to 400
//      break;
//   // 330
   case CompleteCycle:
//      if ((stage_target_time - cycle_target_time)*step_size >= 0.0) {
//         manager_set_calculation_phase_eq_2_reload(); // go to 400 => return
//      }
//      else {
//         // else, stay in loop,  go to 250
//         manager_integration_loop_part1();
//      }
//      break;
//
//   // 340
   case NormalWithSingularity:
//      if ((stage_target_time - cycle_target_time)*step_size >= 0.0) {
//                                                       // inverse go to 345
//         interpolate_y ();
//         current_time = cycle_target_time;
//         // go to 420
//         break;
//      }
//      // 345
//      t_crit_hit = ( std::abs(stage_target_time - t_crit) <=
//         (100.0*epsilon*
//            ( std::abs(stage_target_time) + std::abs(step_size))));
//      if (t_crit_hit) {
//         manager_set_calculation_phase_eq_2_reload(); // go to 400
//         break;
//      }
//
//      FIXME declare tnext outside the switch, but not while this is commented
//      or it will be "unused"
//      double tnext = stage_target_time + step_size*(1.0 + 4.0*epsilon);
//      if ((tnext - t_crit)*step_size > 0.0) {
//         step_size = (t_crit - stage_target_time)*(1.0 - 4.0*epsilon);
//         internal_state = -2;
//      }
//      // stay in loop, go to 250
//      manager_integration_loop_part1();
//      break;
//
////   # ITASK = 5.  See if TCRIT was reached and jump to exit. ---------------;
//   // 350
   case OneStepWithSingularity:
//      t_crit_hit = ( std::abs(stage_target_time - t_crit) <=
//           (100.0*epsilon*
//               (std::abs(stage_target_time) + std::abs(step_size))));
//      manager_set_calculation_phase_eq_2_reload();  // continues to 400
//      break;
   default:
      er7_utils::MessageHandler::fail (__FILE__, __LINE__,
         er7_utils::IntegrationMessages::invalid_request,
         "Illegal calculation task requested, calculation_task = %d.", calculation_task);
      break;
   }
   return;
}


/**
 * Resets the integrator when the timestep changes or when identified
 * as needing a reset.
 */
void
LsodeFirstOrderODEIntegrator::reset_integrator ()
{
   if (first_pass) {
     return;
   }
   stage_target_time = 0.0;
   cycle_target_time = 0.0;
   method_order_current = 0;
   prev_good_step_size = 0.0;
   control_data.initial_step_size = 0.0;
   for (unsigned int ii = 0; ii < control_data.num_odes; ii++) {
     for (unsigned int jj = 0; jj < 13; jj++) {
       arrays.history[ii][jj] = 0.0;
     }
   }
   re_entry_point = InitCalc;
   return;
}



/**
 *
 */
void
LsodeFirstOrderODEIntegrator::manager_set_calculation_phase_eq_2_reload()
{
//##-----------------------------------------------------------------------
//## Block G.
//## The following block handles all successful returns from DLSODE.
//## If ITASK != 1, Y is loaded from YH and T is set accordingly.
//## ISTATE is set to 2, and the optional outputs are loaded into the
//## work arrays before returning.
//##-----------------------------------------------------------------------


// See Implementation-Notes #2.
//   // 400
//   for (int ii = 0; ii < control_data.num_odes; ii++) { // do 410
//   // 410
//      y[ii] = arrays.history[ii][0];
//   }
//
//   current_time = stage_target_time;
//
//   // If monitoring critical time, and it has been hit, override current time
//   // with critical time
//
//   if ((calculation_task == NormalWithSingularity ||
//        calculation_task == OneStepWithSingularity) &&
//        t_crit_hit) {
//      current_time = t_crit;
//   }
//   // continues to 420
   return;
}


// Lines 500 - 600 are intended to provide an opportunity to change some
// integration parameters when the
// integrator runs into problems.  calculation_phase (ISTATE) gets set to
// something negative, which could
// trigger a pause in the execution until it gets fixed.
// However, we have no such capability with JEOD or Trick - the
// sim will just keep running, and when the
// next step is called with a negative calculation_phase (ISTATE),
// it will be failed.  So, it may as well
// be failed right here.
// Therefore, the recover aspect of this code block, lines
// 560-599, is redundant and has been axed.  The
// message aspect of the block is not sensibly constructed.
// Each message is called from only one place, so
// there is little point in moving the messages
// out of those locations.  They have been shifted back to
// their calling location.
//
// Likewise, with the 600-block of code; there is no
// meaningful retun from this block and for the most
// part, each message is called only once (with a couple
// of exceptions, which are called from two
// locations).  This block has also been redistributed.




/**
 * @}
 * @}
 * @}
 * @}
 */
