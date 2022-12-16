/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup RefFrames
 * @{
 *
 * @file models/utils/ref_frames/src/ref_frame.cc
 * Define basic methods for the RefFrame class.
 */

/*******************************************************************************
  Purpose:
    ()

  Library dependency:
    ((ref_frame.o)
     (ref_frame_compute_relative_state.o)
     (ref_frame_items.o)
     (ref_frame_manager.o)
     (ref_frame_messages.o)
     (ref_frame_set_name.o)
     (ref_frame_state.o)
     (subscription.o))

   
*******************************************************************************/


// System includes
#include <cstddef>

// JEOD includes
#include "utils/memory/include/jeod_alloc.hh"

// Model includes
#include "../include/ref_frame.hh"
#include "../include/ref_frame_interface.hh"
#include "../include/tree_links_iterator.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a RefFrame object.
 */
RefFrame::RefFrame (
   void)
:
   name(),
   owner(NULL),
   links(*this),
   update_time(0.0)
{
   set_subscription_mode (Subscription::Subscribe);
}


/**
 * Destroy a RefFrame object.
 */
RefFrame::~RefFrame ()
{
   // Keep removing children until there are no more.
   while (links.has_children()) {
      links.child_tail()->detach();
   }

   // Sever the links from the parent and sibling nodes as well.
   remove_from_parent ();
}


/**
 * Augment Subscription::set_active_status by telling the frame
 * owner that the active/inactive state of this frame has changed.
 * \param[in] value New active value
 */
void
RefFrame::set_active_status (
   bool value)
{

   // Set the status.
   Subscription::set_active_status (value);

   // Report the change to the owner.
   if (owner != NULL) {
      owner->note_frame_status_change (this);
   }
}


/**
 * Move a node to a different place in the tree, keeping the
 * state with respect to the root frame constant.
 * \param[in] new_parent New parent frame
 */
void
RefFrame::transplant_node (
   RefFrame & new_parent)
{
   RefFrameState new_state;

   // Compute the new state.
   compute_relative_state (new_parent, new_state);

   // Transplant the node.
   links.reattach (new_parent.links);

   // Reset the state.
   state = new_state;
}


/**
 * Reparent a node, without updating state.
 * \param[in] new_parent New parent frame
 */
void
RefFrame::reset_parent (
   RefFrame & new_parent)
{
   links.reattach (new_parent.links);
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
