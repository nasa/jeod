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
 * @file models/utils/ref_frames/include/tree_links.hh
 * Define the template class TreeLinks, the class that encapsulates the parent/
 * child links between objects.
 */

/*******************************************************************************

Purpose:
 ()

 
*******************************************************************************/


#ifndef JEOD_TREE_LINKS_HH
#define JEOD_TREE_LINKS_HH

// Model includes
#include "class_declarations.hh"

// JEOD includes
#include "utils/container/include/pointer_vector.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/memory/include/jeod_alloc.hh"

// System includes
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <vector>


//! Namespace jeod
namespace jeod {

// Forward declarations.
template <class Links> class TreeLinksAscendRange;
template <class Links> class TreeLinksDescentRange;
template <class Links> class TreeLinksChildrenRange;

/**
 * Encapsulates links (parent, children, siblings) between objects,
 * in the form of a tree.
 * \tparam Links      The class being template-instantiated.
 * \tparam Container  The class that contains a TreeLinks object.
 * \tparam Messages   A message class; must contain a invalid_node element.
 *                    This class must inherit from TreeLinks.
 * ### Usage ###
 * This template class is designed for use with the "curiously recurring
 * template pattern". The template parameter Links must be a class that derives
 * from TreeLinks:
 *    class DerivedClass : public TreeLinks<DerivedClass, Container, Messages>
 */
template <class Links, class Container, class Messages>
class TreeLinks {

   JEOD_MAKE_SIM_INTERFACES(TreeLinks)

   // The range generators provided in tree_links_iterator.hh
   // are the recommended mechanism for iterating over a set of links.
   template <class RLinks> friend class TreeLinksAscendRange;
   template <class RLinks> friend class TreeLinksDescentRange;
   template <class RLinks> friend class TreeLinksChildrenRange;


 public:


   /**
    * Non-default constructor.
    * @param[in,out] container_in  Object that contains this object
    * @param[in]     path_size     Initial size to reserve for the path
    */
   TreeLinks (
      Container & container_in,
      unsigned int path_size)
   :
      container_(container_in),
      parent_(nullptr),
      children_(),
      path_to_node_()
   {
      path_to_node_.reserve (std::max(1u, path_size));
      set_path_size (1u);
      path_to_node_[0] = static_cast<Links*> (this);
   }


   /**
    * Destructor.
    */
   virtual ~TreeLinks () = default;

   // The default constructor, copy constructor and assignment operator for
   // this class are explicitly deleted.
   TreeLinks () = delete;
   TreeLinks (const TreeLinks&) = delete;
   TreeLinks& operator= (const TreeLinks&) = delete;


   /**
    * Iterator that points to the first child.
    */
   Links* child_head()
   {
       return children_.front();
   }

   /**
    * Iterator that points to the last child.
    */
   Links* child_tail()
   {
       return children_.back();
   }


   /**
    * Is the body atomic -- in other words, is it a leaf node?
    * @return True if the body has no children, false otherwise.
    */
   bool is_atomic ()
   {
      return children_.empty();
   }

   /**
    * Is the body non-atomic -- in other words, does it have children?
    * @return True if the body has children, false otherwise.
    */
   bool has_children ()
   {
      return ! children_.empty();
   }


   /**
    * Is the body a root node?
    * @return True if the parent is null, false otherwise.
    */
   bool is_root ()
   {
      return (parent_ == nullptr);
   }


   /**
    * Accessor for the container, non-const version.
    * @return  Object that contains this object.
    */
   Container & container ()
   {
      return container_;
   }

   /**
    * Accessor for the container, const version.
    * @return  Object that contains this object.
    */
   const Container & container () const
   {
      return container_;
   }


   /**
    * Accessor for the parent links, non-const version.
    * @return  Pointer to this object's parent TreeLinks object.
    */
   Links * links_parent ()
   {
      return parent_;
   }

   /**
    * Accessor for the parent links, const version.
    * @return  Pointer to this object's parent TreeLinks object.
    */
   const Links * links_parent () const
   {
      return parent_;
   }


   /**
    * Accessor for the parent container, non-const version.
    * @return  Pointer to this object's parent Container object.
    */
   Container * parent ()
   {
      return (parent_ != nullptr) ? &(parent_->container_) : nullptr;
   }

