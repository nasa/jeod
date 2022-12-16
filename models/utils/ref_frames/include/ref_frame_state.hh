//=============================================================================
// Notices:
//
// Copyright © 2022 United States Government as represented by the Administrator
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
 * @file models/utils/ref_frames/include/ref_frame_state.hh
 * JEOD 2.0 reference frame tree class definitions
 */

/********************************* TRICK HEADER *******************************

Purpose:
  ()

Library dependencies:
  ((ref_frame_state.o))

 
******************************************************************************/


#ifndef JEOD_KERNEL_REF_FRAME_STATE_HH
#define JEOD_KERNEL_REF_FRAME_STATE_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/quaternion/include/quat.hh"

// Model includes
#include "class_declarations.hh"


//! Namespace jeod
namespace jeod {

/**
 * Represent the translational aspects of a reference frame's state.
 */
class RefFrameTrans {

   JEOD_MAKE_SIM_INTERFACES(RefFrameTrans)


 // Member data
 public:

   /**
    * Position of the subject reference frame origin with respect to the parent
    * frame origin and expressed in parent reference frame coordinates
    */
   double position[3]; //!< trick_units(m)

   /**
    * Velocity of the subject reference frame origin with respect to the parent
    * frame origin and expressed in parent reference frame coordinates
    */
   double velocity[3]; //!< trick_units(m/s)


 // Member functions
 public:
   // Default constructor
   RefFrameTrans (void);

   // Copy constructor
   RefFrameTrans (const RefFrameTrans & source);

   // Assignment operator
   RefFrameTrans & operator = (const RefFrameTrans & source);

   // Destructor.
   ~RefFrameTrans (void);


   // Initialize: Set position, velocity to zero.
   void initialize ();

   // Copy: Copy reference position, velocity.
   void copy (const RefFrameTrans & source);
};


/**
 * Represent the rotational aspects of a reference frame's state.
 */
class RefFrameRot {

   JEOD_MAKE_SIM_INTERFACES(RefFrameRot)


 // Member data
 public:
   /**
    * Left transformation quaternion from the parent reference frame to the
    * subject reference frame
    */
   Quaternion  Q_parent_this; //!< trick_units(--)

   /**
    * Transformation matrix from the parent reference frame to the
    * subject reference frame
    */
   double T_parent_this[3][3]; //!< trick_units(--)

   /**
    * Angular velocity of the subject reference frame with respect to the
    * parent reference frame expressed in subject reference frame coordinates
    */
   double ang_vel_this[3]; //!< trick_units(rad/s)

   /**
    * Magnitude of ang_vel_this
    */
   double ang_vel_mag; //!< trick_units(rad/s)

   /**
    * Unit vector in the direction of ang_vel_this
    */
   double ang_vel_unit[3]; //!< trick_units(--)


 // Member functions
 public:
   // Default constructor
   RefFrameRot (void);

   // Copy constructor
   RefFrameRot (const RefFrameRot & source);

   // Assignment operator
   RefFrameRot & operator = (const RefFrameRot & source);

   // Destructor.
   ~RefFrameRot (void);


   // initialize: Set transformation to identity, rate to zero.
   void initialize ();

   // copy: Copy reference attitude, rate.
   void copy (const RefFrameRot & source);

   // compute_transformation: Compute transformation matrix from quaternion
   void compute_transformation ();

   // compute_quaternion: Compute quaternion from transformation matrix
   void compute_quaternion ();

   // compute_ang_vel_unit: Compute angular velocity unit vector
   void compute_ang_vel_unit ();

   // compute_ang_vel_products: Compute angular velocity magnitude, unit vector
   void compute_ang_vel_products ();
};


/**
 * Represent a reference frame's state.
 */
class RefFrameState {

   JEOD_MAKE_SIM_INTERFACES(RefFrameState)

 // Member data
 public:
   /**
    * Translation state
    */
   RefFrameTrans trans; //!< trick_units(--)

   /**
    * Rotational state
    */
   RefFrameRot rot; //!< trick_units(--)


 // Member functions
 public:

   // Default constructor
   RefFrameState ();

   // Copy constructor
   RefFrameState (const RefFrameState & source);

   // Assignment operator
   RefFrameState & operator = (const RefFrameState & source);

   // Destructor.
   ~RefFrameState (void);


   // initialize: Set transformation to identity, rate to zero.
   void initialize ();

   // copy: Copy reference state.
   void copy (const RefFrameState & source);

   // copy: Copy and negate reference state.
   void negate (const RefFrameState & source);

   // incr_left: 'Add' another frame, left operand
   void incr_left (const RefFrameState & s_ab);

   // incr_right: 'Add' another frame, right operand
   void incr_right (const RefFrameState & s_bc);

   // decr_left: 'Subtract' another frame, left operand
   void decr_left (const RefFrameState & s_ab);

   // decr_right: 'Subtract' another frame, right operand
   void decr_right (const RefFrameState & s_bc);
};


} // End JEOD namespace

#include "ref_frame_state_inline.hh"

#endif

/**
 * @}
 * @}
 * @}
 */
