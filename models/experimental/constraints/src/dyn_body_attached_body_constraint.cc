/**
 * @addtogroup Models
 * @{
 * @addtogroup Experimental
 * @{
 * @addtogroup Constraints
 * @{
 *
 * @file
 * Defines DynBodyAttachedBodyConstraint member functions.
 */

/*
Purpose: ()
*/

#include "../include/dyn_body_attached_body_constraint.hh"

#include "dynamics/dyn_body/include/vehicle_non_grav_state.hh"
#include "dynamics/dyn_body/include/vehicle_properties.hh"

#include "utils/math/include/vector3.hh"

//! Namespace jeod
namespace jeod
{

DynBodyAttachedBodyConstraint::DynBodyAttachedBodyConstraint()
    : DynBodyConstraint(),
      constrained_mass(&constraint_frame),
      force_constraints{ForceConstraintComponent(&constraint_frame, &constrained_mass),
                        ForceConstraintComponent(&constraint_frame, &constrained_mass),
                        ForceConstraintComponent(&constraint_frame, &constrained_mass)},
      torque_constraints{TorqueConstraintComponent(&constraint_frame, &constrained_mass),
                         TorqueConstraintComponent(&constraint_frame, &constrained_mass),
                         TorqueConstraintComponent(&constraint_frame, &constrained_mass)},
      constraint_wrench()
{
    n_dimensions = 6;
    double unit_vector[3];
    for(unsigned ii = 0; ii < 3; ++ii)
    {
        Vector3::unit(ii, unit_vector);
        force_constraints[ii].set_constraint_direction(unit_vector);
        torque_constraints[ii].set_constraint_direction(unit_vector);
    }
}

void DynBodyAttachedBodyConstraint::setup_constraint(const VehicleProperties & vehicle_properties,
                                                     const VehicleNonGravState & non_grav_state)
{
    constrained_mass.setup_constraint(vehicle_properties, non_grav_state);
    setup_component_constraint(vehicle_properties, non_grav_state);

    for(unsigned ii = 0; ii < 3; ++ii)
    {
        force_constraints[ii].set_right_hand_side(-constrained_mass.force_constraint_frame[ii]);
        torque_constraints[ii].set_right_hand_side(-constrained_mass.torque_constraint_frame[ii]);
    }
}

void DynBodyAttachedBodyConstraint::set_constraint_values(const VectorView<double, double> & solution_slice)
{
    DynBodyConstraint::set_constraint_values(solution_slice);

    constraint_wrench.reset_force_and_torque();
    constraint_wrench.set_point(constrained_mass.constraint_position_root);
    for(unsigned ii = 0; ii < 3; ++ii)
    {
        constraint_wrench += force_constraints[ii].get_constraint_wrench();
        constraint_wrench += torque_constraints[ii].get_constraint_wrench();
    }
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
