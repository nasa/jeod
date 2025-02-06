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
 * @addtogroup Container
 * @{
 *
 * @file models/utils/container/include/jeod_list.hh
 * Define the class template JeodList.
 */

/*******************************************************************************

Purpose:
  ()

 

*******************************************************************************/

#ifndef JEOD_MEMORY_LIST_H
#define JEOD_MEMORY_LIST_H

// Model includes
#include "jeod_sequence_container.hh"

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// System includes
#include <list>


//! Namespace jeod
namespace jeod {

/*
Members unique to std::list:
splice
remove
remove_if
unique
merge
sort
reverse
Members common to std::list and std::deque but not to std::vector:
push_front
pop_front
*/


/**
 * The JEOD replacement for std::list.
 */
template <typename ElemType>
class JeodList :
    public JeodSequenceContainer< ElemType, std::list<ElemType> > {

public:

   // Types

   /**
    * This particular JeodList type.
    */
   typedef JeodList<ElemType> this_container_type;

   /**
    * The JeodSequenceContainer type.
    */
   typedef JeodSequenceContainer< ElemType, std::list<ElemType> >
      jeod_sequence_container_type;

   /**
    * The JeodSTLContainer type.
    */
   typedef JeodSTLContainer<ElemType, std::list<ElemType> >
      jeod_stl_container_type;

   /**
    * The std::list itself.
    */
   typedef std::list<ElemType> stl_container_type;


   // Member functions

   // Constructors and destructors
   // NOTE: The constructors are protected. See jeod_stl_container.hh.

   /**
    * Destructor.
    */
   virtual ~JeodList (void) {}


   // Assignment operators

   /**
    * Copy contents from the given source.
    */
   JeodList &
   operator= (const this_container_type & src)
   {
      jeod_stl_container_type::operator= (src);
      return *this;
   }

   /**
    * Copy contents from the given source.
    */
   JeodList &
   operator= (const stl_container_type & src)
   {
      jeod_stl_container_type::operator= (src);
      return *this;
   }


   // Operations

   /**
    * Merge the contents of some other list into this list, emptying the
    * other list.
    * @param other Other list to be merged into this list.
    */
   void
   merge (stl_container_type & other)
   {
      this->contents.merge (other);
   }

   /**
    * Merge the contents of some other list into this list using the provided
    * comparator to guide the merge. The other list is emptied.
    * @param other Other list to be merged into this list.
    * @param comp  Comparison function.
    */
   template <typename Compare>
   void
   merge (stl_container_type & other, Compare comp)
   {
      this->contents.merge (other, comp);
   }

   /**
    * Add an element to the head of the list.
    * @param elem Element to be added.
    */
   void
   push_front (const ElemType & elem)
   {
      this->contents.push_front (elem);
   }

   /**
    * Deletes the element at the head of the list.
    */
   void
   pop_front (void)
   {
      this->contents.pop_front ();
   }

   /**
    * Remove elements from the list that are equal to the provided value.
    */
   void
   remove (const ElemType & value)
   {
      this->contents.remove(value);
   }

   /**
    * Remove elements from the list that pass the provided test.
    * @param pred Predicate function, which must be able to take a const
    *             ref to ElemType as an argument and must return a bool.
    */
   template <typename Predicate>
   void
   remove_if (Predicate pred)
   {
      this->contents.remove_if(pred);
   }

   /**
    * Reverse the list.
    */
   void
   reverse (void)
   {
      this->contents.reverse();
   }

   /**
    * Inserts the contents of @a other before @a position, emptying @a other.
    */
   void
   splice (
      typename jeod_stl_container_type::iterator position,
      stl_container_type & other)
   {
      this->contents.splice(position, other);
   }

   /**
    * Inserts the element @a other_pos of @a other before @a position,
    * deleting that element from @a other.
    */
   void
   splice (
      typename jeod_stl_container_type::iterator position,
      stl_container_type & other,
      typename jeod_stl_container_type::iterator other_pos)
   {
      this->contents.splice(position, other, other_pos);
   }

   /**
    * Inserts elements in @a other from @a first up to but not including
    * @a last before @a position, deleting those element from @a other.
    */
   void
   splice (
      typename jeod_stl_container_type::iterator position,
      stl_container_type & other,
      typename jeod_stl_container_type::iterator first,
      typename jeod_stl_container_type::iterator last)
   {
      this->contents.splice(position, other, first, last);
   }

   /**
    * Sort using the default comparison operator.
    */
   void
   sort (void)
   {
      this->contents.sort();
   }

   /**
    * Sort using the provided comparator.
    * @param comp Comparison function, which must be able to take a pair of
    *             ElemType as arguments and must return a bool.
    */
   template <typename Compare>
   void
   sort (Compare comp)
   {
      this->contents.sort(comp);
   }

   /**
    * Remove duplicates using the default equality operator.
    */
   void
   unique (void)
   {
      this->contents.unique();
   }

   /**
    * Remove duplicates using the provided comparator.
    * @param comp Comparison function, which must be able to take a pair of
    *             ElemType as arguments and must return a bool.
    */
   template <typename BinaryPredicate>
   void
   unique (BinaryPredicate comp)
   {
      this->contents.unique(comp);
   }

protected:

   /**
    * Default constructor.
    */
   JeodList (void) {}

   /**
    * Copy constructor.
    */
   JeodList (const this_container_type & src)
   : jeod_sequence_container_type (src)
   {}

   /**
    * Copy constructor from STL container.
    * @param src Source container to be copied
    */
   explicit JeodList (const stl_container_type & src)
   : jeod_sequence_container_type (src)
   {}
};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
