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
/*******************************************************************************

Purpose:
  (Define data types for testing the propagation of a translational state.)

Reference:
  (((Hammen, David)
    (JSC Engineering Orbital Dynamics: Integration)
    (NASA Johnson Space Center, Houston TX)
    (2010)))

Assumptions and limitations:
  ((This model is intended for IV&V purposes only.))

Library dependencies:
  ((translation_test.o))

 
*******************************************************************************/

#ifndef JEOD_TRANSLATION_TEST_HH
#define JEOD_TRANSLATION_TEST_HH

// System includes
#include <cmath>
#include <cstddef>

// Trick includes
#include "sim_services/Integrator/include/Integrator.hh"

// ER7 utilities includes
#include "er7_utils/integration/core/include/integrator_constructor.hh"

// JEOD includes
#include "environment/time/include/time_manager.hh"
#include "utils/integration/include/restartable_state_integrator.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "angular_variance.hh"
#include "integration_test.hh"
#include "random_orientation.hh"
#include "integration_test_messages.hh"


//! Namespace jeod
namespace jeod {

class TranslationTest;
class TranslationTestOrbit;


/*
Purpose: (Describe a translational state.)
*/
class TranslationState {

 JEOD_MAKE_SIM_INTERFACES(TranslationState)

 friend class TranslationTest;
 friend class TranslationTestOrbit;
 friend class TranslationTestSpringDamper;

 // Member functions
public:

   /**
     Default constructor.
   */
   TranslationState (void) :
      Q_inertial_body(),
      potential_energy(0.0),
      kinetic_energy(0.0),
      total_energy(0.0)
   {
      Matrix3x3::identity (T_inertial_body);
      Vector3::initialize (position);
      Vector3::initialize (velocity);
   }

 // Member data
protected:
   Quaternion Q_inertial_body; /* trick_units(--) @n
      Inertial-to-body quaternion */

   double T_inertial_body[3][3]; /* trick_units(--) @n
      Inertial-to-body transformation */

   double position[3]; /* trick_units(m) @n
      Object position in the TranslationState frame. */

   double velocity[3]; /* trick_units(m/s) @n
      Object velocity in the TranslationState frame. */

   double potential_energy; /* trick_units(m2/s2) @n
      Potential energy T, model-dependent */

   double kinetic_energy; /* trick_units(m2/s2) @n
      Kinetic energy K = 1/2 m v^2 */

   double total_energy; /* trick_units(m2/s2) @n
      Total energy E = T + K */
};


class TranslationTest : public IntegrationTest {

 JEOD_MAKE_SIM_INTERFACES(TranslationTest)

 // Member functions
public:

   /**
     Default constructor.
   */
   TranslationTest (void) :
      IntegrationTest(),
      mass(22026.4657948067),
      true_canon_state(),
      true_integ_state(),
      prop_integ_state(),
      position_scale(1.0),
      velocity_scale(1.0),
      energy_scale(1.0),
      rel_position_err_mag(0.0),
      rel_velocity_err_mag(0.0),
      rel_energy_error(0.0),
      max_position_error(0.0),
      max_position_time(0.0),
      max_velocity_error(0.0),
      max_velocity_time(0.0),
      max_energy_error(0.0),
      max_energy_time(0.0),
      min_energy_error(0.0),
      min_energy_time(0.0),
      integ_generator(state_integrator)
   {
      Vector3::initialize (grav_accel);
      Vector3::initialize (dynamic_accel);
      Vector3::initialize (forc_body);
      Vector3::initialize (position_error);
      Vector3::initialize (velocity_error);
   }

   /**
     Copy constructor.
   */
   TranslationTest (const TranslationTest & source,
                    unsigned int test_incr = 0) :
      IntegrationTest (source, test_incr),
      mass (source.mass),
      true_canon_state(),
      true_integ_state(),
      prop_integ_state(),
      position_scale(1.0),
      velocity_scale(1.0),
      energy_scale(1.0),
      rel_position_err_mag(0.0),
      rel_velocity_err_mag(0.0),
      rel_energy_error(0.0),
      max_position_error(0.0),
      max_position_time(0.0),
      max_velocity_error(0.0),
      max_velocity_time(0.0),
      max_energy_error(0.0),
      max_energy_time(0.0),
      min_energy_error(0.0),
      min_energy_time(0.0),
      integ_generator(state_integrator)
   {}

   /**
     Destructor.
   */
   virtual ~TranslationTest (void) {}

   /**
     Create a copy of the object.
     \return Constructed IntegrationTest object
   */
   virtual IntegrationTest * replicate (unsigned int test_incr) = 0;


   // Overloaded methods
   virtual void pre_initialize (void);

   virtual void post_initialize (void);

   virtual void validate_integrability (
      const er7_utils::IntegratorConstructor & generator __attribute__ ((unused)) )
   { }

   virtual void create_integrators (
      const er7_utils::IntegratorConstructor & generator,
      er7_utils::IntegrationControls & controls,
      const er7_utils::TimeInterface & time_if __attribute__ ((unused)) )
   {
      integ_generator.create_integrator (generator, controls);
   }


