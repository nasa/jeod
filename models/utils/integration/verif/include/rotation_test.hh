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
  (Define data types for testing the propagation of a rotational state.)

Reference:
  (((Hammen, David)
    (JSC Engineering Orbital Dynamics: Integration)
    (NASA Johnson Space Center, Houston TX)
    (2010)))

Assumptions and limitations:
  ((This model is intended for IV&V purposes only.))

Library dependencies:
  ((rotation_test.o))

 

*******************************************************************************/

#ifndef JEOD_ROTATION_TEST_HH
#define JEOD_ROTATION_TEST_HH

// System includes
#include <cmath>

// Trick includes
#include "sim_services/Integrator/include/Integrator.hh"

// ER7 utilities includes
#include "er7_utils/integration/core/include/integrator_constructor.hh"
#include "er7_utils/integration/core/include/left_quaternion_functions.hh"

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

class RotationTest;
class RotationTestTorqueFreeSphere;
class RotationTestTorqueFreeSymTop;
class RotationTestSHOSphere;


/*
Purpose: (Describe a rotational state.)
*/
class RotationState {
JEOD_MAKE_SIM_INTERFACES(RotationState)

 friend class RotationTest;
 friend class RotationTestTorqueFreeSphere;
 friend class RotationTestTorqueFreeSymTop;
 friend class RotationTestSHOSphere;

public:

   // Member functions

   /**
    * Default constructor
    */
   RotationState (void) :
      Q_inertial_body(),
      potential_energy(0.0),
      kinetic_energy(0.0),
      total_energy(0.0)
   {
      Matrix3x3::initialize (inertia);
      Matrix3x3::identity (T_inertial_body);
      Vector3::initialize (body_rate);
   }

protected:

   // Member data

   /**
    * Inertia matrix (body frame).
    */
   double inertia[3][3]; //!< trick_units(kg*m2)

   /**
    * Angular variation from reference.
    */
   double theta; //!< trick_units(rad)

   /**
    * Inertial-to-body quaternion.
    */
   Quaternion Q_inertial_body; //!< trick_units(--)

   /**
    * Inertial-to-body transformation.
    */
   double T_inertial_body[3][3]; //!< trick_units(--)

   /**
    * Body rate wrt inertial in body frame.
    */
   double body_rate[3]; //!< trick_units(rad/s)

   /**
    * Potential energy T, model-dependent.
    */
   double potential_energy; //!< trick_units(m2/s2)

   /**
    * Kinetic energy K = 1/2 (Iw) dot w.
    */
   double kinetic_energy; //!< trick_units(m2/s2)

   /**
    * Total energy E = T + K.
    */
   double total_energy; //!< trick_units(m2/s2)
};


class RotationTest : public IntegrationTest {
JEOD_MAKE_SIM_INTERFACES(RotationTest)

public:

   // Member functions

   /**
    * Default constructor.
    */
   RotationTest (void) :
      IntegrationTest(),
      I_xx(1e8),
      true_canon_state(),
      true_integ_state(),
      prop_integ_state(),
      Q_ref(),
      prop_theta(),
      Qdot_inertial_body(),
      orient_error(),
      rel_energy_error(0.0),
      rate_error_mag(0.0),
      max_angle_error(0.0),
      max_angle_time(0.0),
      max_rate_error(0.0),
      max_rate_time(0.0),
      max_energy_error(0.0),
      max_energy_time(0.0),
      min_energy_error(0.0),
      min_energy_time(0.0),
      integ_generator(NULL)
   {
      zhat[0] = 0.0;
      zhat[1] = 0.0;
      zhat[2] = 1.0;
      Matrix3x3::initialize (inverse_inertia);
      Vector3::initialize (body_accel);
      Vector3::initialize (torq_body);
      Vector3::initialize (rate_error);
   }

