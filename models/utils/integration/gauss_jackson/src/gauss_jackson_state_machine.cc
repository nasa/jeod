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
 * @file models/utils/integration/gauss_jackson/src/gauss_jackson_state_machine.cc
 * Defines member functions for the class GaussJacksonStateMachine.
 */


/*
Purpose: ()
*/


#include "../include/gauss_jackson_state_machine.hh"
#include "../include/gauss_jackson_config.hh"

#include <algorithm>
#include <cassert>
#include <string>



//! Namespace jeod 
namespace jeod {

std::string
GaussJacksonStateMachine::state_name (FsmState state)
{
   switch (state) {
   case Reset:          return "Reset";          break;
   case Priming:        return "Priming";        break;
   case BootstrapEdit:  return "BootstrapEdit";  break;
   case BootstrapStep:  return "BootstrapStep";  break;
   case Operational:    return "Operational";    break;
   default:             return "Unknown";        break;
   }
}


GaussJacksonStateMachine::GaussJacksonStateMachine ()
:
   initial_order(0),
   final_order(0),
   ndoubling_steps(0),
   max_correction_iterations(0),
   max_history_size(0),
   tour_count(0),

   fsm_state(Reset),

   current_order(0),
   history_size(0),
   history_length(0),
   scale_factor(0),

   step_increment(0),
   steps_since_reset(0),
   correction_iterations(0),

   cycle_scale(0.0),
   cycle_start_time(0.0),

