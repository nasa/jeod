/**
 * @addtogroup Models
 * @{
 * @addtogroup Experimental
 * @{
 * @addtogroup Constraints
 * @{
 *
 * @file
 * Defines ConstrainedPointMass member functions.
 */

/*
Purpose: ()
*/

#include "../include/constrained_point_mass.hh"
#include "../include/constraint_frame.hh"

#include "dynamics/dyn_body/include/vehicle_non_grav_state.hh"
#include "dynamics/dyn_body/include/vehicle_properties.hh"

#include "utils/math/include/vector3.hh"

//! Namespace jeod
namespace jeod
{

ConstrainedPointMass::ConstrainedPointMass(const ConstraintFrame * constraint_frame_in)
    : constraint_frame(constraint_frame_in)
{
}

void ConstrainedPointMass::set_constraint_position(const double constraint_position[3])
{
    Vector3::copy(constraint_position, constraint_position_constraint_frame);
    compute_constraint_position_root();
}

void ConstrainedPointMass::compute_constraint_position_root()
{
    Vector3::incr(constraint_frame->origin_position_root,
                  Vector3::transform_transpose(constraint_frame->T_root_constraint,
                                               constraint_position_constraint_frame,
                                               constraint_position_root));
}

void ConstrainedPointMass::compute_constraint_position_com(const VehicleProperties & vehicle_properties)
{
    Vector3::diff(constraint_position_root, vehicle_properties.get_structure_to_body_offset(), constraint_position_com);
}

void ConstrainedPointMass::compute_non_grav_accel(const VehicleNonGravState & non_grav_state)
{
    // Compute the external acceleration at the constraint position
    // due to external forces and torques.
    double wxr[3];
    Vector3::cross(non_grav_state.omega_struct, constraint_position_com, wxr);
    Vector3::cross(non_grav_state.omega_struct, wxr, non_grav_accel_root);
    Vector3::cross_incr(non_grav_state.omega_dot_struct, constraint_position_com, non_grav_accel_root);
    Vector3::incr(non_grav_state.accel_struct, non_grav_accel_root);

    // Transform the above to the constraint frame.
    Vector3::transform(constraint_frame->T_root_constraint, non_grav_accel_root, non_grav_accel_constraint_frame);

    // Compute effective force.
    Vector3::scale(non_grav_accel_constraint_frame, mass, force_constraint_frame);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
