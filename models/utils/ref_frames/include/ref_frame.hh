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
 * @file models/utils/ref_frames/include/ref_frame.hh
 * Define the class RefFrame.
 */

/********************************* TRICK HEADER *******************************

Purpose:
  ()

Library dependencies:
  ((../src/ref_frame.cc))

 

******************************************************************************/


#ifndef JEOD_REF_FRAME_HH
#define JEOD_REF_FRAME_HH


// Model includes
#include "class_declarations.hh"
#include "subscription.hh"
#include "ref_frame_links.hh"
#include "ref_frame_state.hh"

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// System includes
#include <string>


//! Namespace jeod
namespace jeod {

/**
 * Describe a frame of reference and define operations on reference frames.
 * A JEOD reference frame
 * - Is characterized by an origin and and a set of three orthogonal axes.
 * - Provides a mechanism for specifying the translational and rotational
 *   states of an object in space (particularly, Cartesian three space).
 * - Is itself an object whose translational and rotational states can be
 *   specified/determined in terms of some other reference frame.
 * - Is a node in a rooted tree of reference frames, each of which has some
 *   specific state with respect to another node in the tree.
 * - Can be active (or inactive). An active frame supposedly will have a
 *   (fairly) current state. All bets are off if the frame is inactive.
 * - Can have subscribers, which are external entities that for some reason
 *   need the frame to be active.
 *
 * Reference frames are one of the key concepts that define JEOD 2.0.
 */
class RefFrame : public Subscription {

   JEOD_MAKE_SIM_INTERFACES(RefFrame)

   friend class RefFrameLinks;

 // Member data
 public:
   /**
    * The translational and rotational state of the reference frame
    * with respect to its parent.
    */
   RefFrameState state; //!< trick_units(--)

 protected:

   /**
    * The identifier for this reference frame.
    */
   std::string name; //!< trick_units(--)

   /**
    * The object that "owns" this frame.
    */
   RefFrameOwner * owner; //!< trick_units(--)

   /**
    * Specifies the parent/child/sibling linkages between frames
    */
   RefFrameLinks links; //!< trick_units(--)

   /**
    * The time that the frame was lasted updated, dynamic time seconds
    */
   double update_time; //!< trick_units(s)


 // Member functions

 private:
 // Make the copy constructor and assignment operator private
 // (and unimplemented) to avoid erroneous copies.

   /**
    * Not implemented.
    */
   RefFrame (const RefFrame & frame);

   /**
    * Not implemented.
    */
   RefFrame & operator= (const RefFrame & frame);

 public:

   // Default constructor.
   RefFrame (void);

   // Destructor.
   ~RefFrame (void) override;

   // set_name: Set the frame name (multiple methods provided).
   // The name is formed by conjoining the name items with "."
   void set_name (const char * name_item1);

   void set_name (const char * name_item1, const char * name_item2);

   void set_name (
      const char * name_item1,
      const char * name_item2,
      const char * name_item3);

   void set_name (
      const char * name_item1,
      const char * name_item2,
      const char * name_item3,
      const char * name_item4);

   void set_name (
      const char * name_item1,
      const char * name_item2,
      const char * name_item3,
      const char * name_item4,
      const char * name_item5);

   void set_name (
      const char * name_item1,
      const char * name_item2,
      const char * name_item3,
      const char * name_item4,
      const char * name_item5,
      const char * name_item6);

   void set_name (
      const char * name_item1,
      const char * name_item2,
      const char * name_item3,
      const char * name_item4,
      const char * name_item5,
      const char * name_item6,
      const char * name_item7);

   // get_name: Return the name
   virtual const char * get_name (void) const;


   // set_timestamp: Set the last update time
   virtual void set_timestamp (double time);

   // timestamp: Return the last update time
   virtual double timestamp (void) const;


   // set_owner: Identify who contains/updates this frame
   virtual void set_owner (RefFrameOwner * new_owner);

   // get_owner: Return the owner
   virtual RefFrameOwner * get_owner (void) const;

   // Change active/inactive state with report to the owner
   void set_active_status (bool value) override;


   // get_parent: Return the parent frame
   const RefFrame * get_parent (void) const;

   // get_root: Return the root frame
   const RefFrame * get_root (void) const;

   // make_root: Make this frame a root frame
   virtual void make_root (void);

   // add_child: Make a frame a child of this frame
   virtual void add_child (RefFrame & frame);

   // remove_from_parent: Delete this frame as a child of it's parent frame
   virtual void remove_from_parent (void); //cppcheck-suppress virtualCallInConstructor

   // is_progeny_of: Is the subject frame an ancestor of this frame?
   bool is_progeny_of (const RefFrame & frame) const;

   // transplant_node: Move a node from one place to another in the tree,
   // maintaining state.
   virtual void transplant_node (RefFrame & new_parent);

   // reset_parent: Move a node from one place to another in the tree,
   // state remains unchanged.
   virtual void reset_parent (RefFrame & new_parent);


   // compute_relative_state: Compute the relative state between frames
   virtual void compute_relative_state (
      const RefFrame & wrt_frame, RefFrameState & rel_state) const;

   // compute_relative_state: Compute the relative state between frames
   virtual void compute_relative_state (
      const RefFrame & wrt_frame,
      bool reverse_sense,
      RefFrameState & rel_state) const;

   // compute_state_wrt_pred: Compute the relative state between frames
   virtual void compute_state_wrt_pred (
      const RefFrame & wrt_frame, RefFrameState & rel_state) const;

   // compute_state_wrt_pred: Compute the relative state between frames
   virtual void compute_state_wrt_pred (
      unsigned int wrt_frame_index, RefFrameState & rel_state) const;

   // compute_pred_rel_state: Compute the relative state between frames
   virtual void compute_pred_rel_state (
      const RefFrame & wrt_frame, RefFrameState & rel_state) const;

   // compute_pred_rel_state: Compute the relative state between frames
   virtual void compute_pred_rel_state (
      unsigned int wrt_frame_index, RefFrameState & rel_state) const;

   // compute_position_from: Compute relative position between frames
   virtual void compute_position_from (
      const RefFrame& in_frame, double rel_pos[3]) const;

   // find_last_common_node: Find the point of departure between nodes
   const RefFrame * find_last_common_node (const RefFrame& frame) const;

 protected:

   // find_last_common_index: Find the point of departure between nodes
   int find_last_common_index (const RefFrame& frame) const;

};


} // End JEOD namespace

#include "ref_frame_inline.hh"


#ifdef TRICK_VER
#include "ref_frame_interface.hh"
#endif

#endif

/**
 * @}
 * @}
 * @}
 */
