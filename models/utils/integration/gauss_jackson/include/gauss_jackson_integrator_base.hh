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
 * @file models/utils/integration/gauss_jackson/include/gauss_jackson_integrator_base.hh
 * Defines the template class GaussJacksonIntegratorBase,
 * which provides the basis for Gauss-Jackson integration.
 */


/*
Purpose: ()
*/

#ifndef JEOD_GAUSS_JACKSON_INTEGRATOR_BASE_HH
#define JEOD_GAUSS_JACKSON_INTEGRATOR_BASE_HH

// Local includes
#include "two_d_array.hh"
#include "gauss_jackson_coeffs.hh"
#include "gauss_jackson_config.hh"
#include "gauss_jackson_integration_controls.hh"
#include "gauss_jackson_state_machine.hh"

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// ER7 utilities includes
#include "er7_utils/integration/core/include/integrator_result.hh"
#include "er7_utils/integration/core/include/integ_utils.hh"
#include "er7_utils/interface/include/alloc.hh"

// System includes
#include <algorithm>
#include <cassert>


//! Namespace jeod 
namespace jeod {

/**
 * Base template class for integrating state via the Gauss-Jackson technique.
 * @tparam  State   Structure that contains the state.
 * @tparam  Primer  Class for priming the Gauss-Jackson integrator.
 */
template<
   typename State,
   typename Primer>
class GaussJacksonIntegratorBase {

public:

   // Member data.
   // Note: The data members need to be public so that Trick can see them.

   /**
    * The summed Adams and Gauss-Jackson coefficients, in ordinate form.
    */
   const GaussJacksonCoeffs * coeff; //!< trick_units(--)

   /**
    * The Gauss-Jackson state machine.
    */
   const GaussJacksonStateMachine * state_machine; //!< trick_units(--)

   /**
    * The integrator used to prime the Gauss-Jackson integration process.
    */
   Primer * primer; //!< trick_units(--)

   /**
    * The state at the time of the last reset.
    */
   State init_state; //!< trick_units(--)

   /**
    * Inverse backward differences.
    */
   State delinv; //!< trick_units(--)

   /**
    * Speed hack for the corrector.
    */
   State corrector_sum; //!< trick_units(--)

   /**
    * Acceleration history.
    */
   er7_utils::DoubleTwoDArray acc_hist; //!< trick_units(--)

   /**
    * Position history (or velocity history in case of a first order ODE).
    */
   er7_utils::DoubleTwoDArray pos_hist; //!< trick_units(--)

   /**
    * Number that indicates the allowable relative difference for two
    * states to be considered converged.
    */
   double relative_tolerance; //!< trick_units(--)

   /**
    * Number that indicates the allowable absolute difference for two
    * states to be considered converged.
    */
   double absolute_tolerance; //!< trick_units(--)

   /**
    * Correction coefficient for the first integral (velocity).
    */
   double velocity_corrector; //!< trick_units(--)

   /**
    * Correction coefficient for the second integral (position).
    */
   double position_corrector; //!< trick_units(--)

   /**
    * Finite state machine state.
    */
   GaussJacksonStateMachine::FsmState fsm_state; //!< trick_units(--)

   /**
    * Maximum history size.
    */
   unsigned int max_history_size; //!< trick_units(--)

   /**
    * Initial order.
    */
   unsigned int initial_order; //!< trick_units(--)

   /**
    * Current order.
    */
   unsigned int order; //!< trick_units(--)

   /**
    * State size.
    */
   unsigned int size; //!< trick_units(--)

   /**
    * Current history length.
    */
   unsigned int history_length; //!< trick_units(--)


   // Constructors, destructor, etc.

   /**
    * Default constructor.
    */
   GaussJacksonIntegratorBase ()
   :
      coeff (NULL),
      state_machine (NULL),
      primer (NULL),

      init_state (),
      delinv (),
      corrector_sum (),
      acc_hist (),
      pos_hist (),

      relative_tolerance (0.0),
      absolute_tolerance (0.0),

      velocity_corrector (0.0),
      position_corrector (0.0),

