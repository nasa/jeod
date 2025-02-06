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

// Model includes
#include "../include/body_force_collect.hh"
#include "../include/frame_derivs.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Default constructor.
 */
BodyForceCollect::BodyForceCollect()
    : collect_effector_forc(),
      collect_environ_forc(),
      collect_no_xmit_forc(),
      collect_effector_torq(),
      collect_environ_torq(),
      collect_no_xmit_torq()
{
    // Register the checkpointable items.
    JEOD_REGISTER_CLASS(BodyForceCollect);
    JEOD_REGISTER_CLASS(CollectForce);
    JEOD_REGISTER_CLASS(CollectTorque);
    JEOD_REGISTER_CHECKPOINTABLE(this, collect_effector_forc);
    JEOD_REGISTER_CHECKPOINTABLE(this, collect_environ_forc);
    JEOD_REGISTER_CHECKPOINTABLE(this, collect_no_xmit_forc);
    JEOD_REGISTER_CHECKPOINTABLE(this, collect_effector_torq);
    JEOD_REGISTER_CHECKPOINTABLE(this, collect_environ_torq);
    JEOD_REGISTER_CHECKPOINTABLE(this, collect_no_xmit_torq);
}

/**
 * Destructor.
 */
BodyForceCollect::~BodyForceCollect()
{
    // Remove registrations of containers.
    JEOD_DEREGISTER_CHECKPOINTABLE(this, collect_effector_forc);
    JEOD_DEREGISTER_CHECKPOINTABLE(this, collect_environ_forc);
    JEOD_DEREGISTER_CHECKPOINTABLE(this, collect_no_xmit_forc);
    JEOD_DEREGISTER_CHECKPOINTABLE(this, collect_effector_torq);
    JEOD_DEREGISTER_CHECKPOINTABLE(this, collect_environ_torq);
    JEOD_DEREGISTER_CHECKPOINTABLE(this, collect_no_xmit_torq);

    // Release JEOD-allocated elements in the force and torque collections.
    release_vector(collect_effector_forc);
    release_vector(collect_environ_forc);
    release_vector(collect_no_xmit_forc);
    release_vector(collect_effector_torq);
    release_vector(collect_environ_torq);
    release_vector(collect_no_xmit_torq);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
