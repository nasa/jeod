/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/src/dyn_body_init_lvlh_state.cc
 * Define methods for the DynBodyInitLvlhState class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((dyn_body_init_lvlh_state.o)
   (dynamics/derived_state/lvlh_relative_derived_state.o)
   (utils/ref_frames/ref_frame.o))


*******************************************************************************/


// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/derived_state/include/lvlh_relative_derived_state.hh"
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/ref_frames/include/ref_frame_items.hh"

// Model includes
#include "../include/body_action_messages.hh"
#include "../include/dyn_body_init_lvlh_state.hh"



//! Namespace jeod
namespace jeod {

/**
 * DynBodyInitLvlhState default constructor.
 */
DynBodyInitLvlhState::DynBodyInitLvlhState (
   void)
:
   DynBodyInitPlanetDerived(),
   lvlh_type (LvlhType::Rectilinear),
   lvlh_object_ptr (nullptr)
{
   required_items = RefFrameItems::Pos_Vel;
   return;
}


/**
 * DynBodyInitLvlhState destructor.
 */
DynBodyInitLvlhState::~DynBodyInitLvlhState (
   void)
{
   return;
}

/**
 * Cache a pointer to a user-supplied LvlhFrame object.
 * \param[in] lvlh_frame_object LVLH frame object
 */
void
DynBodyInitLvlhState::set_lvlh_frame_object (
   LvlhFrame & lvlh_frame_object)
{
   lvlh_object_ptr = &lvlh_frame_object;
}


/**
 * Initialize the initializer.
 * \param[in,out] dyn_manager Dynamics manager
 */
void
DynBodyInitLvlhState::initialize (
   DynManager & dyn_manager)
{

   // If an LVLH frame has been user-provided, then ref_body is not required
   if (lvlh_object_ptr != nullptr) {
      body_is_required = false;
   }
   else {
      body_is_required = true;
   }

   // Pass the message up the chain. This will initialize the inherited
   // characteristics of the instance.
   DynBodyInitPlanetDerived::initialize (dyn_manager);

   return;
}


/**
 * Apply the initializer: Construct the reference LVLH frame so the
 * parent initializer can compute the vehicle's state relative to the
 * vehicle's inertial frame.
 * \param[in,out] dyn_manager Dynamics manager
 */
void
DynBodyInitLvlhState::apply (
   DynManager & dyn_manager)
{
   LvlhFrame lvlh_frame;         // -- Reference vehicle LVLH frame
   RefFrameItems test_items(set_items); // -- The state components to be set

   if (lvlh_object_ptr != nullptr) {
      lvlh_object_ptr->update();
      reference_ref_frame = &lvlh_object_ptr->frame;
   } else {
      // Construct the LVLH frame.
      lvlh_frame.set_subject_frame (ref_body->composite_body);
      lvlh_frame.set_planet (*planet);
      lvlh_frame.initialize (dyn_manager);
      lvlh_frame.update ();
      // The position/velocity/attitude/rate data pertain to this newly
      // constructed LVLH frame.
      reference_ref_frame = &lvlh_frame.frame;
   }

   if (lvlh_type == LvlhType::CircularCurvilinear) {
      // Curvilinear LVLH provided, must be converted to rectilinear
      // Declare and initialize a temporary LVLH relative state.
      LvlhRelativeDerivedState lvlh_state;

      // Set the direction sense to the only value that makes sense in this case
         lvlh_state.direction_sense = RelativeDerivedState::ComputeSubjectStateinTarget;

      // Set the target frame for lvlh_state
      lvlh_state.set_target_frame (*reference_ref_frame);

      // Store body action variables in a reference frame state
      RefFrameState circ_rel_state;

      Vector3::copy(position, circ_rel_state.trans.position);
      Vector3::copy(velocity, circ_rel_state.trans.velocity);

      orientation.compute_transform();
      Matrix3x3::copy(orientation.trans, circ_rel_state.rot.T_parent_this);
      circ_rel_state.rot.compute_quaternion();

      Vector3::copy(ang_velocity, circ_rel_state.rot.ang_vel_this);
      circ_rel_state.rot.compute_ang_vel_products();


      // Convert provided curvilinear inputs into rectilinear. Note
      // the result of the conversion is stored in lvlh_state.rel_state.
      lvlh_state.convert_circ_to_rect(circ_rel_state);


      // Extract info into appropriate variables for application.
      Vector3::copy(lvlh_state.rel_state.trans.position, position);
      Vector3::copy(lvlh_state.rel_state.trans.velocity, velocity);
      Vector3::copy(lvlh_state.rel_state.rot.ang_vel_this, ang_velocity);

      Matrix3x3::copy(lvlh_state.rel_state.rot.T_parent_this, orientation.trans);

      orientation.quat = lvlh_state.rel_state.rot.Q_parent_this;

      Orientation::compute_euler_angles_from_matrix
          (orientation.trans, orientation.euler_sequence,
           orientation.euler_angles);
      Orientation::compute_eigen_rotation_from_matrix
          (orientation.trans, &(orientation.eigen_angle),
           orientation.eigen_axis);
   }

   else if (lvlh_type == LvlhType::EllipticalCurvilinear) {

      // Not implemented yet as of March 2015; for now, fail.
      MessageHandler::fail (
         __FILE__, __LINE__, BodyActionMessages::illegal_value,
         "EllipticalCurvilinear option not yet implemented for \
           DynBodyInitLvlhState");

      // Not reached
      return;
   }

   else if (lvlh_type != LvlhType::Rectilinear) {

      MessageHandler::fail (
         __FILE__, __LINE__, BodyActionMessages::illegal_value,
         "No valid value of enum LvlhType indicated for \
           DynBodyInitLvlhState");

      // Not reached
      return;
   }


// Compute the state relative to the parent given the user inputs
   // relative to the reference reference frame.
   apply_user_inputs ();

   // Pass the message up the chain, with the reference frame for the
   // state set to this newly-constructed LVLH frame.
   DynBodyInitPlanetDerived::apply (dyn_manager);


   // The LVLH frame will go out of scope on returning from this function.
   // The reference_ref_frame should no longer point to this frame.
   reference_ref_frame = nullptr;

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