      fsm_state (GaussJacksonStateMachine::Reset),
      max_history_size (0),
      initial_order (0),
      order (0),
      size (0),
      history_length (0)
   {}


   /**
    * Non-default constructor.
    * @param  priming_constructor  Integrator constructor for the technique
    *                              used during priming.
    * @param  controls             The Gauss-Jackson integration controls that
    *                              drives this state integrator.
    * @param  size_in              State size.
    * @param  priming_controls     Integration controls used during priming.
    */
   GaussJacksonIntegratorBase (
      const er7_utils::IntegratorConstructor & priming_constructor,
      const GaussJacksonIntegrationControls & controls,
      unsigned int size_in,
      er7_utils::IntegrationControls & priming_controls)
   :
      coeff (&(controls.get_coeff())),
      state_machine (&(controls.get_state_machine())),

      primer (NULL),

      init_state (),
      delinv (),
      corrector_sum (),
      acc_hist (),
      pos_hist (),

      relative_tolerance (controls.get_config().relative_tolerance),
      absolute_tolerance (controls.get_config().absolute_tolerance),

      velocity_corrector (0.0),
      position_corrector (0.0),

      fsm_state (GaussJacksonStateMachine::Reset),
      max_history_size (state_machine->get_max_history_size()),
      initial_order (controls.get_config().initial_order),
      order (0),
      size (size_in),
      history_length (0)
   {
      // Create the primer.
      primer = create_primer (priming_constructor, size_in, priming_controls);

      // Allocate State items.
      allocate_state_contents (init_state);
      allocate_state_contents (delinv);
      allocate_state_contents (corrector_sum);

      // Allocate histories.
      acc_hist.allocate (max_history_size, size);
      pos_hist.allocate (max_history_size, size);
   }


   /**
    * Copy constructor.
    * @param  src  Item to be copied.
    */
   GaussJacksonIntegratorBase (
      const GaussJacksonIntegratorBase & src)
   :
      coeff (src.coeff),
      state_machine (src.state_machine),

      primer (NULL),

      init_state (),
      delinv (),
      corrector_sum (),
      acc_hist (src.acc_hist),
      pos_hist (src.pos_hist),

      relative_tolerance (src.relative_tolerance),
      absolute_tolerance (src.absolute_tolerance),

      velocity_corrector (src.velocity_corrector),
      position_corrector (src.position_corrector),

      fsm_state (src.fsm_state),
      max_history_size (src.max_history_size),
      initial_order (src.initial_order),
      order (src.order),
      size (src.size),
      history_length (src.history_length)
   {
      primer = replicate_primer (src.primer);

      replicate_state (src.init_state, init_state);
      replicate_state (src.delinv, delinv);
      replicate_state (src.corrector_sum, corrector_sum);
   }

   /**
    * Destructor.
    */
   ~GaussJacksonIntegratorBase ()
   {
      er7_utils::alloc::delete_object (primer);

      deallocate_state_contents (init_state);
      deallocate_state_contents (delinv);
      deallocate_state_contents (corrector_sum);
   }


protected:


   /**
    * Reset the integrator.
    */
   void base_reset ()
   {
      fsm_state = GaussJacksonStateMachine::Reset;
      history_length = 0;
      order = initial_order;
   }

