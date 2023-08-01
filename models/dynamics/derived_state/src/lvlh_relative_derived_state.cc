/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DerivedState
 * @{
 *
 * @file models/dynamics/derived_state/src/lvlh_relative_derived_state.cc
 * Define methods for the LVLH relative state class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((lvlh_relative_derived_state.cc)
   (relative_derived_state.cc)
   (derived_state.cc)
   (derived_state_messages.cc)
   (dynamics/mass/src/mass_point_state.cc)
   (utils/message/src/message_handler.cc)
   (utils/ref_frames/src/ref_frame.cc)
   (utils/ref_frames/src/ref_frame_compute_relative_state.cc))



*******************************************************************************/


// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/lvlh_relative_derived_state.hh"
#include "../include/derived_state_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Default Constructor.
 */
LvlhRelativeDerivedState::LvlhRelativeDerivedState (
   void)
:
   lvlh_type(LvlhType::Rectilinear),
   use_theta_dot_correction (false)
{
   // Set direction_sense to its only valid value for an LVLH reference.
   direction_sense = RelativeDerivedState::ComputeSubjectStateinTarget;
}



/**
 * Begin initialization of an LvlhRelativeDerivedState.
 * The initialize method for all subclasses of DerivedState *must* pass
 * the initialize call to their immediate parent class.
 * \param[in,out] subject_body Subject body
 * \param[in,out] dyn_manager Dynamics manager
 */
void
LvlhRelativeDerivedState::initialize (
   DynBody & subject_body,
   DynManager & dyn_manager)
{

   // Perform base initializations
   RelativeDerivedState::initialize (subject_body, dyn_manager);
}


/**
 * Update the state.
 */
void
LvlhRelativeDerivedState::update (
   void)
{
   if (lvlh_type == LvlhType::Rectilinear) {
      subject_frame->compute_relative_state (*target_frame, rel_state);
   }

   else if (lvlh_type == LvlhType::CircularCurvilinear) {

      // Calculate rectilinear coordinates.
      RefFrameState rect_rel_state;
      subject_frame->compute_relative_state (*target_frame, rect_rel_state);

      // Transform using phase angle; note result stored in this->rel_state.
      convert_rect_to_circ(rect_rel_state);
   }

   else if (lvlh_type == LvlhType::EllipticalCurvilinear) {

      // Not implemented yet as of JEOD 3.2 release; for now, fail.
      MessageHandler::fail (
         __FILE__, __LINE__, DerivedStateMessages::illegal_value,
         "EllipticalCurvilinear option not yet implemented for \
           LvlhRelativeDerivedState");

      // Not reached
      return;
   }

   else {

      MessageHandler::fail (
         __FILE__, __LINE__, DerivedStateMessages::illegal_value,
         "No valid value of enum LvlhType indicated for \
           LvlhRelativeDerivedState");

      // Not reached
      return;
   }
}


/**
 * Convert from rectilinear LVLH coordinates to circular curvilinear.
 * \param[in] rect_rel_state Source state
 */
void
LvlhRelativeDerivedState::convert_rect_to_circ (
   const RefFrameState & rect_rel_state)
{

   // Locally store values to reduce lookups
   double rect_position[3];
   double rect_velocity[3];
   Vector3::copy(rect_rel_state.trans.position, rect_position);
   Vector3::copy(rect_rel_state.trans.velocity, rect_velocity);

   // Calculate the radial distance of the target LVLH frame from the origin
   // of the planet used to define that LVLH frame. Note that <planet>.inertial
   // is the direct parent of the target LVLH frame by assignation.
   double reference_radius =
             Vector3::vmag(target_frame->state.trans.position);

   // Protect for invalid or zero value for reference radius
   if (reference_radius <= 1e-9) {

      MessageHandler::fail (
         __FILE__, __LINE__, DerivedStateMessages::illegal_value,
         "Derived state %s: Reference radius for curvilinear LVLH calculation"
           " is negative or nearly zero.", name.c_str());

      // Not reached
      return;
   }


   // Calculate the orbital phase angle between the target and subject.
   double theta = std::atan2(rect_position[0],
                             reference_radius - rect_position[2]);

   // Calculate the CLVLH x-position using the phase angle
   rel_state.trans.position[0] = reference_radius * theta;

   // For this implementation, the y-axis for the RLVLH and CLVLH frames
   // are co-aligned, i.e., the CLVLH y-axis does not curve
   rel_state.trans.position[1] = rect_position[1];

   // The CLVLH z-position is the difference between the radial
   // distances of the two vehicles.
   double subject_radius = std::sqrt((reference_radius - rect_position[2]) *
                                     (reference_radius - rect_position[2]) +
                                     rect_position[0] * rect_position[0]);
   rel_state.trans.position[2] = reference_radius - subject_radius;


   // Construct the rotation matrix from RLVLH to CLVLH using phase angle
   double xform_rect2curvi[3][3];
   Matrix3x3::initialize(xform_rect2curvi);
   double cos_theta = std::cos(theta);
   double sin_theta = std::sin(theta);

   xform_rect2curvi[0][0] =  cos_theta;
   xform_rect2curvi[0][2] =  sin_theta;
   xform_rect2curvi[1][1] =  1.0;
   xform_rect2curvi[2][0] = -sin_theta;
   xform_rect2curvi[2][2] =  cos_theta;

   // Transform subject_vehicle's RLVLH velocity by the phase angle to
   // obtain the CLVLH velocity
   Vector3::transform(xform_rect2curvi, rect_velocity, rel_state.trans.velocity);

   // Adjust subject_vehicle's RLVLH attitude by
   // the phase angle as well to get the corresponding CLVLH values
   Matrix3x3::product_right_transpose(rect_rel_state.rot.T_parent_this,
                      xform_rect2curvi, rel_state.rot.T_parent_this);

   //Account for the rotation of the rectilinear LVLH frame
   Vector3::copy(rect_rel_state.rot.ang_vel_this,
                      rel_state.rot.ang_vel_this);
   // Correct the angular velocity to account for theta dot
   do_theta_dot_correction (rel_state.rot.ang_vel_this, rect_rel_state,
                            reference_radius, false);
   rel_state.rot.compute_quaternion();
   rel_state.rot.compute_ang_vel_products();
}


/**
 * Convert from circular curvilinear LVLH coordinates to rectilinear.
 * \param[in] curvi_rel_state Source state
 */
void
LvlhRelativeDerivedState::convert_circ_to_rect (
   const RefFrameState & curvi_rel_state)
{

   // Locally store values to reduce lookups
   double curvi_position[3];
   double curvi_velocity[3];
   Vector3::copy(curvi_rel_state.trans.position, curvi_position);
   Vector3::copy(curvi_rel_state.trans.velocity, curvi_velocity);

   // Calculate the radial distance of the target LVLH frame from the origin
   // of the planet used to define that LVLH frame. Note that <planet>.inertial
   // is the direct parent of the target LVLH frame by assignation.
   double reference_radius =
             Vector3::vmag(target_frame->state.trans.position);

   // Protect for invalid or zero value for reference radius
   if (reference_radius <= 1e-9) {

      MessageHandler::fail (
         __FILE__, __LINE__, DerivedStateMessages::illegal_value,
         "Derived state %s: Reference radius for curvilinear LVLH calculation"
           " is negative or nearly zero.", name.c_str());

      // Not reached
      return;
   }


   // Calculate the radius of subject vehicle; note that CLVLH z-pos is
   // the difference between the radial distances of the two vehicles.
   double subject_radius  = reference_radius - curvi_position[2];


   // Calculate the orbital phase angle between the target and subject.
   // Note curvilinear x-position = reference_radius * theta.
   double theta = curvi_position[0] / reference_radius;

   double cos_theta = std::cos(theta);
   double sin_theta = std::sin(theta);


   // Project CLVLH z-position into RLVLH coordinates using phase angle.
   rel_state.trans.position[2] = reference_radius - (subject_radius * cos_theta);

   // For this implementation, the y-axis for the RLVLH and CLVLH frames
   // are co-aligned, i.e., the CLVLH y-axis does not curve.
   rel_state.trans.position[1] = curvi_position[1];

   // Calculate the RLVLH x-position using the phase angle.
   rel_state.trans.position[0] = subject_radius * sin_theta;


   // Construct the rotation matrix from CLVLH to RLVLH using phase angle
   double xform_curvi2rect[3][3];
   Matrix3x3::initialize(xform_curvi2rect);

   xform_curvi2rect[0][0] =  cos_theta;
   xform_curvi2rect[0][2] = -sin_theta;
   xform_curvi2rect[1][1] =  1.0;
   xform_curvi2rect[2][0] =  sin_theta;
   xform_curvi2rect[2][2] =  cos_theta;

   // Transform subject_vehicle's CLVLH velocity by the phase angle to
   // obtain the RLVLH velocity
   Vector3::transform(xform_curvi2rect, curvi_velocity, rel_state.trans.velocity);

   // Adjust subject_vehicle's CLVLH attitude by
   // the phase angle as well to get the corresponding RLVLH value
   Matrix3x3::product_right_transpose(curvi_rel_state.rot.T_parent_this,
                      xform_curvi2rect, rel_state.rot.T_parent_this);
   Vector3::copy(curvi_rel_state.rot.ang_vel_this, rel_state.rot.ang_vel_this);
   do_theta_dot_correction(rel_state.rot.ang_vel_this, rel_state,
                           reference_radius, true);
   rel_state.rot.compute_quaternion();
   rel_state.rot.compute_ang_vel_products();
}

/**
 * Compute thetadot correction to omega.
 */
void LvlhRelativeDerivedState::do_theta_dot_correction (double omega[3],
                                                        const RefFrameState &s,
                                                        const double r,
                                                        bool c2r) {
   if (! use_theta_dot_correction) {
      return;
   }

   double x = r-s.trans.position[2];
   double y=s.trans.position[0];
   double rdot=Vector3::dot(target_frame->state.trans.position,
                            target_frame->state.trans.velocity)/r;
   double xdot=rdot-s.trans.velocity[2];
   double ydot=s.trans.velocity[0];
   double delta_omega[3] = {0, (x*ydot-y*xdot)/(x*x+y*y), 0};

   Vector3::transform(s.rot.T_parent_this, delta_omega);
   if (c2r) {
      Vector3::decr(delta_omega, omega);
   } else {
      Vector3::incr(delta_omega, omega);
   }
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
