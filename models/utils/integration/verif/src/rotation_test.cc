/*******************************************************************************

Purpose:
  (Define methods for the class RotationTest.)

Reference:
  (((TBS)))

Library dependencies:
  ((rotation_test.cc)
   (angular_variance.cc)
   (integration_test.cc)
   (new_orientation.cc)
   (random.cc)
   (utils/quaternion/src/quat.cc)
   (utils/quaternion/src/quat_norm.cc)
   (utils/quaternion/src/quat_to_mat.cc))



*******************************************************************************/

// System includes
#include <cmath>
#include <cstddef>
#include <cstdio>

// JEOD includes
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/named_item/include/named_item.hh"

// Model includes
#include "../include/rotation_test.hh"

//! Namespace jeod
namespace jeod
{

// Attributes used in allocations
JEOD_DECLARE_ATTRIBUTES(RotationTestTorqueFreeSymTop)
JEOD_DECLARE_ATTRIBUTES(RotationTestTorqueFreeSphere)
JEOD_DECLARE_ATTRIBUTES(RotationTestSHOSphere)

void RotationTest::validate_integrability(const er7_utils::IntegratorConstructor & generator)
{
    if(!active)
    {
        return;
    }

    bool is_ok = false;

    if(deprecated_rotation_integration)
    {
        is_ok = generator.implements(er7_utils::Integration::GeneralizedDerivSecondOrderODE);
    }

    else
    {
        is_ok = generator.implements(er7_utils::Integration::GeneralizedStepSecondOrderODE);
    }

    // Disable. Silently?
    if(!is_ok)
    {
        active = false;
        MessageHandler::warn(__FILE__,
                             __LINE__,
                             IntegrationTestMessages::internal_error,
                             "Invalid integration technique for rotation");
    }
}

void RotationTest::create_integrators(const er7_utils::IntegratorConstructor & generator,
                                      er7_utils::IntegrationControls & controls,
                                      const er7_utils::TimeInterface & time_if __attribute__((unused)))
{
    if(deprecated_rotation_integration)
    {
        if(generator.implements(er7_utils::Integration::GeneralizedDerivSecondOrderODE))
        {
            JEOD_REGISTER_NONEXPORTED_CLASS(GeneralizedDerivQuatInteg);
            integ_generator = JEOD_ALLOC_CLASS_OBJECT(GeneralizedDerivQuatInteg, (state_integrator));
        }
    }

    else
    {
        if(generator.implements(er7_utils::Integration::GeneralizedStepSecondOrderODE))
        {
            JEOD_REGISTER_NONEXPORTED_CLASS(LieGroupQuatInteg);
            integ_generator = JEOD_ALLOC_CLASS_OBJECT(LieGroupQuatInteg, (state_integrator));
        }
    }

    if(integ_generator != nullptr)
    {
        integ_generator->create_integrator(generator, controls);
    }
    else
    {
        active = false;
        MessageHandler::error(__FILE__,
                              __LINE__,
                              IntegrationTestMessages::internal_error,
                              "Invalid integration technique");
    }
}

/*
Purpose:
  (Pre-initialize an RotationTest instance.)
*/
void RotationTest::pre_initialize()
{
    // Perform base initializations.
    IntegrationTest::pre_initialize();
}

/*
Purpose:
  (Post-initialize a RotationTest instance.)
*/
void RotationTest::post_initialize()
{
    Quaternion tempquat;
    double ang_mom[3];
    double Imin;

    // Compute the canonical frame inertial-to-body matrix.
    // Derived classes should have already set the inertial-to-body quaternion.
    true_canon_state.Q_inertial_body.left_quat_to_transformation(true_canon_state.T_inertial_body);

    // Compute the integration inertial-to-body quaternion and matrix.
    true_canon_state.Q_inertial_body.multiply(Q_iinteg_icanon, tempquat);
    Q_bcanon_binteg.multiply(tempquat, true_integ_state.Q_inertial_body);
    true_integ_state.Q_inertial_body.left_quat_to_transformation(true_integ_state.T_inertial_body);

    // Transform the canonical body rate to the propagation body frame.
    Vector3::transform(T_bcanon_binteg, true_canon_state.body_rate, true_integ_state.body_rate);

    // Transform the canonical inertia matrix to the propagation body frame.
    Matrix3x3::transform_matrix(T_bcanon_binteg, true_canon_state.inertia, true_integ_state.inertia);

    // Get rid of annoying small values in the above.
    Imin = I_xx * 1e-17;
    Vector3::zero_small(Imin, true_integ_state.inertia[0]);
    Vector3::zero_small(Imin, true_integ_state.inertia[1]);
    Vector3::zero_small(Imin, true_integ_state.inertia[2]);

    // Compute initial kinetic energy (potential was done by derived class).
    Vector3::transform(true_canon_state.inertia, true_canon_state.body_rate, ang_mom);
    true_canon_state.kinetic_energy = 0.5 * Vector3::dot(ang_mom, true_canon_state.body_rate);
    true_canon_state.total_energy = true_canon_state.potential_energy + true_canon_state.kinetic_energy;

    Vector3::transform(true_integ_state.inertia, true_integ_state.body_rate, ang_mom);
    true_integ_state.kinetic_energy = 0.5 * Vector3::dot(ang_mom, true_integ_state.body_rate);

    // Initialize the propagated state with the true state in the
    // propagation frame.
    Matrix3x3::copy(true_integ_state.inertia, prop_integ_state.inertia);
    prop_integ_state.Q_inertial_body = true_integ_state.Q_inertial_body;
    Matrix3x3::copy(true_integ_state.T_inertial_body, prop_integ_state.T_inertial_body);
    Vector3::copy(true_integ_state.body_rate, prop_integ_state.body_rate);
    prop_integ_state.kinetic_energy = true_integ_state.kinetic_energy;

    // Initialize the integrated angular variance.
    Q_bcanon_binteg.multiply(Q_iinteg_icanon, Q_ref);
    prop_integ_state.Q_inertial_body.multiply_conjugate(Q_ref, tempquat);
    prop_theta.initialize(tempquat, prop_integ_state.body_rate);

    // The object under study does not change mass.
    // Compute the inverse of the inertia tensor.
    Matrix3x3::invert_symmetric(prop_integ_state.inertia, inverse_inertia);

    // Complete base initializations.
    IntegrationTest::post_initialize();
}

/*
Purpose:
  (Compute state derivative.)
 */
void RotationTest::compute_derivatives()
{
    double ang_mom[3];       // kg*M^2/s    Angular momentum, body referenced
    double inertial_torq[3]; // kg*M^2/s^2  Inertial torque
    double total_torq[3];    // kg*M^2/s^2  External torque less inertial torque

    // Compute torque (if any), body referenced.
    compute_torque();

    // Compute the inertial torque.
    Vector3::transform(prop_integ_state.inertia, prop_integ_state.body_rate, ang_mom);
    Vector3::cross(prop_integ_state.body_rate, ang_mom, inertial_torq);

    // Subtract from the external torque to form the body-frame
    // apparent torque.
    Vector3::diff(torq_body, inertial_torq, total_torq);

    /* Solve the rotational EOM for body accelerations. */
    Vector3::transform(inverse_inertia, total_torq, body_accel);
}

/*
Purpose:
  (Integrate the integrated state.)
 */
er7_utils::IntegratorResult RotationTest::integrate( // Return: -- Integration status
    double dyn_dt,                                   // In:     s  Step size
    unsigned int target_stage)                       // In:     -- Target stage number
{
    // We should always have an integrator at this point.
    if(state_integrator == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             IntegrationTestMessages::internal_error,
                             "Invalid integration technique");
        return false;
    }