   /**
    * Accessor for the parent container, const version.
    * @return  Pointer to this object's parent Container object.
    */
   const Container * parent () const
   {
      return (parent_ != nullptr) ? &(parent_->container_) : nullptr;
   }


   /**
    * Accessor for the root links object, non-const version.
    * @return Root links object
    */
   Links * links_root ()
   {
      return path_to_node_[0];
   }

   /**
    * Accessor for the root links object, const version.
    * @return Root links object
    */
   const Links * links_root () const
   {
      return path_to_node_[0];
   }


   /**
    * Accessor for the root container object, non-const version.
    * @return Root container object
    */
   Container * root ()
   {
      return &(path_to_node_[0]->container_);
   }

   /**
    * Accessor for the root container object, const version.
    * @return Root container object
    */
   const Container * root () const
   {
      return &(path_to_node_[0]->container_);
   }


   /**
    * Return the length of the path_to_node_ vector.
    */
   unsigned int path_length () const
   {
      return  path_to_node_.size();
   }


   /**
    * Find the index of the specified link in the path_to_node_.
    */
   unsigned int find_path_index (const Links& link) const
   {
      return  std::find(path_to_node_.begin(), path_to_node_.end(), &link)
         - path_to_node_.begin();
   }


   /**
    * Accessor for the nth_from_root frame, non-const version.
    * @param index Path index (root=0)
    * @return Nth links container
    */
   Container * nth_from_root (unsigned int index)
   {
      return (index < path_length()) ?
             &(path_to_node_[index]->container_) : nullptr;
   }

   /**
    * Accessor for the nth_from_root frame, const version.
    * @param index Path index (root=0)
    * @return Nth links container
    */
   const Container * nth_from_root (unsigned int index) const
   {
      return (index < path_length()) ?
             &(path_to_node_[index]->container_) : nullptr;
   }


   /**
    * Make the links object a root object.
    */
   void make_root ()
   {
      // Sanity check: The links should have a null parent and no siblings.
      if (parent_ != nullptr) {
         MessageHandler::fail (__FILE__, __LINE__,
                               Messages::invalid_node,
                               "Object has non-null linkages");
         // Not reached
         return;
      }

      construct_path_to_node ();
   }


   /**
    * Add this object as a child of the frame containing these links.
    * This object must have no parent, no siblings.
    * @param new_parent Links object that is to be the parent of this object.
    */
   void attach (Links & new_parent)
   {
      // Sanity check: The links should have a null parent and no siblings.
      if (parent_ != nullptr) {
         MessageHandler::fail (__FILE__, __LINE__,
                               Messages::invalid_node,
                               "Object is already attached");
         // Not reached
         return;
      }

      // Sanity check: Do not violate tree structure.
      if (new_parent.path_to_node_[0] == path_to_node_[0]) {
         MessageHandler::fail (__FILE__, __LINE__,
                               Messages::invalid_node,
                               "Attachment would create non-tree structure.");
         // Not reached
         return;
      }

      // Attach the child to the parent.
      attach_internal (new_parent);

      // Construct the path_to_node for the child and all its children.
      construct_path_to_node ();
   }


   /**
    * Detach a node from its parent.
    */
   void detach ()
   {
      // Sever the links between child and parent, and siblings.
      detach_internal ();

      // Construct the path_to_node for the child and all its children.
      construct_path_to_node ();
   }


   /**
    * Attach a node somewhere else.
    * @param new_parent Links object that is to be the parent of this object.
    */
   void reattach (
      Links & new_parent)
   {

      // Remove links to the containing frame.
      detach_internal ();

      // Reattach.
      attach_internal (new_parent);

      // Construct the path_to_node for the child and all its children.
      construct_path_to_node ();
   }


   /**
    * Determine if a node is the progeny of another.
    * @param target Target links object
    * @return True if target is an ancestor of this node, false otherwise.
    */
   bool is_progeny_of (const Links & target) const
   {
      bool result = false;

      for (auto* link : path_to_node_) {
         if (link == &target) {
            result = true;
            break;
         }
      }

      return result;
   }


