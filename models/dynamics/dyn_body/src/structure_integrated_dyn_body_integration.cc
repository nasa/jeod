/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynBody
 * @{
 *
 * @file
 * Define StructureIntegratedDynBody member functions related to
 * state integration.
 */

/*
Purpose: ()
*/


#include "../include/structure_integrated_dyn_body.hh"

#include "dynamics/dyn_body/include/dyn_body_messages.hh"
#include "utils/math/include/vector3.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/ref_frames/include/ref_frame_items.hh"

#include "er7_utils/integration/core/include/second_order_ode_integrator.hh"

#include <cstddef>
#include <cmath>


//! Namespace jeod 
namespace jeod {

// Integrate the translational state of a StructureIntegratedDynBody.
er7_utils::IntegratorResult
StructureIntegratedDynBody::trans_integ (
   double dyn_dt,
   unsigned int target_stage)
{
    RefFrameTrans & trans_state = structure.state.trans;

    // Integrate the translational state.
    return trans_integrator.integrate (
               dyn_dt, target_stage,
               struct_derivs.trans_accel,
               trans_state.velocity, trans_state.position);
}


// Integrate the rotational state of a StructureIntegratedDynBody.
er7_utils::IntegratorResult
StructureIntegratedDynBody::rot_integ (
    double dyn_dt,
    unsigned int target_stage)
{
    RefFrameRot & rot_state = structure.state.rot;

    // Integrate the rotational state.
    er7_utils::IntegratorResult status =
        rot_integrator.integrate (
            dyn_dt, target_stage,
            struct_derivs.rot_accel,
            rot_state.ang_vel_this, rot_state.Q_parent_this);

    // Normalize the integrated quaternion.
    rot_state.Q_parent_this.normalize_integ ();

    // Compute the corresponding transformation matrix.
    rot_state.Q_parent_this.left_quat_to_transformation (
       rot_state.T_parent_this);

    // Compute the quaternion derivative.
    rot_state.Q_parent_this.compute_left_quat_deriv (
       rot_state.ang_vel_this, derivs.Qdot_parent_this);

    // Compute the angular velocity magnitude and unit vector.
    rot_state.ang_vel_mag = Vector3::vmag (rot_state.ang_vel_this);
    if (std::fpclassify(rot_state.ang_vel_mag) != FP_ZERO)
    {
        Vector3::scale (rot_state.ang_vel_this,
                        1.0 / rot_state.ang_vel_mag,
                        rot_state.ang_vel_unit);
    }
    else
    {
       Vector3::initialize (rot_state.ang_vel_unit);
    }

    return status;
}


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
