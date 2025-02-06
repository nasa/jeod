/**
 * @addtogroup Models
 * @{
 * @addtogroup Experimental
 * @{
 * @addtogroup Constraints
 * @{
 *
 * @file
 * Define PendulumConstraintComponent member functions.
 */

/*
Purpose: ()
*/


#include "../include/pendulum_constraint_component.hh"

#include "../include/base_pendulum_model.hh"
#include "../include/dyn_body_constraints_solver.hh"

#include "dynamics/dyn_body/include/dyn_body.hh"
#include "dynamics/dyn_body/include/vehicle_non_grav_state.hh"
#include "dynamics/dyn_body/include/vehicle_properties.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"
#include "experimental/message/include/fail_simulation.hh"

#include "er7_utils/integration/core/include/base_integration_group.hh"

#include <cmath>


//! Namespace jeod 
namespace jeod {

PendulumConstraintComponent::PendulumConstraintComponent (
    ConstraintFrame* constraint_frame_in,
    ConstrainedPointMass* constrained_mass_in,
    BasePendulumModel* pendulum_model_in)
:
    ForceConstraintComponent(constraint_frame_in, constrained_mass_in),
    er7_utils::IntegrableObject(),
    pendulum_model(pendulum_model_in),
    bob_integrator(),
    integ_results_merger(),
    cos_sq_theta_angular(0.45),
    cos_sq_theta_cartesian(0.55),
    pendulum_length(0.0),
    theta(0.0),
    phi(0.0),
    theta_dot(0.0),
    phi_dot(0.0),
    theta_dot_dot(0.0),
    phi_dot_dot(0.0),
    sin_theta(0.0),
    cos_theta(0.0),
    sin_phi(0.0),
    cos_phi(0.0),
    null_wrench(true),
    damping_wrench(),
    mode(PendulumConstraintMode::Inactive)
{
    Vector3::initialize (bob_position);
    Vector3::initialize (bob_velocity);
    Vector3::initialize (bob_accel);
    Matrix3x3::identity (constraint_to_spherical_transform);
    Vector3::initialize (omega_constraint_frame);
    Vector3::initialize (omega_dot_constraint_frame);
    Vector3::initialize (omega_cross_pos);
    Vector3::initialize (omega_cross_vel);
    Vector3::initialize (rho_hat_dot);
}


void
PendulumConstraintComponent::create_integrators (
   const er7_utils::IntegratorConstructor & generator,
   er7_utils::IntegrationControls & controls,
   const er7_utils::TimeInterface &)
{

    // FIXME: JEOD bug:
    // This is something that should be done by
    // JeodIntegrationGroup::add_integrable_object.
    er7_utils::BaseIntegrationGroup* integ_group =
        dyn_body->get_integration_group();
    if (integ_group == nullptr)
    {
        FAIL_SIMULATION ("DynBody object is not in an integration group");
    }
    set_integration_group (*integ_group);

    // Create / configure the integrator and results merge objects.
    bob_integrator.create_integrator(generator, controls);
    integ_results_merger.configure (generator);
}


void
PendulumConstraintComponent::reset_integrators()
{
    pendulum_model->update_pendulum_model ();
    constrained_mass->mass = pendulum_model->get_pendulum_mass();
    pendulum_model->get_hinge_point (
        constrained_mass->constraint_position_constraint_frame);
    set_pendulum_length (pendulum_model->get_pendulum_length());

    bob_integrator.reset_integrator ();

    if (new_mode != mode)
    {
        if (new_mode == PendulumConstraintMode::Reset)
        {
            Vector3::initialize(bob_accel);
            Vector3::initialize(bob_velocity);
            Vector3::initialize(bob_position);
            compute_bob_state_cartesian();
            new_mode = PendulumConstraintMode::Vertical;
        }
        else if (new_mode == PendulumConstraintMode::Horizontal)
        {
            transition_to_horizontal();
        }
        mode = new_mode;
    }
}


void
PendulumConstraintComponent::attach_to_solver (
    DynBodyConstraintsSolver*,
    DynBody* dyn_body_in)
{
    dyn_body = dyn_body_in;
    dyn_body->add_integrable_object(*this);
}


void
PendulumConstraintComponent::detach_from_solver (
    DynBodyConstraintsSolver *,
    DynBody*)
{
    if (dyn_body != nullptr)
    {
        dyn_body->remove_integrable_object(*this);
        dyn_body = nullptr;
    }
}


void
PendulumConstraintComponent::activate ()
{
    mode = new_mode = PendulumConstraintMode::Vertical;
    Vector3::initialize (bob_position);
    Vector3::initialize (bob_velocity);
    bob_position[2] = 1.0;
    cos_theta = cos_sq_theta = 1.0;
}


void
PendulumConstraintComponent::deactivate ()
{
    mode = PendulumConstraintMode::Inactive;
}


bool
PendulumConstraintComponent::check_for_reset ()
{
    // Need to reset the integrators if this has been forced from the outside.
    if (new_mode == PendulumConstraintMode::Reset)
    {
        return true;
    }

    // Need to reset the integrators if the pendulum's integrated state is the
    // cartesian state but it now would be better to integrate angular state.
    else if (((mode == PendulumConstraintMode::Vertical) ||
              (mode == PendulumConstraintMode::Inverted)) &&
             (cos_sq_theta < cos_sq_theta_angular))
    {
        new_mode = PendulumConstraintMode::Horizontal;
        return true;
    }

    // Need to reset the integrators if the pendulum's integrated state is the
    // angular state but it now would be better to integrate cartesian state.
    else if ((mode == PendulumConstraintMode::Horizontal) &&
             (cos_sq_theta > cos_sq_theta_cartesian))
    {
        if (bob_position[2] > 0.0)
        {
            new_mode = PendulumConstraintMode::Vertical;
        }
        else
        {
            new_mode = PendulumConstraintMode::Inverted;
        }
        return true;
    }

    else
    {
        new_mode = mode;
        return false;
    }
}


void
PendulumConstraintComponent::transition_to_horizontal ()
{
    double rsq = bob_position[0]*bob_position[0] + bob_position[1]*bob_position[1];
    double r = std::sqrt(rsq);
    sin_theta = r / pendulum_length;
    cos_theta = bob_position[2] / pendulum_length;
    sin_phi = bob_position[1] / r;
    cos_phi = bob_position[0] / r;
    theta = std::acos(cos_theta);
    phi = std::atan2(sin_phi, cos_phi);
    construct_constraint_to_spherical_transform ();
    theta_dot = Vector3::dot(
        bob_velocity, constraint_to_spherical_transform[1])
        / pendulum_length;
    phi_dot = Vector3::dot(
        bob_velocity, constraint_to_spherical_transform[2])
        / (pendulum_length*sin_theta);
    compute_rho_hat_dot ();
}


void
PendulumConstraintComponent::set_pendulum_length (
    double new_length)
{
    if (!Numerical::compare_exact(pendulum_length, new_length))
    {
        pendulum_length = new_length;
        if (mode == PendulumConstraintMode::Horizontal)
        {
            compute_bob_state_spherical();
        }
        else
        {
            compute_bob_state_cartesian();
        }
    }
}


void
PendulumConstraintComponent::setup_constraint (
    const VehicleProperties& vehicle_properties,
    const VehicleNonGravState& non_grav_state)
{
    constrained_mass->mass = pendulum_model->get_pendulum_mass();
    pendulum_model->get_hinge_point (
        constrained_mass->constraint_position_constraint_frame);
    set_pendulum_length (pendulum_model->get_pendulum_length());

    constrained_mass->setup_constraint (
        vehicle_properties, non_grav_state);

    ForceConstraintComponent::setup_constraint (
        vehicle_properties, non_grav_state);

    Vector3::transform (
        constraint_frame->T_root_constraint,
        non_grav_state.omega_struct,
        omega_constraint_frame);
    Vector3::cross(omega_constraint_frame, bob_position, omega_cross_pos);
    Vector3::cross(omega_constraint_frame, bob_velocity, omega_cross_vel);

    // See section 5 of the pendulum slosh document for derivation.
    right_hand_side =
        constrained_mass->mass
        * (
           (  Vector3::vmagsq(bob_velocity)
              + Vector3::vmagsq(omega_cross_pos)
              - 2.0*Vector3::dot(omega_cross_vel, bob_position)) / pendulum_length)
        - Vector3::dot(
              constrained_mass->force_constraint_frame,
               constraint_direction_constraint_frame);
}


er7_utils::IntegratorResult
PendulumConstraintComponent::integrate(
    double dyn_dt,
    unsigned int target_stage)
{
    if (mode == PendulumConstraintMode::Horizontal)
    {
        return integrate_spherical (dyn_dt, target_stage);
    }

    else if (mode != PendulumConstraintMode::Inactive)
    {
        return integrate_cartesian (dyn_dt, target_stage);
    }

    else
    {
        return er7_utils::IntegratorResult(false);
    }
}


void
PendulumConstraintComponent::construct_constraint_to_spherical_transform ()
{
    constraint_to_spherical_transform[0][0] =  sin_theta*cos_phi;
    constraint_to_spherical_transform[0][1] =  sin_theta*sin_phi;
    constraint_to_spherical_transform[0][2] =  cos_theta;
    constraint_to_spherical_transform[1][0] =  cos_theta*cos_phi;
    constraint_to_spherical_transform[1][1] =  cos_theta*sin_phi;
    constraint_to_spherical_transform[1][2] = -sin_theta;
    constraint_to_spherical_transform[2][0] = -sin_phi;
    constraint_to_spherical_transform[2][1] =  cos_phi;
    constraint_to_spherical_transform[2][2] =  0.0;
}


er7_utils::IntegratorResult
PendulumConstraintComponent::integrate_spherical(
    double dyn_dt,
    unsigned int target_stage)
{
    er7_utils::IntegratorResult result = bob_integrator.integrate(
        dyn_dt, target_stage, &theta_dot_dot, &theta_dot, &theta);

    sin_theta = std::sin(theta);
    cos_theta = std::cos(theta);
    sin_phi = std::sin(phi);
    cos_phi = std::cos(phi);

    construct_constraint_to_spherical_transform ();
    compute_rho_hat_dot();
    compute_bob_state_spherical();

    cos_sq_theta = cos_theta * cos_theta;

    return result;
}


er7_utils::IntegratorResult
PendulumConstraintComponent::integrate_cartesian (
    double dyn_dt,
    unsigned int target_stage)
{
    er7_utils::IntegratorResult result = bob_integrator.integrate(
        dyn_dt, target_stage, bob_accel, bob_velocity, bob_position);
    compute_bob_state_cartesian();

    cos_theta = bob_position[2] / pendulum_length;
    cos_sq_theta = cos_theta * cos_theta;

    return result;
}


void
PendulumConstraintComponent::compute_rho_hat_dot ()
{
    rho_hat_dot[0] = theta_dot*constraint_to_spherical_transform[1][0]
        + phi_dot*sin_theta*constraint_to_spherical_transform[2][0];
    rho_hat_dot[1] = theta_dot*constraint_to_spherical_transform[1][1]
        + phi_dot*sin_theta*constraint_to_spherical_transform[2][1];
    rho_hat_dot[2] = theta_dot*constraint_to_spherical_transform[1][2];
}


void
PendulumConstraintComponent::compute_bob_state_spherical ()
{
    Vector3::scale (
        constraint_to_spherical_transform[0],
        pendulum_length,
        bob_position);
    Vector3::scale (
        rho_hat_dot,
        pendulum_length,
        bob_velocity);

    set_constraint_direction (constraint_to_spherical_transform[0]);
}


void
PendulumConstraintComponent::compute_bob_state_cartesian ()
{
    // The above only integrated the x & y position and velocity.
    // Compute the z position and velocity.
    double rsq = bob_position[0]*bob_position[0] + bob_position[1]*bob_position[1];
    double lsq = pendulum_length*pendulum_length;
    double zsq = lsq - rsq;

    bob_position[2] = std::sqrt(zsq);
    bob_velocity[2] =
        -(bob_position[0]*bob_velocity[0] + bob_position[1]*bob_velocity[1])
        / bob_position[2];

    if (mode == PendulumConstraintMode::Inverted)
    {
        bob_position[2] = -bob_position[2];
        bob_velocity[2] = -bob_velocity[2];
    }

    cos_theta = bob_position[2] / pendulum_length;
    cos_sq_theta = cos_theta * cos_theta;

    // Set the constraint direction for the next time around.
    double bob_direction[3];
    Vector3::scale (bob_position, 1.0/pendulum_length, bob_direction);
    set_constraint_direction (bob_direction);
}


void
PendulumConstraintComponent::compute_constraint_response (
    const VehicleProperties&,
    const VehicleNonGravState& non_grav_state)
{
    constrained_mass->compute_non_grav_accel (non_grav_state);

    double tension_accel = constraint_force / constrained_mass->mass;
    double damping_factor = pendulum_model->compute_damping_factor(
        Vector3::vmag(constrained_mass->non_grav_accel_root));
    // Or std::abs(tension_accel)

    /*
     * Note: Tension accel could be calculated using the same expression
     * used in setup_constraint(), but there's no point. The solution
     * to the constraints system of equations for this constrained object
     * is the tension force.
     */

    Vector3::transform (
        constraint_frame->T_root_constraint,
        non_grav_state.omega_struct,
        omega_constraint_frame);
    Vector3::transform (
        constraint_frame->T_root_constraint,
        non_grav_state.omega_dot_struct,
        omega_dot_constraint_frame);

    if (mode == PendulumConstraintMode::Horizontal)
    {
        double omega_rho = Vector3::dot(constraint_to_spherical_transform[0],
            omega_constraint_frame);
        double accel_theta = Vector3::dot(constraint_to_spherical_transform[1],
            constrained_mass->non_grav_accel_constraint_frame);
        double omega_theta = Vector3::dot(constraint_to_spherical_transform[1],
            omega_constraint_frame);
        double omega_dot_theta = Vector3::dot(constraint_to_spherical_transform[1],
            omega_dot_constraint_frame);
        double accel_phi = Vector3::dot(constraint_to_spherical_transform[2],
            constrained_mass->non_grav_accel_constraint_frame);
        double omega_phi = Vector3::dot(constraint_to_spherical_transform[2],
            omega_constraint_frame);
        double omega_dot_phi = Vector3::dot(constraint_to_spherical_transform[2],
            omega_dot_constraint_frame);

        theta_dot_dot = -(
            damping_factor*theta_dot
            + accel_theta/pendulum_length
            + omega_rho*(omega_theta - 2.0*phi_dot*sin_theta)
            + omega_dot_phi
            - phi_dot*phi_dot*sin_theta*cos_theta);
        phi_dot_dot = -(
            damping_factor*phi_dot
            + (  accel_phi/pendulum_length
               + omega_rho*(omega_phi + 2.0*theta_dot)
               - omega_dot_theta
               + 2.0*theta_dot*phi_dot*cos_theta
              ) / sin_theta);
    }
    else
    {
        double tension_accel_div_length = tension_accel / pendulum_length;
        double omega_dot_cross_pos[3];
        double omega_cross_omega_cross_pos[3];
        double* non_grav_accel_constraint_frame =
            constrained_mass->non_grav_accel_constraint_frame;

        Vector3::cross(omega_dot_constraint_frame, bob_position, omega_dot_cross_pos);
        Vector3::cross(
            omega_constraint_frame, omega_cross_pos, omega_cross_omega_cross_pos);

        for (int ii = 0; ii < 2; ++ii)
        {
            // Calculation is done in two steps for numerical reasons.
            // The first part will typically be smaller than the second.
            bob_accel[ii] =
               -(  damping_factor * bob_velocity[ii]           // Drag
                 + omega_dot_cross_pos[ii]                      // Euler
                 + 2.0*omega_cross_vel[ii]                     // Coriolis
                 + omega_cross_omega_cross_pos[ii])              // Centrifugal
               -(  tension_accel_div_length * bob_position[ii]   // Tension
                 + non_grav_accel_constraint_frame[ii]);         // Inertial
        }
    }

    double damping_torque_constraint_frame[3];
    double damping_torque_root[3];
    Vector3::cross (bob_position, bob_velocity, damping_torque_constraint_frame);
    Vector3::scale (
        constrained_mass->mass * damping_factor,
        damping_torque_constraint_frame);
    Vector3::transform_transpose (
        constraint_frame->T_root_constraint,
        damping_torque_constraint_frame,
        damping_torque_root);
    damping_wrench.set_torque(damping_torque_root);
}


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