    // Integrate the rotational state, caching the result in the return status.
    // Note well: This apparent copy copy-elided away thanks to RVO.
    er7_utils::IntegratorResult status = state_integrator->integrate(dyn_dt,
                                                                     target_stage,
                                                                     body_accel,
                                                                     prop_integ_state.body_rate,
                                                                     prop_integ_state.Q_inertial_body);

    // Normalize the quaternion.
    prop_integ_state.Q_inertial_body.normalize_integ();

    // Compute the corresponding transformation matrix.
    prop_integ_state.Q_inertial_body.left_quat_to_transformation(prop_integ_state.T_inertial_body);

    // Compute the quaternion derivative.
    prop_integ_state.Q_inertial_body.compute_left_quat_deriv(prop_integ_state.body_rate, Qdot_inertial_body);

    // Return the status of the integration.
    return status;
}

void RotationTest::propagate(double dyn_time, bool update_errors)
{
    Quaternion tempquat;
    double ang_mom[3];
    double abs_err;

    // Compute the integrated angular variance from reference.
    prop_theta.update(prop_integ_state.Q_inertial_body, Q_ref, prop_integ_state.body_rate);
    prop_integ_state.theta = prop_theta.theta();

    // Compute integrated potential energy.
    compute_potential();

    // Compute the integrated kinetic and total energy.
    Vector3::transform(prop_integ_state.inertia, prop_integ_state.body_rate, ang_mom);
    prop_integ_state.kinetic_energy = 0.5 * Vector3::dot(ang_mom, prop_integ_state.body_rate);
    prop_integ_state.total_energy = prop_integ_state.potential_energy + prop_integ_state.kinetic_energy;

    // Compute the true kinetic and total energy.
    Vector3::transform(true_canon_state.inertia, true_canon_state.body_rate, ang_mom);
    true_canon_state.kinetic_energy = 0.5 * Vector3::dot(ang_mom, true_canon_state.body_rate);
    true_canon_state.total_energy = true_canon_state.potential_energy + true_canon_state.kinetic_energy;

    // Transform computed canonical state to the integration frames.
    true_integ_state.theta = true_canon_state.theta;

    true_canon_state.Q_inertial_body.multiply(Q_iinteg_icanon, tempquat);
    Q_bcanon_binteg.multiply(tempquat, true_integ_state.Q_inertial_body);
    true_integ_state.Q_inertial_body.left_quat_to_transformation(true_integ_state.T_inertial_body);

    Vector3::transform(T_bcanon_binteg, true_canon_state.body_rate, true_integ_state.body_rate);

    true_integ_state.potential_energy = true_canon_state.potential_energy;
    true_integ_state.kinetic_energy = true_canon_state.kinetic_energy;
    true_integ_state.total_energy = true_canon_state.total_energy;

    // Compute errors
    rel_energy_error = prop_integ_state.total_energy / true_integ_state.total_energy - 1.0;

    Vector3::diff(prop_integ_state.body_rate, true_integ_state.body_rate, rate_error);
    rate_error_mag = Vector3::vmag(rate_error);

    orient_error.update(prop_integ_state.Q_inertial_body, true_integ_state.Q_inertial_body, rate_error);

    // Track maximum errors
    if(update_errors)
    {
        if(max_energy_error < rel_energy_error)
        {
            max_energy_error = rel_energy_error;
            max_energy_time = dyn_time;
        }

        if(min_energy_error > rel_energy_error)
        {
            min_energy_error = rel_energy_error;
            min_energy_time = dyn_time;
        }

        abs_err = fabs(orient_error.theta());
        if(max_angle_error < abs_err)
        {
            max_angle_error = abs_err;
            max_angle_time = dyn_time;
        }

        if(max_rate_error < rate_error_mag)
        {
            max_rate_error = rate_error_mag;
            max_rate_time = dyn_time;
        }
    }
}