   /**
    * Destroy the translation test state integrator.
    */
   virtual void destroy_integrators (
      void)
   {
      integ_generator.destroy_integrator ();
   }

   // Propagating over time.
   virtual void propagate (double dyn_time, bool update_errors);

   // Integrate over delta-time.
   virtual er7_utils::IntegratorResult integrate (
      double dyn_dt,
      unsigned int target_stage);

   // Compute derivatives; assumed to be time-independent.
   virtual void compute_derivatives (void);

   // Generate shutdown report.
   virtual void shutdown (double sim_time, double dyn_time, FILE * report);


   /**
    Compute the integrated and true potential energy.
    The default implementation is that there is no potential.
   */
   virtual void compute_potential (void) {
      prop_integ_state.potential_energy =
      true_canon_state.potential_energy = 0.0;
   }

   /**
    Compute the external force on the object.
    The default implementation is that there is no external force.
   */
   virtual void compute_force (void) {
      Vector3::initialize (forc_body);
   }


 // Member data
protected:
   double mass; /* trick_units(kg) @n
      Object mass. */

   TranslationState true_canon_state; /* trick_units(--) @n
      Computed canonical state. */

   TranslationState true_integ_state; /* trick_units(--) @n
      Computed state transformed to integration frames. */

   TranslationState prop_integ_state; /* trick_units(--) @n
      Propagated state. */

   double grav_accel[3]; /* trick_units(--) @n
      Integ. frame acceleration due to gravity at propagated position. */

   double dynamic_accel[3]; /* trick_units(--) @n
      Integ. frame total acceleration (grav_accel + F/m). */

   double forc_body[3]; /* trick_units(--) @n
      Propagation body frame external force. */

   double position_scale; /* trick_units(m) @n
      Position error scale factor. */

   double velocity_scale; /* trick_units(m/s) @n
      Velocity error scale factor. */

   double energy_scale; /* trick_units(m2/s2) @n
      Velocity error scale factor. */

   double position_error[3]; /* trick_units(m) @n
      Position error, integrated - true, in integration inertial coordinates. */

   double rel_position_err_mag; /* trick_units(--) @n
      Magnitude of position_error scaled by position_scale. */

   double velocity_error[3]; /* trick_units(m/s) @n
      Velocity error, integrated - true, in integration inertial coordinates. */

   double rel_velocity_err_mag; /* trick_units(--) @n
      Magnitude of velocity_error scaled by velocity_scale. */

   double rel_energy_error; /* trick_units(--) @n
      Relative energy error. */

   double max_position_error; /* trick_units(--) @n
      Maximum of rel_position_err_mag. */
   double max_position_time; /* trick_units(s) @n
      Time at which above was set */

   double max_velocity_error; /* trick_units(--) @n
      Maximum of rel_velocity_err_mag. */
   double max_velocity_time; /* trick_units(s) @n
      Time at which above was set */

   double max_energy_error; /* trick_units(--) @n
      Maximum of rel_energy_error */
   double max_energy_time; /* trick_units(s) @n
      Time at which above was set */

   double min_energy_error; /* trick_units(--) @n
      Minimum of rel_energy_error */
   double min_energy_time; /* trick_units(s) @n
      Time at which above was set */

   RestartableSimpleSecondOrderODEIntegrator<3> integ_generator; /* trick_io(**) @n
      Restartable integrator generator. */

private:
   /**
    Assignment operator is private and unimplemented.
   */
   TranslationTest & operator = (const TranslationTest & source);
};


class TranslationTestOrbit : public TranslationTest {

 JEOD_MAKE_SIM_INTERFACES(TranslationTestOrbit)

public:

   /**
     Default constructor.
   */
   TranslationTestOrbit(void) :
      TranslationTest(),
      sma(6811.137e3),
      ecc(0.0),
      manom0(0.0),
      mdot(1.1231543952404041e-03),
      ecp(1.0),
      GMb(0.0)
   {}

   /**
     Copy constructor.
   */
   TranslationTestOrbit (const TranslationTestOrbit & source,
                         unsigned int test_incr = 0) :
      TranslationTest(source, test_incr),
      sma (source.sma),
      ecc (source.ecc),
      manom0 (source.manom0),
      mdot (source.mdot)
   {}

   // Overloaded methods
   virtual IntegrationTest * replicate (unsigned int test_incr);

   virtual void pre_initialize (void);

   virtual void post_initialize (void);

   virtual void propagate (double dyn_dt, bool update_errors);

   virtual void compute_force (void);

   virtual void compute_potential (void);

   void compute_true_state (double dyn_time);


 // Member data
protected:
   double sma; /* trick_units(m) @n
      Semi-major axis length. */

   double ecc; /* trick_units(--) @n
      Eccentricity. */

   double manom0; /* trick_units(rad) @n
      Initial mean anomaly. */

   double mdot; /* trick_units(rad/s) @n
      Mean motion. */

