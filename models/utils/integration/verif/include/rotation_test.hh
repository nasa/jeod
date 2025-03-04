//=============================================================================
// Notices:
//
// Copyright © 2025 United States Government as represented by the Administrator
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
  ((../src/rotation_test.cc))



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
#include "integration_test_messages.hh"
#include "random_orientation.hh"

// Define macros
#include "utils/math/include/macro_def.hh"

//! Namespace jeod
namespace jeod
{

class RotationTest;
class RotationTestTorqueFreeSphere;
class RotationTestTorqueFreeSymTop;
class RotationTestSHOSphere;

/*
Purpose: (Describe a rotational state.)
*/
class RotationState
{
    JEOD_MAKE_SIM_INTERFACES(jeod, RotationState)

    friend class RotationTest;
    friend class RotationTestTorqueFreeSphere;
    friend class RotationTestTorqueFreeSymTop;
    friend class RotationTestSHOSphere;

public:
    RotationState() = default;

protected:
    // Member data

    /**
     * Inertia matrix (body frame).
     */
    double inertia[3][3]{}; //!< trick_units(kg*m2)

    /**
     * Angular variation from reference.
     */
    double theta{}; //!< trick_units(rad)

    /**
     * Inertial-to-body quaternion.
     */
    Quaternion Q_inertial_body; //!< trick_units(--)

    /**
     * Inertial-to-body transformation.
     */
    double T_inertial_body[3][3]{IDENTITY_3X3}; //!< trick_units(--)

    /**
     * Body rate wrt inertial in body frame.
     */
    double body_rate[3]{}; //!< trick_units(rad/s)

    /**
     * Potential energy T, model-dependent.
     */
    double potential_energy{}; //!< trick_units(m2/s2)

    /**
     * Kinetic energy K = 1/2 (Iw) dot w.
     */
    double kinetic_energy{}; //!< trick_units(m2/s2)

    /**
     * Total energy E = T + K.
     */
    double total_energy{}; //!< trick_units(m2/s2)
};

class RotationTest : public IntegrationTest
{
    JEOD_MAKE_SIM_INTERFACES(jeod, RotationTest)

public:
    // Member functions
    RotationTest() = default;

    /**
     * Copy constructor.
     */
    RotationTest(const RotationTest & source, unsigned int test_incr = 0)
        : IntegrationTest(source, test_incr),
          I_xx(source.I_xx)
    {
    }

    /**
     * Destructor.
     */
    ~RotationTest() override
    {
        JEOD_DELETE_OBJECT(integ_generator);
    }

    RotationTest & operator=(const RotationTest & source) = delete;

    /**
      Create a copy of the object.
      \return Constructed IntegrationTest object
    */
    IntegrationTest * replicate(unsigned int test_incr) override = 0;

    // Initialization methods.
    void pre_initialize() override;

    void post_initialize() override;

    void validate_integrability(const er7_utils::IntegratorConstructor & generator) override;

    void create_integrators(const er7_utils::IntegratorConstructor & generator,
                            er7_utils::IntegrationControls & controls,
                            const er7_utils::TimeInterface & time_if) override;

    /**
     * Destroy the translation test state integrator.
     */
    void destroy_integrators() override
    {
        integ_generator->destroy_integrator();
        JEOD_DELETE_OBJECT(integ_generator);
    }

    // Propagating over time.
    void propagate(double dyn_time, bool update_errors) override;

    // Integrate over delta-time.
    er7_utils::IntegratorResult integrate(double dyn_dt, unsigned int target_stage) override;

    // Compute derivatives; assumed to be time-independent.
    void compute_derivatives() override;

    // Generate shutdown report.
    void shutdown(double sim_time, double dyn_time, FILE * report) override;

    /**
     * Compute the integrated and true potential energy.
     * The default implementation is that there is no potential.
     */
    virtual void compute_potential()
    {
        prop_integ_state.potential_energy = true_canon_state.potential_energy = 0.0;
    }

    /**
     Compute the torque on the object.
     The default implementation is that there is no torque.
    */
    virtual void compute_torque()
    {
        Vector3::initialize(torq_body);
    }

protected:
    // Data types