   /**
    * Propagate state to the specified target_stage.
    * @param[in]     dyn_dt        Dynamic time step, in dynamic time seconds.
    * @param[in]     target_stage  The stage of the integration process
    *                              that the integrator should try to attain.
    * @param[in]     deriv         Acceleration vector.
    * @param[in,out] state         State vector(s).
    *
    * @return The status (time advance, pass/fail status) of the integration.
    */
   er7_utils::IntegratorResult base_integrate (
      double dyn_dt,
      unsigned int target_stage,
      double const * ER7_UTILS_RESTRICT deriv,
      State state)
   {
      er7_utils::IntegratorResult result;
      bool passed = true;

      // Optimization for operational mode.
      // The integrator will spend most of its time in this mode.
      // Note that this only applies if the state is already operational.
      // It does not cover the very first step in operational mode;
      // that transition is covered in the else below.
      if (fsm_state == GaussJacksonStateMachine::Operational) {

         // Perform start of cycle operations at the start of a cycle:
         // Rotate the acceleration history and save the incoming accels.
         // This is what start_cycle() would do, but that function would do so
         // with a meaningless function call and a bunch of meaningless tests.
         if (target_stage == 1) {
            rotate_acc_hist ();
            er7_utils::integ_utils::copy_array (
               deriv, size, acc_hist[order]);
         }

         // Integrate state via the Gauss-Jackson integrator.
         passed = integrate_gj (
                     dyn_dt,       // Dynamic time step
                     target_stage, // 1=predict, 2=correct
                     order,        // acc_hist index to be used for advance
                     order,        // pos_hist index for computed state
                     deriv,        // Input derivatives
                     acc_hist,     // Acceleration history (no offset)
                     state);       // Output state
      }

      // Things are a bit hairier until we reach operational mode.
      else {

         // Perform start of cycle operations at the start of a cycle.
         // Note that each edit step is at start of cycle.
         // Also note that this can transition to operational mode.
         if (target_stage == 1) {
            start_cycle (dyn_dt, deriv, state);
         }

         // Integrate per the state machine's state.
         switch (fsm_state) {

         // Priming: Integrate via the primer, saving the integrated state
         // for use during BootstrapEdit.
         case GaussJacksonStateMachine::Priming:
            result = integrate_primer (dyn_dt, target_stage, deriv, state);
            save_comparison_data (state, pos_hist[history_length]);
            break;

         // BootstrapEdit: Edit the point specified by history_length.
         case GaussJacksonStateMachine::BootstrapEdit:
            result.set_time_scale (history_length);
            passed = edit_point (dyn_dt, deriv, state);
            break;

         // BootstrapStep: Integrate via Gauss-Jackson proper,
         // using the order+1 most recent history points.
         // Note: acc_hist+history_length-(order+1) points to the start of the
         // subarray that contains the order+1 most recent history points.
         case GaussJacksonStateMachine::BootstrapStep:
            passed = integrate_gj (
                        dyn_dt,           // Dynamic time step
                        target_stage,     // 1=predict, 2=correct
                        history_length-1, // Index to be used for advance
                        history_length,   // pos_hist index for computed state
                        deriv,            // Input derivatives
                        acc_hist +        // Acceleration history
                           history_length-(order+1), // offset
                        state);           // Output state
            break;

         // Operational: Integrate via Gauss-Jackson proper.
         // History always starts at element 0 in operational mode.
         // Note that this case will be hit once. After that the code will
         // use the operational mode optimization above.
         case GaussJacksonStateMachine::Operational:
            passed = integrate_gj (
                        dyn_dt,       // Dynamic time step
                        target_stage, // 1=predict, 2=correct
                        order,        // acc_hist index to be used for advance
                        order,        // pos_hist index for computed state
                        deriv,        // Input derivatives
                        acc_hist,     // Acceleration history (no offset)
                        state);       // Output state
            break;
         case GaussJacksonStateMachine::Reset:
         default:
            assert ("Internal error" == NULL);
            break;
         }
      }

      // Mark the result as failed if the integration didn't converge.
      if (! passed) {
         result.set_failed ();
      }
      return result;
   }

   /**
    * Non-throwing swap.
    * @param  other  Item whose contents are to be swapped with this.
    */
   void swap (
      GaussJacksonIntegratorBase & other)
   {
      std::swap (coeff, other.coeff);
      std::swap (state_machine, other.state_machine);

      std::swap (primer, other.primer);

      swap_state (init_state, other.init_state);
      swap_state (delinv, other.delinv);
      swap_state (corrector_sum, other.corrector_sum);

      acc_hist.swap (other.acc_hist);
      pos_hist.swap (other.pos_hist);

      std::swap (relative_tolerance, other.relative_tolerance);
      std::swap (absolute_tolerance, other.absolute_tolerance);
      std::swap (velocity_corrector, other.velocity_corrector);
      std::swap (position_corrector, other.position_corrector);
      std::swap (fsm_state, other.fsm_state);
      std::swap (max_history_size, other.max_history_size);
      std::swap (initial_order, other.initial_order);
      std::swap (order, other.order);
      std::swap (size, other.size);
      std::swap (history_length, other.history_length);
   }


private:

   // Static functions.
   // The implementations are specific to the template instantiation.

   /**
    * Create the integrator to be used during priming.
    *
    * @param  priming_constructor  Integrator constructor for the technique
    *                              used during priming.
    * @param  size_in              State size.
    * @param  priming_controls     Integration controls used during priming.
    *
    * @return Constructed primer.
    */
   static Primer* create_primer (
      const er7_utils::IntegratorConstructor & priming_constructor,
      unsigned int size_in,
      er7_utils::IntegrationControls & priming_controls);

   /**
    * Create a replica of the provided primer.
    * @param  src  Primer to be replicated.
    * @return Constructed primer.
    */
   static Primer* replicate_primer (
      const Primer * src);


   // Member functions.

   /**
    * Start an integration cycle.
    * @param[in]  dt             Dynamic time step, in dynamic time seconds.
    * @param[in]  acc            Acceleration vector.
    * @param[in]  state          State vector(s).
    */
   void start_cycle (
      double dt,
      const double* ER7_UTILS_RESTRICT acc,
      State & state)
   {
      // Reset: Save the epoch point and the initial acceleration.
      if (fsm_state == GaussJacksonStateMachine::Reset) {
         save_epoch_data (acc, state);
         history_length = 1;
      }

      // Non-reset start of cycle: Save incoming acceleration.
      else {
         er7_utils::integ_utils::copy_array (
            acc, size, acc_hist[history_length]);
         ++history_length;
      }

      // Set the state per the state machine.
      fsm_state = state_machine->get_fsm_state();

      // Downsample if so indicated.
      if (state_machine->get_at_downsample()) {
         downsample_hist ();
      }

      // Change order if so indicated.
      if (state_machine->get_at_order_change()) {
         order = state_machine->get_current_order();
         velocity_corrector = 1.0 + coeff->corrector[order].sa_coefs[order];
         position_corrector = coeff->corrector[order].gj_coefs[order];
      }

      // Reinitialize the integration constants if so indicated.
      if (state_machine->get_at_reinitialize()) {
         if (fsm_state == GaussJacksonStateMachine::BootstrapEdit) {
            initialize_edit_integration_constants (dt);
            history_length = 1;
         }
         else {
            initialize_predictor_integration_constants (dt);
         }
      }
   }


   /**
    * Edit the specified point using the mid-corrector that pertains
    * to the point being edited, which is that at history_length.
    * @param[in]  dt             Dynamic time step, in dynamic time seconds.
    * @param[in]  acc            Acceleration vector.
    * @param[out] state          State vector(s).
    */
   bool edit_point (
      double dt,
      const double* ER7_UTILS_RESTRICT acc ER7_UTILS_UNUSED,
      State & state)
   {
      assert (history_length <= order);
      advance_edit_integration_constants (history_length);
      mid_correct (history_length, dt, state);
      return test_for_convergence (state, pos_hist[history_length]);
   }

   /**
    * Integrate using the Gauss-Jackson predictor and corrector.
    * @param[in]  dt             Dynamic time step, in dynamic time seconds.
    * @param[in]  target_stage   The stage of the integration process
    *                            that the integrator should try to attain.
    * @param[in]  advance_index  Acceleration history index.
    * @param[in]  target_index   Position history index.
    * @param[in]  acc            Acceleration vector.
    * @param[in]  ahist          Acceleration vector history.
    * @param[out] state          State vector(s).
    *
    * @return True if step was successful, false otherwise.
    */
   bool integrate_gj (
      double dt,
      unsigned int target_stage,
      int advance_index,
      int target_index,
      const double* ER7_UTILS_RESTRICT acc,
      const double* const * ahist,
      State & state)
   {
      if (target_stage == 1) {
         advance_predictor_integration_constants (advance_index);
         predict (dt, ahist, state);
         save_comparison_data (state, pos_hist[target_index]);

         coeff->corrector[order].apply (size, order, ahist+1, corrector_sum);
         return true;
      }
      else {
         correct (dt, acc, state);
         return test_for_convergence (state, pos_hist[target_index]);
      }
   }