   bootstrap_edit_redo_needed(false),
   at_downsample(false),
   at_reinitialize(false),
   at_order_change(false),
   at_end_of_tour(false)
{
}


void
GaussJacksonStateMachine::configure (
   const GaussJacksonConfig & config)
{
   initial_order = config.initial_order;
   final_order = config.final_order;
   ndoubling_steps = config.ndoubling_steps;
   max_correction_iterations = config.max_correction_iterations;

   unsigned int last_doubling_order =
      (ndoubling_steps == 0) ?
      0 : std::min (final_order, initial_order+2*ndoubling_steps);
   max_history_size = std::max (2*last_doubling_order, final_order) + 1;

   tour_count = 1 << ndoubling_steps;
}


void
GaussJacksonStateMachine::reset ()
{
   // "Reset" is the starting point of the state machine.
   // Any history collected is now trash, so history_length is set to zero.
   // Setting history_size to one makes it so that the next call to perform_step
   // will result in a state transition out of Reset (to Priming).
   fsm_state = Reset;
   history_length = 0;
   history_size = 1;

   step_increment = 1;
   steps_since_reset = 0;

   current_order = initial_order;

   scale_factor = tour_count;
   cycle_scale = 1.0 / double(tour_count);
   cycle_start_time = 0.0;

   at_downsample = false;
   at_reinitialize = false;
   at_order_change = false;
   at_end_of_tour = false;
}


void
GaussJacksonStateMachine::set_bootstrap_edit_redo_needed ()
{
   assert (fsm_state == BootstrapEdit);
   assert (correction_iterations < max_correction_iterations);
   bootstrap_edit_redo_needed = true;
}


void
GaussJacksonStateMachine::perform_step ()
{
   at_downsample = false;
   at_reinitialize = false;
   at_order_change = false;
   at_end_of_tour = false;

   ++history_length;

   if (history_length == history_size) {
      transition_state ();
   }

   if (fsm_state == BootstrapEdit) {
      steps_since_reset = history_length * step_increment;
      cycle_start_time = double(steps_since_reset) / double(tour_count);
   }
   else {
      cycle_start_time = double(steps_since_reset) / double(tour_count);
      steps_since_reset += step_increment;
      assert ((steps_since_reset == history_length * step_increment));
      at_end_of_tour = (steps_since_reset % tour_count) == 0;
   }
}


void
GaussJacksonStateMachine::transition_state ()
{
   switch (fsm_state) {

   // Reset:
   // Transition to Priming, with a history length of initial_order+1 needed to
   // make the next transition (which will be to BootstrapEdit).
   case Reset :
      fsm_state = Priming;
      current_order = 0;
      history_size = initial_order+1;
      steps_since_reset = 0;
      break;

   // Priming:
   // Transition to BootstrapEdit.
   case Priming:
      exit_priming ();
      break;

   // BootstrapEdit:
   // Transition out of edit if the edit succeeded, redo otherwise.
   case BootstrapEdit:
      exit_bootstrap_edit ();
      break;

   // BootstrapStep:
   // Transition to BootstrapEdit, but with added twists of
   // downsampling/step doubling and/or increasing the order.
   case BootstrapStep:
      exit_bootstrap_step ();
      break;

   // Operational: You can checkout but you can never leave.
   case Operational:
      break;

   default:
      assert("State machine called incorrectly" == nullptr);
      break;
   }
}


void
GaussJacksonStateMachine::exit_priming ()
{
   current_order = initial_order;
   at_order_change = true;
   at_reinitialize = true;

   // Corrections enabled: Transition to BootstrapEdit.
   if (max_correction_iterations > 0) {
      bootstrap_edit_redo_needed = false;
      history_length = 1;
      correction_iterations = 1;
      fsm_state = BootstrapEdit;
   }

   // Corrections disabled: Act as if edit has succeeded.
   else {
      bootstrap_edit_redo_needed = false;
      exit_bootstrap_edit();
   }
}


void
GaussJacksonStateMachine::exit_bootstrap_edit ()
{
   assert (history_length == history_size);
   assert (history_size == current_order+1);
   assert (steps_since_reset == step_increment*(history_length - 1));

   at_reinitialize = true;

   // Edit failed:
   // Redo the edit, but now using the newly collected derivatives.
   if (bootstrap_edit_redo_needed) {
      bootstrap_edit_redo_needed = false;
      history_length = 1;
      ++correction_iterations;
      fsm_state = BootstrapEdit;
   }

   // Edit passed, and at final order, final step size:
   // Transition to Operational.
   else if ((scale_factor == 1) && (current_order == final_order)) {
      fsm_state = Operational;
   }

   // Edit passed, but not at both final order, final step size:
   // Transition to BootstrapStep.
   else {
      fsm_state = BootstrapStep;

      // Set the history size to that needed for the action at completion
      // of BootstrapStep, which will be to bump order by two,
      // downsample and double the step size, or both.
      if (scale_factor == 1) {
         // Change order only:
         // Need history_size-1 = current_order + 2
         history_size = (current_order+2) + 1;
      }
      else if (current_order == final_order) {
         // Downsample only:
         // Need (history_size-1)/2 = current_order
         history_size = 2*current_order + 1;
      }
      else {
         // Downsample and change order:
         // Need (history_size-1)/2 = current_order+2
         history_size = 2*(current_order+2) + 1;
      }
   }
}


void
GaussJacksonStateMachine::exit_bootstrap_step ()
{
   at_reinitialize = true;

   // Downsample if not at the final step size.
   if (scale_factor != 1) {
      at_downsample = true;
      history_size = (history_size+1)/2;
      scale_factor /= 2;
      step_increment *= 2;
      cycle_scale *= 2.0;
   }

   // Increase the order if not at the final order.
   if (current_order != final_order) {
      at_order_change = true;
      current_order += 2;
   }

   // At least one of the above should be the case.
   assert (at_downsample || at_order_change);

   // Corrections enabled: Transition to BootstrapEdit.
   if (max_correction_iterations > 0) {
      bootstrap_edit_redo_needed = false;
      history_length = 1;
      correction_iterations = 1;
      fsm_state = BootstrapEdit;
   }

   // Corrections disabled: Act as if edit has succeeded.
   else {
      bootstrap_edit_redo_needed = false;
      exit_bootstrap_edit();
   }
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 * @}
 */
