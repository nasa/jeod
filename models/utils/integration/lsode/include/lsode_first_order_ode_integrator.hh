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
 * @addtogroup Lsode
 * @{
 *
 * @file models/utils/integration/lsode/include/lsode_first_order_ode_integrator.hh
 * Define the class LsodeFirstOrderODEIntegrator, the Jeod-compatible
 * version of the Livermore ODE solver, LSODE.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Library dependencies:
  ((lsode_first_order_ode_integrator__integrator.o)
   (lsode_first_order_ode_integrator__manager.o)
   (lsode_first_order_ode_integrator__support.o)
   (lsode_first_order_ode_integrator__utility.o)
   (lsode_data_classes.o)
   (lsode_control_data_interface.o))

 

*******************************************************************************/


#ifndef JEOD_LSODE_FIRST_ORDER_ODE_INTEGRATOR_HH
#define JEOD_LSODE_FIRST_ORDER_ODE_INTEGRATOR_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Integration includes
#include "er7_utils/integration/core/include/first_order_ode_integrator.hh"
#include "er7_utils/integration/core/include/integrator_result.hh"
#include "er7_utils/integration/core/include/integration_controls.hh"

// Model includes
#include "lsode_control_data_interface.hh"
#include "lsode_data_classes.hh"
#include "lsode_integration_controls.hh"

namespace jeod
{
/**
 * Jeod-compatible version of the Livermore ODE solver, LSODE.
 */
class LsodeFirstOrderODEIntegrator : public er7_utils::FirstOrderODEIntegrator
{

// JEOD_MAKE_SIM_INTERFACES(LsodeFirstOrderODEIntegrator)
JEOD_MAKE_SIM_INTERFACES(LsodeFirstOrderODEIntegrator)

 //Enumerations:
 public:

   /**
    *
    */
   enum EntryPoint {
     CycleStartFinish= 0,  ///< Default value.  Assumption is that the current
                           //   pass will result in the completion of a cycle;
                           //   the value is reset otherwise
     InitCalc        = 1,  ///< Reset during initialization.  Valid only during
                           //   first pass when the integrator is being
                           //   initialized
     JacobianPrep    = 2,  ///< Set at the end of the initialization of the
                           //   Jacobian-preparation sequences (DPREPJ).
                           //   This value is then retained for as long as the
                           //   DPREPJ loop keeps going.
     ResetIterLoop   = 3,  ///< Set when the iteration loop (part of DSTODE) has
                           //   been reset.
     IterationLoop   = 4,  ///< Set during the routine operation of the iteration
                           //   loop (DSTODE) when another iteration is needed.
     DstodeResetStep = 5   ///< Set in dstode_640  when there have been too many
                           //   failures and the step size has to be reduced.
   };

   /**
    *
    */
   enum CalculationTask {
      Normal                  = 1, ///< Normal operation. Interpolate to target
                                   //   time.
                                   //   Only mode currently supported
      OneStep                 = 2, ///< Take only one step and return.
                                   //   NOT SUPPORTED
      CompleteCycle           = 3, ///< Stop at first mesh point at or beyond
                                   //   cycle_target_time and return
                                   //   NOT SUPPORTED
      NormalWithSingularity   = 4, ///< Normal computation, with safeguard on
                                   //   singularity time, t_crit
                                   //   NOT SUPPORTED
      OneStepWithSingularity  = 5  ///< Take one step without passing t_crit.
                                   //   NOT SUPPORTED
   };

// Methods:
public:
   // Constructors and destructor.

   /**
    * LsodeFirstOrderODEIntegrator default constructor.
    */
   LsodeFirstOrderODEIntegrator(void);


   /**
    * LsodeFirstOrderODEIntegrator non-default constructor.
    * @param[in]     size      State size
    * @param[in,out] controls  Integration controls
    */
   LsodeFirstOrderODEIntegrator(  const LsodeControlDataInterface & data_in,
                                  er7_utils::IntegrationControls & controls,
                                  unsigned int size);
   /**
    * LsodeFirstOrderODEIntegrator destructor.
    */
   virtual ~LsodeFirstOrderODEIntegrator();


   // Member functions.

private:

   /**
    * Deleted assignment operator.
    */
   LsodeFirstOrderODEIntegrator & operator=(const LsodeFirstOrderODEIntegrator &);


   /**
    * Deleted copy constructor.
    */
   LsodeFirstOrderODEIntegrator (const LsodeFirstOrderODEIntegrator &);

public:
   /**
    * Create a copy of 'this' LsodeFirstOrderODEIntegrator object.
    * @return Clone of 'this'.
    */
   virtual LsodeFirstOrderODEIntegrator * create_copy() const;


   /**
    * Get re_entry_point member.
    */
   EntryPoint get_re_entry_point() {return re_entry_point;}


   // Manager (top-level) methods.  Most of these are from the DLSODE method
   // See lsode_first_order_ode_integrator__manager.cc

   /**
    * Propagate state via Lsode's method.
    * @param[in] dyn_dt        Integration interval step, dynamic time seconds.
    * @param[in] target_stage  The stage of the integration process
    *                          that the integrator should try to attain.
    * @param[in,out] y_dot     Generalized velocity vector.
    * @param[in,out] y         Generalized position vector.
    *
    * @return The status (time advance, pass/fail status) of the integration.
    */
   virtual er7_utils::IntegratorResult integrate (
                                double dyn_dt,
                                unsigned int target_stage,
                                double const * ER7_UTILS_RESTRICT y_dot,
                                double * ER7_UTILS_RESTRICT y);

   void reset_integrator ();

protected:
   void process_entry_point_cycle_start();
   void manager_initialize_calculation_part1();       // was DLSODE
   void manager_initialize_calculation_part2();       // was DLSODE
   int  manager_check_stop_conditions();              // was DLSODE
   void manager_integration_loop_part1();             // was DLSODE
   void manager_integration_loop_part2();             // was DLSODE
   void manager_integration_loop_part3();             // was DLSODE
   void manager_set_calculation_phase_eq_2_reload();  // was DLSODE

   // Integrator methods.  These are from the DSTODE function
   // See lsode_first_order_ode_integrator__integrator.cc
   void integrator_core();
   void integrator_reset_method_coeffs();
   void integrator_test_stepsize_change();
   void integrator_reset_yh();
   void integrator_predict();
   void integrator_reset_iteration_loop_part1();
   void integrator_reset_iteration_loop_part2();
   void integrator_corrector_iteration();
   void integrator_corrector_failed_part1();
   void integrator_corrector_failed_part2();
   void integrator_corrector_converged();
   void integrator_error_test_failed();
   void integrator_compute_new_order_prep();
   void integrator_compute_new_order();
   void integrator_compute_new_order_check_step_error();
   void integrator_set_new_order();
   void integrator_fail_reset_order_1_part1();
   void integrator_fail_reset_order_1_part2();
   void integrator_wrapup();
   void integrator_terminate();

   // Support methods.  These are methods that support DLSODE and DSTODE and
   // have little if any versatility outside the integrator.
   // See lsode_first_order_ode_integrator__support.cc
   void calculate_epsilon();                          // was DUMACH
   void calculate_integration_coefficients();         // was DCFODE
   void interpolate_y();                              // was DINTDY
   void jacobian_prep_init();                         // was DPREPJ
   bool jacobian_prep_loop();                         // was DPREPJ
   bool jacobian_prep_wrap_up();                      // was DPREPJ
   void linear_chord_iteration();                     // was DSOLSY, also SLVS
   void load_ew_values();                             // was DEWSET



   // Utility methods.  These are general mathematical tools.
   // See lsode_first_order_ode_integrator__utility.cc
public:
   void update_control_data();
protected:
   double magnitude_of_weighted_array( double * v);                   // was DVNORM
   double magnitude_of_weighted_array( unsigned int ix, double ** v); // was DVNORM
   int gauss_elim_factor();                                           // was DGEFA
   void linear_solver();                                              // was DGESL
   unsigned int index_of_max_magnitude(unsigned int num_points,       // was IDAMAX
                                       double ** mx,
                                       int starting_ix);
    void load_derivatives(double * derivs);                       // was a part of F


// Variables:

// NOTE - variables indicate their equivalent name in the original Fortran code,
//        and often where they were found.  Many variables come from the
//        DLS001 COMMON block.
public:


// State
   /**
    * Was Y.
    * State vector (zeroth derivative).
    */
   double * y;     //!< trick_units(--)
   /**
    * Was .
    * State vector (first derivative).
    */
   const double * y_dot; //!< trick_units(--)




// Current "position"
   /**
    * Was TOUT.
    * The overall integration target time, reset on each externally-commanded cycle.
    */
   double cycle_target_time; //!< trick_units(--)


// Flow logic



// Control values
   /**
    * was CONIT, in DLS001 common block.
    */
   double convergence_factor; //!< trick_units(--)
   /**
    * was CRATE, in DLS001 common block.
    */
   double convergence_rate;  //!< trick_units(--)
   /**
    * Was IALTH, in DLS001 common block.
    * 0: Select optimal step size and method order
    * 1: If prev_success_order < maximum_order, save vector so that an increase
    * can be considered.
    * >1: Perform neither.
    */
   unsigned int order_select_para; //!< trick_units(--)





// Sizing data
   /**
    * Was NYH, in DLS001 common block.
    * Number of ODEs to be solved in the current problem.
    * In this implementation, num_odes = num_equations.
    */
   unsigned int num_equations; //!< trick_units(--)
   /**
    * Was L, in DLS001 common block
    * Number of columns in Nordsiek array.
    * This appears to be a variable that s equal to the current order of
    * the integrator + 1.
    */
   unsigned int num_nordsiek_cols; //!< trick_units(--)
   /**
    * Was LMAX, in DLS001 common block
    * Maximum allowable number of histories.
    */
   unsigned int max_history_size; //!< trick_units(--)
   /**
    * Was NQNYH, in DLS001 common block.
    * Number of elements of history array that are changed by predictor.
    */
   unsigned int num_predictor_elements; //!< trick_units(--)







// Integration methods
   /**
    * Was NQ, in DLS001 common block.
    * Method order being tried on this or next step.
    */
   unsigned int method_order_current; //!< trick_units(--)









// Step size and determinators
   /**
    * was TN, in DLS001 common block.
    * Value of the independent variable, typically time, to which the
    * integrator has successfully advanced, or to which it will advance
    * in the next step/stage.
    */
   double stage_target_time; //!< trick_units(--)
   /**
    * was RMAX, in DLS001 common block.
    * Max ratio by which step size may be increased.
    */
   double max_step_increase_ratio; //!< trick_units(--)






// Data associated with Jacobian matrix
   /**
    * was CCMAX, in DLS001 common block.
    * Max relative change to (step_size * method_coeff_first) before
    * Jacobian matrix is updated.  see also rel_change_since_jacobian.
    */
   double max_rel_change_without_jacobian; //!< trick_units(--)

// Used for corrector_method = 4 or 5, which are not supported.
//   int jacobian_lower_half_bandwidth; /* trick_units(--) @n
//       was ML/
//       */
//   int jacobian_upper_half_bandwidth; /* trick_units(--) @n
//       was MU.
//       */







// Miscellaneous

// Not supporting integration to pre-defined critical times.
//   double t_crit; /* trick_units(--) @n
//       Was TCRIT.
//       A critical value of the independent variable (typically time) that the
//       integrator cannot integrate through, such as a singularity, and so
//       must avoid.  Use calculation_task = 4 or 5 if this is set.*/



protected:

// Groupings of data:
   /**
    * data used exclusively for the DPREPJ method.
    */
   LsodeDataJacobianPrep data_prepj; //!< trick_units(--)
   /**
    * data arrays, multiple purposes.
    */
   LsodeDataArrays arrays;           //!< trick_units(--)
   /**
    * data used exclusively for the DSTODE method.
    */
   LsodeDataStode data_stode;        //!< trick_units(--)




// Current "position"
   /**
    * Was NST, in DLS001 common block.
    * Number of steps taken for this problem.
    */
   unsigned int num_steps_taken; //!< trick_units(--)
   /**
    * Was NSLAST, in DLS001 common block.
    * Number of steps taken for the problem prior to this call to Lsode.
    */
   unsigned int prior_num_steps; //!< trick_units(--)
   /**
    * Was KFLAG, in DLS001 common block
    * 0:    step was successful
    * -1:   requested accuracy could not be achieved.
    * <=-2: repeated convergence failures.
    */
   int step_error; //!< trick_units(--)
   /**
    * Was NHNIL, in DLS001 common block.
    * Number of small-step encounters fo the problem so far.
    */
   unsigned int num_small_step_warnings; //!< trick_units(--)
   /**
    * Was NJE, in DLS001 common block
    * Number of jacobian evaluations so far for the problem.
    */
   unsigned int num_jacobian_evals; //!< trick_units(--)
   /**
    * Was DEL, in DSTODE, local variable.
    * RMS value of {y dot error_weight_data}
    */
   double iter_delta; //!< trick_units(--)
   /**
    * Was DELP, in DSTODE, local variable.
    * Previous value of iter_delta, used for comparison to identify
    * rate at which iteration is converging /  identifying divergence
    * of iteration.
    */
   double prev_iter_delta; //!< trick_units(--)





// Flow logic
   /**
    * was ISTATE.
    * true:   was IASTATE = 1: first call for the problem, require initialization.
    * false:  was IASTATE = 2: subsequent call, no change to input parameters.
    * not covered:IASTATE = 3: subsequent call, input parameters have changed.
    */
   bool first_pass; //!< trick_units(--)
   /**
    * Indicates where in the integrator to return to following an
    * exit to gether new derivatives.
    */
   EntryPoint re_entry_point; //!< trick_units(--)
   /**
    * was INIT, in DLS001 common block.
    * Flag representing whether the problem has been initialized.
    */
   bool initialized; //!< trick_units(--)
   /**
    * Was JSTART, in DLS001 common block.
    * 0:  First step for problem
    * 1:  Continue normal calculation
    * -1:  Next step has new values of step-size, order, or methods.
    * -2:  Undocumented.
    */
   int internal_state; //!< trick_units(--)






// Control values
   /**
    * Was ITASK.
    * 1: Normal
    * 2:Take one step and return.
    * 3:Stop at first mesh point at or beyond cycle_target_time and return
    * 4:Normal computation, with safeguard on singularity time, t_crit
    * 5:Take one step without passing t_crit.
    * This implementaiton only allows for calculation_task = 1, so it is a
    * protected variable until such time as it is exended to include
    * additional options.
    *
    * Only case 1 is supported.
    */
   CalculationTask calculation_task; //!< trick_units(--)
   /**
    * Was MAXORD.  Populated from IWORK[5]
    * Maximum order allowable.
    */
   unsigned int max_order_internal; //!< trick_units(--)

   LsodeControlDataInterface control_data;




// Integration methods
   /**
    * Was MEO, in DLS001 common block
    * Integration method used in previous call (see integration_method).
    */
   LsodeControlDataInterface::IntegrationMethod prev_integration_method; //!< trick_units(--)
   /**
    * Was NQU, in DLS001 common block.
    * Method order used in last successful step.
    */
   unsigned int prev_method_order; //!< trick_units(--)





// Integration Coefficients
   /**
    * was EL0, in DLS001 common block.
    * method coefficient l_0 for current method and order.
    */
   double method_coeff_first; //!< trick_units(--)
   /**
    * was EL, in DLS001 common block.
    */
   double method_coeffs_current[13]; //!< trick_units(--)
   /**
    * was ELCO, in DLS001 common block.
    * The array of all of the method coefficients.
    */
   double method_coeffs_complete[13][12]; //!< trick_units(--)
   /**
    * was TESCO, in DLS001 common block.
    * The array of all of the test coefficients
    */
   double test_coeffs_complete[3][12] ; //!< trick_units(--)







// Step size and determinators
   /**
    * was H, in DLS001 common block.
    * Step size used on this step, or to be attempted on next.
    */
   double step_size; //!< trick_units(--)
   /**
    * was HOLD, in DLS001 common block.
    */
   double prev_step_size; //!< trick_units(--)
   /**
    * was HU, in DLS001 common block.
    * The size of the last successful step.
    */
   double prev_good_step_size; //!< trick_units(--)
   /**
    * was HMXI.
    * Inverse of maximum absolute step size allowable.
    * Default to 0.0 (i.e. there is no upper bound),
    * calculated from max_step_size if max_step_size is user-specified.
    */
   double max_step_size_inv;  //!< trick_units(--)








// Data associated with Jacobian matrix
   /**
    * Was JCUR, in DLS001 common block
    * Is the jacobian current.
    */
   bool jacobian_current; //!< trick_units(--)
   /**
    * Was IPUP, in DLS001 common block.
    * Flag to indicate whether it is necessary to update the Jacobian.
    */
   bool update_jacobian; //!< trick_units(--)
   /**
    * Was NSLP, in DLS001 common block.
    * Step number at last Jacobian update.
    */
   unsigned int step_at_last_jacobian_update; //!< trick_units(--)
   /**
    * Was ICF, in DLS001 common block.
    * 0: Solution converged
    * 1: Convergence failed; Jacobian is not current.
    * 2: Convergence failed; Jacobian is current or not needed.
    */
   unsigned int convergence_jacobian_flag; //!< trick_units(--)
   /**
    * was RC, in DLS001 common block.
    * Relative change to (step_size * method_coeff_first) since last update
    * to Jacobian matrix.
    */
   double rel_change_since_jacobian; //!< trick_units(--)





// Safety checks

// Not modeling critical times.
//   bool t_crit_hit;   /* trick_units(--) @n
//      Was IHIT.
//      Indicates that the time has come perilously close to the critical-time,
//      a value that the integrator cannot pass through. */
   /**
    * Was IERPJ, in DLS001 common block.
    * false: Iteration  matrix was successfully LU-decomposed
    * (iteration-method = 1,2,4,5) or inverted (iteration-method = 3).
    * true:   Matrix is singular.
    */
   bool iteration_matrix_singular; //!< trick_units(--)
   /**
    * Was IERSL, in DLS001 common block.
    * Like iteration_matrix_singular, only applied to the iteration matrix
    * that has been modified to account for the new step for
    * iteration-method 3.
    */
   bool modified_iteration_matrix_singular; //!< trick_units(--)
   unsigned int iteration_count;





//Miscellaneous
   /**
    * was UROUND, in DLS001 common block.  Small number.
    */
   double epsilon;  //!< trick_units(--)
   /**
    * NEW.
    * square root of epsilon.
    */
   double sqrt_epsilon; //!< trick_units(--)

};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
