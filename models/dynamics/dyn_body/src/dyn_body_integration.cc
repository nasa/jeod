/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynBody
 * @{
 *
 * @file models/dynamics/dyn_body/src/dyn_body_integration.cc
 * Define methods for frame switching.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((dyn_body_integration.cc)
   (dyn_body.cc)
   (dyn_body_messages.cc)
   (environment/gravity/src/gravity_interaction.cc)
   (utils/ref_frames/src/ref_frame.cc)
   (utils/integration/src/jeod_integration_time.cc)
   (utils/integration/src/generalized_second_order_ode_technique.cc))



*******************************************************************************/


// System includes
#include <cstddef>

// ER7 utilities includes
#include "er7_utils/integration/core/include/second_order_ode_integrator.hh"

// JEOD includes
#include "dynamics/dyn_manager/include/base_dyn_manager.hh"
#include "dynamics/dyn_manager/include/dynamics_integration_group.hh"
#include "environment/ephemerides/ephem_interface/include/ephem_ref_frame.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/named_item/include/named_item.hh"
#include "utils/integration/include/jeod_integration_time.hh"
#include "utils/integration/include/generalized_second_order_ode_technique.hh"

// Model includes
#include "../include/dyn_body.hh"
#include "../include/dyn_body_messages.hh"


//! Namespace jeod
namespace jeod {

// Set the integration frame for this body and all its child bodies.
void
DynBody::set_integ_frame (                // Return: -- Void
   EphemerisRefFrame & new_integ_frame)   // In:     -- New integration frame
{

   // Body does not yet have an integration frame:
   // Link each of the three primary frames (core, composite, structure) and
   // each of the vehicle point frames to the provided integration frame.
   if (integ_frame == nullptr) {
      new_integ_frame.add_child (core_body);
      new_integ_frame.add_child (composite_body);
      new_integ_frame.add_child (structure);
      for (std::list<BodyRefFrame *>::iterator it = vehicle_points.begin();
           it != vehicle_points.end();
           ++it) {
         BodyRefFrame * point = *it;
         new_integ_frame.add_child (*point);
      }
   }

   // Body has an existing integration frame:
   // Free resources (unsubscribe), re-link the primary frames and the
   // registered vehicle points to the new integration frame, and
   // reset the integrators.
   // NOTE WELL: This uses the low-level reset_parent(). It does not
   // update state.
   else {
      dyn_manager->unsubscribe_to_frame (*integ_frame);

      core_body.reset_parent      (new_integ_frame);
      composite_body.reset_parent (new_integ_frame);
      structure.reset_parent      (new_integ_frame);
      for (std::list<BodyRefFrame *>::iterator it = vehicle_points.begin();
           it != vehicle_points.end();
           ++it) {
         BodyRefFrame * point = *it;
         point->reset_parent (new_integ_frame);
      }

      get_dynamics_integration_group()->reset_integrators();
   }


   // Propagate the change to all child bodies.
   for (std::list<DynBody *>::iterator it = dyn_children.begin();
        it != dyn_children.end();
        ++it) {
      DynBody * child = *it;
      child->set_integ_frame (new_integ_frame);
   }


   // Add a subscription to the integration frame. The subscription will stay
   // in effect until the body switches integration frames.
   dyn_manager->subscribe_to_frame (new_integ_frame);

   // Keep the gravity interaction model in sync with this body.
   grav_interaction.set_integ_frame (new_integ_frame, *dyn_manager);

   // Set the integration frame for this body.
   integ_frame = &new_integ_frame;

   return;
}


// Set the integration frame for this body and all its child bodies.
void
DynBody::set_integ_frame (                // Return: -- Void
   const char * new_integ_frame_name)     // In:     -- New integration frame
{
   EphemerisRefFrame * new_integ_frame;

   new_integ_frame = dyn_manager->find_integ_frame (new_integ_frame_name);
   if (new_integ_frame == nullptr) {
      MessageHandler::fail (
         __FILE__, __LINE__, DynBodyMessages::invalid_name,
         "Reference frame '%s' sent to DynBody '%s'\n"
         "is not a valid integration frame.",
         new_integ_frame_name, name.c_str());
      return;
   }

   set_integ_frame (*new_integ_frame);
}


// Switch to the specified integration frame.
void
DynBody::switch_integration_frames (      // Return: -- Void
   EphemerisRefFrame & new_integ_frame)   // In:     -- New integration frame
{
   RefFrameState new_state;

   // Pass the buck to the root body.
   // FIXME: Should calling this for a non-root body be an error?
   if (dyn_parent != nullptr) {
      dyn_parent->switch_integration_frames (new_integ_frame);
      return;
   }

   // Validate the provided frame.
   if (! dyn_manager->is_integ_frame (new_integ_frame)) {
      MessageHandler::fail (
         __FILE__, __LINE__, DynBodyMessages::invalid_frame,
         "Reference frame '%s' sent to DynBody '%s'\n"
         "is not a valid integration frame.",
         new_integ_frame.get_name(), name.c_str());

      // Not reached
      return;
   }


   // Update the integrated state from the sources.
   update_integrated_state ();

   // Compute the integrated frame's state wrt the new integration frame.
   integrated_frame->compute_relative_state (new_integ_frame, new_state);

   // Set the integration frame.
   set_integ_frame (new_integ_frame);

   // Update the integrated state and propagate downward.
   integrated_frame->state = new_state;
   propagate_state();
}


// Switch the integration frame for this body and all its child bodies.
void
DynBody::switch_integration_frames (      // Return: -- Void
   const char * new_integ_frame_name)     // In:     -- New integration frame
{
   EphemerisRefFrame * new_integ_frame;

   new_integ_frame = dyn_manager->find_integ_frame (new_integ_frame_name);
   if (new_integ_frame == nullptr) {
      MessageHandler::fail (
         __FILE__, __LINE__, DynBodyMessages::invalid_name,
         "Reference frame '%s' sent to DynBody '%s'\n"
         "is not a valid integration frame.",
         new_integ_frame_name, name.c_str());
      return;
   }

   switch_integration_frames (*new_integ_frame);
}

/**
 * Create the translational and rotational integrators for a
 * DynBody. A DynBody integrates forces and torques
 * in the body frame and forces induced by changes in mass properties.
 * @param[in] generator  Integrator constructor to be used to create
 *                       state integrators.
 * @param[in] controls   The integration ontrols created the integrator
 *                       constructor's create_integration_controls method.
 * @param[in] time_mngr  The JEOD time manager object.
 */
void
DynBody::create_body_integrators (
   const er7_utils::IntegratorConstructor & generator,
   er7_utils::IntegrationControls & controls,
   const JeodIntegrationTime & time_mngr)
{
   // Create the translational state integrator.
   trans_integrator.create_integrator (generator, controls);

   // Create the rotational state integrator, if enabled.
   if (! three_dof) {

     // Validate the user-specified technique against the
     // user-specified integration method.
     rotation_integration =
        GeneralizedSecondOrderODETechnique::validate_technique (
           generator, rotation_integration,
           __FILE__, __LINE__, "DynBody", name.c_str());

      // Create the rotational state integrator.
      rot_integrator.create_integrator (
         rotation_integration, generator, controls);
   }

   // Configure the object that merges results to use the integration technique
   // corresponding to the generator.
   integ_results_merger.configure (generator);

   // FIXME. This has nothing to do with the code at hand.
   time_manager = &time_mngr;
}


/**
 * This interface is required by er7_utils::IntegrableObject. It should not be
 * used. Use DynBody::create_body_integrators instead.
 * @param[in] generator  Unused.
 * @param[in] controls   Unused.
 * @param[in] time_if    Unused.
 */
void
DynBody::create_integrators (
   const er7_utils::IntegratorConstructor & generator,
   er7_utils::IntegrationControls & controls,
   const er7_utils::TimeInterface & time_if)
{
    // Attempt to correct improper call if cast is possible
    const JeodIntegrationTime* time_mngr =
            dynamic_cast<const JeodIntegrationTime*>(&time_if);
    if( time_mngr != nullptr )
    {
        create_body_integrators( generator, controls, *time_mngr );
    }
    else
    {
        MessageHandler::fail (
                __FILE__, __LINE__, DynBodyMessages::internal_error,
                "This interface should not be used to create integrators "
                "for a DynBody.\n"
                "The correct interface is create_body_integrators()");
    }
}

/** Destroy the integrators. Does nothing, but must be implemented to
   * complete abstract function from the inherited IntegrableObject
   */
void
DynBody::destroy_integrators (void)
{
    // Nothing to do.
}


/**
 * Reset the translational and rotational integrators.
 */
void
DynBody::reset_integrators (
   void)
{
   if (translational_dynamics) {
      trans_integrator.reset_integrator();
   }
   if (rotational_dynamics) {
      rot_integrator.reset_integrator();
   }
}


/**
 * Integrate the translational and rotational state and propagate
 * the integrated state to derived states.
 * @param[in]     dyn_dt        Dynamic time step, in dynamic time seconds.
 * @param[in]     target_stage  The stage of the integration process
 *                              that the integrator should try to attain.
 * @return The status (time advance, pass/fail status) of the integration.
 */
er7_utils::IntegratorResult
DynBody::integrate (
   double dyn_dt,
   unsigned int target_stage)
{
   er7_utils::IntegratorResult status (false);

   if(!frame_attach.isAttached()) {

      // Integrate the translational state if enabled to do so.
      if (translational_dynamics) {
         integ_results_merger.merge_integrator_result (
            trans_integ (dyn_dt, target_stage),
            status);
      }

      // Integrate the rotational state if enabled to do so.
      if (rotational_dynamics) {
         integ_results_merger.merge_integrator_result (
            rot_integ (dyn_dt, target_stage),
            status);
      }
   }
   else
   {
       // Get the integ_frame to parent_frame relative state
       RefFrameState X_iframe_to_pframe;
       frame_attach.get_parent_frame()->compute_relative_state(*integ_frame, X_iframe_to_pframe);
       // Add the parent_frame to struct offset from the attachment
       RefFrameState X_iframe_to_struct(X_iframe_to_pframe);
       X_iframe_to_pframe.incr_right(frame_attach.get_attach_offset());
       // Reset the structure state to the new attached state relative to integ_frame
       set_state(RefFrameItems::Pos_Vel_Att_Rate, X_iframe_to_pframe, structure);
   }

   // Mark the state as updated.
   initialized_states.set (RefFrameItems::Pos_Vel_Att_Rate);

   // Propagate the integrated state to other state descriptions.
   propagate_state ();

   return status;
}


/**
 * Integrate the translational state of a DynBody.
 * @param[in]     dyn_dt        Dynamic time step, in dynamic time seconds.
 * @param[in]     target_stage  The stage of the integration process
 *                              that the integrator should try to attain.
 * @return The status (time advance, pass/fail status) of the integration.
 */
er7_utils::IntegratorResult
DynBody::trans_integ (
   double dyn_dt,
   unsigned int target_stage)
{
   // Integrate the translational state.
   return trans_integrator.integrate (
             dyn_dt, target_stage,
             derivs.trans_accel,
             composite_body.state.trans.velocity, composite_body.state.trans.position);
}


/**
 * Integrate the rotational state of a DynBody.
 * @param[in]     dyn_dt        Dynamic time step, in dynamic time seconds.
 * @param[in]     target_stage  The stage of the integration process
 *                              that the integrator should try to attain.
 * @return The status (time advance, pass/fail status) of the integration.
 */
er7_utils::IntegratorResult
DynBody::rot_integ (
   double dyn_dt,
   unsigned int target_stage)
{
   RefFrameRot & rot_state = composite_body.state.rot;

   // Integrate the rotational state.
   er7_utils::IntegratorResult status =
      rot_integrator.integrate (
         dyn_dt, target_stage,
         derivs.rot_accel,
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
   if (std::fpclassify(rot_state.ang_vel_mag) != FP_ZERO) {
      Vector3::scale (rot_state.ang_vel_this,
                      1.0 / rot_state.ang_vel_mag,
                      rot_state.ang_vel_unit);
   }
   else {
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