   /**
    * Find the index of the node that represents the point of departure
    * in the tree containing two nodes.
    * @param target Some other node in the tree
    * @return Index of the last common node
    */
   int find_last_common_index (const Links & target) const
   {
      int max_index;
      int last_common;

      // Both links must have a path and must be part of the same tree. If this
      // is not the case, return a negative value to indicate failure.
      if ((path_length() == 0) ||
          (target.path_length() == 0) ||
          (path_to_node_[0] != target.path_to_node_[0])) {
         last_common = -1;
      }

      else {
         // Determine the maximum possible index as the shortest length - 1.
         max_index = std::min(path_length(), target.path_length()) - 1;

         // Paths are common up to max_index: This is the last common node.
         if (path_to_node_[max_index] == target.path_to_node_[max_index]) {
            last_common = max_index;
         }

         // Paths diverge before end: Find point of departure.
         else {
            for (last_common = 0;
                 path_to_node_[last_common+1] ==
                    target.path_to_node_[last_common+1];
                 ++last_common) {
               ; // Empty -- Loop will terminate at last common index.
            }
         }
      }

      return last_common;
   }


   /**
    * Find the node that represents the point of departure in the tree
    * containing two nodes.
    * @param target Some other node in the tree
    * @return Pointer to last common node
    */
   const Links * find_last_common_node (
      const Links & target)
   const
   {
      int last_common = find_last_common_index(target);
      return (last_common < 0) ? nullptr : path_to_node_[last_common];
   }


 protected:

   /**
    * Recursively construct the path_to_node.
    */
   void construct_path_to_node ()
   {
      // Non-null parent: Resize the path to the parent's size + 1 and
      // copy the parent's path to form the start of the new path.
      if (parent_ != nullptr) {
         set_path_size (parent_->path_length()+1);
         std::copy (
            parent_->path_to_node_.begin(), parent_->path_to_node_.end(),
            path_to_node_.begin());
      }

      // Null parent (root): Resize path to one.
      else {
         set_path_size (1u);
      }

      // The path always ends with this object.
      path_to_node_[path_to_node_.size() - 1] = static_cast<Links*> (this);

      // Reconstruct the path for each child.
      for (auto* child : children_) {
         child->construct_path_to_node ();
      }
   }


 private:

   /**
    * Add a frame as a child of the frame containing these links.
    * @param new_parent The node to which this object is to be attached.
    */
   void attach_internal (Links & new_parent)
   {
      // Attach the child to the parent.
      parent_ = &new_parent;

      // Add the child as a child of the parent.
      new_parent.children_.push_back (static_cast<Links*>(this));
   }


   /**
    * Detach a node from its parent.
    */
   void detach_internal ()
   {
      // Sever the links between child and parent.
      if (parent_ != nullptr) {
         parent_->children_.erase(
            std::find(
               parent_->children_.begin(), parent_->children_.end(), this));
      }

      // Break links between this and former parent_.
      parent_ = nullptr;
   }


   /**
    * Ensures the path size is at least as large as specified,
    * resizing the path_to_node array if needed.
    * @param new_size Requested size
    */
   void set_path_size (unsigned int new_size)
   {
      path_to_node_.resize (new_size, nullptr);
      std::fill (path_to_node_.begin(), path_to_node_.end(), nullptr);
   }


 // Member data
 JEOD_NOMINALLY_PRIVATE:

   /**
    * The object to which this set of links pertains; the container.
    */
   Container & container_; //!< trick_units(--)

   /**
    * The TreeLinks object that is the immediate parent of this TreeLinks
    * object in the directed tree that contains this TreeLinks object.
    * This pointer is null for all root objects.
    */
   Links* parent_; //!< trick_units(--)

   /**
    * The TreeLinks object's children.
    */
   typename std::vector<Links*> children_; //!< trick_units(--)

   /**
    * Vector of pointers to TreeLinks nodes containing the sequence of links
    * from the root node of the tree to this TreeLinks object.
    * The path_to_node_ remains empty until the links object is made viable
    * by either a call to attach() or to make_root().
    * The zeroth element of this array is the root object.
    * The last element is this node.
    */
   typename std::vector<Links*> path_to_node_; //!< trick_units(--)
};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
