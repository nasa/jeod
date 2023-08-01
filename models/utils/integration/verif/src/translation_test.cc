/*******************************************************************************

Purpose:
  (Define methods for the class TranslationTest.)

Reference:
  (((TBS)))

Library dependencies:
  ((translation_test.cc)
   (integration_test.cc)
   (new_orientation.cc)
   (random.cc))

 

*******************************************************************************/


// System includes
#include <cmath>
#include <cstddef>
#include <cstdio>

// JEOD includes
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/named_item/include/named_item.hh"
#include "utils/math/include/numerical.hh"

// Model includes
#include "../include/translation_test.hh"

//! Namespace jeod
namespace jeod {

// Attributes used in allocations
JEOD_DECLARE_ATTRIBUTES (TranslationTestOrbit)
JEOD_DECLARE_ATTRIBUTES (TranslationTestSpringDamper)

// Local definitions
#define PIx2 6.28318530717958647692528676655900577


/*
Purpose:
  (Pre-initialize a TranslationTest instance.)
*/
void
TranslationTest::pre_initialize (   // Return: -- Void
   void)
{

   // Perform base initializations.
   IntegrationTest::pre_initialize ();
}


/*
Purpose:
  (Post-initialize a TranslationTest instance.)
 */
void
TranslationTest::post_initialize (  // Return: -- Void
   void)
{

   // Transform the canonical state to the propagation frame.
   Vector3::transform_transpose (T_iinteg_icanon, true_canon_state.position,
                                 true_integ_state.position);
   Vector3::transform_transpose (T_iinteg_icanon, true_canon_state.velocity,
                                 true_integ_state.velocity);

   // Compute initial specific kinetic energy (potential done by derived class).
   true_canon_state.kinetic_energy =
      0.5 * Vector3::vmagsq (true_canon_state.velocity);
   true_canon_state.total_energy =
      true_canon_state.potential_energy + true_canon_state.kinetic_energy;

   // Initialize the propagated state with the true state in the
   // propagation frame.
   Vector3::copy (true_integ_state.position, prop_integ_state.position);
   Vector3::copy (true_integ_state.velocity, prop_integ_state.velocity);

   // Complete base initializations.
   IntegrationTest::post_initialize ();
}


/*
Purpose:
  (Compute state derivative.)
 */
void
TranslationTest::compute_derivatives (   // Return: -- Void
   void)
{
   double forc_integ[3];

   // Compute external forces and gravity.
   compute_force ();
   Vector3::transform_transpose (
      prop_integ_state.T_inertial_body, forc_body, forc_integ);

   // Compute the total acceleration.
   Vector3::scale (forc_integ, 1.0/mass, dynamic_accel);
   Vector3::incr (grav_accel, dynamic_accel);
}


/*
Purpose:
  (Integrate the integrated state.)
 */
er7_utils::IntegratorResult
TranslationTest::integrate (       // Return: -- Integration status
   double dyn_dt,                  // In:     s  Step size
   unsigned int target_stage)      // In:     -- Target stage number
{
   if (state_integrator == nullptr) {
      return er7_utils::IntegratorResult(false);
   }

   return state_integrator->integrate (
             dyn_dt*time_scale, target_stage,
             dynamic_accel,
             prop_integ_state.velocity, prop_integ_state.position);
}


void
TranslationTest::propagate (    // Return: -- Void
   double dyn_time,             // In:     s  Dynamic time
   bool update_errors)
{

   // Compute integrated potential energy.
   compute_potential ();

   // Compute the integrated kinetic and total energy.
   prop_integ_state.kinetic_energy =
      0.5 * Vector3::vmagsq (prop_integ_state.velocity);
   prop_integ_state.total_energy =
      prop_integ_state.potential_energy + prop_integ_state.kinetic_energy;

   // Compute the true kinetic and total energy.
   true_canon_state.kinetic_energy =
      0.5 * Vector3::vmagsq (true_canon_state.velocity);
   true_canon_state.total_energy =
      true_canon_state.potential_energy + true_canon_state.kinetic_energy;

   // Transform computed canonical state to the integration frames.
   Vector3::transform_transpose (T_iinteg_icanon, true_canon_state.position,
                                 true_integ_state.position);
   Vector3::transform_transpose (T_iinteg_icanon, true_canon_state.velocity,
                                 true_integ_state.velocity);

   true_integ_state.potential_energy = true_canon_state.potential_energy;
   true_integ_state.kinetic_energy   = true_canon_state.kinetic_energy;
   true_integ_state.total_energy     = true_canon_state.total_energy;

   // Compute errors
   rel_energy_error =
      (prop_integ_state.total_energy - true_integ_state.total_energy) /
      energy_scale;

   Vector3::diff (prop_integ_state.position, true_integ_state.position,
                  position_error);
   rel_position_err_mag = Vector3::vmag (position_error) / position_scale;

   Vector3::diff (prop_integ_state.velocity, true_integ_state.velocity,
                  velocity_error);
   rel_velocity_err_mag = Vector3::vmag (velocity_error) / velocity_scale;

   // Track maximum errors
   if (update_errors) {
      if (max_energy_error < rel_energy_error) {
         max_energy_error = rel_energy_error;
         max_energy_time  = dyn_time;
      }

      if (min_energy_error > rel_energy_error) {
         min_energy_error = rel_energy_error;
         min_energy_time  = dyn_time;
      }

      if (max_position_error < rel_position_err_mag) {
         max_position_error = rel_position_err_mag;
         max_position_time  = dyn_time;
      }

      if (max_velocity_error < rel_velocity_err_mag) {
         max_velocity_error = rel_velocity_err_mag;
         max_velocity_time  = dyn_time;
      }
   }
}


void
TranslationTest::shutdown (        // Return: -- Void
   double sim_time,                // In:     s  Simulation time
   double dyn_time,                // In:     s  Dynamic time
   FILE * report)                  // Inout:  -- Output stream
{

   // Report defaults to stdout if not set.
   if (report == nullptr) {
      report = stdout;
   }

   // Print the common header
   IntegrationTest::shutdown (sim_time, dyn_time, report);

    /* Print summary and maximum errors. */
   fprintf (report,
            "  max pos err    (--) = %g\n"
            "    @ t               = %g\n"
            "  max vel err    (--) = %g\n"
            "    @ t               = %g\n"
            "  max energy err (--) = %g\n"
            "    @ t               = %g\n"
            "  min energy err (--) = %g\n"
            "    @ t               = %g\n",
            max_position_error,
            max_position_time,
            max_velocity_error,
            max_velocity_time,
            max_energy_error,
            max_energy_time,
            min_energy_error,
            min_energy_time);
}


/*
Purpose:
  (Create a copy of the object.)
*/
IntegrationTest *
TranslationTestOrbit::replicate ( // Return: -- Constructed object
   unsigned int test_incr)        // In:     -- Test increment
{
   return JEOD_ALLOC_CLASS_OBJECT (TranslationTestOrbit,
                                   (*this, test_incr));
}


/*
Purpose:
  (Pre-initialize a TranslationTestOrbit instance.)
*/
void
TranslationTestOrbit::pre_initialize ( // Return: -- Void
   void)
{

   // Force the test to run at the specified orbital rate.
   // Note that this will eventually set the test's time_scale.
   omega = mdot;

   // Perform base initializations.
   TranslationTest::pre_initialize ();
}


/*
Purpose:
  (Post-initialize a TranslationTestOrbit instance.)
 */
void
TranslationTestOrbit::post_initialize ( // Return: -- Void
   void)
{

   // Compute the mean motion, sqrt(1-e^2), and the gravitational constant.
   mdot = omega;
   ecp  = sqrt (1.0 - ecc*ecc);
   GMb  = (sma * sma * sma) * (mdot * mdot);

   // Compute scale factors for error monitoring.
   position_scale = sma;
   velocity_scale = sma * omega;
   energy_scale   = 0.5 * velocity_scale * velocity_scale;

   // Compute the initial state in the canonical frame.
   compute_true_state (0.0);

   // Complete base initializations.
   TranslationTest::post_initialize ();
}


/*
Purpose:
  (Compute forces, including gravitational accel.)
 */
void
TranslationTestOrbit::compute_force (   // Return: -- Void
   void)
{
   double rsq;
   double scale;

   // The only forces here are gravity.
   Vector3::initialize (forc_body);

   // Compute the gravitational acceleration.
   rsq = Vector3::vmagsq (prop_integ_state.position);
   scale = -GMb / (rsq * sqrt(rsq));
   Vector3::scale (prop_integ_state.position, scale, grav_accel);
}


/*
Purpose:
  (Compute the integrated state potential energy.)
*/
void
TranslationTestOrbit::compute_potential (
   void)
{
   prop_integ_state.potential_energy =
      -GMb / Vector3::vmag (prop_integ_state.position);
}


/*
Purpose:
  (Compute the true state and evaluate.)
*/
void
TranslationTestOrbit::propagate (
   double dyn_time,
   bool update_errors)
{

   // Compute the true state.
   compute_true_state (dyn_time);

   // Pass the method up the inheritance chain.
   TranslationTest::propagate (dyn_time, update_errors);
}


/*
Purpose:
  (Compute the true state.)
*/
void
TranslationTestOrbit::compute_true_state (
   double dyn_time)
{
   double manom;
   double eanom;
   double err;
   double abserr;
   double cosE;
   double sinE;
   double ecosE;
   double omecosE;
   double aEdot;

   // Compute the mean anomaly.
   manom = fmod (mdot * dyn_time, PIx2);
   if (std::fpclassify(manom0) != FP_ZERO) {
      manom = fmod (manom0 + manom, PIx2);
   }

   // Solve Kepler's equation for the eccentric anomaly.
   if (std::fpclassify(manom) == FP_ZERO) {
      eanom = 0.0;
   }
   else {
      eanom = (ecc < 0.31) ? manom : M_PI;
      do {
         cosE    = cos(eanom);
         sinE    = sin(eanom);
         err     = ((eanom - manom) - ecc * sinE) / (1 - ecc * cosE);
         abserr  = fabs(err);
         eanom  -= err;
      } while ((abserr > 1e-24) && (abserr > fabs(eanom)*2e-16));
   }

   // Compute terms leading to the state.
   cosE    = cos(eanom);
   sinE    = sin(eanom);
   ecosE   = ecc * cosE;
   omecosE = 1.0 - ecosE;
   aEdot   = sma * mdot / omecosE;

   // Compute position, velocity, energy.
   true_canon_state.position[0] = sma * (cosE - ecc);
   true_canon_state.position[1] = sma * ecp * sinE;
   true_canon_state.position[2] = 0.0;

   true_canon_state.velocity[0] = -aEdot * sinE;
   true_canon_state.velocity[1] =  aEdot * ecp * cosE;
   true_canon_state.velocity[2] = 0.0;

   true_canon_state.potential_energy = - GMb / (sma * omecosE);
}


/*
Purpose:
  (Create a copy of the object.)
*/
IntegrationTest *
TranslationTestSpringDamper::replicate ( // Return: -- Constructed object
   unsigned int test_incr)               // In:     -- Test increment
{
   return JEOD_ALLOC_CLASS_OBJECT (TranslationTestSpringDamper,
                                   (*this, test_incr));
}


/*
Purpose:
  (Pre-initialize a TranslationTestSpringDamper instance.)
*/
void
TranslationTestSpringDamper::pre_initialize ( // Return: -- Void
   void)
{

   // Force the test to run at the specified undamped oscillator frequency.
   // Note that this will eventually set the test's time_scale.
   omega = omega_0;

   // Perform base initializations.
   TranslationTest::pre_initialize ();
}


/*
Purpose:
  (Post-initialize a TranslationTestSpringDamper instance.)
 */
void
TranslationTestSpringDamper::post_initialize ( // Return: -- Void
   void)
{
   omega_0  = omega;
   omega_sq = omega_0 * omega_0;

   // This is really a user error, and a bad one ...
   if (lambda_value < 0.0) {
      lambda_value = -lambda_value;
   }

   // Compute derived constants of motion.
   if (lambda_value < 1.0) {
      double sinphi = lambda_value;
      double cosphi = sqrt(1.0 - lambda_value*lambda_value);
      k_under     = omega_0 * sinphi;
      omega_under = omega_0 * cosphi;
      phi         = atan (sinphi/cosphi);
      xA          = x0 / cosphi;
      vA          = xA * omega_0;
      damping     = UnderDamped;
   }
   else if (Numerical::compare_exact(lambda_value,1.0)) {
      damping     = CriticallyDamped;
   }
   else {
      double rad = sqrt(lambda_value*lambda_value - 1.0);
      k_fast      = omega_0 * (lambda_value + rad);
      k_slow      = omega_0 * (lambda_value - rad);
      xA          = x0 / (k_fast - k_slow);
      vA          = xA * omega_sq;
      damping     = OverDamped;
   }

   // Compute scale factors for error monitoring.
   position_scale = x0;
   velocity_scale = x0 * omega;
   energy_scale   = velocity_scale * velocity_scale;

   // Compute the initial state in the canonical frame.
   compute_true_state (0.0);

   // Complete base initializations.
   TranslationTest::post_initialize ();
}


/*
Purpose:
  (Compute forces, including gravitational accel.)
 */
void
TranslationTestSpringDamper::compute_force (   // Return: -- Void
   void)
{
   double forc_integ[3];

   // The spring and drag exert forces on the object.
   Vector3::scale (prop_integ_state.position, -mass*omega_sq,
                   forc_integ);
   Vector3::scale_incr (prop_integ_state.velocity,
                        -2.0*mass*lambda_value*omega_0,
                        forc_integ);

   // Transform to body.
   Vector3::transform (
      prop_integ_state.T_inertial_body, forc_integ, forc_body);


   // There is no gravitational acceleration.
   Vector3::initialize (grav_accel);
}


/*
Purpose:
  (Compute the integrated state potential energy.)
*/
void
TranslationTestSpringDamper::compute_potential (
   void)
{
   prop_integ_state.potential_energy =
      -omega_sq * Vector3::vmagsq (prop_integ_state.position);
}


/*
Purpose:
  (Compute the true state and evaluate.)
*/
void
TranslationTestSpringDamper::propagate (
   double dyn_time,
   bool update_errors)
{

   // Compute the true state.
   compute_true_state (dyn_time);

   // Pass the method up the inheritance chain.
   TranslationTest::propagate (dyn_time, update_errors);
}


/*
Purpose:
  (Compute the true state.)
*/
void
TranslationTestSpringDamper::compute_true_state (
   double dyn_time)
{
   switch (damping) {
   case UnderDamped: {
      double omegat = omega_under*dyn_time;
      double expkt  = exp(-k_under*dyn_time);
      true_canon_state.position[0] = xA * cos(omegat-phi) * expkt;
      true_canon_state.position[1] =
      true_canon_state.position[2] = 0.0;

      true_canon_state.velocity[0] = -vA * sin(omegat) * expkt;
      true_canon_state.velocity[1] =
      true_canon_state.velocity[2] = 0.0;
      break;
   }

   case CriticallyDamped: {
      double omegat = omega_0*dyn_time;
      double expkt  = exp(-omegat);
      true_canon_state.position[0] = x0 * (1.0+omegat) * expkt;
      true_canon_state.position[1] =
      true_canon_state.position[2] = 0.0;

      true_canon_state.velocity[0] = -x0 * omega_0 * omegat * expkt;
      true_canon_state.velocity[1] =
      true_canon_state.velocity[2] = 0.0;
      break;
   }

   case OverDamped: {
      double expkfastt = exp(-k_fast*dyn_time);
      double expkslowt = exp(-k_slow*dyn_time);
      true_canon_state.position[0] = xA * (k_fast*expkslowt - k_slow*expkslowt);
      true_canon_state.position[1] =
      true_canon_state.position[2] = 0.0;

      true_canon_state.velocity[0] = -vA * (expkslowt - expkfastt);
      true_canon_state.velocity[1] =
      true_canon_state.velocity[2] = 0.0;
      break;
   }

   default:
      break;
   }

   true_canon_state.potential_energy =
      -omega_sq * Vector3::vmagsq (true_canon_state.position);
}
} // End JEOD namespace
