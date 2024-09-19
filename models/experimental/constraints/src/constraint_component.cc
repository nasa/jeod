/**
 * @addtogroup Models
 * @{
 * @addtogroup Experimental
 * @{
 * @addtogroup Constraints
 * @{
 *
 * @file
 * Defines ConstraintComponent member functions.
 */

/*
Purpose: ()
*/

#include "../include/constraint_component.hh"

#include "../include/constraint_frame.hh"
#include "dynamics/dyn_body/include/vehicle_properties.hh"

#include "experimental/message/include/fail_simulation.hh"

#include "utils/math/include/vector3.hh"

//! Namespace jeod
namespace jeod
{

ConstraintComponent::ConstraintComponent(ConstraintFrame * constraint_frame_in)
    : constraint_frame(constraint_frame_in),
      constraint_wrench()
{
}

SolverTypes::ConstDecayedVector3T ConstraintComponent::get_rotational_response(const VehicleProperties &) const
{
    return constraint_omega_dot_root;
}

SolverTypes::DecayedVector3T ConstraintComponent::get_rotational_response(const VehicleProperties & vehicle_properties,
                                                                          SolverTypes::Vector3T response) const
{
    return Vector3::copy(get_rotational_response(vehicle_properties), response);
}

// Save the input direction and transform to the root structural frame.
void ConstraintComponent::set_constraint_direction(SolverTypes::ConstVector3T constraint_direction)
{
    Vector3::copy(constraint_direction, constraint_direction_constraint_frame);
    compute_root_direction();
}

// Transform the constraint direction to the root structural frame.
void ConstraintComponent::compute_root_direction()
{
    Vector3::transform_transpose(constraint_frame->T_root_constraint,
                                 constraint_direction_constraint_frame,
                                 constraint_direction_root);
}

// Compute the vehicle's rotation acceleration in response to a torque:
// \dot{\omega} = T_{struct->body}^T (I^{-1} (T_{struct->body} torque))
void ConstraintComponent::compute_rotational_response(const VehicleProperties & vehicle_properties,
                                                      const double torque[3])
{
    double omega_dot_body[3];

    // omega_dot_body = I^{-1} * (T_{struct->body} * torque);
    Vector3::transform(vehicle_properties.get_inverse_inertia(),
                       Vector3::transform(vehicle_properties.get_structure_to_body_transform(),
                                          torque,
                                          omega_dot_body));

    // Transform the angular accel due to the torque to structure.
    Vector3::transform_transpose(vehicle_properties.get_structure_to_body_transform(),
                                 omega_dot_body,
                                 constraint_omega_dot_root);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
