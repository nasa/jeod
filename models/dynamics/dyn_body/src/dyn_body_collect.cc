/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynBody
 * @{
 *
 * @file models/dynamics/dyn_body/src/dyn_body_collect.cc
 * Define DynBody methods related to force and torque accumulation and
 * propagation.
 */

/*******************************************************************************

Purpose:
  ()

Assumptions and limitations:
  ((Forces and torques are represented in the structural frame of the body under
    which they are collected, but act through/about the center of mass of that
    body.)
   (Gravitational accelerations have already been computed and accumulated
    prior to calling this method.))

Library dependencies:
  ((dyn_body_collect.o)
   (dyn_body.o)
   (dynamics/mass/mass_point_state.o)
   (utils/ref_frames/ref_frame.o))



*******************************************************************************/


// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_manager/include/base_dyn_manager.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"

// Model includes
#include "../include/dyn_body.hh"


//! Namespace jeod
namespace jeod {

/**
 * Accumulate forces acting on a vehicle.
 *
 * \param[in] vec Forces
 * \param[out] cumulation Accumulated force
 **/
static inline void
accumulate_forces (
   const JeodPointerVector<CollectForce>::type & vec,
   double * cumulation)
{
   unsigned int nitems = vec.size();

   Vector3::initialize (cumulation);
   for (unsigned int ii = 0; ii < nitems; ii++) {
      if (vec[ii]->is_active()) {
         Vector3::incr (vec[ii]->force, cumulation);
      }
   }
}


/**
 * Accumulate torques acting on a vehicle.
 *
 * \param[in] vec Torques
 * \param[out] cumulation Accumulated torque
 **/
static inline void
accumulate_torques (
   const JeodPointerVector<CollectTorque>::type & vec,
   double * cumulation)
{
   unsigned int nitems = vec.size();

   Vector3::initialize (cumulation);
   for (unsigned int ii = 0; ii < nitems; ii++) {
      if (vec[ii]->is_active()) {
         Vector3::incr (vec[ii]->torque, cumulation);
      }
   }
}


// Collect forces and torques acting on the vehicle.
void
DynBody::collect_forces_and_torques ()
{

   // Translational dynamics on: Accumulate the external forces on this DynBody.
   if (translational_dynamics) {

      accumulate_forces (collect.collect_effector_forc, collect.effector_forc);
      accumulate_forces (collect.collect_environ_forc, collect.environ_forc);
      accumulate_forces (collect.collect_no_xmit_forc, collect.no_xmit_forc);
   }

   // Translational dynamics off: Zero-out force accumulators.
   else {

      Vector3::initialize (collect.effector_forc);
      Vector3::initialize (collect.environ_forc);
      Vector3::initialize (collect.no_xmit_forc);
   }


   // Rotational dynamics on: Accumulate the external torques on this DynBody.
   if (rotational_dynamics) {

      accumulate_torques (collect.collect_effector_torq, collect.effector_torq);
      accumulate_torques (collect.collect_environ_torq, collect.environ_torq);
      accumulate_torques (collect.collect_no_xmit_torq, collect.no_xmit_torq);
   }

   // Rotational dynamics off: Zero-out torque accumulators.
   else {

      Vector3::initialize (collect.effector_torq);
      Vector3::initialize (collect.environ_torq);
      Vector3::initialize (collect.no_xmit_torq);
   }


   // Collect forces and torques on attached bodies and add them to the
   // force and torque collections for this body.
   for (std::list<DynBody *>::iterator it = dyn_children.begin();
        it != dyn_children.end();
        ++it) {
      DynBody * child = *it;
      child->collect_forces_and_torques ();
   }


   // At this point forces and torques have been accumulated with the body
   // and the method will have been recursively invoked on all child bodies.
   // The remaining actions depend on whether this body is a child body
   // or a root body.

   // Child body: Propagate forces, torques to the parent body.
   if (dyn_parent != nullptr) {
      double effector_forc_pstr[3];
      double environ_forc_pstr[3];
      double effector_torq_pstr[3];
      double environ_torq_pstr[3];
      double pcm_to_ccm[3];

      // Translational dynamics is on:
      // Transmit forces to the parent (but in the parent's structural frame).
      if (translational_dynamics) {

         // Transform transmittable forces to parent structural.
         // The inherited (Mass) structure_point object provides the
         // transformation from parent structural to child structural.
         // The transpose is needed to transform from child to parent.
         Vector3::transform_transpose (
            mass.structure_point.T_parent_this, collect.effector_forc,
            effector_forc_pstr);
         Vector3::transform_transpose (
            mass.structure_point.T_parent_this, collect.environ_forc,
            environ_forc_pstr);

         // Transmit these forces to the parent dyn body.
         Vector3::incr (effector_forc_pstr,
                        dyn_parent->collect.effector_forc);
         Vector3::incr (environ_forc_pstr,
                        dyn_parent->collect.environ_forc);
      }

      // Translational dynamics is off:
      // Zero out the transmitted forces as these become torques in the parent.
      else {
         Vector3::initialize (effector_forc_pstr);
         Vector3::initialize (environ_forc_pstr);
      }

      // Rotational dynamics is on:
      // Transmit torques to the parent (but in the parent's structural frame).
      if (rotational_dynamics) {

         // Transform transmittable torques to parent structural.
         Vector3::transform_transpose (
            mass.structure_point.T_parent_this, collect.effector_torq,
            effector_torq_pstr);
         Vector3::transform_transpose (
            mass.structure_point.T_parent_this, collect.environ_torq,
            environ_torq_pstr);

         // Compute the parent cm to child cm offset in parent structural.
         Vector3::diff (mass.composite_wrt_pstr.position,
                        dyn_parent->mass.composite_properties.position,
                        pcm_to_ccm);

         // Compute the torque contributions of the child forces.
         Vector3::cross_incr (pcm_to_ccm, effector_forc_pstr,
                              effector_torq_pstr);
         Vector3::cross_incr (pcm_to_ccm, environ_forc_pstr,
                              environ_torq_pstr);

         // Transmit the torques to the parent dyn body.
         Vector3::incr (effector_torq_pstr,
                        dyn_parent->collect.effector_torq);
         Vector3::incr (environ_torq_pstr,
                        dyn_parent->collect.environ_torq);
      }

      // There is nothing to do here if rotational dynamics is off.
      else {
      }
   }

   // Root body: Compute total forces and torques and resultant accelerations.
   else {

      // Translational dynamics is on:
      // Compute total force on the body, structural and inertial referenced,
      // and from this, compute translational accelerations.
      // Note: The gravitational acceleration must have already been computed.
      if (translational_dynamics) {

         // Compute the total force, structural referenced.
         Vector3::sum (collect.effector_forc,
                       collect.environ_forc,
                       collect.no_xmit_forc,
                       collect.extern_forc_struct);

         // Transform the total external force to inertial.
         // The structure member provides the transformation from inertial to
         // structural. The transpose is needed to transform to inertial.
         Vector3::transform_transpose (structure.state.rot.T_parent_this,
                                       collect.extern_forc_struct,
                                       collect.extern_forc_inrtl);

         // Compute the translational acceleration.
         Vector3::scale (collect.extern_forc_inrtl, mass.composite_properties.inverse_mass,
                         derivs.non_grav_accel);
         Vector3::sum (derivs.non_grav_accel,
                       grav_interaction.grav_accel,
                       derivs.trans_accel);
      }

      // Translational dynamics is off:
      // Zero out the collected forces and the translational accelerations.
      else {

         Vector3::initialize (collect.extern_forc_struct);
         Vector3::initialize (collect.extern_forc_inrtl);
         Vector3::initialize (derivs.non_grav_accel);
         Vector3::initialize (derivs.trans_accel);
      }

      // Rotational dynamics is on:
      if (rotational_dynamics) {
         double ang_mom[3];     // kg*M^2/s    Angular momentum, body referenced
         double torque_body[3]; // kg*M^2/s^2  Total torque, body referenced

         // Compute the total torque, structural referenced.
         Vector3::sum (collect.effector_torq,
                       collect.environ_torq,
                       collect.no_xmit_torq,
                       collect.extern_torq_struct);

         // Transform the structural reference torque to body.
         // The inherited (Mass) composite_properties member provides the
         // transformation from structure to body.
         Vector3::transform (mass.composite_properties.T_parent_this,
                             collect.extern_torq_struct,
                             collect.extern_torq_body);

         // Compute the inertial torque, w x L, where w is the body-referenced
         // angular velocity and L is the body-referenced angular momentum.
         Vector3::transform (mass.composite_properties.inertia,
                             composite_body.state.rot.ang_vel_this,
                             ang_mom);
         Vector3::cross (composite_body.state.rot.ang_vel_this, ang_mom,
                         collect.inertial_torq);

         // Subtract from the external torque to form the body-frame
         // apparent torque.
         Vector3::diff (collect.extern_torq_body, collect.inertial_torq,
                        torque_body);

         // Solve the rotational EOM for body accelerations.
         Vector3::transform (mass.composite_properties.inverse_inertia, torque_body, derivs.rot_accel);

         // Truncate small rotational accelerations to avoid numerical problems.
         Vector3::zero_small (1e-20, derivs.rot_accel);
      }

      // Rotational dynamics is off:
      // Zero out the collected torques and the rotational acceleration.
      else {

         Vector3::initialize (collect.extern_torq_struct);
         Vector3::initialize (collect.extern_torq_body);
         Vector3::initialize (collect.inertial_torq);
         Vector3::initialize (derivs.rot_accel);
      }

   }

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
