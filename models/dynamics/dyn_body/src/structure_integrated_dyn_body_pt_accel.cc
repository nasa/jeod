/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynBody
 * @{
 *
 * @file
 * Define StructureIntegratedDynBody::compute_vehicle_point_derivatives.
 */

/*
Purpose: ()
*/


#include "../include/structure_integrated_dyn_body.hh"

#include "dynamics/dyn_body/include/dyn_body_messages.hh"
#include "utils/math/include/vector3.hh"
#include "utils/message/include/message_handler.hh"

#include <cstring>
#include <cstdio>


//! Namespace jeod 
namespace jeod {

 // Compute the state derivatives at a vehicle point.
void
StructureIntegratedDynBody::compute_vehicle_point_derivatives (
    const BodyRefFrame & vehicle_pt,
    FrameDerivs & pt_derivs)
{
    // The root body for this body.
    // Only the root body's derivs element contains valid data.
    const StructureIntegratedDynBody * root_body =
        dynamic_cast<const StructureIntegratedDynBody*>(get_root_body());

    // The root body's structure.
    const MassPoint & root_ppty = root_body->mass.structure_point;

    // The root body's structural rotational state.
    const RefFrameRot & root_rot = root_body->structure.state.rot;

    // The derivatives for this root body.
    const FrameDerivs & root_derivs = root_body->struct_derivs;

    // Other local variables
    MassPointState str_to_point;  // --   Vehicle pt state wrt structure
    double str_to_point_inrtl[3];  // M    Point wrt CoM, inertial coordinates
    double wxr[3];              // M/s  Omega cross r
    double wxwxr[3];            // M/s2 Centrifugal accel
    double axr[3];              // M/s2 Acceleration due to angular accel
    double accel_rot_body[3];     // M/s2 Acceleration due to rotation
    double accel_rot_inrtl[3];    // M/s2 Acceleration due to rotation
    double delta_acc_grav[3];     // M/s2 Acceleration due to gravity gradient
    double delta_acc_non_grav[3];  // M/s2 Delta acceleration, non-gravity


    // Sanity check: Never update the root body's derivs structure. Ever.
    if (&pt_derivs == &root_derivs)
    {
        // Complain vehemently if the attempt is utterly invalid.
        // (Requesting the root body to update its composite derivatives
        // is silly, but no harm, no foul, so no complaint.)
        if (&vehicle_pt != &root_body->composite_body)
        {
            MessageHandler::error (
                 __FILE__, __LINE__, DynBodyMessages::invalid_frame,
                 "Illegal attempt to update the root body '%s' FrameDerivs\n",
                 root_body->name.c_str());
       }
       return;
    }


    // Compute the vehicle point state wrt the composite center of mass.
    vehicle_pt.mass_point->compute_relative_state (root_ppty, str_to_point);

    // Transform position to inertial.
    Vector3::transform_transpose (root_rot.T_parent_this,
                                  str_to_point.position,
                                  str_to_point_inrtl);

    // Compute accelerations caused by vehicle rotation.
    Vector3::cross (root_rot.ang_vel_this, str_to_point.position, wxr);
    Vector3::cross (root_rot.ang_vel_this, wxr, wxwxr);
    Vector3::cross (root_derivs.rot_accel, str_to_point.position, axr);
    Vector3::sum (wxwxr, axr, accel_rot_body);
    Vector3::transform_transpose (root_rot.T_parent_this,
                                  accel_rot_body,
                                  accel_rot_inrtl);

    // Compute delta gravitational acceleration.
    Vector3::transform (grav_interaction.grav_grad, str_to_point_inrtl,
                        delta_acc_grav);

    // Compute accelerations.
    Vector3::diff (accel_rot_inrtl, delta_acc_grav, delta_acc_non_grav);
    Vector3::sum (root_derivs.non_grav_accel, delta_acc_non_grav,
                  pt_derivs.non_grav_accel);
    Vector3::sum (root_derivs.trans_accel, accel_rot_inrtl,
                  pt_derivs.trans_accel);
    Vector3::transform (str_to_point.T_parent_this, root_derivs.rot_accel,
                        pt_derivs.rot_accel);
    vehicle_pt.state.rot.Q_parent_this.compute_left_quat_deriv (
        vehicle_pt.state.rot.ang_vel_this, pt_derivs.Qdot_parent_this);
}


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