   /**
    * Copy constructor.
    */
   RotationTest (const RotationTest & source,
                 unsigned int test_incr = 0) :
      IntegrationTest (source, test_incr),
      I_xx (source.I_xx),
      true_canon_state(),
      true_integ_state(),
      prop_integ_state(),
      Q_ref(),
      prop_theta(),
      Qdot_inertial_body(),
      orient_error(),
      rel_energy_error(0.0),
      rate_error_mag(0.0),
      max_angle_error(0.0),
      max_angle_time(0.0),
      max_rate_error(0.0),
      max_rate_time(0.0),
      max_energy_error(0.0),
      max_energy_time(0.0),
      min_energy_error(0.0),
      min_energy_time(0.0),
      integ_generator(NULL)
   {
      zhat[0] = 0.0;
      zhat[1] = 0.0;
      zhat[2] = 1.0;
      Matrix3x3::initialize (inverse_inertia);
      Vector3::initialize (body_accel);
      Vector3::initialize (torq_body);
      Vector3::initialize (rate_error);
   }

   /**
    * Destructor.
    */
   virtual ~RotationTest (void)
   {
      JEOD_DELETE_OBJECT (integ_generator);
   }


   /**
     Create a copy of the object.
     \return Constructed IntegrationTest object
   */
   virtual IntegrationTest * replicate (unsigned int test_incr) = 0;


   // Initialization methods.
   virtual void pre_initialize (void);

   virtual void post_initialize (void);

   virtual void validate_integrability (
      const er7_utils::IntegratorConstructor & generator);

   void
   create_integrators (
      const er7_utils::IntegratorConstructor & generator,
      er7_utils::IntegrationControls & controls,
      const er7_utils::TimeInterface & time_if);

   /**
    * Destroy the translation test state integrator.
    */
   virtual void destroy_integrators (
      void)
   {
      integ_generator->destroy_integrator ();
      JEOD_DELETE_OBJECT (integ_generator);
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
    * Compute the integrated and true potential energy.
    * The default implementation is that there is no potential.
    */
   virtual void
   compute_potential (void)
   {
      prop_integ_state.potential_energy =
      true_canon_state.potential_energy = 0.0;
   }

   /**
    Compute the torque on the object.
    The default implementation is that there is no torque.
   */
   virtual void
   compute_torque (void)
   {
      Vector3::initialize (torq_body);
   }


protected:

   // Data types

   /**
    * Quaternion-based rotational state integrator that uses the
    * quaternion derivative as the basis for integration.
    */
   typedef RestartableGeneralizedDerivSecondOrderODEIntegrator<
              4, 3, er7_utils::LeftQuaternionGeneralizedPositionFunctions>
           GeneralizedDerivQuatInteg;

   /**
    * Quaternion-based rotational state integrator that uses the
    * quaternion exponential as the basis for integration.
    */
   typedef RestartableGeneralizedStepSecondOrderODEIntegrator<
              4, 3, er7_utils::LeftQuaternionGeneralizedPositionFunctions>
           LieGroupQuatInteg;


   // Member data

   /**
    * User-input x-axis moment of inertia (canonical body)
    */
   double I_xx; //!< trick_units(kg*m2)

   /**
    * Unit vector in the z-hat direction.
    * This is set in the constructor and never modified.
    * Please do not touch.
    */
   double zhat[3]; //!< trick_io(*o) trick_units(--)

   /**
    * Computed canonical state.
    */
   RotationState true_canon_state; //!< trick_units(--)

   /**
    * Computed state transformed to integration frames.
    */
   RotationState true_integ_state; //!< trick_units(--)

   /**
    * Propagated state.
    */
   RotationState prop_integ_state; //!< trick_units(--)

   /**
    * Reference orientation for the angular variance from reference.
    */
   Quaternion Q_ref; //!< trick_units(--)

   /**
    * Propagated state displacement from some reference orientation,
    * including wrapping.
    */
   AngularVariance prop_theta; //!< trick_units(--)

   /**
    * Inverse of prop_integ_state.inertia.
    */
   double inverse_inertia[3][3]; //!< trick_units(1/kg/m2)

   /**
    * Time derivative of prop_integ->Q_inertial_body.
    */
   Quaternion Qdot_inertial_body; //!< trick_units(--)

   /**
    * Time derivative of prop_integ->body_rate.
    */
   double body_accel[3]; //!< trick_units(rad/s2)

   /**
    * Torque in body frame.
    */
   double torq_body[3]; //!< trick_units(N*m)

   /**
    * Error in the integrated orientation.
    */
   AngularVariance orient_error; //!< trick_units(--)

   /**
    * Relative energy error.
    */
   double rel_energy_error; //!< trick_units(--)

   /**
    * Error in the propagated rate.
    */
   double rate_error[3]; //!< trick_units(rad/s)

   /**
    * Magnitude of the above.
    */
   double rate_error_mag; //!< trick_units(rad/s)

   /**
    * Maximum of abs(orient_error(theta)).
    */
   double max_angle_error; //!< trick_units(rad)

   /**
    * Time at which above was set.
    */
   double max_angle_time; //!< trick_units(s)

   /**
    * Maximum of rate_error_mag.
    */
   double max_rate_error; //!< trick_units(rad/s)

   /**
    * Time at which above was set.
    */
   double max_rate_time; //!< trick_units(s)

   /**
    * Maximum of rel_energy_error.
    */
   double max_energy_error; //!< trick_units(--)

   /**
    * Time at which above was set.
    */
   double max_energy_time; //!< trick_units(s)

   /**
    * Minimum of rel_energy_error.
    */
   double min_energy_error; //!< trick_units(--)

   /**
    * Time at which above was set.
    */
   double min_energy_time; //!< trick_units(s)

#ifndef SWIG

   /**
    * Rotational state checkpointable/restartable integrator generator.
    */
   RestartableSecondOrderODEIntegrator * integ_generator; //!< trick_io(**)

#endif


private:

   /**
    Assignment operator is private and unimplemented.
   */
   RotationTest & operator = (const RotationTest & source);
};


class RotationTestTorqueFreeSphere : public RotationTest {

