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
 * @file models/utils/integration/gauss_jackson/include/gauss_jackson_state_machine.hh
 * Defines the class GaussJacksonStateMachine, which guides
 * the Gauss-Jackson integration process.
 */


/*
Purpose: ()
Library dependencies:
  ((../src/gauss_jackson_state_machine.cc))
*/


#ifndef JEOD_GAUSS_JACKSON_STATE_MACHINE_HH
#define JEOD_GAUSS_JACKSON_STATE_MACHINE_HH

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// System includes
#include <string>


//! Namespace jeod 
namespace jeod {

class GaussJacksonConfig;

/**
 * Guides the behavior of the Gauss-Jackson integration process via
 * a finite state machine.
 *
 * The Gauss-Jackson integration process comprises four distinct modes:
 *  - Priming: Using an alternate integrator, the primer, to build the
 *    requisite number of data points needed by the initial Gauss-Jackson
 *    algorithm.
 *  - Editing: Using a Gauss-Jackson mid-corrector to make the collected
 *    data consistent with the Gauss-Jackson technique.
 *  - Stepping: Using a Gauss-Jackson predictor/corrector to build the
 *    requisite number of data points needed by the next step of the
 *    Gauss-Jackson algorithm.
 *  - Operational: Using the Gauss-Jackson predictor/corrector at the
 *    final user-specified time step and order.
 */
class GaussJacksonStateMachine {
JEOD_MAKE_SIM_INTERFACES(GaussJacksonStateMachine)
public:

   /**
    * Specifies the Gauss-Jackson finite state machine states.
    */
   enum FsmState {
      Reset,          //!< Module was just commanded to reset itself.
      Priming,        //!< Using primer to build initial set of data.
      BootstrapEdit,  //!< Editing primer / lower-level Gauss-Jackson data.
      BootstrapStep,  //!< Building toward downsample / change in order.
      Operational     //!< At desired rate and order.
   };


   /**
    * Translates a finite state machine state value to a string.
    */
   static std::string state_name (FsmState state);


   /**
    * Default constructor.
    */
   GaussJacksonStateMachine ();

   // Note: The destructor, copy constructor, and assignment operator are not
   // declared. The implicitly defined implementations suit just right.


   // Getters and setters. Note that, except for bootstrap_edit_redo_needed,
   // there are no setters.

   /**
    * Get the finite state machine state.
    */
   FsmState get_fsm_state () const
   { return fsm_state; }

   /**
    * Get the maximum history size.
    */
   unsigned int get_max_history_size () const
   { return max_history_size; }

   /**
    * Get the current order.
    */
   unsigned int get_current_order () const
   { return current_order; }

   /**
    * Get the current history length.
    */
   unsigned int get_history_length () const
   { return history_length; }

   /**
    * Get the current time scale factor.
    */
   double get_cycle_scale () const
   { return cycle_scale; }

   /**
    * Get the current cycle start time.
    */
   double get_cycle_start_time () const
   { return cycle_start_time; }

   /**
    * Get the at_downsample flag.
    */
   bool get_at_downsample () const
   { return at_downsample; }

   /**
    * Get the at_reinitialize flag.
    */
   bool get_at_reinitialize () const
   { return at_reinitialize; }

   /**
    * Get the at_order_change flag.
    */
   bool get_at_order_change () const
   { return at_order_change; }

   /**
    * Get the at_end_of_tour flag.
    */
   bool get_at_end_of_tour () const
   { return at_end_of_tour; }

   /**
    * Tell the state machine that the edit did not pass a convergence test.
    */
   void set_bootstrap_edit_redo_needed ();


   /**
    * Configure (or reconfigure) the Gauss-Jackson state machine.
    */
   void configure (const GaussJacksonConfig & config);

   /**
    * Reset the Gauss-Jackson state machine.
    */
   void reset ();

   /**
    * Advance the state machine by one step.
    */
   void perform_step ();


private:

   // Member data.


   /**
    * The order to be used immediately after priming is complete.
    * This must be an even, non-negative integer.
    */
   unsigned int initial_order; //!< trick_units(--)

