/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup RefFrames
 * @{
 *
 * @file models/utils/ref_frames/src/ref_frame_state.cc
 * Define methods for the RefFrameState class.
 */

/*******************************************************************************
  Purpose:
    ()

  Library dependencies:
    ((ref_frame_state.o)
     (utils/quaternion/quat.o)
     (utils/quaternion/quat_norm.o)
     (utils/quaternion/quat_to_mat.o))

   
*******************************************************************************/


// System includes
#include <cmath>

// JEOD includes
#include "utils/math/include/vector3.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/numerical.hh"

// Model includes
#include "../include/ref_frame_state.hh"


//! Namespace jeod
namespace jeod {

/*******************************************************************************

  Nomenclature of mathematical formulations.
    x,v,Q,T,w = position, velocity, quaternion, transform, rate, respectively.
    A,B,C     = various reference frames.


  Long-hand notations.
    x_A->B:C  = Position of frame B origin with respect to frame A origin,
                expressed in frame A coordinates.
    v_A->B:C  = Velocity of frame B origin with respect to frame A origin,
                expressed in frame C (observer is fixed wrt frame C).
    w_A->B:C  = Angular velocity of frame B axes wrt frame A axes,
                expressed in frame C (observer is fixed wrt frame C).
    T_A->B    = Column vector transformation matrix from frame A to frame B.


  Short-hand notations.
    x_A:B     = x_A->B:A (Location of frame B origin in frame A coordinates.)
    v_A:B     = v_A->B:A
    T_A:B     = T_A->B
    w_A:B     = w_A->B:B (Note that w_A:B is expressed in frame B coordinates.)
    S_A:B     = {x_A:B, V_A:B, T_A:B, w_A:B}
                In other words, the state of frame with respect to frame B.


  Operators.
  Conceptually, there are three primitive operators on reference frame states:
  assignment (=), unary negation (-), and addition (+).  Note that addition
  is nonabelian. Subtraction is in terms of addition and negation.

  Negation: S_B:A = - S_A:B
    T_B:A =   T_A:B^T
    w_B:A = - (T_B:A * w_A:B)
    x_B:A = - (T_A:B * x_A:B)
    v_B:A = -(T_A:B * v_A:B + w_A:B X x_B:A)

  Addition: S_A:C = S_A:B + S_B:C
    T_A:C = T_B:C * T_A:B
    w_A:C = T_B:C * w_A:B + w_B:C
    x_A:C = x_A:B + T_A:B^T * x_B:C
    v_A:C = v_A:B + T_A:B^T * (v_B:C + w_A:B X x_B:C)


  Methods.
    Method       This(input)  Operand  Result
    negate       N/A          S_A:B    this <- -S_A:B = S_B:A
    incr_left    S_B:C        S_A:B    this <- arg + this = S_A:C
    incr_right   S_A:B        S_B:C    this <- this + arg = S_A:C
    decr_left    S_A:C        S_A:B    this <- (-arg) + this = S_B:C
    decr_right   S_A:C        S_B:C    this <- this + (-arg) = S_A:B


  Shortcuts.
  The methods defined below two shortcuts to bypass various calculations.

  - Identity transformation.
    There is no need to transform a vector if the transformation is an identity.
    The scalar part of the left transformation quaternion is exactly one in the
    case of an identity transformation.

  - Null rotation.
    This is no need to calculate wXr and the like if the angular velocity is
    identically zero. The magnitude of the angular velocity is used to detect
    whether this is the case.


*******************************************************************************/



/**
 * Assignment operator; copies state from the source.
 * @return Pointer to this
 * \param[in] source Source state
 */



/*******************************************************************************
  Function: RefFrameTrans::operator =
  Purpose: (Assignment operator; copies state from the source.)
*******************************************************************************/
RefFrameTrans &
RefFrameTrans::operator = (
   const RefFrameTrans & source)
{
   if (this != &source) {
      copy (source);
   }
   return *this;
}


/**
 * Assignment operator; copies state from the source.
 * @return Pointer to this
 * \param[in] source Source state
 */
RefFrameRot &
RefFrameRot::operator = (
   const RefFrameRot & source)
{
   if (this != &source) {
      copy (source);
   }
   return *this;
}



/**
 * RefFrameState default constructor.
 */
RefFrameState::RefFrameState (
   void)
{
   initialize ();
}


/**
 * RefFrameState copy constructor.
 * \param[in] source Source state
 */
RefFrameState::RefFrameState (
   const RefFrameState & source)
{
   copy (source);
}


/**
 * Assignment operator; copies state from the source.
 * @return Pointer to this
 * \param[in] source Source state
 */
RefFrameState &
RefFrameState::operator = (
   const RefFrameState & source)
{
   if (this != &source) {
      copy (source);
   }
   return *this;
}


/**
 * Copy a reference frame state, negated.
 * \param[in] source Source state
 */
void
RefFrameState::negate (
   const RefFrameState & source)
{
   // Relevant equations:
   //   T_B:A =   T_A:B^T
   //   w_B:A = - (T_B:A * w_A:B)
   //   x_B:A = - (T_A:B * x_A:B)
   //   v_B:A = -(T_A:B * v_A:B + w_A:B X x_B:A)

   // Transform the matrix and translational state to the alternate frame.
   // This only needs to be performed for nontrivial transformation matrices.
   if (!Numerical::compare_exact(source.rot.Q_parent_this.scalar,1.0)) {
      source.rot.Q_parent_this.conjugate (rot.Q_parent_this);
      Matrix3x3::transpose (source.rot.T_parent_this, rot.T_parent_this);
      Vector3::transform_transpose (rot.T_parent_this, source.trans.position,
                                    trans.position);
      Vector3::transform_transpose (rot.T_parent_this, source.trans.velocity,
                                    trans.velocity);
   }

   // Identity transform: Just copy.
   else {
      rot.initialize();
      Vector3::copy (source.trans.position, trans.position);
      Vector3::copy (source.trans.velocity, trans.velocity);
   }

   // Compute the wXr velocity component and transform the angular velocity.
   // This only needs to be performed for non-zero angular velocities.
   if (std::fpclassify(source.rot.ang_vel_mag) != FP_ZERO) {

       Vector3::cross_decr (source.rot.ang_vel_this, trans.position,
                           trans.velocity);

      Vector3::transform (rot.T_parent_this, source.rot.ang_vel_this,
                          rot.ang_vel_this);
   }

   // Zero rate: Set the new rate to zero (the unit vector is ill-defined).
   else {
      rot.ang_vel_mag = 0.0;
      Vector3::initialize (rot.ang_vel_unit);
      Vector3::initialize (rot.ang_vel_this);
   }

   // The above made the vectors have the wrong sign. Fix that.
   Vector3::negate (trans.position);
   Vector3::negate (trans.velocity);
   Vector3::negate (rot.ang_vel_this);

   return;
}


/**
 * Compute S_A:C = S_A:B + S_B:C,
 * with this initially containing S_B:C,
 * the supplied argument containing S_A:B, and
 * the resultant composition of states stored in this.
 * \param[in] s_ab Left addend
 */
void
RefFrameState::incr_left (
   const RefFrameState & s_ab)
{
   // Relevant equations:
   //   T_A:C = T_B:C * T_A:B
   //   w_A:C = T_B:C * w_A:B + w_B:C
   //   x_A:C = x_A:B + T_A:B^T * x_B:C
   //   v_A:C = v_A:B + T_A:B^T * (v_B:C + w_A:B X x_B:C)

   // Order dependency:
   //   Quantity   Uses S_B:C items  Uses S_A:C items   Order
   //   T_A:C      T                 None               2
   //   w_A:C      w, T              None               1
   //   x_A:C      x                 None               4
   //   v_A:C      v, x              None               3


   // Compute the angular velocity of frame C wrt frame A in frame C:
   //   w_A:C = T_B:C * w_A:B + w_B:C
   // Shortcut: Don't bother if s_ab is not rotating wrt its parent.
   if (std::fpclassify(s_ab.rot.ang_vel_mag) != FP_ZERO) {

      // Compute as this <- this + T_B:C * w_A:B
      Vector3::transform_incr (rot.T_parent_this, s_ab.rot.ang_vel_this,
                               rot.ang_vel_this);

      // Compute the angular velocity magnitude and unit vector.
      rot.compute_ang_vel_products ();
   }

   // Compute T_A:C, v_A:C, and x_A:C via
   //   T_A:C = T_B:C * T_A:B
   //   x_A:C = x_A:B + T_A:B^T * x_B:C
   //   v_A:C = v_A:B + T_A:B^T * (v_B:C + w_A:B X x_B:C)
   // Shortcut: only do the transformations if T_A:B is not the identity matrix.
   if (!Numerical::compare_exact(s_ab.rot.Q_parent_this.scalar,1.0)) {

      // Compute Q_A:C = Q_B:C * Q_A:B (and then normalize result)
      rot.Q_parent_this.multiply (s_ab.rot.Q_parent_this);
      rot.Q_parent_this.normalize ();

      // Compute the corresponding transformation matrix.
      rot.compute_transformation ();

      // Compute v_B->C:A = T_A:B^T * (v_B:C + w_a:B X x_B:C)
      // Note: This must be performed prior to computing the position
      // because it uses x_B:C = trans.position.
      if (std::fpclassify(s_ab.rot.ang_vel_mag) != FP_ZERO) {
         Vector3::cross_incr (s_ab.rot.ang_vel_this, trans.position,
                              trans.velocity);
      }

      // Transform x_B:C to frame A.
      Vector3::transform_transpose (s_ab.rot.T_parent_this,
                                    trans.position);

      Vector3::transform_transpose (s_ab.rot.T_parent_this,
                                    trans.velocity);
   }

   // T_A:B is identity. The only quantity needed is w_A:B X x_B:C.
   else {
      if (std::fpclassify(s_ab.rot.ang_vel_mag) != FP_ZERO) {
         Vector3::cross_incr (s_ab.rot.ang_vel_this, trans.position,
                              trans.velocity);
      }
   }

   // Add the X_A:B, v_A:B to the state.
   Vector3::incr (s_ab.trans.velocity, trans.velocity);
   Vector3::incr (s_ab.trans.position, trans.position);

   return;
}


/**
 * Compute S_A:C = S_A:B + S_B:C,
 * with this initially containing S_A:B,
 * the supplied argument containing S_B:C, and
 * the resultant composition of states stored in this.
 * Note that this function is untested, as it is not
 * used in the Reference Frame Model at any point, and
 * is only given here as a utility function.
 * \param[in] s_bc Right addend
 */
void
RefFrameState::incr_right (
   const RefFrameState & s_bc)
{
   // Relevant equations:
   //   T_A:C = T_B:C * T_A:B
   //   w_A:C = T_B:C * w_A:B + w_B:C
   //   x_A:C = x_A:B + T_A:B^T * x_B:C
   //   v_A:C = v_A:B + T_A:B^T * (v_B:C + w_A:B X x_B:C)

   // Order dependency:
   //   Quantity   Uses S_A:B items  Uses S_A:C items   Order
   //   T_A:C      T                 None               5
   //   w_A:C      w                 None               2
   //   x_A:C      x, T              None               3
   //   v_A:C      v, T, w           None               1,4


   double v_bc_in_b_obs_a[3];   // M/s  Vel. of C wrt B in B, observed from A


   // The velocity of frame C origin wrt frame B as observed in frame A
   // includes the velocity itself plus and omega cross r term.
   // The velocity term does not depend on the frame rate: copy it.
   Vector3::copy (s_bc.trans.velocity, v_bc_in_b_obs_a);

   // Compute
   // 1. The velocity of frame C wrt frame B observed from frame A
   //      v_B->C:B(A) = v_B:C + w_A:B X x_B:C
   // 2. The angular velocity of frame C wrt frame A in frame C:
   //      w_A:C = T_B:C * w_A:B + w_B:C
   // Shortcut: Don't bother if frame B is not rotating wrt frame A.
   if (std::fpclassify(rot.ang_vel_mag) != FP_ZERO) {

      // Add the omega cross r term to the observed velocity.
      Vector3::cross_incr (rot.ang_vel_this, s_bc.trans.position,
                           v_bc_in_b_obs_a);

      // Compute w_A:C as this = T_B:C * this + w_B:C
      Vector3::transform (s_bc.rot.T_parent_this, rot.ang_vel_this);
      Vector3::incr (s_bc.rot.ang_vel_this, rot.ang_vel_this);

      // Compute the angular velocity magnitude and unit vector.
      rot.compute_ang_vel_products ();
   }

   // Compute x_A:C, v_A:C, and T_A:C via
   //   T_A:C = T_B:C * T_A:B
   //   x_A:C = x_A:B + T_A:B^T * x_B:C
   //   v_A:C = v_A:B + T_A:B^T * (v_B:C + w_A:B X x_B:C)
   // Shortcut: only do the transformations if T_A:B is not the identity matrix.
   if (!Numerical::compare_exact(rot.Q_parent_this.scalar,1.0)) {

      // Compute x_A:C and v_A:C.
      // Note: These must be computed before computing T_A:C as they use T_A:B.
      Vector3::transform_transpose_incr (rot.T_parent_this, s_bc.trans.position,
                                         trans.position);
      Vector3::transform_transpose_incr (rot.T_parent_this, v_bc_in_b_obs_a,
                                         trans.velocity);

      // Compute Q_A:C = Q_B:C * Q_A:B (and then normalize result)
      rot.Q_parent_this.multiply_left (s_bc.rot.Q_parent_this);
      rot.Q_parent_this.normalize ();

      // Compute the corresponding transformation matrix.
      rot.compute_transformation ();
   }

   // Shortcuts for the case T_A:B is identity.
   else {
      Vector3::incr (s_bc.trans.position, trans.position);
      Vector3::incr (s_bc.trans.velocity, trans.velocity);
      rot.copy (s_bc.rot);
   }

   return;
}


/**
 * Compute S_B:C = (-S_A:B) + S_A:C,
 * with this initially containing S_A:C,
 * the supplied argument containing S_A:B, and
 * the resultant composition of states stored in this.
 * \param[in] s_ab Left addend
 */
void
RefFrameState::decr_left (
   const RefFrameState & s_ab)
{
   // Relevant equations:
   //   T_B:C = T_A:C * T_A:B^T
   //   w_B:C = w_A:C - T_B:C * w_A:B
   //   x_B:C = T_A:B * (x_A:C - x_A:B)
   //   v_B:C = T_A:B * (v_A:C - v_A:B) - w_A:B X x_B:C

   // Order dependency:
   //   Quantity   Uses S_A:C items  Uses S_B:C items   Order
   //   T_B:C      T                 None               1
   //   w_B:C      w                 T                  2
   //   x_B:C      x                 None               3
   //   v_B:C      v                 x                  4


   // Compute T_B:C, v_B:C, and x_B:C via
   //   T_B:C = T_A:C * T_A:B^T
   //   x_B:C = T_A:B * (x_A:C - x_A:B)
   //   v_B:C = T_A:B * (v_A:C - v_A:B) - w_A:B X x_B:C
   // Shortcut: only do the transformations if T_A:B is not the identity matrix.
   if (!Numerical::compare_exact(s_ab.rot.Q_parent_this.scalar,1.0)) {

      // Compute Q_B:C = Q_A:C * Q_A:B^Q
      rot.Q_parent_this.multiply_conjugate (s_ab.rot.Q_parent_this);
      rot.Q_parent_this.normalize ();

      // Compute the corresponding transformation matrix.
      rot.compute_transformation ();

      // Compute x_B:C and v_B:C.
      // Note that x_B:C must be computed before v_B:C as V_B:C depends on
      // the former.
      Vector3::decr (s_ab.trans.position, trans.position);
      Vector3::transform (s_ab.rot.T_parent_this, trans.position);

      Vector3::decr (s_ab.trans.velocity, trans.velocity);
      Vector3::transform (s_ab.rot.T_parent_this, trans.velocity);
      Vector3::cross_decr (s_ab.rot.ang_vel_this, trans.position,
                           trans.velocity);
   }

   // Shortcuts for the case T_A:B is identity.
   else {
      Vector3::decr (s_ab.trans.position, trans.position);
      Vector3::decr (s_ab.trans.velocity, trans.velocity);
      Vector3::cross_decr (s_ab.rot.ang_vel_this, trans.position,
                           trans.velocity);
   }

   // Compute w_B:C = w_A:C - T_B:C * w_A:B and related products.
   if (std::fpclassify(s_ab.rot.ang_vel_mag) != FP_ZERO) {
      Vector3::transform_decr (rot.T_parent_this, s_ab.rot.ang_vel_this,
                               rot.ang_vel_this);
      rot.compute_ang_vel_products ();
   }

   return;
}


/**
 * Compute S_A:B = S_A:C + (-S_B:C)
 * with this initially containing S_A:C,
 * the supplied argument containing S_B:C, and
 * the resultant composition of states stored in this.
 * \param[in] s_bc Left addend
 */
void
RefFrameState::decr_right (
   const RefFrameState & s_bc)
{
   // Relevant equations:
   //   T_A:B = T_B:C^T * T_A:C
   //   w_A:B = T_B:C^T * (w_A:C - w_B:C)
   //   x_A:B = x_A:C - T_A:B^T * x_B:C
   //   v_A:B = v_A:C - T_A:B^T * (v_B:C + w_A:B X x_B:C)

   // Order dependency:
   //   Quantity   Uses S_A:C items  Uses S_A:B items   Order
   //   T_A:B      T                 None               1
   //   w_A:B      w                 None               2
   //   x_A:B      x                 T                  3
   //   v_A:B      v                 T, w               4


   double v_bc_in_b_obs_a[3];   // M/s  Vel. of C wrt B in B, observed from A


   // Compute T_A:B, w_A:B, via
   //   T_A:B = T_B:C^T * T_A:C
   //   w_A:B = T_B:C^T * (w_A:C - w_B:C)
   // Shortcut: only do the transformations if T_B:C is not the identity matrix.
   if (!Numerical::compare_exact(s_bc.rot.Q_parent_this.scalar,1.0)) {
      rot.Q_parent_this.multiply_left_conjugate (s_bc.rot.Q_parent_this);
      rot.Q_parent_this.normalize ();
      rot.compute_transformation ();

      Vector3::decr (s_bc.rot.ang_vel_this, rot.ang_vel_this);
      Vector3::transform_transpose (s_bc.rot.T_parent_this, rot.ang_vel_this);
      rot.compute_ang_vel_products ();
   }

   // Shortcuts for the case T_B:C is identity.
   else if (std::fpclassify(s_bc.rot.ang_vel_mag) != FP_ZERO) {
      Vector3::decr (s_bc.rot.ang_vel_this, rot.ang_vel_this);
      rot.compute_ang_vel_products ();
   }


   // Compute the velocity of frame C wrt frame B observed from frame A
   //   v_B->C:B(A) = v_B:C + w_A:B X x_B:C
   Vector3::copy (s_bc.trans.velocity, v_bc_in_b_obs_a);
   if (std::fpclassify(rot.ang_vel_mag) != FP_ZERO) {
      Vector3::cross_incr (rot.ang_vel_this, s_bc.trans.position,
                           v_bc_in_b_obs_a);
   }

   // Compute x_A:B, V_A:B via
   //   x_A:B = x_A:C - T_A:B^T * x_B:C
   //   v_A:B = v_A:C - T_A:B^T * (v_B:C + w_A:B X x_B:C)
   if (!Numerical::compare_exact(rot.Q_parent_this.scalar,1.0)) {
      Vector3::transform_transpose_decr (rot.T_parent_this, s_bc.trans.position,
                                         trans.position);
      Vector3::transform_transpose_decr (rot.T_parent_this, v_bc_in_b_obs_a,
                                         trans.velocity);
   }
   // Shortcuts for the case T_A:B is identity.
   else {
      Vector3::decr (s_bc.trans.position, trans.position);
      Vector3::decr (v_bc_in_b_obs_a, trans.velocity);
   }

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