 JEOD_MAKE_SIM_INTERFACES(RotationTestTorqueFreeSphere)

 public:
   /**
     Default constructor.
   */
   RotationTestTorqueFreeSphere(void) :
      RotationTest()
   {}

   /**
     Copy constructor.
   */
   RotationTestTorqueFreeSphere (const RotationTestTorqueFreeSphere & source,
                                 unsigned int test_incr = 0) :
      RotationTest(source, test_incr)
   {}

   /**
     Destructor.
   */
   ~RotationTestTorqueFreeSphere(void)
   {}

 private:
   /**
    Assignment operator is private and unimplemented.
   */
   RotationTestTorqueFreeSphere & operator = (
      const RotationTestTorqueFreeSphere & source);

 public:

   // Overloaded methods
   virtual IntegrationTest * replicate (unsigned int test_incr);

   virtual void pre_initialize (void);

   virtual void post_initialize (void);

   virtual void propagate (double dyn_dt, bool update_errors);
};


class RotationTestTorqueFreeSymTop : public RotationTest {

 JEOD_MAKE_SIM_INTERFACES(RotationTestTorqueFreeSymTop)

 // Member functions
 public:
   /**
     Default constructor.
   */
   RotationTestTorqueFreeSymTop(void) :
      RotationTest(),
      f_zz(0.8),
      theta(50.0*M_PI/180.0),
      omega_z(0.0),
      omega_xy(0.0),
      phi(0.0),
      omega_theta(0.0),
      cap_omega(0.0)
   {}

   /**
     Non-default constructor.
   */
   RotationTestTorqueFreeSymTop (double f_zz_in, double theta_degrees_in) :
      RotationTest(),
      f_zz(f_zz_in),
      theta(theta_degrees_in*M_PI/180.0),
      omega_z(0.0),
      omega_xy(0.0),
      phi(0.0),
      omega_theta(0.0),
      cap_omega(0.0)
   {}

   /**
     Copy constructor.
   */
   RotationTestTorqueFreeSymTop (const RotationTestTorqueFreeSymTop & source,
                                 unsigned int test_incr = 0) :
      RotationTest(source, test_incr),
      f_zz(source.f_zz),
      theta(source.theta),
      omega_z(0.0),
      omega_xy(0.0),
      phi(0.0),
      omega_theta(0.0),
      cap_omega(0.0)
   {}

   /**
     Destructor.
   */
   ~RotationTestTorqueFreeSymTop(void)
   {}