   /**
    * The order to be used in operational mode.
    * This must be an even integer and must not be less than initial_order.
    */
   unsigned int final_order; //!< trick_units(--)

   /**
    * The number of times the time step is to be doubled between priming
    * and operational modes.
    */
   unsigned int ndoubling_steps; //!< trick_units(--)

   /**
    * The maximum number of corrections to be performed.
    */
   unsigned int max_correction_iterations; //!< trick_units(--)

   /**
    * The maximum history size. This is calculated for the benefit of
    * state integrators.
    */
   unsigned int max_history_size; //!< trick_units(--)

   /**
    * The number of small steps that represent a step to the simulation engine,
    * 2**n_doubling_steps.
    */
   unsigned int tour_count; //!< trick_units(--)


   /**
    * The finite state machine state.
    */
   FsmState fsm_state; //!< trick_units(--)

   /**
    * The current order. This is incremented by two on transitions from
    * BootstrapStep to BootstrapEdit until the final_order is reached.
    */
   unsigned int current_order; //!< trick_units(--)

   /**
    * The current history size, the number of history elements that must be
    * be accumulated to transition to the next state.
    */
   unsigned int history_size; //!< trick_units(--)

   /**
    * The current history length, the number of history elements that have
    * been accumulated so far.
    */
   unsigned int history_length; //!< trick_units(--)

   /**
    * A power of two that starts at 2**ndoubling_steps and is halved with each
    * downsample. When the scale_factor reaches 1 it is time to transition to
    * operational mode.
    */
   unsigned int scale_factor; //!< trick_units(--)

   /**
    * A power of two that starts at 1 and is doubled with each downsample.
    */
   unsigned int step_increment; //!< trick_units(--)

   /**
    * The number of steps since the reset, measured in units of priming cycle
    * steps. The counter is incremented by the step_increment upon completion
    * of a cycle and is reset to zero on entry into BootstrapEdit.
    */
   unsigned int steps_since_reset; //!< trick_units(--)

   /**
    * The number of correction iterations made during BoostrapEdit.
    */
   unsigned int correction_iterations; //!< trick_units(--)


   /**
    * The unitless time step size of the current integration cycle, measured
    * in integration tour time step units. This starts at 2**(-ndoubling_steps)
    * and doubles with each downsample.
    */
   double cycle_scale; //!< trick_units(--)

   /**
    * The unitless start time of the current integration cycle, measured
    * in integration tour time step units.
    */
   double cycle_start_time; //!< trick_units(--)


   /**
    * Flag indicating that the current edit sequence has failed to converge.
    * This flag is set externally by the Gauss-Jackson integration controls.
    */
   bool bootstrap_edit_redo_needed; //!< trick_units(--)

   /**
    * Flag indicating that history data are to be downsampled and the time step
    * is to be doubled. The flag is set on transitions from BootstrapStep to
    * BootstrapEdit when the step size has not yet reached the desired value,
    * clear otherwise.
    */
   bool at_downsample; //!< trick_units(--)

   /**
    * Flag indicating that the Gauss-Jackson integration constants are to be
    * reinitialized. The flag is set on entry to any state except Reset and
    * Priming, clear otherwise.
    */
   bool at_reinitialize; //!< trick_units(--)

   /**
    * Flag indicating that the order is to be increased. The flag is set on
    * on transitions from BootstrapStep to BootstrapEdit when the order has
    * has not yet reached the desired value, clear otherwise.
    */
   bool at_order_change; //!< trick_units(--)

   /**
    * Flag indicating that the current integration cycle is the last one in
    * an integration tour (i.e., that a major time step will be completed).
    * The flag is set at the start of the cycle that completes the tour,
    * clear otherwise. This flag is never set during BootstrapEdit.
    */
   bool at_end_of_tour; //!< trick_units(--)


   // Member functions

   /**
    * Make a state transition.
    */
   void transition_state ();

   /**
    * Make the transition out of Priming.
    */
   void exit_priming ();

   /**
    * Make a transition out of BootstrapEdit.
    */
   void exit_bootstrap_edit ();

   /**
    * Make a transition out of BootstrapStep.
    */
   void exit_bootstrap_step ();
};

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