    /**
     * Quaternion-based rotational state integrator that uses the
     * quaternion derivative as the basis for integration.
     */
    using GeneralizedDerivQuatInteg = RestartableGeneralizedDerivSecondOrderODEIntegrator<
        4,
        3,
        er7_utils::LeftQuaternionGeneralizedPositionFunctions>;

    /**
     * Quaternion-based rotational state integrator that uses the
     * quaternion exponential as the basis for integration.
     */
    using LieGroupQuatInteg =
        RestartableGeneralizedStepSecondOrderODEIntegrator<4, 3, er7_utils::LeftQuaternionGeneralizedPositionFunctions>;

    // Member data

    /**
     * User-input x-axis moment of inertia (canonical body)
     */
    double I_xx{1e8}; //!< trick_units(kg*m2)

    /**
     * Unit vector in the z-hat direction.
     * This is set in the constructor and never modified.
     * Please do not touch.
     */
    double zhat[3]{0.0, 0.0, 1.0}; //!< trick_io(*o) trick_units(--)

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
    double inverse_inertia[3][3]{}; //!< trick_units(1/kg/m2)

    /**
     * Time derivative of prop_integ->Q_inertial_body.
     */
    Quaternion Qdot_inertial_body; //!< trick_units(--)

    /**
     * Time derivative of prop_integ->body_rate.
     */
    double body_accel[3]{}; //!< trick_units(rad/s2)

    /**
     * Torque in body frame.
     */
    double torq_body[3]{}; //!< trick_units(N*m)

    /**
     * Error in the integrated orientation.
     */
    AngularVariance orient_error; //!< trick_units(--)

    /**
     * Relative energy error.
     */
    double rel_energy_error{}; //!< trick_units(--)

    /**
     * Error in the propagated rate.
     */
    double rate_error[3]{}; //!< trick_units(rad/s)

    /**
     * Magnitude of the above.
     */
    double rate_error_mag{}; //!< trick_units(rad/s)

    /**
     * Maximum of abs(orient_error(theta)).
     */
    double max_angle_error{}; //!< trick_units(rad)

    /**
     * Time at which above was set.
     */
    double max_angle_time{}; //!< trick_units(s)

    /**
     * Maximum of rate_error_mag.
     */
    double max_rate_error{}; //!< trick_units(rad/s)

    /**
     * Time at which above was set.
     */
    double max_rate_time{}; //!< trick_units(s)

    /**
     * Maximum of rel_energy_error.
     */
    double max_energy_error{}; //!< trick_units(--)

    /**
     * Time at which above was set.
     */
    double max_energy_time{}; //!< trick_units(s)

    /**
     * Minimum of rel_energy_error.
     */
    double min_energy_error{}; //!< trick_units(--)

    /**
     * Time at which above was set.
     */
    double min_energy_time{}; //!< trick_units(s)

#ifndef SWIG

    /**
     * Rotational state checkpointable/restartable integrator generator.
     */
    RestartableSecondOrderODEIntegrator * integ_generator{}; //!< trick_io(**)

#endif
};

class RotationTestTorqueFreeSphere : public RotationTest
{
    JEOD_MAKE_SIM_INTERFACES(jeod, RotationTestTorqueFreeSphere)

public:
    RotationTestTorqueFreeSphere() = default;
    ~RotationTestTorqueFreeSphere() override = default;
    RotationTestTorqueFreeSphere & operator=(const RotationTestTorqueFreeSphere &) = delete;

    /**
      Copy constructor.
    */
    RotationTestTorqueFreeSphere(const RotationTestTorqueFreeSphere & source, unsigned int test_incr = 0)
        : RotationTest(source, test_incr)
    {
    }

    // Overloaded methods

    IntegrationTest * replicate(unsigned int test_incr) override;

    void pre_initialize() override;

    void post_initialize() override;

    void propagate(double dyn_dt, bool update_errors) override;
};

class RotationTestTorqueFreeSymTop : public RotationTest
{
    JEOD_MAKE_SIM_INTERFACES(jeod, RotationTestTorqueFreeSymTop)

    // Member functions
public:
    RotationTestTorqueFreeSymTop() = default;
    ~RotationTestTorqueFreeSymTop() override = default;
    RotationTestTorqueFreeSymTop & operator=(const RotationTestTorqueFreeSymTop &) = delete;

