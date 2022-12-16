/**
 * @addtogroup Models
 * @{
 * @addtogroup Experimental
 * @{
 * @addtogroup Constraints
 * @{
 *
 * @file
 * Defines ForceConstraintComponent member functions.
 */

/*
Purpose: ()
*/


#include "../include/force_constraint_component.hh"

#include "../include/constrained_point_mass.hh"
#include "../include/constraint_frame.hh"

#include "dynamics/dyn_body/include/vehicle_properties.hh"
#include "experimental/math/include/matrix_view.hh"
#include "utils/math/include/vector3.hh"


//! Namespace jeod 
namespace jeod {

ForceConstraintComponent::ForceConstraintComponent (
    ConstraintFrame* constraint_frame_in,
    ConstrainedPointMass* constrained_mass_in)
:
    ConstraintComponent(constraint_frame_in),
    constrained_mass(constrained_mass_in),
    constraint_force(0.0)
{
    Vector3::initialize (constraint_com_accel_root);
}


// Dependencies:
// The constraint direction and position wrt the root structure frame
// and position wrt the root CoM must be correct.
void
ForceConstraintComponent::setup_constraint (
    const VehicleProperties& vehicle_properties,
    const VehicleNonGravState&)
{
    // Set the constraint to be a pure unit force at the constraint position
    // and in the constraint direction.
    constraint_wrench.set_force (
        constraint_direction_root,
        constrained_mass->constraint_position_root);

    // Compute the vehicle's translational acceleration in response to this
    // constraint at the vehicle's center of mass.
    Vector3::scale (
        constraint_direction_root, vehicle_properties.get_inverse_mass(),
        constraint_com_accel_root);

    // Compute the vehicle's rotational acceleration in response to this
    // constraint.
    double torque[3];
    Vector3::cross (
        constrained_mass->constraint_position_com,
        constraint_direction_root,
        torque);
    compute_rotational_response (vehicle_properties, torque);
}


// Set the constraint matrix elements that pertain to this constrained
// object with respect to some other constrained object.
double
ForceConstraintComponent::get_cross_coeff (
    const VehicleProperties& vehicle_properties,
    const ConstraintComponent& other) const
{
    double accel[3];

    other.get_translational_response(
        vehicle_properties,
        constrained_mass->constraint_position_com,
        accel);

    return constrained_mass->mass * Vector3::dot(constraint_direction_root, accel);
}


SolverTypes::DecayedVector3T
ForceConstraintComponent::get_translational_response (
    const VehicleProperties&,
    SolverTypes::ConstVector3T point,
    SolverTypes::Vector3T response) const
{
    Vector3::incr (
        constraint_com_accel_root,
        Vector3::cross (constraint_omega_dot_root, point, response));

    return response;
}


void
ForceConstraintComponent::set_constraint_value (
    double constraint_value)
{
    constraint_force = constraint_value;
    constraint_wrench.scale_force(constraint_value);
}


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