   /**
    * Downsample the acceleration and position histories.
    */
   void downsample_hist ()
   {
      assert (history_length & 1);
      int new_len = (history_length+1)/2;
      pos_hist.downsample (new_len);
      acc_hist.downsample (new_len);
      history_length = new_len;
   }

   /**
    * Rotate the acceleration history.
    */
   void rotate_acc_hist ()
   {
      acc_hist.rotate_down (order);
   }


   // The remaining member functions are template-argument specific.

   /**
    * Integrate state using the primer.
    * @param[in]     dyn_dt        Dynamic time step, in dynamic time seconds.
    * @param[in]     target_stage  The stage of the integration process
    *                              that the integrator should try to attain.
    * @param[in]     deriv         Acceleration vector.
    * @param[in,out] state         State vector(s).
    *
    * @return The status (time advance, pass/fail status) of the integration.
    */
   er7_utils::IntegratorResult integrate_primer (
      double dyn_dt,
      unsigned int target_stage,
      double const * deriv,
      State & state);

   /**
    * Save epoch data.
    * @param  acc            Acceleration to be saved.
    * @param  state          State to be saved.
    */
   void save_epoch_data (
      const double * acc,
      const State & state);

   /**
    * Save comparison data.
    * @param  state          State to be saved.
    * @param  pos_hist_elem  Element of the position history to be updated.
    */
   void save_comparison_data (
      const State & state,
      double * pos_hist_elem);

   /**
    * Initialize the integration constants (i.e., delinv).
    * @param  dt         Dynamic time step.
    */
   void initialize_edit_integration_constants (double dt);

   /**
    * Initialize the integration constants (i.e., delinv).
    * @param  dt         Dynamic time step.
    */
   void initialize_predictor_integration_constants (double dt);

   /**
    * Advance the integration constants by one cycle.
    * @param  index  Coefficient index.
    */
   void advance_edit_integration_constants (unsigned int index);

   /**
    * Advance the integration constants by one cycle.
    * @param  index  Coefficient index.
    */
   void advance_predictor_integration_constants (unsigned int index);

   /**
    * Apply a mid-corrector.
    * @param  coeff_idx  Coefficient index; item to be corrected.
    * @param  dt         Dynamic time step.
    * @param  state      Corrected state.
    */
   void mid_correct (
      unsigned int coeff_idx,
      double dt,
      State & state);

   /**
    * Apply the predictor.
    * @param  dt     Dynamic time step.
    * @param  ahist  Acceleration history.
    * @param  state  Corrected state.
    */
   void predict (
      double dt,
      double const * const * ahist,
      State & state);

   /**
    * Apply the corrector.
    * @param  dt     Dynamic time step.
    * @param  acc    Acceleration data.
    * @param  state  Corrected state.
    */
   void correct (
      double dt,
      const double* ER7_UTILS_RESTRICT acc,
      State & state);

   /**
    * Test for convergence.
    * @param  state      Item to be compared.
    * @param  hist_data  Previous state value.
    */
   bool test_for_convergence (
      const State & state,
      double * ER7_UTILS_RESTRICT hist_data);


   /**
    * Swap state data with another of the same.
    * @param  item        State item.
    * @param  other_item  The other state item.
    */
   void swap_state (State & item, State & other_item);

   /**
    * Replicate state data.
    * @param  source  State item to be copied.
    * @param  target  Replicated state item.
    */
   void replicate_state (const State & source, State & target);

   /**
    * Allocate memory for a state item.
    * @param  item  State item to be allocated.
    */
   void allocate_state_contents (State & item);

   /**
    * Deallocate state item memory.
    * @param  item  State item to be deallocated.
    */
   void deallocate_state_contents (State & item);


   // Unimplemented functions.

   /**
    * Not implemented.
    */
   GaussJacksonIntegratorBase& operator= (const GaussJacksonIntegratorBase &);
};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