 private:
   /**
    Assignment operator is private and unimplemented.
   */
   RotationTestTorqueFreeSymTop & operator = (
      const RotationTestTorqueFreeSymTop & source);

 public:

   // Overloaded methods
   virtual IntegrationTest * replicate (unsigned int test_incr);

   virtual void pre_initialize (void);

   virtual void post_initialize (void);

   virtual void propagate (double dyn_dt, bool update_errors);


 // Member data
 protected:

   /**
    * User-input z-axis moment of inertia factor (canonical body frame).
    * The z-axis moment of inertia is the product of this factor and the
    * inherited x-axis moment of inertia, I_xx.
    */
   double f_zz; //!< trick_units(--)

   /**
    * User-input angle between the canonical body z axis and the angular
    * velocity vector.
    */
   double theta; //!< trick_units(rad)

   /**
    * Z-axis (canonical body) component of the angular velocity.
    */
   double omega_z; //!< trick_units(rad/s)

   /**
    * Non-z component (canonical body) of the angular velocity.
    */
   double omega_xy; //!< trick_units(rad/s)

   /**
    * Angle between the angular momentum and angular velocity vectors.
    */
   double phi; //!< trick_units(rad)

   /**
    * Body precession rate.
    */
   double omega_psi; //!< trick_units(rad/s)

   /**
    * Body rotation rate.
    */
   double omega_theta; //!< trick_units(rad/s)

   /**
    * Angular velocity precession rate.
    */
   double cap_omega; //!< trick_units(rad/s)
};


class RotationTestTorqueFreeSymTopB : public RotationTestTorqueFreeSymTop {

 JEOD_MAKE_SIM_INTERFACES(RotationTestTorqueFreeSymTopB)

 // Member functions
 public:
   /**
     Default constructor.
   */
   RotationTestTorqueFreeSymTopB(void) :
      RotationTestTorqueFreeSymTop(0.8, 1.0)
   {}

   /**
     Copy constructor.
   */
   RotationTestTorqueFreeSymTopB (const RotationTestTorqueFreeSymTopB & source,
                                  unsigned int test_incr = 0) :
      RotationTestTorqueFreeSymTop (source, test_incr)
   {}

   /**
     Destructor.
   */
   ~RotationTestTorqueFreeSymTopB(void)
   {}


 private:
   /**
    Assignment operator is private and unimplemented.
   */
   RotationTestTorqueFreeSymTopB & operator = (
      const RotationTestTorqueFreeSymTopB & source);

 public:

   // Overloaded methods
   virtual IntegrationTest * replicate (unsigned int test_incr);
};


class RotationTestSHOSphere : public RotationTest {

 JEOD_MAKE_SIM_INTERFACES(RotationTestSHOSphere)

 public:

   /**
     Default constructor.
   */
   RotationTestSHOSphere(void) :
      RotationTest(),
      theta_max(175.0*M_PI/180.0),
      k_sho(0.0),
      ang_var()
   {}

   /**
     Copy constructor.
   */
   RotationTestSHOSphere (const RotationTestSHOSphere & source,
                          unsigned int test_incr = 0) :
      RotationTest(source, test_incr),
      theta_max(source.theta_max),
      k_sho(0.0),
      ang_var()
   {}

   /**
     Destructor.
   */
   ~RotationTestSHOSphere(void)
   {}

 private:
   /**
    Assignment operator is private and unimplemented.
   */
   RotationTestSHOSphere & operator = (const RotationTestSHOSphere & source);

 public:

   // Overloaded methods.
   virtual IntegrationTest * replicate (unsigned int test_incr);

   virtual void pre_initialize (void);

   virtual void post_initialize (void);

   virtual void propagate (double dyn_dt, bool update_errors);

   virtual void compute_potential (void);
   virtual void compute_torque (void);


 // Member data
 protected:

   /**
    * Initial SHO angular displacement.
    */
   double theta_max; //!< trick_units(rad)

   /**
    * Restoring torque constant.
    */
   double k_sho; //!< trick_units(N*m)

   /**
    * Angular variance, used to compute torque.
    */
   AngularVariance ang_var; //!< trick_units(--)
};

} // End JEOD namespace


#endif
