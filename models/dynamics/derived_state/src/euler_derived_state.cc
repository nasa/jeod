/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DerivedState
 * @{
 *
 * @file models/dynamics/derived_state/src/euler_derived_state.cc
 * Define methods for the Euler attitude derived state class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((euler_derived_state.o)
   (derived_state.o)
   (dynamics/mass/mass_point_state.o)
   (utils/ref_frames/ref_frame.o)
   (utils/ref_frames/ref_frame_compute_relative_state.o))



*******************************************************************************/


// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/orientation/include/orientation.hh"

// Model includes
#include "../include/euler_derived_state.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a EulerDerivedState object.
 */
EulerDerivedState::EulerDerivedState (
   void)
:
   sequence (Orientation::Roll_Pitch_Yaw),
   rel_frame (NULL)
{
   ref_body_angles[0] = 0.0;
   ref_body_angles[1] = 0.0;
   ref_body_angles[2] = 0.0;
   body_ref_angles[0] = 0.0;
   body_ref_angles[1] = 0.0;
   body_ref_angles[2] = 0.0;
}



/**
 * Destruct a EulerDerivedState object.
 */
EulerDerivedState::~EulerDerivedState (
   void)
{
   // Remove the initialization-time subscription to the target frame.
   if (rel_frame != NULL) {
      rel_frame->unsubscribe();
   }
}



/**
 * Begin initialization of a EulerDerivedState.
 * The initialize method for all subclasses of DerivedState *must* pass
 * the initialize call to their immediate parent class, which in turn
 * must do the same, eventually invoking this method.
 * \param[in,out] subject_body Subject body.
 * \param[in,out] dyn_manager Dynamics manager.
 */
void
EulerDerivedState::initialize (
   DynBody    & subject_body,
   DynManager & dyn_manager)
{
   // Call the parent class initialization method.
   DerivedState::initialize (subject_body, dyn_manager);
}



/**
 * Begin initialization of a EulerDerivedState.
 * The initialize method for all subclasses of DerivedState *must* pass
 * the initialize call to their immediate parent class, which in turn
 * must do the same, eventually invoking this method.
 * \param[in] ref_frame Reference frame for angles.
 * \param[in,out] subject_body Subject body.
 * \param[in,out] dyn_manager Dynamics manager.
 */
void
EulerDerivedState::initialize (
   RefFrame   & ref_frame,
   DynBody    & subject_body,
   DynManager & dyn_manager)
{
   // Call the parent class initialization method.
   DerivedState::initialize (subject_body, dyn_manager);

   // Set the reference frame for the relative angles.
   rel_frame = &ref_frame;

   // Subscribe to the frame to ensure that relative state can be computed.
   rel_frame->subscribe();
}


/**
 * Compute the Euler angles.
 *
 * \par Assumptions and Limitations
 *  - Depends upon the Trick Euler angle math macros and routines.
 */
void
EulerDerivedState::update (
   void)
{
   double T_this_parent[3][3];

   // Invoke the parent class update method.
   DerivedState::update(); // This really doesn't do anything!

   // Check to see if Euler angles are relative to another frame.
   if ((rel_frame == NULL) ||
       (subject->composite_body.get_parent() == rel_frame)) {

      // Copy the body's state information for convenience.
      rel_state.copy (subject->composite_body.state);

   }
   else {

      // Compute the relative state from the specified relative frame and expressed in
      // the relative frame
      subject->composite_body.compute_relative_state (*rel_frame, rel_state);

   }

   // Compute the Euler angles from the parent frame to the body.
   Orientation::compute_euler_angles_from_matrix (
      rel_state.rot.T_parent_this, sequence, ref_body_angles);

   // Compute the Euler angles from the body to the parent frame.
   Matrix3x3::transpose (rel_state.rot.T_parent_this, T_this_parent);
   Orientation::compute_euler_angles_from_matrix (
      T_this_parent, sequence, body_ref_angles);
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
