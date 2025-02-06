/**
 * @addtogroup Models
 * @{
 * @addtogroup Experimental
 * @{
 * @addtogroup Constraints
 * @{
 *
 * @file
 * Defines ConstrainedRigidMass member functions.
 */

/*
Purpose: ()
*/


#include "../include/constrained_rigid_mass.hh"
#include "../include/constraint_frame.hh"

#include "dynamics/dyn_body/include/vehicle_non_grav_state.hh"
#include "dynamics/dyn_body/include/vehicle_properties.hh"

#include "utils/math/include/vector3.hh"


//! Namespace jeod 
namespace jeod {

ConstrainedRigidMass::ConstrainedRigidMass (
    const ConstraintFrame* constraint_frame_in)
:
    ConstrainedPointMass(constraint_frame_in)
{
    Matrix3x3::initialize (inertia);
}


void
ConstrainedRigidMass::setup_constraint (
    const VehicleProperties& vehicle_properties,
    const VehicleNonGravState& non_grav_state)
{
    ConstrainedPointMass::setup_constraint (vehicle_properties, non_grav_state);

    double omega_constraint_frame[3];
    double omega_dot_constraint_frame[3];
    double ang_mom_constraint_frame[3];

    Vector3::transform (
        constraint_frame->T_root_constraint,
        non_grav_state.omega_struct,
        omega_constraint_frame);
    Vector3::transform (
        constraint_frame->T_root_constraint,
        non_grav_state.omega_dot_struct,
        omega_dot_constraint_frame);
    Vector3::transform (
        inertia,
        omega_constraint_frame,
        ang_mom_constraint_frame);
    Vector3::transform (
        inertia,
        omega_dot_constraint_frame,
        torque_constraint_frame);
    Vector3::cross_incr (
        omega_constraint_frame,
        ang_mom_constraint_frame,
        torque_constraint_frame);
}


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
