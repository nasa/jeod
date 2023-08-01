/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup Mass
 * @{
 *
 * @file models/dynamics/mass/src/mass_point.cc
 * Define basic methods for the MassPoint class.
 */

/*******************************************************************************
  Purpose:
    ()

  Reference:
    (((TBS)))

  Assumptions and limitations:
    ((N/A))

  Class:
    (N/A)

  LIBRARY DEPENDENCY:
    ((mass_point.cc)
     (mass_messages.cc)
     (mass_point_state.cc)
     (utils/message/src/message_handler.cc)
     (utils/quaternion/src/quat_norm.cc)
     (utils/quaternion/src/quat_to_mat.cc))



*******************************************************************************/


// Model includes
#include "../include/mass_point.hh"
#include "../include/mass_messages.hh"

// JEOD includes
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/ref_frames/include/tree_links_iterator.hh"

// System includes
#include <cstddef>


//! Namespace jeod
namespace jeod {

/**
 * Destroy a MassPoint object.
 */
MassPoint::~MassPoint ()
{

   // If frame has children, all those children and their progeny contain
   // pointers to this frame. Sever those links by making each child a root.
   while ( links.has_children()) {
      MassPointLinks* child_links = links.child_tail();
      child_links->detach();
   }

   // Sever the links from the parent and sibling nodes as well.
   links.detach ();
}


/**
 * Initialize a mass point.
 */
void
MassPoint::initialize_mass_point ()
{
   detach();
   MassPointState::initialize_mass_point ();
}


/**
 * Compute the state of this point with respect to some reference
 * point, which may be any point in the same tree as this point.
 * \param[in] ref_point The point with respect to which the state is to be expressed
 * \param[out] rel_state The relative state
 */
void
MassPoint::compute_relative_state (
   const MassPoint & ref_point,
   MassPointState & rel_state)
const
{
   int common_node_index;                    /* Index of last node in common
                                                between this point and the
                                                ref_point */
   const MassPoint * common_node_point; /* Last common point */


   // Determine the point of intersection in the paths to the root node
   // between this point and the reference point.
   common_node_index = find_last_common_index (ref_point);
   common_node_point = links.nth_from_root (common_node_index);

   // A negative number indicates a *serious* problem.
   if (common_node_index < 0) {
      MessageHandler::fail (
         __FILE__, __LINE__, MassBodyMessages::invalid_node,
         "Mass points are not in the same tree");

      // Not reached
      return;
   }

   // There are four cases to cover:
   // 1. The trivial case: The ref_point is this point.
   // 2. The ref_point is a direct predecessor of this point.
   // 3. This point is a direct predecessor of the ref_point.
   // 4. General case.

   // Trivial case.
   if (&ref_point == this) {
      rel_state.initialize_mass_point ();
   }

   // Case 2: The ref_point is a direct predecessor of this point.
   else if (common_node_point == &ref_point) {
      compute_state_wrt_pred (common_node_index, rel_state);
   }

   // Case 3: This point is a direct predecessor of the ref_point.
   else if (common_node_point == this) {
      ref_point.compute_pred_rel_state (*this, rel_state);
   }

   // General case: Nodes descend from some common node.
   else {

      // Compute this point's state wrt the common point.
      compute_state_wrt_pred (common_node_index, rel_state);

      // Move the reference down to the ref_point
      for (auto* link : TreeLinksDescentRange<const MassPointLinks>(
              ref_point.links, common_node_index+1)) {
         rel_state.decr_left (link->container());
      }

   }
}


/**
 * Compute the state of this point with respect to some reference
 * point, which must be at or above this point via the parent links.
 * \param[in] ref_point Reference point
 * \param[out] rel_state Point state wrt ref. pt.
 */
void
MassPoint::compute_state_wrt_pred (
   const MassPoint & ref_point,
   MassPointState & rel_state)
const
{
   unsigned ref_point_idx = links.find_path_index(ref_point.links);

   // Sanity check: Reaching the end means the supplied frame is not
   // a predecessor.
   if (ref_point_idx >= links.path_length()) {
      MessageHandler::fail (
         __FILE__, __LINE__,
         MassBodyMessages::invalid_node,
         "Mass point does not descend from supplied reference point");

      // Not reached
      return;
   }

   compute_state_wrt_pred (ref_point_idx, rel_state);
}


/**
 * Compute the state of this point with respect to some reference
 * point, which must be at or above this point via the parent links.
 * \param[in] ref_point_index Reference point index
 * \param[out] rel_state Point state wrt ref. pt.
 */
void
MassPoint::compute_state_wrt_pred (
   unsigned int ref_point_index,
   MassPointState & rel_state)
const
{

   // The state of a point with respect to itself is the trivial state.
   if (ref_point_index == links.path_length()-1) {
      rel_state.initialize_mass_point ();
   }

   // Child body: Walk up the point tree, accumulating offsets and transforms.
   else {
      double r_temp[3];               // M   Transformed point location

      // Copy the current state to the relative state.
      //   r_B->C:B = vector from frame B origin to frame C origin
      //   T_B:C    = transform from frame B to frame C
      // Frame B = parent, frame C = point frame
      Vector3::copy (position, rel_state.position);
      Matrix3x3::copy (T_parent_this, rel_state.T_parent_this);
      rel_state.Q_parent_this = Q_parent_this;

      // Walk up the tree, accumulating offsets and transforms.
      for (auto* link : TreeLinksAscendRange<const MassPointLinks>(
              links, links.path_length()-1, ref_point_index+1)) {
         const MassPoint& node = link->container();

         // r_A->C:A = r_A->B:A + T_A:B^T * r_B:C:B
         Vector3::transform_transpose (
            node.T_parent_this, rel_state.position, r_temp);
         Vector3::sum (node.position, r_temp, rel_state.position);

         // T_A:C = T_B:C * T_A:B
         // Q_A:C = Q_B:C * Q_A:B
         rel_state.Q_parent_this.multiply (node.Q_parent_this);
         rel_state.Q_parent_this.normalize ();
         rel_state.Q_parent_this.left_quat_to_transformation (
            rel_state.T_parent_this);
      }
   }
}


/**
 * Compute the complete state of the invoking reference
 * frame with respect to the supplied reference frame, which *must*
 * be a predecessor of the invoking frame.
 *
 * \par Assumptions and Limitations
 *  - The predecessor frame is a predecessor.
 * \param[in] ref_point The point with respect to which the state is to be expressed
 * \param[out] rel_state The relative state
 */
void
MassPoint::compute_pred_rel_state (
   const MassPoint & ref_point,
   MassPointState & rel_state)
const
{
   unsigned ref_point_idx = links.find_path_index(ref_point.links);

   // Sanity check: Reaching the end means the supplied frame is not
   // a predecessor.
   if (ref_point_idx >= links.path_length()) {
      MessageHandler::fail (
         __FILE__, __LINE__,
         MassBodyMessages::invalid_node,
         "Mass point does not descend from supplied reference point");

      // Not reached
      return;
   }

   compute_pred_rel_state (ref_point_idx, rel_state);
}


/**
 * Compute the state of some reference point with respect to this
 * point. The reference point must be at or above this point via the
 * parent links.
 * \param[in] ref_point_index Reference point index
 * \param[out] rel_state Relative state
 */
void
MassPoint::compute_pred_rel_state (
   unsigned int ref_point_index,
   MassPointState & rel_state)
const
{

   // The state of a root point wrt itself is the trivial state.
   if (ref_point_index == links.path_length()-1) {
      rel_state.initialize_mass_point ();
   }

   // Child body: Walk up the point tree, accumulating offsets and transforms.
   else {

      // Reverse the current state to form the parent state wrt the point.
      //   T_C:B    = T_B:C^T
      //   r_C->B:C = - r_B->C:C = - T_B:C * r_B->C:B
      Vector3::transform (T_parent_this, position, rel_state.position);
      Vector3::negate (rel_state.position);
      Matrix3x3::transpose (T_parent_this, rel_state.T_parent_this);
      Q_parent_this.conjugate (rel_state.Q_parent_this);

      // Walk up the tree, accumulating offsets and transforms.
      for (auto* link : TreeLinksAscendRange<const MassPointLinks>(
              links, links.path_length()-1, ref_point_index+1)) {
         const MassPoint& node = link->container();

         // T_C:A = T_B:A * T_C:B = T_A:B^T * T_C:B
         // Q_C:A = Q_B:A * Q_C:B = Q_A:B^T * Q_C:B
         rel_state.Q_parent_this.multiply_left_conjugate (node.Q_parent_this);
         rel_state.Q_parent_this.normalize ();
         rel_state.Q_parent_this.left_quat_to_transformation (
            rel_state.T_parent_this);

         // r_C->A:C = r_C->B:C + r_B->A:C
         //   = r_C->B:C - r_A->B:C
         //   = r_C->B:C - T_A:C r_A->B:A
         Vector3::transform_transpose_decr (
            rel_state.T_parent_this, node.position, rel_state.position);
      }
   }
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