void RotationTest::shutdown( // Return: -- Void
    double sim_time,         // In:     s  Simulation time
    double dyn_time,         // In:     s  Dynamic time
    FILE * report)           // Inout:  -- Output stream
{
    // Report defaults to stdout if not set.
    if(report == nullptr)
    {
        report = stdout;
    }

    // Print the common header.
    IntegrationTest::shutdown(sim_time, dyn_time, report);

    // Print summary and maximum errors.
    fprintf(report,
            "  max angle err   (d) = %g\n"
            "    scaled            = %g\n"
            "    @ t               = %g\n"
            "  max energy err (--) = %g\n"
            "    @ t               = %g\n"
            "  min energy err (--) = %g\n"
            "    @ t               = %g\n"
            "  max rate err  (d/s) = %g\n"
            "    scaled            = %g\n"
            "    @ t               = %g\n",
            max_angle_error * 180.0 / M_PI,
            max_angle_error / (omega * max_angle_time),
            max_angle_time,
            max_energy_error,
            max_energy_time,
            min_energy_error,
            min_energy_time,
            max_rate_error * 180.0 / M_PI,
            max_rate_error / omega,
            max_rate_time);
}

/*
Purpose:
  (Create a copy of the object.)
*/
IntegrationTest * RotationTestTorqueFreeSphere::replicate( // Return: -- Constructed object
    unsigned int test_incr)                                // In:     -- Test increment
{
    return JEOD_ALLOC_CLASS_OBJECT(RotationTestTorqueFreeSphere, (*this, test_incr));
}