    /**
      Non-default constructor.
    */
    RotationTestTorqueFreeSymTop(double f_zz_in, double theta_degrees_in)
        : f_zz(f_zz_in),
          theta(theta_degrees_in * M_PI / 180.0)
    {
    }

    /**
      Copy constructor.
    */
    RotationTestTorqueFreeSymTop(const RotationTestTorqueFreeSymTop & source, unsigned int test_incr = 0)
        : RotationTest(source, test_incr),
          f_zz(source.f_zz),
          theta(source.theta)
    {
    }

    // Overloaded methods
    IntegrationTest * replicate(unsigned int test_incr) override;

    void pre_initialize() override;

    void post_initialize() override;

    void propagate(double dyn_dt, bool update_errors) override;

    // Member data
protected:
    /**
     * User-input z-axis moment of inertia factor (canonical body frame).
     * The z-axis moment of inertia is the product of this factor and the
     * inherited x-axis moment of inertia, I_xx.
     */
    double f_zz{0.8}; //!< trick_units(--)

    /**
     * User-input angle between the canonical body z axis and the angular
     * velocity vector.
     */
    double theta{50.0 * M_PI / 180.0}; //!< trick_units(rad)

    /**
     * Z-axis (canonical body) component of the angular velocity.
     */
    double omega_z{}; //!< trick_units(rad/s)

    /**
     * Non-z component (canonical body) of the angular velocity.
     */
    double omega_xy{}; //!< trick_units(rad/s)

    /**
     * Angle between the angular momentum and angular velocity vectors.
     */
    double phi{}; //!< trick_units(rad)

    /**
     * Body precession rate.
     */
    double omega_psi{}; //!< trick_units(rad/s)

    /**
     * Body rotation rate.
     */
    double omega_theta{}; //!< trick_units(rad/s)

    /**
     * Angular velocity precession rate.
     */
    double cap_omega{}; //!< trick_units(rad/s)
};

class RotationTestTorqueFreeSymTopB : public RotationTestTorqueFreeSymTop
{
    JEOD_MAKE_SIM_INTERFACES(jeod, RotationTestTorqueFreeSymTopB)

    // Member functions
public:
    /**
      Default constructor.
    */
    RotationTestTorqueFreeSymTopB()
        : RotationTestTorqueFreeSymTop(0.8, 1.0)
    {
    }

    /**
      Copy constructor.
    */
    RotationTestTorqueFreeSymTopB(const RotationTestTorqueFreeSymTopB & source, unsigned int test_incr = 0)
        : RotationTestTorqueFreeSymTop(source, test_incr)
    {
    }

    ~RotationTestTorqueFreeSymTopB() override = default;
    RotationTestTorqueFreeSymTopB & operator=(const RotationTestTorqueFreeSymTopB &) = delete;

    // Overloaded methods
    IntegrationTest * replicate(unsigned int test_incr) override;
};

class RotationTestSHOSphere : public RotationTest
{
    JEOD_MAKE_SIM_INTERFACES(jeod, RotationTestSHOSphere)

public:
    RotationTestSHOSphere() = default;
    ~RotationTestSHOSphere() override = default;
    RotationTestSHOSphere & operator=(const RotationTestSHOSphere &) = delete;

    /**
      Copy constructor.
    */
    RotationTestSHOSphere(const RotationTestSHOSphere & source, unsigned int test_incr = 0)
        : RotationTest(source, test_incr),
          theta_max(source.theta_max)
    {
    }

    // Overloaded methods.
    IntegrationTest * replicate(unsigned int test_incr) override;

    void pre_initialize() override;

    void post_initialize() override;

    void propagate(double dyn_dt, bool update_errors) override;

    void compute_potential() override;
    void compute_torque() override;

    // Member data
protected:
    /**
     * Initial SHO angular displacement.
     */
    double theta_max{175.0 * M_PI / 180.0}; //!< trick_units(rad)

    /**
     * Restoring torque constant.
     */
    double k_sho{}; //!< trick_units(N*m)

    /**
     * Angular variance, used to compute torque.
     */
    AngularVariance ang_var; //!< trick_units(--)
};

} // namespace jeod

#include "utils/math/include/macro_undef.hh"

#endif
