/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynBody
 * @{
 *
 * @file models/dynamics/dyn_body/src/aux_classes.cc
 * Define base methods for various small JEOD DynBody classes.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((aux_classes.cc)
   (utils/quaternion/src/quat.cc))



*******************************************************************************/


// System includes

// JEOD includes
#include "utils/math/include/vector3.hh"

// Model includes
#include "../include/body_force_collect.hh"
#include "../include/frame_derivs.hh"


//! Namespace jeod
namespace jeod {

/**
 * Default constructor.
 */
BodyForceCollect::BodyForceCollect (
   void)
:
   collect_effector_forc (),
   collect_environ_forc (),
   collect_no_xmit_forc (),
   collect_effector_torq (),
   collect_environ_torq (),
   collect_no_xmit_torq ()
{

   // Register the checkpointable items.
   JEOD_REGISTER_CLASS (BodyForceCollect);
   JEOD_REGISTER_CLASS (CollectForce);
   JEOD_REGISTER_CLASS (CollectTorque);
   JEOD_REGISTER_CHECKPOINTABLE (this, collect_effector_forc);
   JEOD_REGISTER_CHECKPOINTABLE (this, collect_environ_forc);
   JEOD_REGISTER_CHECKPOINTABLE (this, collect_no_xmit_forc);
   JEOD_REGISTER_CHECKPOINTABLE (this, collect_effector_torq);
   JEOD_REGISTER_CHECKPOINTABLE (this, collect_environ_torq);
   JEOD_REGISTER_CHECKPOINTABLE (this, collect_no_xmit_torq);

   // Initialize the vectors to zero vectors
   Vector3::initialize (effector_forc);
   Vector3::initialize (environ_forc);
   Vector3::initialize (no_xmit_forc);
   Vector3::initialize (extern_forc_struct);
   Vector3::initialize (extern_forc_inrtl);
   Vector3::initialize (effector_torq);
   Vector3::initialize (environ_torq);
   Vector3::initialize (no_xmit_torq);
   Vector3::initialize (inertial_torq);
   Vector3::initialize (extern_torq_struct);
   Vector3::initialize (extern_torq_body);
}


/**
 * Destructor.
 */
BodyForceCollect::~BodyForceCollect (
   void)
{
   // Remove registrations of containers.
   JEOD_DEREGISTER_CHECKPOINTABLE (this, collect_effector_forc);
   JEOD_DEREGISTER_CHECKPOINTABLE (this, collect_environ_forc);
   JEOD_DEREGISTER_CHECKPOINTABLE (this, collect_no_xmit_forc);
   JEOD_DEREGISTER_CHECKPOINTABLE (this, collect_effector_torq);
   JEOD_DEREGISTER_CHECKPOINTABLE (this, collect_environ_torq);
   JEOD_DEREGISTER_CHECKPOINTABLE (this, collect_no_xmit_torq);

   // Release JEOD-allocated elements in the force and torque collections.
   release_vector (collect_effector_forc);
   release_vector (collect_environ_forc);
   release_vector (collect_no_xmit_forc);
   release_vector (collect_effector_torq);
   release_vector (collect_environ_torq);
   release_vector (collect_no_xmit_torq);
}


/**
 * Default constructor.
 */
FrameDerivs::FrameDerivs (
   void)
:
   Qdot_parent_this (0.0)
{
   Vector3::initialize (non_grav_accel);
   Vector3::initialize (trans_accel);
   Vector3::initialize (rot_accel);
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