/*
Purpose:
  (Pre-initialize a RotationTestTorqueFreeSphere instance.)
*/
void RotationTestTorqueFreeSphere::pre_initialize()
{
    // Perform base initializations.
    RotationTest::pre_initialize();
}

/*
Purpose:
  (Post-initialize a RotationTestTorqueFreeSphere instance.)
 */
void RotationTestTorqueFreeSphere::post_initialize()
{
    // The object is rotating aroung the canonical zhat axis,
    // starting with the trivial orientation.
    Vector3::scale(zhat, omega, true_canon_state.body_rate);
    true_canon_state.Q_inertial_body.make_identity();

    // Initialize the angular
    true_canon_state.theta = prop_integ_state.theta = 0.0;

    // Torque-free => No potential energy.
    true_canon_state.potential_energy = prop_integ_state.potential_energy = 0.0;

    // Construct the inertia tensor.
    // This is a spherical body.
    Matrix3x3::initialize(true_canon_state.inertia);
    true_canon_state.inertia[0][0] = true_canon_state.inertia[1][1] = true_canon_state.inertia[2][2] = I_xx;

    // Complete base initializations.
    RotationTest::post_initialize();
}

void RotationTestTorqueFreeSphere::propagate(double dyn_time, bool update_errors)
{
    true_canon_state.theta = omega * dyn_time;
    true_canon_state.Q_inertial_body.left_quat_from_eigen_rotation(true_canon_state.theta, zhat);

    RotationTest::propagate(dyn_time, update_errors);
}

/*
Purpose:
  (Create a copy of the object.)
*/
IntegrationTest * RotationTestTorqueFreeSymTop::replicate( // Return: -- Constructed object
    unsigned int test_incr)                                // In:     -- Test increment
{
    return JEOD_ALLOC_CLASS_OBJECT(RotationTestTorqueFreeSymTop, (*this, test_incr));
}

/*
Purpose:
  (Pre-initialize a RotationTestTorqueFreeSymTop instance.)
*/
void RotationTestTorqueFreeSymTop::pre_initialize()
{
    // Perform base initializations.
    RotationTest::pre_initialize();
}

/*
Purpose:
  (Post-initialize a RotationTestTorqueFreeSymTop instance.)
 */
void RotationTestTorqueFreeSymTop::post_initialize()
{
    double fwz;
    double xhat[3] = {1.0, 0.0, 0.0};

    /* The behavior of a torque-free symmetric top is classical problem in
     * classical physics. The angular momentum vector is constant in inertial
     * space. The anguler velocity vector is constant in magnitude but not in
     * orientation; its tip traces out a cone over time. The angle between the
     * angular momentum and angular velocity vectors is a constant of motion,
     * denoted as phi here. */

    omega_xy = omega * sin(theta);
    omega_z = omega * cos(theta);

    fwz = f_zz * omega_z;
    phi = atan2(omega_xy, fwz);
    omega_psi = sqrt(omega_xy * omega_xy + fwz * fwz);
    cap_omega = omega_z - fwz;
    omega_theta = cap_omega + omega_psi;

    // Construct the initial angular velocity vector.
    true_canon_state.body_rate[0] = 0.0;
    true_canon_state.body_rate[1] = omega_xy;
    true_canon_state.body_rate[2] = omega_z;

    // Initialize the object in the canonical orientation.
    true_canon_state.Q_inertial_body.left_quat_from_eigen_rotation(phi, xhat);

    true_canon_state.theta = prop_integ_state.theta = phi;

    // Construct the inertia tensor.
    // This is a symmetric top; the canonical body z-axis is the axis
    // of symmetry.
    Matrix3x3::initialize(true_canon_state.inertia);
    true_canon_state.inertia[0][0] = true_canon_state.inertia[1][1] = I_xx;
    true_canon_state.inertia[2][2] = f_zz * I_xx;

    // Complete base initializations.
    RotationTest::post_initialize();
}

