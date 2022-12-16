/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup RefFrames
 * @{
 *
 * @file models/utils/ref_frames/src/ref_frame_compute_relative_state.cc
 * Define relative state methods for the RefFrame class.
 */

/*******************************************************************************
  Purpose:
    ()

  Library Dependencies:
    ((ref_frame_compute_relative_state.o)
     (ref_frame_messages.o)
     (ref_frame_state.o)
     (utils/message/message_handler.o))

   
*******************************************************************************/


// System includes
#include <cstddef>

// JEOD includes
#include "utils/math/include/vector3.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/math/include/numerical.hh"

// Model includes
#include "../include/ref_frame.hh"
#include "../include/ref_frame_messages.hh"
#include "../include/ref_frame_state.hh"
#include "../include/tree_links_iterator.hh"



//! Namespace jeod
namespace jeod {

/**
 * Compute the complete state of the invoking reference frame (*this)
 * with respect to the supplied wrt_frame reference frame.
 * The state will include:
 *   - The position and velocity of the invoking frame with respect to the
 *     supplied wrt_frame, expressed in the coordinates of the wrt_frame.
 *   - The angular velocity of the invoking frame with respect to the
 *     supplied wrt_frame, expressed in the coordinates of invoking frame.
 *   - The transformation (as a matrix and a quaternion) from the supplied
 *     wrt_frame to the invoking frame.
 *
 * \par Assumptions and Limitations
 *  - The two frames are in the same tree.
 * \param[in] wrt_frame The frame with respect to which the state is to be expressed
 * \param[out] rel_state The relative state
 */
void
RefFrame::compute_relative_state (
   const RefFrame & wrt_frame,
   RefFrameState & rel_state)
const
{
   int common_node_index;              /* Index of last node in common between
                                          this frame and the wrt_frame */
   const RefFrame * common_node_frame; /* Last common frame */
/*
   printf("%s - position: %f %f %f\n",
      this->get_name(),
      this->state.trans.position[0],
      this->state.trans.position[1],
      this->state.trans.position[2]);
  printf("%s - position: %f %f %f\n",
      wrt_frame.get_name(),
      wrt_frame.state.trans.position[0],
      wrt_frame.state.trans.position[1],
      wrt_frame.state.trans.position[2]);
printf("compute_relative_state\n\n");
*/
   // Find the index of the node below which the path to the two frames diverge.
   common_node_index = find_last_common_index (wrt_frame);

   // A negative number indicates a *serious* problem.
   if (common_node_index < 0) {
      MessageHandler::fail (__FILE__, __LINE__,
                            RefFrameMessages::invalid_node,
                            "Frames '%s' and '%s' are not in the same tree",
                            name.c_str(), wrt_frame.name.c_str());

      // Not reached
      return;
   }

   // Get the frame corresponding to the common node.
   common_node_frame = links.nth_from_root (common_node_index);


   // There are four cases to cover:
   // 1. The trivial case: The wrt_frame is this frame.
   // 2. The wrt_frame is a direct predecessor of this frame.
   // 3. This frame is a direct predecessor of the wrt_frame.
   // 4. General case.

   // Trivial case.
   if (&wrt_frame == this) {
      rel_state.initialize ();

   }

   // Case 2: The wrt_frame is a direct predecessor of this frame.
   else if (common_node_frame == &wrt_frame) {
      compute_state_wrt_pred (common_node_index, rel_state);
   }

   // Case 3: This frame is a direct predecessor of the wrt_frame.
   else if (common_node_frame == this) {
      wrt_frame.compute_pred_rel_state (common_node_index, rel_state);
   }

   // General case: Nodes descend from some common node.
   else {

      // Compute this frame's state wrt the common frame.
      compute_state_wrt_pred (common_node_index, rel_state);

      // Move the relative state down to the wrt_frame
      for (auto* link : TreeLinksDescentRange<const RefFrameLinks>(
              wrt_frame.links, common_node_index+1)) {
         rel_state.decr_left (link->container().state);
      }
   }
}


/**
 * Compute the complete state of the invoking reference frame (*this)
 * with respect to the supplied wrt_frame reference frame.
 * If reverse_sense is false, the results are those from
 * the simpler two argument form of RefFrame::compute_relative_state.
 * If reverse_sense is true, the results from the two
 * argument form are transformed as follows:
 *   - The position and velocity are those the invoking frame with respect to the
 *     supplied wrt_frame, but expressed in invoking frame coordinates.
 *   - The angular velocity of the invoking frame with respect to the
 *     supplied wrt_frame, expressed in the coordinates of supplied wrt_frame.
 *   - The transformation (as a matrix and a quaternion) from the invoking
 *     frame to the supplied wrt_frame.
 *
 * \par Assumptions and Limitations
 *  - The two frames are in the same tree.
 * \param[in] wrt_frame The frame with respect to which the state is to be expressed
 * \param[in] reverse_sense Express position and velocity in this frame,
 *    angular velocity in the wrt_frame, and the transformations from this
 *    frame to the wrt_frame.
 * \param[out] rel_state The relative state
 */
void
RefFrame::compute_relative_state (
   const RefFrame & wrt_frame,
   bool reverse_sense,
   RefFrameState & rel_state)
const
{
   // Compute the relative state as if express_in_wrt_frame is true.
   compute_relative_state (wrt_frame, rel_state);

   // Reverse the sense of the computed state if so indicated.
   if (reverse_sense) {
       // Transform relative pos and vel from the wrt_frame to this frame.
      Vector3::transform (
         rel_state.rot.T_parent_this, rel_state.trans.position);
      Vector3::transform (
         rel_state.rot.T_parent_this, rel_state.trans.velocity);

       // Transform relative angular vel. from this frame to the wrt_frame.
      Vector3::transform_transpose (
         rel_state.rot.T_parent_this, rel_state.rot.ang_vel_this);
      Vector3::transform_transpose (
         rel_state.rot.T_parent_this, rel_state.rot.ang_vel_unit);

       // Transpose the tranformations so they are from this to the wrt frame.
       Matrix3x3::transpose (rel_state.rot.T_parent_this);
       rel_state.rot.Q_parent_this.conjugate ();
   }
}


/**
 * Compute the complete state of the invoking reference
 * frame with respect to the supplied reference frame, which *must*
 * be a predecessor of the invoking frame.
 *
 * \par Assumptions and Limitations
 *  - The predecessor frame is a predecessor.
 * \param[in] pred_frame The frame with respect to which the state is to be expressed
 * \param[out] rel_state The relative state
 */
void
RefFrame::compute_state_wrt_pred (
   const RefFrame & pred_frame,
   RefFrameState & rel_state)
const
{
   unsigned pred_frame_idx = links.find_path_index(pred_frame.links);

   // Sanity check: Reaching the end means the supplied frame is not
   // a predecessor.
   if (pred_frame_idx >= links.path_length()) {
      MessageHandler::fail (__FILE__, __LINE__,
                            RefFrameMessages::invalid_node,
                            "Frame '%s' is not a predecessor of frame '%s'",
                            pred_frame.name.c_str(), name.c_str());

      // Not reached
      return;
   }

   compute_state_wrt_pred (pred_frame_idx, rel_state);
}


/**
 * Compute the complete state of the invoking reference
 * frame with respect to the supplied reference frame, which *must*
 * be a predecessor of the invoking frame.
 *
 * \par Assumptions and Limitations
 *  - The predecessor frame is a predecessor.
 * \param[in] pred_frame_index  The frame with respect to which the state is to be expressed
 * \param[out] rel_state        The relative state
 */
void
RefFrame::compute_state_wrt_pred (
   unsigned int pred_frame_index,
   RefFrameState & rel_state)
const
{
   // Initialize the relative state as that of the state of the invoking frame
   // wrt its immediate parent.
   rel_state.copy (state);

   // Compute the state of this frame with respect to the predecessor node by
   // successively computing the state of this frame with respect to frames in
   // this frame's link path from the bottom up.
   // The loop below starts just above the last entry in the link path (note
   // that rel_state was initialized above to be the state with respect to
   // this frame's immediate parent) and ends one shy of the common node.
   // For each step, the state is re-represented with respect to the link
   // frame's parent frame.
   // Note: This loop will not be executed if the common node is this frame
   // (the relative state is a null transformation) or if the common node
   // is the immediate parent (relative state already has desired contents).
   for (auto* link : TreeLinksAscendRange<const RefFrameLinks>(
           links, links.path_length()-1, pred_frame_index+1)) {

      // Update the relative state by adding (on the left) the state of the
      // current node with respect to it's parent. Conceptually,
      //   rel_state = link_frame->state + rel_state;
      rel_state.incr_left (link->container().state);
   }
}


/**
 * Compute the complete state of the invoking reference
 * frame with respect to the supplied reference frame, which *must*
 * be a predecessor of the invoking frame.
 *
 * \par Assumptions and Limitations
 *  - The predecessor frame is a predecessor.
 * \param[in] pred_frame The frame with respect to which the state is to be expressed
 * \param[out] rel_state The relative state
 */
void
RefFrame::compute_pred_rel_state (
   const RefFrame & pred_frame,
   RefFrameState & rel_state)
const
{
   unsigned pred_frame_idx = links.find_path_index(pred_frame.links);

   // Sanity check: Reaching the end means the supplied frame is not
   // a predecessor.
   if (pred_frame_idx >= links.path_length()) {
      MessageHandler::fail (__FILE__, __LINE__,
                            RefFrameMessages::invalid_node,
                            "Frame '%s' is not a predecessor of frame '%s'",
                            pred_frame.name.c_str(), name.c_str());

      // Not reached
      return;
   }

   compute_pred_rel_state (pred_frame_idx, rel_state);
}


/**
 * Compute the complete state of the supplied reference frame
 * wrt the invoking reference frame. The supplied reference frame
 * must be a predecessor of the invoking frame.
 *
 * \par Assumptions and Limitations
 *  - The predecessor frame is a predecessor.
 * \param[in] pred_frame_index  The frame with respect to which the state is to be expressed
 * \param[out] rel_state        The relative state
 */
void
RefFrame::compute_pred_rel_state (
   unsigned int pred_frame_index,
   RefFrameState & rel_state)
const
{
   // Initialize the relative state as the negative of state of the invoking
   // frame wrt its immediate parent.
   rel_state.negate (state);

   // Compute the state of this frame with respect to the common node by
   // successive computing the state of this frame with respect to frames in
   // this frame's link path from the bottom up.
   // The loop below starts just above the last entry in the link path (note
   // that rel_state was initialized above to be the state with respect to
   // this frame's immediate parent) and ends one shy of the common node.
   // For each step, the state is re-represented with respect to the link
   // frame's parent frame.
   // Note: This loop will not be executed if the common node is this frame
   // (the relative state is a null transformation) or if the common node
   // is the immediate parent (relative state already has desired contents).
   for (auto* link : TreeLinksAscendRange<const RefFrameLinks>(
           links, links.path_length()-1, pred_frame_index+1)) {

      // Update the relative state by adding (on the right) the state of the
      // current node's parent with respect to the current node. This is
      // the equivalent of subtracting (on the left) the state of the current
      // node wrt its parent. Conceptually,
      //   rel_state = rel_state - link_frame->state;
      rel_state.decr_right (link->container().state);
   }
}


/**
 * Compute the relative position vector from the origin of the
 * supplied reference frame to the origin of this reference frame,
 * expressed in the coordinates of the supplied frame.
 * \param[in] in_frame Relative position vector origin
 * \param[out] rel_pos Relative position vector\n Units: M
 */
void
RefFrame::compute_position_from (
   const RefFrame& in_frame,
   double rel_pos[3])
const
{
   int common_node_index;              /* Index of last node in common between
                                          this frame and the wrt_frame */
   const RefFrame * link_frame;        /* Frame in path to node */


   // Find the index of the node below which the path to the two frames diverge.
   common_node_index = find_last_common_index (in_frame);

   // A negative number indicates a *serious* problem.
   if (common_node_index < 0) {
      MessageHandler::fail (__FILE__, __LINE__,
                            RefFrameMessages::invalid_node,
                            "Frames '%s' and '%s' are not in the same tree",
                            name.c_str(), in_frame.name.c_str());

      // Not reached
      return;
   }


   Vector3::initialize (rel_pos);

   // First we step the current position back to the common node
   // The end result will be the position of the invoking frame in the
   // common node frame.
   for (auto* link : TreeLinksAscendRange<const RefFrameLinks>(
           links, links.path_length(), common_node_index+1)) {
      link_frame = &link->container();

      // Transform the current position to the link frame's parent
      if (!Numerical::compare_exact(link_frame->state.rot.Q_parent_this.scalar,1.0)) {
          Vector3::transform_transpose (link_frame->state.rot.T_parent_this,
                                        rel_pos);
      }

      // Add in the position to the link frame's parent.
      Vector3::incr (link_frame->state.trans.position, rel_pos);
   }

   // Next step the current position down to the reference node.
   for (auto* link : TreeLinksDescentRange<const RefFrameLinks>(
           in_frame.links, common_node_index+1)) {
      link_frame = &link->container();

      // Subtract the position to the link frame's parent.
      Vector3::decr (link_frame->state.trans.position, rel_pos);

      // Transform the current position to the link frame
      if (!Numerical::compare_exact(link_frame->state.rot.Q_parent_this.scalar,1.0)) {
          Vector3::transform (link_frame->state.rot.T_parent_this, rel_pos);
      }
   }
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
