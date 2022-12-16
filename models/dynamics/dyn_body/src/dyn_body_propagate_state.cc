/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynBody
 * @{
 *
 * @file models/dynamics/dyn_body/src/dyn_body_propagate_state.cc
 * Define DynBody state propagation / update methods.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((dyn_body_propagate_state.o)
   (dyn_body.o)
   (dyn_body_messages.o)
   (dynamics/mass/mass_point_state.o)
   (environment/time/time_manager.o)
   (utils/ref_frames/ref_frame.o))



*******************************************************************************/


// System includes
#include <cstddef>

// JEOD includes
#include "utils/integration/include/jeod_integration_time.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/dyn_body.hh"
#include "../include/dyn_body_messages.hh"


//! Namespace jeod
namespace jeod {

// Compute the relative state between the integrated frame's mass point
// and the source frame's mass point.
void
DynBody::compute_ref_point_transform (
   const BodyRefFrame & source_frame,
   const MassPoint ** const ref_point,
   MassPointState & rel_state)
{
   const MassPoint * source_point;
   const MassPoint * target_point;
   const DynBody * source_body =
      dynamic_cast < const DynBody *> (source_frame.get_owner());


   // Integrated frame is the structural frame:
   // The source frame's mass point is a part of the tree rooted by the root
   // body's structural point, which corresponds to the integration frame.
   if (integrated_frame == &structure) {
      source_point = source_frame.mass_point;

      // The relative state only needs to be computed if there was no previous
      // computation or if the previous computation was for a different point.
      if (*ref_point != source_point) {
         *ref_point = source_point;
         source_point->compute_state_wrt_pred (
            mass.structure_point, rel_state);
      }
   }

   // Integrated frame is the composite body frame:
   // The path from the source frame to the root body's composite body frame
   // is taken in one or two steps, depending on the source frame.
   // First set the source_point and target_point, where
   // - The source_point is one of source body's mass points that corresponds
   //   to the source_frame.
   // - The target_point is the root body's mass point that is the root of the
   //   tree containing the source_point.
   // Ideally, the target_point will correspond to the root body's composite
   // body frame. That ideal is not always possible. If not, a second step
   // will be needed to go from the target_point to the composite body frame.
   // The selections are
   // - Source frame is a composite body frame:
   //   source_point is the source body's composite_wrt_pbdy point,
   //   target_point is the root body's composite_wrt_pbdy point.
   // - Source frame is a core body frame:
   //   source_point is the source body's core_wrt_composite point,
   //   target_point is the root body's composite_wrt_pbdy point.
   // - Source frame is a structural frame or vehicle point frame:
   //   source_point is the source frame's mass_point,
   //   target_point is the root body's structure_point.
   else if (integrated_frame == &composite_body) {
      if (&source_frame == &source_body->composite_body) {
         target_point = &mass.composite_wrt_pbdy;
         source_point = &source_body->mass.composite_wrt_pbdy;
      } else if (&source_frame == &source_body->core_body) {
         target_point = &mass.composite_wrt_pbdy;
         source_point = &source_body->mass.core_wrt_composite;
      } else {
         target_point = &mass.structure_point;
         source_point = source_frame.mass_point;
      }

      // Only perform the computation if this is a new relative state.
      if (*ref_point != source_point) {
         *ref_point = source_point;

         // Compute state of source point with respect to the target.
         source_point->compute_state_wrt_pred (
            *target_point, rel_state);

         // If the target point is the structure point, the rel_state computed
         // above is the state of the source point/frame wrt the root body's
         // structure origin/fram
         // Incorporate the structure-to-body state into the rel_state.
         if (target_point == &mass.structure_point) {

            // Nomenclature:
            //   C = source point
            //   B = structure
            //   A = composite
            // Want r_A->C:A, T_A->C in rel_state
            // Have r_B->C:B, T_B->C in rel_state (needs to be updated)
            // Have r_B->A:B, T_B->A in composite_properties
            // Relevant math:
            //   r_A->C:A = T_B->A * (r_B->C:B - r_B->A:B)
            //   T_A->C = T_B->C * T_A->B = T_B->C * T_B->A^T
            Vector3::decr (
               mass.composite_properties.position, rel_state.position);
            Vector3::transform (
               mass.composite_properties.T_parent_this, rel_state.position);

            rel_state.Q_parent_this.multiply_conjugate (
               mass.composite_properties.Q_parent_this);
            rel_state.Q_parent_this.normalize ();
            rel_state.Q_parent_this.left_quat_to_transformation (
               rel_state.T_parent_this);
         }
      }
   }

   // The only valid integration frames are structure and composite.
   else {
      MessageHandler::fail (
         __FILE__, __LINE__, DynBodyMessages::invalid_frame,
         "Body '%s' has an invalid integration frame.", name.c_str());
   }
}


// Compute a derived state given the source state and the position/
// attitude transformation from the source to the derived state.
void
DynBody::compute_derived_state_forward (
   const BodyRefFrame & source_frame,
   const MassPoint & rel_state,
   BodyRefFrame & derived_frame)
const
{
   double wxr[3];

   // A = integration frame
   // B = source frame
   // C = derived frame

   // T_A->C = T_B->C * T_A->B
   rel_state.Q_parent_this.multiply (
      source_frame.state.rot.Q_parent_this,
      derived_frame.state.rot.Q_parent_this);
   derived_frame.state.rot.Q_parent_this.normalize ();
   derived_frame.state.rot.compute_transformation ();

   // w_A->C:C = T_B->C w_A->B:B
   Vector3::transform (
      rel_state.T_parent_this, source_frame.state.rot.ang_vel_this,
      derived_frame.state.rot.ang_vel_this);
   derived_frame.state.rot.compute_ang_vel_products ();

   // r_A->C:A = r_A->B:A + r_B->C:A
   //   = r_A->B:A + T_A->B^T * r_B->C:B
   Vector3::copy (
      source_frame.state.trans.position,
      derived_frame.state.trans.position);
   Vector3::transform_transpose_incr (
      source_frame.state.rot.T_parent_this, rel_state.position,
      derived_frame.state.trans.position);

   // v_A->C:A = v_A->B:A + v_B->C:A
   //   = v_A->B:A + T_A->B^T * (w_A->B:B X r_B->C:B)
   Vector3::copy (
      source_frame.state.trans.velocity,
      derived_frame.state.trans.velocity);
   Vector3::cross (
      source_frame.state.rot.ang_vel_this, rel_state.position, wxr);
   Vector3::transform_transpose_incr (
      source_frame.state.rot.T_parent_this, wxr,
      derived_frame.state.trans.velocity);

   // Denote that the state has been fully set.
   derived_frame.initialized_items.set (RefFrameItems::Pos_Vel_Att_Rate);

   // Time stamp the derived frame per the source frame's timestamp.
   derived_frame.set_timestamp (source_frame.timestamp ());
}


// Compute selected aspects of the derived state.
void
DynBody::compute_state_elements_forward (
   const BodyRefFrame & source_frame,
   const MassPoint & rel_state,
   const RefFrameItems & state_items,
   BodyRefFrame & derived_frame)
const
{
   double wxr[3];

   // A = integration frame
   // B = source frame
   // C = derived frame

   // T_A->C = T_B->C * T_A->B
   if (state_items.contains (RefFrameItems::Att)) {
      rel_state.Q_parent_this.multiply (
         source_frame.state.rot.Q_parent_this,
         derived_frame.state.rot.Q_parent_this);
      derived_frame.state.rot.Q_parent_this.normalize ();
      derived_frame.state.rot.compute_transformation ();
   }

   // w_A->C:C = T_B->C w_A->B:B
   if (state_items.contains (RefFrameItems::Rate)) {
      Vector3::transform (
         rel_state.T_parent_this, source_frame.state.rot.ang_vel_this,
         derived_frame.state.rot.ang_vel_this);
      derived_frame.state.rot.compute_ang_vel_products ();
   }

   // r_A->C:A = r_A->B:A + r_B->C:A
   //   = r_A->B:A + T_A->B^T * r_B->C:B
   if (state_items.contains (RefFrameItems::Pos)) {
      Vector3::copy (
         source_frame.state.trans.position,
         derived_frame.state.trans.position);
      Vector3::transform_transpose_incr (
         source_frame.state.rot.T_parent_this, rel_state.position,
         derived_frame.state.trans.position);
   }

   // v_A->C:A = v_A->B:A + v_B->C:A
   //   = v_A->B:A + T_A->B^T * (w_A->B:B X r_B->C:B)
   if (state_items.contains (RefFrameItems::Vel)) {
      Vector3::copy (
         source_frame.state.trans.velocity,
         derived_frame.state.trans.velocity);
      Vector3::cross (
         source_frame.state.rot.ang_vel_this, rel_state.position, wxr);
      Vector3::transform_transpose_incr (
         source_frame.state.rot.T_parent_this, wxr,
         derived_frame.state.trans.velocity);
   }

   // Denote that the propagated items have been set.
   derived_frame.initialized_items.add (state_items.get ());

   // Time stamp the derived frame per the source frame's timestamp.
   derived_frame.set_timestamp (source_frame.timestamp ());
}


// Compute a derived state given the source state.
void
DynBody::compute_derived_state_reverse (
   const BodyRefFrame & source_frame,
   const MassPoint & rel_state,
   BodyRefFrame & derived_frame)
const
{
   double wxr[3];

   // A = integration frame
   // B = source frame
   // C = derived frame

   // T_A->C = T_B->C * T_A->B = T_C->B^T * T_A->B
   rel_state.Q_parent_this.conjugate_multiply (
      source_frame.state.rot.Q_parent_this,
      derived_frame.state.rot.Q_parent_this);
   derived_frame.state.rot.Q_parent_this.normalize ();
   derived_frame.state.rot.compute_transformation ();

   // w_A->C:C = T_B->C w_A->B:B = T_C->B^T * w_A->B:B
   Vector3::transform_transpose (
      rel_state.T_parent_this, source_frame.state.rot.ang_vel_this,
      derived_frame.state.rot.ang_vel_this);
   derived_frame.state.rot.compute_ang_vel_products ();

   // r_A->C:A = r_A->B:A + r_B->C:A
   //   = r_A->B:A - T_A->C^T * r_C->B:C
   Vector3::copy (
      source_frame.state.trans.position,
      derived_frame.state.trans.position);
   Vector3::transform_transpose_decr (
      derived_frame.state.rot.T_parent_this, rel_state.position,
      derived_frame.state.trans.position);

   // v_A->C:A = v_A->B:A + v_B->C:A
   //   = v_A->B:A - T_A->C^T * (w_A->C:C X r_C->B:C)
   Vector3::copy (
      source_frame.state.trans.velocity,
      derived_frame.state.trans.velocity);
   Vector3::cross (
      derived_frame.state.rot.ang_vel_this, rel_state.position, wxr);
   Vector3::transform_transpose_decr (
      derived_frame.state.rot.T_parent_this, wxr,
      derived_frame.state.trans.velocity);

   // Denote that the state has been fully set.
   derived_frame.initialized_items.set (RefFrameItems::Pos_Vel_Att_Rate);

   // Time stamp the derived frame per the source frame's timestamp.
   derived_frame.set_timestamp (source_frame.timestamp ());
}


// Compute selected aspects of the derived state.
void
DynBody::compute_state_elements_reverse (
   const BodyRefFrame & source_frame,
   const MassPoint & rel_state,
   const RefFrameItems & state_items,
   BodyRefFrame & derived_frame)
const
{
   double wxr[3];

   // A = integration frame
   // B = source frame
   // C = derived frame

   // T_A->C = T_B->C * T_A->B
   if (state_items.contains (RefFrameItems::Att)) {
      rel_state.Q_parent_this.conjugate_multiply (
         source_frame.state.rot.Q_parent_this,
         derived_frame.state.rot.Q_parent_this);
      derived_frame.state.rot.Q_parent_this.normalize ();
      derived_frame.state.rot.compute_transformation ();
   }

   // w_A->C:C = T_B->C w_A->B:B
   if (state_items.contains (RefFrameItems::Rate)) {
      Vector3::transform_transpose (
         rel_state.T_parent_this, source_frame.state.rot.ang_vel_this,
         derived_frame.state.rot.ang_vel_this);
      derived_frame.state.rot.compute_ang_vel_products ();
   }

   // r_A->C:A = r_A->B:A + r_B->C:A
   //   = r_A->B:A + T_A->B^T * r_B->C:B
   if (state_items.contains (RefFrameItems::Pos)) {
      Vector3::copy (
         source_frame.state.trans.position,
         derived_frame.state.trans.position);
      Vector3::transform_transpose_decr (
         derived_frame.state.rot.T_parent_this, rel_state.position,
         derived_frame.state.trans.position);
   }

   // v_A->C:A = v_A->B:A + v_B->C:A
   //   = v_A->B:A + T_A->B^T * (w_A->B:B X r_B->C:B)
   if (state_items.contains (RefFrameItems::Vel)) {
      Vector3::copy (
         source_frame.state.trans.velocity,
         derived_frame.state.trans.velocity);
      Vector3::cross (
         derived_frame.state.rot.ang_vel_this, rel_state.position, wxr);
      Vector3::transform_transpose_decr (
         derived_frame.state.rot.T_parent_this, wxr,
         derived_frame.state.trans.velocity);
   }

   // Denote that the propagated items have been set.
   derived_frame.initialized_items.add (state_items.get ());

   // Time stamp the derived frame per the source frame's timestamp.
   derived_frame.set_timestamp (source_frame.timestamp ());
}


// Propagate state from state owners to the integrated state.
void
DynBody::update_integrated_state ()
{

   // Non-root bodies do not have an integrated state.
   // Pass the buck to the root body.
   if (dyn_parent != nullptr) {
      get_root_body_internal()->update_integrated_state ();
      return;
   }

   MassPointState rel_state; /* --
      Postion/attitude state of the integrated state with respect to the source
      of some component of the state, computed by means of the mass tree rather
      than the ref frame tree (the latter not being known yet). */

   const MassPoint * ref_point; /* --
      The point to which the rel_state refers. */

   // Nomenclature for below:
   //   T_X->Y     Transformation from frame X to frame Y
   //   r_X->Y:Z   Vector from frame X to frame Y origin, expressed in frame Z
   //   v_X->Y:Z   Time derivative of r_X->Y:Z, as observed in frame Z
   //   w_X->Y:Z   Angular velocity of frame X wrt frame Y, expressed in frame Z
   // Reference frames:
   //   A = integration frame
   //   C = integrated frame
   //   B = source frame

   ref_point = nullptr;

   // Attitude.
   // Note that attitude is initialized if the source is the integrated frame.
   if (attitude_source == integrated_frame) {
      initialized_states.add (RefFrameItems::Att);
   }

   // Have attitude data, but source is not the integrated frame:
   // Convert to the integrated frame and note that attitude is initialized.
   else if (attitude_source != nullptr) {

      // Compute the position/attitude state of the attitude source frame
      // with respect to the integrated frame.
      compute_ref_point_transform (*attitude_source, &ref_point, rel_state);

      // Compute the orientation of the integrated frame wrt the integration
      // frame. Have at hand
      //   T_A->B  Transformation from integration frame to source frame,
      //           in attitude_source->state.rot.T_parent_this
      //   T_C->B  Transformation from integrated frame to source frame,
      //           in rel_state.T_parent_this
      // Conceptually,
      //   T_A->C = T_C->B^T * T_A->B
      // Calculation is done with left transformation quaternions, which chain
      // like transformation matrices.
      rel_state.Q_parent_this.conjugate_multiply (
         attitude_source->state.rot.Q_parent_this,
         integrated_frame->state.rot.Q_parent_this);
      integrated_frame->state.rot.Q_parent_this.normalize ();
      integrated_frame->state.rot.compute_transformation ();

      // Mark attitude as initialized and transfer attitude ownership to the
      // integrated frame.
      initialized_states.add (RefFrameItems::Att);
      attitude_source = integrated_frame;
   }


   // Attitide rate:
   // Note that rate is initialized if the source is the integrated frame.
   if (rate_source == integrated_frame) {
      initialized_states.add (RefFrameItems::Rate);
   }

   // Have rate data, but source is not the integrated frame:
   // Convert to the integrated frame if possible and note that rate is
   // initialized. Attitude is needed to perform this conversion.
   else if ((rate_source != nullptr) &&
            (attitude_source == integrated_frame)) {

      // Compute the position/attitude state of the rate source frame
      // with respect to the integrated frame.
      compute_ref_point_transform (*rate_source, &ref_point, rel_state);

      // With the rigid body assumption, all attached bodies have exactly the
      // same angular velocity with respect to inertial, but different frames
      // will have different representations of this vector.
      // Transform the rate source's angular velocity to root body coordinates:
      //   w_A->C:C = T_C->B^T * w_A->B:B
      Vector3::transform_transpose (
         rel_state.T_parent_this, rate_source->state.rot.ang_vel_this,
         integrated_frame->state.rot.ang_vel_this);
      integrated_frame->state.rot.compute_ang_vel_products ();

      // Mark rate as initialized and transfer rate ownership to the
      // integrated frame.
      initialized_states.add (RefFrameItems::Rate);
      rate_source = integrated_frame;
   }


   // Position:
   // Note that position is initialized if the source is the integrated frame.
   if (position_source == integrated_frame) {
      initialized_states.add (RefFrameItems::Pos);
   }

   // Have position data, but source is not the integrated frame:
   // Convert to the integrated frame if possible and note that rate is
   // initialized. Attitude is needed to perform this conversion.
   else if ((position_source != nullptr) &&
            (attitude_source == integrated_frame)) {

      // Compute the position/attitude state of the position source frame
      // with respect to the integrated frame.
      compute_ref_point_transform (*position_source, &ref_point, rel_state);

      // r_A->C:A = r_A->B:A + r_B->C:A
      //   = r_A->B:A - T_A->C^T * r_C->B:C
      Vector3::copy (
         position_source->state.trans.position,
         integrated_frame->state.trans.position);
      Vector3::transform_transpose_decr (
         integrated_frame->state.rot.T_parent_this, rel_state.position,
         integrated_frame->state.trans.position);

      // Mark position as initialized and transfer position ownership to the
      // integratet frame.
      initialized_states.add (RefFrameItems::Pos);
      position_source = integrated_frame;
   }


   // Velocity:
   // Note that velocity is initialized if the source is the integrated frame.
   if (velocity_source == integrated_frame) {
      initialized_states.add (RefFrameItems::Vel);
   }

   // Have velocity data, but source is not the integrated frame:
   // Convert to the integrated frame if possible and note that rate is
   // initialized. Attitude and rate are needed to perform this conversion.
   else if ((velocity_source != nullptr) &&
            (attitude_source == integrated_frame) &&
            (rate_source == integrated_frame)) {
      double wxr[3];

      // Compute the position/attitude state of the velocity source frame
      // with respect to the integrated frame.
      compute_ref_point_transform (*velocity_source, &ref_point, rel_state);

      // v_A->C:A = v_A->B:A + v_B->C:A
      //   = v_A->B:A - T_A->C^T * (w_A->C:C * r_C->B:C)
      Vector3::copy (
         velocity_source->state.trans.velocity,
         integrated_frame->state.trans.velocity);
      Vector3::cross (
         integrated_frame->state.rot.ang_vel_this, rel_state.position, wxr);
      Vector3::transform_transpose_decr (
         integrated_frame->state.rot.T_parent_this, wxr,
         integrated_frame->state.trans.velocity);

      // Mark velocity as initialized and transfer velocity ownership to the
      // integrated frame.
      initialized_states.add (RefFrameItems::Vel);
      velocity_source = integrated_frame;
   }

   integrated_frame->initialized_items = initialized_states;

   // Time stamp the integrated frame per the time manager.
   if (time_manager != nullptr) {
      integrated_frame->set_timestamp (time_manager->get_timestamp_time());
   }
}


// Propagate state to attached bodies.
void
DynBody::propagate_state ()
{

   // Pass the buck to the root body.
   if (dyn_parent != nullptr) {
      dyn_parent->propagate_state();
      return;
   }

   // Update the integrated state from the sources.
   update_integrated_state ();

   // Propagate the state so long as it is not empty.
   // NOTE: 3DOF simulations still need attitude it's just not propagated.
   if (! initialized_states.is_empty ()) {
      if (integrated_frame == &structure) {
         propagate_state_from_structure ();
      }

      else if (integrated_frame == &composite_body) {
         propagate_state_from_composite ();
      }

      else {
         MessageHandler::fail (
            __FILE__, __LINE__, DynBodyMessages::invalid_frame,
            "Body '%s' has an invalid integration frame.", name.c_str());
      }
   }
}


// Propagate state to attached bodies starting from the structural frame.
void
DynBody::propagate_state_from_structure ()
{
   // Use the full propagation methods if the state is full.
   if (structure.initialized_items.is_full ()) {

      // Propagate the structure state to the composite and core body states
      // for this object.
      compute_derived_state_forward (
         structure, mass.composite_properties, composite_body);

      compute_derived_state_forward (
         structure, mass.core_properties, core_body);

      // Propagate state to the vehicle points.
      if (autoupdate_vehicle_points) {
         compute_vehicle_point_states (RefFrameItems::Pos_Vel_Att_Rate);
      }

      // Propagate the structure state to all child bodies.
      for (std::list<DynBody *>::iterator it = dyn_children.begin();
           it != dyn_children.end();
           ++it) {
         DynBody * child = *it;

         // Propagate the structure state to the child body's structure state.
         compute_derived_state_forward (
            structure, child->mass.structure_point, child->structure);

         // Mark the child's known states.
         child->initialized_states.set (RefFrameItems::Pos_Vel_Att_Rate);

         // Make the child body recursively propagate its structure state.
         child->propagate_state_from_structure ();
      }
   }

   // State is not full:
   // Use the partial propagation methods, propagating only those states
   // that can be reliably set.
   else {
      RefFrameItems propagated_states (structure.initialized_items.get ());
      RefFrameItems::Items propagated_items;

      // Remove state items whose state becomes indeterminate upon propagation.
      if (! propagated_states.is_full ()) {
         if (! propagated_states.contains (RefFrameItems::Att)) {
            propagated_states.remove (RefFrameItems::Pos_Vel);
         }
         else if (! propagated_states.contains (RefFrameItems::Rate)) {
            propagated_states.remove (RefFrameItems::Vel);
         }
      }

      propagated_items = propagated_states.get ();

      // Propagate the structure state to the composite and core body states
      // for this object.
      compute_state_elements_forward (
         structure, mass.composite_properties, propagated_states,
         composite_body);
      compute_state_elements_forward (
         structure, mass.core_properties, propagated_states,
         core_body);

      // Propagate state to the vehicle points.
      if (autoupdate_vehicle_points) {
         compute_vehicle_point_states (propagated_items);
      }

      // Propagate the structure state to all child bodies.
      for (std::list<DynBody *>::iterator it = dyn_children.begin();
           it != dyn_children.end();
           ++it) {
         DynBody * child = *it;

         // Propagate the structure state to the child body's structure state.
         compute_state_elements_forward (
            structure, child->mass.structure_point, propagated_states,
            child->structure);

         // Mark the child's known states.
         child->initialized_states.add (propagated_items);

         // Make the child body recursively propagate its structure state.
         child->propagate_state_from_structure ();
      }
   }
}


// Propagate state to attached bodies starting from the composite frame.
void
DynBody::propagate_state_from_composite ()
{
   // Use the full propagation methods if the state is full.
   if (composite_body.initialized_items.is_full ()) {

      // Propagate the composite state to the structure and core body states
      // for this object.
      compute_derived_state_reverse (
         composite_body, mass.composite_properties, structure);
      compute_derived_state_forward (
         composite_body, mass.core_wrt_composite, core_body);

      // Propagate state to the vehicle points.
      if (autoupdate_vehicle_points) {
         compute_vehicle_point_states (RefFrameItems::Pos_Vel_Att_Rate);
      }

      // Propagate the structure state to all child bodies.
      for (std::list<DynBody *>::iterator it = dyn_children.begin();
           it != dyn_children.end();
           ++it) {
         DynBody * child = *it;

         // Propagate the composite state to the child body's composite state.
         compute_derived_state_forward (
            composite_body, child->mass.composite_wrt_pbdy, child->composite_body);

         // Mark the child's known states.
         child->initialized_states.set (RefFrameItems::Pos_Vel_Att_Rate);

         // Make the child body recursively propagate its composite state.
         child->propagate_state_from_composite ();
      }
   }

   // State is not full:
   // Use the partial propagation methods, propagating only those states
   // that can be reliably set.
   else {
      RefFrameItems propagated_states (composite_body.initialized_items.get ());
      RefFrameItems::Items propagated_items;

      // Remove state items whose state becomes indeterminate upon propagation.
      if (! propagated_states.is_full ()) {
         if (! propagated_states.contains (RefFrameItems::Att)) {
            propagated_states.remove (RefFrameItems::Pos_Vel);
         }
         else if (! propagated_states.contains (RefFrameItems::Rate)) {
            propagated_states.remove (RefFrameItems::Vel);
         }
      }

      propagated_items = propagated_states.get ();

      // Propagate the composite state to the structure and core body states
      // for this object.
      compute_state_elements_reverse (
         composite_body, mass.composite_properties, propagated_states, structure);
      compute_state_elements_forward (
         composite_body, mass.core_wrt_composite, propagated_states, core_body);

      // Propagate state to the vehicle points.
      if (autoupdate_vehicle_points) {
         compute_vehicle_point_states (propagated_items);
      }

      // Propagate the structure state to all child bodies.
      for (std::list<DynBody *>::iterator it = dyn_children.begin();
           it != dyn_children.end();
           ++it) {
         DynBody * child = *it;

         // Propagate the structure state to the child body's structure state.
         compute_state_elements_forward (
            composite_body, child->mass.composite_wrt_pbdy, propagated_states,
            child->composite_body);

         // Mark the child's known states.
         child->initialized_states.add (propagated_items);

         // Make the child body recursively propagate its structure state.
         child->propagate_state_from_structure ();
      }
   }
}


// Propagate structure frame state to vehicle points.
void
DynBody::compute_vehicle_point_states (
   RefFrameItems::Items set_items)
{

   // Have the full state: Compute vehicle point states from structure via
   // compute_derived_state_forward.
   if (set_items == RefFrameItems::Pos_Vel_Att_Rate) {
      for (std::list<BodyRefFrame *>::iterator it = vehicle_points.begin();
           it != vehicle_points.end();
           ++it) {
         BodyRefFrame * point = *it;
         compute_derived_state_forward (
            structure, *(point->mass_point), *point);
      }
   }

   // Have the partial state: Compute vehicle point states from structure via
   // compute_state_elements_forward.
   else {
      for (std::list<BodyRefFrame *>::iterator it = vehicle_points.begin();
           it != vehicle_points.end();
           ++it) {
         BodyRefFrame * point = *it;
         compute_state_elements_forward (
            structure, *(point->mass_point), set_items, *point);
      }
   }
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