void RotationTestTorqueFreeSymTop::propagate(double dyn_time, bool update_errors)
{
    double wt;
    double uhat[3];
    Quaternion Q_psi;
    Quaternion Q_z;

    wt = cap_omega * dyn_time;
    true_canon_state.body_rate[0] = omega_xy * sin(wt);
    true_canon_state.body_rate[1] = omega_xy * cos(wt);
    true_canon_state.body_rate[2] = omega_z;

    wt = omega_psi * dyn_time;
    uhat[0] = cos(wt);
    uhat[1] = sin(wt);
    uhat[2] = 0.0;
    Q_psi.left_quat_from_eigen_rotation(phi, uhat);

    wt = omega_theta * dyn_time;
    Q_z.left_quat_from_eigen_rotation(wt, zhat);

    Q_z.multiply(Q_psi, true_canon_state.Q_inertial_body);

    RotationTest::propagate(dyn_time, update_errors);
}

/*
Purpose:
  (Create a copy of the object.)
*/
IntegrationTest * RotationTestTorqueFreeSymTopB::replicate( // Return: -- Constructed object
    unsigned int test_incr)                                 // In:     -- Test increment
{
    return JEOD_ALLOC_CLASS_OBJECT(RotationTestTorqueFreeSymTopB, (*this, test_incr));
}

/*
Purpose:
  (Create a copy of the object.)
*/
IntegrationTest * RotationTestSHOSphere::replicate( // Return: -- Constructed object
    unsigned int test_incr)                         // In:     -- Test increment
{
    return JEOD_ALLOC_CLASS_OBJECT(RotationTestSHOSphere, (*this, test_incr));
}

/*
Purpose:
  (Pre-initialize a RotationTestSHOSphere instance.)
*/
void RotationTestSHOSphere::pre_initialize()
{
    // Perform base initializations.
    RotationTest::pre_initialize();
}

/*
Purpose:
  (Post-initialize a RotationTestSHOSphere instance.)
 */
void RotationTestSHOSphere::post_initialize()
{
    /* The EOM for the simple harmonic oscillator are
     *   theta     =             theta_max * cos(omega * t)
     *   theta_dot  = -omega   * theta_max * sin(omega * t)
     *   theta_ddot = -omega^2 * theta
     * The dynamics dictate that
     *   tau = -k_sho * theta = I_xx * theta_ddot = -I_xx * omega^2 * theta
     * Thus the SHO coefficient k_sho = I_xx * omega^2. */
    k_sho = I_xx * omega * omega;

    // The SHO starts at maximal angular displacement and zero angular velocity.
    true_canon_state.theta = theta_max;
    Vector3::initialize(true_canon_state.body_rate);

    // The canonical reference orientation is the SHO rest orientation.
    // Construct the initial canonical quaternion.
    true_canon_state.Q_inertial_body.left_quat_from_eigen_rotation(true_canon_state.theta, zhat);

    // Compute the initial potential energy.
    true_canon_state.potential_energy = 0.5 * k_sho * theta_max * theta_max;

    // Construct the inertia tensor.
    // This is a spherical body.
    Matrix3x3::initialize(true_canon_state.inertia);
    true_canon_state.inertia[0][0] = true_canon_state.inertia[1][1] = true_canon_state.inertia[2][2] = I_xx;

    // Complete base initializations.
    RotationTest::post_initialize();
}

/*
Purpose:
  (Compute the integrated and true potential energy.)
*/
void RotationTestSHOSphere::compute_potential()
{
    prop_integ_state.potential_energy = 0.5 * k_sho * prop_integ_state.theta * prop_integ_state.theta;
    true_canon_state.potential_energy = 0.5 * k_sho * true_canon_state.theta * true_canon_state.theta;
}

/*
Purpose:
  (Compute the integrated and true potential energy.)
*/
void RotationTestSHOSphere::propagate(double dyn_time, bool update_errors)
{
    double wt = omega * dyn_time;
    double coswt = cos(wt);
    double sinwt = sin(wt);

    // Compute the true state.
    true_canon_state.theta = theta_max * coswt;
    true_canon_state.Q_inertial_body.left_quat_from_eigen_rotation(true_canon_state.theta, zhat);
    Vector3::scale(zhat, -omega * theta_max * sinwt, true_canon_state.body_rate);

    // Pass the method up the inheritance chain.
    RotationTest::propagate(dyn_time, update_errors);
}

/*
Purpose:
  (Compute the torque on the object.)
*/
void RotationTestSHOSphere::compute_torque()
{
    ang_var = prop_theta;
    ang_var.update(prop_integ_state.Q_inertial_body, Q_ref, prop_integ_state.body_rate);
    Vector3::scale(ang_var.uhat(), -k_sho * ang_var.theta(), torq_body);
}
} // namespace jeod
