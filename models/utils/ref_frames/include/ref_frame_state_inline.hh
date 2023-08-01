//=============================================================================
// Notices:
//
// Copyright © 2023 United States Government as represented by the Administrator
// of the National Aeronautics and Space Administration.  All Rights Reserved.
//
//
// Disclaimers:
//
// No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF
// ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED
// TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO SPECIFICATIONS, ANY
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
// FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL BE ERROR
// FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO THE
// SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN
// ENDORSEMENT BY GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS,
// RESULTING DESIGNS, HARDWARE, SOFTWARE PRODUCTS OR ANY OTHER APPLICATIONS
// RESULTING FROM USE OF THE SUBJECT SOFTWARE.  FURTHER, GOVERNMENT AGENCY
// DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING THIRD-PARTY SOFTWARE,
// IF PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT "AS IS."
//
// Waiver and Indemnity:  RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS AGAINST THE
// UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
// PRIOR RECIPIENT.  IF RECIPIENT'S USE OF THE SUBJECT SOFTWARE RESULTS IN ANY
// LIABILITIES, DEMANDS, DAMAGES, EXPENSES OR LOSSES ARISING FROM SUCH USE,
// INCLUDING ANY DAMAGES FROM PRODUCTS BASED ON, OR RESULTING FROM, RECIPIENT'S
// USE OF THE SUBJECT SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE
// UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
// PRIOR RECIPIENT, TO THE EXTENT PERMITTED BY LAW.  RECIPIENT'S SOLE REMEDY FOR
// ANY SUCH MATTER SHALL BE THE IMMEDIATE, UNILATERAL TERMINATION OF THIS
// AGREEMENT.
//
//=============================================================================
//
//
/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup RefFrames
 * @{
 *
 * @file models/utils/ref_frames/include/ref_frame_state_inline.hh
 * Define inline methods for the RefFrameState class and its component.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

 

*******************************************************************************/


#ifndef JEOD_REF_FRAME_STATE_INLINE_HH
#define JEOD_REF_FRAME_STATE_INLINE_HH


// JEOD includes
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"

// Model includes
#include "ref_frame_state.hh"


//! Namespace jeod
namespace jeod {

/**
 * Default constructor; initializes state to a null translation.
 */
inline
RefFrameTrans::RefFrameTrans (
   void)
{
   initialize ();
}


/**
 * Copy constructor; initializes state to that of the source.
 * \param[in] source Source state
 */
inline
RefFrameTrans::RefFrameTrans (
   const RefFrameTrans & source)
{
   copy (source);
}


/**
 * Destructor; does nothing.
 */
inline
RefFrameTrans::~RefFrameTrans (
   void)
{
   ;
}


/**
 * Initialize a RefFrameTrans to a null offset.
 */
inline void
RefFrameTrans::initialize (
   void)
{
   Vector3::initialize (position);
   Vector3::initialize (velocity);
}


/**
 * Initialize a RefFrameTrans from a source state.
 * \param[in] source Source state
 */
inline void
RefFrameTrans::copy (
   const RefFrameTrans & source)
{
   Vector3::copy (source.position, position);
   Vector3::copy (source.velocity, velocity);
}



/**
 * Default constructor; initializes state to a null rotation.
 */
inline
RefFrameRot::RefFrameRot (
   void)
{
   initialize ();
}


/**
 * Copy constructor; initializes state to that of the source.
 * \param[in] source Source state
 */
inline
RefFrameRot::RefFrameRot (
   const RefFrameRot & source)
{
   copy (source);
}


/**
 * Destructor; does nothing.
 */
inline
RefFrameRot::~RefFrameRot (
   void)
{
   ;
}


/**
 * Initialize a RefFrameRot to a null offset.
 */
inline void
RefFrameRot::initialize (
   void)
{
   Q_parent_this.make_identity();
   Matrix3x3::identity (T_parent_this);
   Vector3::initialize (ang_vel_this);
   Vector3::initialize (ang_vel_unit);
   ang_vel_mag = 0.0;
}


/**
 * Initialize a RefFrameRot from a source state.
 * \param[in] source Source state
 */
inline void
RefFrameRot::copy (
   const RefFrameRot & source)
{
   Q_parent_this = source.Q_parent_this;
   Matrix3x3::copy (source.T_parent_this, T_parent_this);
   Vector3::copy (source.ang_vel_this, ang_vel_this);
   Vector3::copy (source.ang_vel_unit, ang_vel_unit);
   ang_vel_mag = source.ang_vel_mag;
}


/**
 * Compute the transformation matrix from the left quaternion.
 */
inline void
RefFrameRot::compute_transformation (
   void)
{
   Q_parent_this.left_quat_to_transformation (T_parent_this);
}


/**
 * Compute the left quaternion from the transformation matrix.
 */
inline void
RefFrameRot::compute_quaternion (
   void)
{
   Q_parent_this.left_quat_from_transformation (T_parent_this);
}


/**
 * Compute the angular velocity unit vector.
 *
 * \par Assumptions and Limitations
 *  - Angular velocity magnitude has already been computed.
 */
inline void
RefFrameRot::compute_ang_vel_unit (
   void)
{
   if (std::fpclassify(ang_vel_mag) != FP_ZERO) {
      Vector3::scale (ang_vel_this, 1.0 / ang_vel_mag, ang_vel_unit);
   }
   else {
      Vector3::initialize (ang_vel_unit);
   }
}


/**
 * Compute the angular velocity magnitude and unit vector.
 */
inline void
RefFrameRot::compute_ang_vel_products (
   void)
{
   ang_vel_mag = Vector3::vmag (ang_vel_this);
   compute_ang_vel_unit();
}



/**
 * Destructor; does nothing.
 */
inline
RefFrameState::~RefFrameState (
   void)
{
   ;
}


/**
 * Initialize a RefFrameState to a null offset.
 */
inline void
RefFrameState::initialize (
   void)
{
   trans.initialize();
   rot.initialize();
}


/**
 * Initialize a RefFrameState from a source state.
 * \param[in] source Source state
 */
inline void
RefFrameState::copy (
   const RefFrameState & source)
{
   trans.copy (source.trans);
   rot.copy (source.rot);
}


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
