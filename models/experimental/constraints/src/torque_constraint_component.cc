/**
 * @addtogroup Models
 * @{
 * @addtogroup Experimental
 * @{
 * @addtogroup Constraints
 * @{
 *
 * @file
 * Defines TorqueConstraintComponent member functions.
 */

/*
Purpose: ()
*/


#include "../include/torque_constraint_component.hh"

#include "../include/constrained_rigid_mass.hh"
#include "../include/constraint_frame.hh"

#include "dynamics/dyn_body/include/vehicle_properties.hh"
#include "experimental/math/include/matrix_view.hh"
#include "utils/math/include/vector3.hh"


//! Namespace jeod 
namespace jeod {

TorqueConstraintComponent::TorqueConstraintComponent (
    ConstraintFrame* constraint_frame_in,
    ConstrainedRigidMass* constrained_mass_in)
:
    ConstraintComponent(constraint_frame_in),
    constrained_mass(constrained_mass_in),
    constraint_torque(0.0)
{ }


// Dependencies:
// The constraint direction and position wrt the root structure frame
// and position wrt the root CoM must be correct.
void
TorqueConstraintComponent::setup_constraint (
    const VehicleProperties& vehicle_properties,
    const VehicleNonGravState&)
{
    // Set the constraint to be a pure unit torque in the constraint direction.
    constraint_wrench.set_torque (constraint_direction_root);

    // Compute the vehicle's rotational acceleration in response to this
    // constraint.
    compute_rotational_response (vehicle_properties, constraint_wrench.get_torque());
}


// Get the constraint matrix element that pertains to this constrained
// object with respect to some other constrained object.
double
TorqueConstraintComponent::get_cross_coeff (
    const VehicleProperties& vehicle_properties,
    const ConstraintComponent& other) const
{
    double omega_dot_constraint_frame[3];
    double torque_constraint_frame[3];

    Vector3::transform (
        constraint_frame->T_root_constraint,
        other.get_rotational_response (vehicle_properties),
        omega_dot_constraint_frame);
    Vector3::transform (
        constrained_mass->inertia,
        omega_dot_constraint_frame,
        torque_constraint_frame);

    return Vector3::dot (
        constraint_direction_constraint_frame, torque_constraint_frame);
}


SolverTypes::DecayedVector3T
TorqueConstraintComponent::get_translational_response (
    const VehicleProperties&,
    SolverTypes::ConstVector3T point,
    SolverTypes::Vector3T response) const
{
    return Vector3::cross (constraint_omega_dot_root, point, response);
}


void
TorqueConstraintComponent::set_constraint_value (
    double constraint_value)
{
    constraint_torque = constraint_value;
    constraint_wrench.scale_torque(constraint_value);
}


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