   double ecp; /* trick_units(--) @n
      sqrt(1-e^2). */

   double GMb; /* trick_units(m3/s2) @n
      Gravitational constant. */

private:
   /**
    Assignment operator is private and unimplemented.
   */
   TranslationTestOrbit & operator = (const TranslationTestOrbit & source);
};


class TranslationTestSpringDamper : public TranslationTest {

 JEOD_MAKE_SIM_INTERFACES(TranslationTestSpringDamper)

public:

   enum Damping {
      UnderDamped,
      CriticallyDamped,
      OverDamped
   };


   /**
     Default constructor.
   */
   TranslationTestSpringDamper(void) :
      TranslationTest(),
      lambda_value(1.0),
      omega_0(1.0),
      x0(1.0),
      omega_sq(1.0),
      phi(0.0),
      omega_under(0.0),
      k_under(0.0),
      k_fast(0.0),
      k_slow(0.0),
      xA(0.0),
      vA(0.0),
      damping(CriticallyDamped)
   {}

   /**
     Copy constructor.
   */
   TranslationTestSpringDamper (const TranslationTestSpringDamper & source,
                                unsigned int test_incr = 0) :
      TranslationTest (source, test_incr),
      lambda_value (source.lambda_value),
      omega_0 (source.omega_0),
      x0 (source.x0)
   {}

   // Overloaded methods
   virtual IntegrationTest * replicate (unsigned int test_incr);

   virtual void pre_initialize (void);

   virtual void post_initialize (void);

   virtual void propagate (double dyn_dt, bool update_errors);

   virtual void compute_force (void);

   virtual void compute_potential (void);

   void compute_true_state (double dyn_time);

   double lambda_value; /* trick_units(--) @n
      Damping factor. */

 // Member data
protected:

   double omega_0; /* trick_units(1/s) @n
      Undamped system frequency. */


   double x0; /* trick_units(m) @n
      Initial displacement. */


   double omega_sq; /* trick_units(1/s2) @n
      Square of omega_0. */

   double phi; /* trick_units(rad) @n
      Phase angle (underdamped only) */

   double omega_under; /* trick_units(rad/s) @n
      Underdamped oscillator frequency. */

   double k_under; /* trick_units(1/s) @n
      Underdamped exponential decay factor. */

   double k_fast; /* trick_units(1/s) @n
      Fast exponential decay factor:
      k_fast = omega_0 (lambda_value + sqrt(lambda_value^2-1)) */

   double k_slow; /* trick_units(1/s) @n
      Slow exponential decay factor:
      k_slow = omega_0 (lambda_value - sqrt(lambda_value^2-1)) */

   double xA; /* trick_units(m) @n
      Underdamped:       x_0 / cos(phi)
      Overdamped:        x_0 / (k_fast - k_slow)
      Critically damped: Unused. */

   double vA; /* trick_units(m) @n
      Underdamped:       x_A * omega_0
      Overdamped:        x_A * omega_0^2
      Critically damped: Unused. */

   Damping damping; /* trick_units(--) @n
      Damping type. */

private:
   /**
    Assignment operator is private and unimplemented.
   */
   TranslationTestSpringDamper & operator = (
      const TranslationTestSpringDamper & source);
};

/*
a = -(2 lambda_value omega_0 v + omega_0^2 x)
  = -omega_0 (2 lambda_value v + omega_0 x)
ddot x + 2 lambda_value omega_0 ddot x + omega_0^2 x = 0

F = -k x
a = -k/m x = -omega_0^2 x
phi = 0.5 k x^2
k = -m omega_0^2

Ignore mass; compute specific energy.
phi = 0.5 omega_0^2 x^2

lambda_value < 1:
sin(phi) = lambda_value
cos(phi) = sqrt(1-lambda_value^2)
k = omega_0 lambda_value_0         = omega_0 sin(phi)
w = omega_0 sqrt(1-lambda_value^2) = omega_0 cos(phi)
phi = atan(k/w) = atan(lambda_value/sqrt(1-lambda_value^2))
A = x_0 / cos(phi)
x = A cos(wt-phi) exp(-kt)
v = -A omega_0 sin(wt) exp(-kt)


lambda_value = 1:
x = (A+Bt) exp(-omega_0 t)
xdot = -omega_0 x + B exp(-omegat)
     = (-omega_0 A+B(1-omega_0 t)) exp(-omega_0 t)
xdot(0) = 0 => B = omega_0 A
x = x_0 (1+omega_0 t) exp(-omega_0 t)
v = -x_0 omega_0^2 t exp(-omega_0 t)

lambda_value > 1:
a = omega_0 (lambda_value + sqrt(lambda_value^2-1))
b = omega_0 (lambda_value - sqrt(lambda_value^2-1))
ab = omega_0^2
b = omega^2/a
A = x_0 / (1-a/b)
x = A(exp(-at)-a/b exp(-bt))
v = -Aa(exp(-at)-exp(-bt))
*/

} // End JEOD namespace


#endif
