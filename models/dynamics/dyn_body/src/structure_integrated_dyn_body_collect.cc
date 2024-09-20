/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynBody
 * @{
 *
 * @file
 * Define StructureIntegratedDynBody methods related to force and torque
 * accumulation and propagation.
 */

/*
Purpose: ()
Library dependencies:
  ((structure_integrated_dyn_body.cc))
*/

#include "../include/structure_integrated_dyn_body.hh"

#include "dynamics/dyn_manager/include/base_dyn_manager.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"

#include <cstddef>

//! Namespace jeod
namespace jeod
{

/**
 * Accumulate forces acting on a vehicle.
 *
 * \param[in] vec Forces
 * \param[out] cumulation Accumulated force
 **/
static inline void accumulate_forces(const JeodPointerVector<CollectForce>::type & vec, double * cumulation)
{
    Vector3::initialize(cumulation);
    for(auto elem : vec)
    {
        if(elem->is_active())
        {
            Vector3::incr(elem->force, cumulation);
        }
    }
}

/**
 * Accumulate torques acting on a vehicle.
 *
 * \param[in] vec Torques
 * \param[out] cumulation Accumulated torque
 **/
static inline void accumulate_torques(const JeodPointerVector<CollectTorque>::type & vec, double * cumulation)
{
    Vector3::initialize(cumulation);
    for(auto elem : vec)
    {
        if(elem->is_active())
        {
            Vector3::incr(elem->torque, cumulation);
        }
    }
}

// Collect forces and torques acting on the vehicle.
void StructureIntegratedDynBody::collect_forces_and_torques()
{
    // Collect forces and torques that directly act on this body.
    collect_local_forces_and_torques();

    // Collect forces and torques on attached bodies and add them to the
    // force and torque collections for this body.
    for(auto child : dyn_children)
    {
        child->collect_forces_and_torques();
    }

    // At this point forces and torques have been accumulated with the body
    // and the method will have been recursively invoked on all child bodies.
    // The remaining actions depend on whether this body is a child body
    // or a root body.

    // Child body: Propagate forces, torques to the parent body.
    if(dyn_parent != nullptr)
    {
        PropagateForcesAndTorques();
    }

    // Root body: Compute total forces and torques and resultant accelerations.
    else
    {
        Wrench effector_wrench_co_m;

        // Translational and rotational dynamics on:
        // Translate the structure-referenced wrench to the center of mass.
        if(translational_dynamics && rotational_dynamics)
        {
            effector_wrench_co_m = effector_wrench.transform_to_point(mass.composite_properties.position);
        }
        // No else; the default constructor did the right thing.

        // Rotational dynamics is on:
        if(rotational_dynamics)
        {
            // Compute the body-referenced inertial torque (omega \cross L).
            compute_inertial_torque();

            // Compute the total torque, structural referenced.
            Vector3::sum(collect.effector_torq, collect.environ_torq, collect.no_xmit_torq, collect.extern_torq_struct);
            Vector3::incr(effector_wrench_co_m.get_torque(), collect.extern_torq_struct);

            // Compute the rotational acceleration of the body.
            compute_rotational_acceleration();
        }

        // Rotational dynamics is off:
        // Zero out the collected torques and the rotational acceleration.
        else
        {
            Vector3::initialize(collect.extern_torq_struct);
            Vector3::initialize(collect.extern_torq_body);
            Vector3::initialize(collect.inertial_torq);
            Vector3::initialize(derivs.rot_accel);
            Vector3::initialize(struct_derivs.rot_accel);
        }

        // Translational dynamics is on:
        // Compute total force on the body, structural and inertial referenced,
        // and from this, compute translational accelerations.
        // Note: The gravitational acceleration must have already been computed.
        if(translational_dynamics)
        {
            // Compute the total force, structural referenced.
            Vector3::sum(collect.effector_forc, collect.environ_forc, collect.no_xmit_forc, collect.extern_forc_struct);
            Vector3::incr(effector_wrench_co_m.get_force(), collect.extern_forc_struct);

            // Compute the inertial-referenced translational acceleration
            // at the structure origin.
            compute_translational_acceleration();
        }

        // Translational dynamics is off:
        // Zero out the collected forces and the translational accelerations.
        else
        {
            Vector3::initialize(collect.extern_forc_struct);
            Vector3::initialize(collect.extern_forc_inrtl);
            Vector3::initialize(derivs.non_grav_accel);
            Vector3::initialize(derivs.trans_accel);
            Vector3::initialize(struct_derivs.non_grav_accel);
            Vector3::initialize(struct_derivs.trans_accel);
        }
    }
}

// Collect the local forces and torques acting on the vehicle.
void StructureIntegratedDynBody::collect_local_forces_and_torques()
{
    // Translational and rotational dynamics on:
    // Accumulate the external wrenches on this body about the structure origin.
    if(translational_dynamics && rotational_dynamics)
    {
        effector_wrench_collection.accumulate(effector_wrench);
    }
    else
    {
        effector_wrench.reset_force_and_torque();
    }

    // Translational dynamics on: Accumulate the external forces on this body.
    if(translational_dynamics)
    {
        accumulate_forces(collect.collect_effector_forc, collect.effector_forc);
        accumulate_forces(collect.collect_environ_forc, collect.environ_forc);
        accumulate_forces(collect.collect_no_xmit_forc, collect.no_xmit_forc);
    }

    // Translational dynamics off: Zero-out force accumulators.
    else
    {
        Vector3::initialize(collect.effector_forc);
        Vector3::initialize(collect.environ_forc);
        Vector3::initialize(collect.no_xmit_forc);
    }

    // Rotational dynamics on: Accumulate the external torques on this body.
    if(rotational_dynamics)
    {
        accumulate_torques(collect.collect_effector_torq, collect.effector_torq);
        accumulate_torques(collect.collect_environ_torq, collect.environ_torq);
        accumulate_torques(collect.collect_no_xmit_torq, collect.no_xmit_torq);
    }

    // Rotational dynamics off: Zero-out torque accumulators.
    else
    {
        Vector3::initialize(collect.effector_torq);
        Vector3::initialize(collect.environ_torq);
        Vector3::initialize(collect.no_xmit_torq);
    }
}

// Propagate forces and torques up the kinematic chain.
void StructureIntegratedDynBody::PropagateForcesAndTorques()
{
    double effector_force_parent_structure[3];
    double environ_force_parent_structure[3];

    // Translational and rotational dynamics on:
    // Transmit accumulated wrenches to the parent,
    // but in the parent's structural frame.
    if(translational_dynamics && rotational_dynamics)
    {
        auto * struct_parent = dynamic_cast<StructureIntegratedDynBody *>(dyn_parent);
        struct_parent->effector_wrench += effector_wrench.transform_to_parent(mass.structure_point);
    }

    // Translational dynamics is on:
    // Transmit accumulated forces to the parent,
    // but in the parent's structural frame.
    if(translational_dynamics)
    {
        // Transform transmittable forces to parent structural.
        // The inherited (Mass) structure_point object provides the
        // transformation from parent structural to child structural.
        // The transpose is needed to transform from child to parent.
        Vector3::transform_transpose(mass.structure_point.T_parent_this,
                                     collect.effector_forc,
                                     effector_force_parent_structure);
        Vector3::transform_transpose(mass.structure_point.T_parent_this,
                                     collect.environ_forc,
                                     environ_force_parent_structure);

        // Transmit these forces to the parent dyn body.
        Vector3::incr(effector_force_parent_structure, dyn_parent->collect.effector_forc);
        Vector3::incr(environ_force_parent_structure, dyn_parent->collect.environ_forc);
    }

    // Translational dynamics is off:
    // Zero out transmitted forces as these become torques in the parent.
    else
    {
        Vector3::initialize(effector_force_parent_structure);
        Vector3::initialize(environ_force_parent_structure);
    }

    // Rotational dynamics is on:
    // Transmit accumulated torques to the parent,
    // but in the parent's structural frame.
    if(rotational_dynamics)
    {
        double effector_torque_parent_structure[3];
        double environ_torque_parent_structure[3];
        double parent_co_m_to_child_co_m[3];
        // Reconstruct composite relative state
        RefFrameState composite_wrt_pstr;
        composite_body.compute_relative_state(dyn_parent->structure, composite_wrt_pstr);
        // Transform transmittable torques to parent structural.
        Vector3::transform_transpose(mass.structure_point.T_parent_this,
                                     collect.effector_torq,
                                     effector_torque_parent_structure);
        Vector3::transform_transpose(mass.structure_point.T_parent_this,
                                     collect.environ_torq,
                                     environ_torque_parent_structure);

        // Compute the parent cm to child cm offset in parent structural.
        Vector3::diff(composite_wrt_pstr.trans.position,
                      dyn_parent->mass.composite_properties.position,
                      parent_co_m_to_child_co_m);

        // Compute the torque contributions of the child forces.
        Vector3::cross_incr(parent_co_m_to_child_co_m,
                            effector_force_parent_structure,
                            effector_torque_parent_structure);
        Vector3::cross_incr(parent_co_m_to_child_co_m, environ_force_parent_structure, environ_torque_parent_structure);

        // Transmit the torques to the parent dyn body.
        Vector3::incr(effector_torque_parent_structure, dyn_parent->collect.effector_torq);
        Vector3::incr(environ_torque_parent_structure, dyn_parent->collect.environ_torq);
    }

    // There is nothing to do here if rotational dynamics is off.
    else
    {
    }
}

// Compute the inertial torque.
void StructureIntegratedDynBody::compute_inertial_torque()
{
    double ang_vel_body[3]; // Body referenced angular velocity
    double ang_mom_body[3]; // Body referenced angular momentum

    // Transform the structural-referenced angular velocity to body.
    // The inherited (Mass) composite_properties member provides the
    // transformation from structure to body.
    Vector3::transform(mass.composite_properties.T_parent_this, structure.state.rot.ang_vel_this, ang_vel_body);

    // Compute body-referenced angular momentum L and inertial torque
    // w cross L, where w is the body-referenced angular velocity.
    Vector3::transform(mass.composite_properties.inertia, ang_vel_body, ang_mom_body);
    Vector3::cross(ang_vel_body, ang_mom_body, collect.inertial_torq);
}

// Compute the body- and structure-referenced rotational acceleration vectors.
void StructureIntegratedDynBody::compute_rotational_acceleration()
{
    double torque_body[3]; // Body referenced total torque

    // Transform the structural-referenced torque to body.
    Vector3::transform(mass.composite_properties.T_parent_this, collect.extern_torq_struct, collect.extern_torq_body);

    // Compute the body-referenced apparent torque via
    // \dot L_body = \dot L_inertial - omega \cross L.
    Vector3::diff(collect.extern_torq_body, collect.inertial_torq, torque_body);

    // Solve the rotational EOM for body accelerations.
    Vector3::transform(mass.composite_properties.inverse_inertia, torque_body, derivs.rot_accel);

    // Truncate small rotational accels to avoid numerical problems.
    Vector3::zero_small(1e-20, derivs.rot_accel);

    // We're integrating the structural frame; transform to structural.
    Vector3::transform_transpose(mass.composite_properties.T_parent_this, derivs.rot_accel, struct_derivs.rot_accel);
}

// Compute the inertial-referenced translational acceleration vector.
void StructureIntegratedDynBody::compute_translational_acceleration()
{
    // Transform the total external force to inertial.
    // The structure member provides the transformation from inertial to
    // structural. The transpose is needed to transform to inertial.
    Vector3::transform_transpose(structure.state.rot.T_parent_this,
                                 collect.extern_forc_struct,
                                 collect.extern_forc_inrtl);

    // Compute accelerations at the composite center of mass.
    Vector3::scale(collect.extern_forc_inrtl, mass.composite_properties.inverse_mass, derivs.non_grav_accel);

    // Compute fictitious acceleration due to rotational velocity.
    double omega_cross_r[3];

    Vector3::cross(mass.composite_properties.position, structure.state.rot.ang_vel_this, omega_cross_r);
    Vector3::cross(structure.state.rot.ang_vel_this, omega_cross_r, inertial_accel_struct_omega);

    complete_translational_acceleration();
}

// Compute the inertial-referenced translational acceleration vector.
void StructureIntegratedDynBody::complete_translational_acceleration()
{
    // Compute translational acceleration at the composite center of mass.
    Vector3::sum(derivs.non_grav_accel, grav_interaction.grav_accel, derivs.trans_accel);

    // Compute acceleration due to rotation.
    // Note that this ignores the Coriolis effect due to movement of
    // the center of mass inside the vehicle.
    Vector3::cross(mass.composite_properties.position, struct_derivs.rot_accel, inertial_accel_struct_omega_dot);
    Vector3::sum(inertial_accel_struct_omega, inertial_accel_struct_omega_dot, inertial_accel_struct);
    Vector3::transform_transpose(structure.state.rot.T_parent_this, inertial_accel_struct, inertial_accel_inrtl);

    // Compute accelerations at the structural origin.
    Vector3::sum(derivs.non_grav_accel, inertial_accel_inrtl, struct_derivs.non_grav_accel);
    Vector3::sum(struct_derivs.non_grav_accel, grav_interaction.grav_accel, struct_derivs.trans_accel);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
