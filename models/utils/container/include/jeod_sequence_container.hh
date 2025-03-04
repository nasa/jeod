//=============================================================================
// Notices:
//
// Copyright © 2025 United States Government as represented by the Administrator
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
 * @file models/utils/container/include/jeod_sequence_container.hh
 * Define checkpointable replacements for STL sequence containers.
 * This file defines class template JeodSequenceContainer, the basis for the
 * concept. The ultimate goal is to provide the full functionality of the
 * ISO/IEC 14882:2003 STL sequence containers as transparently as possible
 * in the form of checkpointable class templates.
 */

/*******************************************************************************

Purpose:
  ()



*******************************************************************************/

#ifndef JEOD_MEMORY_SEQUENCE_CONTAINER_H
#define JEOD_MEMORY_SEQUENCE_CONTAINER_H

// Model includes
#include "jeod_stl_container.hh"

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod
{

// What's missing:
// Some constructors (probably never)
// Methods common to sequence and associative containers, and methods unique
// to sequence containers but not common all sequence containers.

/**
 * This is the base class for the JEOD replacements of the STL sequence
 * containers. The class derives from JeodSTLContainer, the base class for
 * the JEOD replacements of the STL containers.
 *
 * A key goal of the JEOD STL sequence container replacement effort is to
 * provide checkpointable replacements that transparently provide the full
 * functionality of the ISO/IEC 14882:2003 STL sequence containers.
 * This class begins that effort by defining types and member functions
 * common to the STL deque, list, and vector class templates.
 * Non-common methods are the responsibility of derived class templates
 * specialized to a specific container types.
 *
 * @note Exceptions to full functionality goal:
 * The above goal is not and never will be fully achieved. Exceptions are:
 *  - JEOD doesn't supply a replacement for std::deque.
 *    JEOD doesn't use deques.
 *  - The full panoply of STL sequence container constructors is not supplied.
 */
template<typename ElemType, typename ContainerType>
class JeodSequenceContainer : public JeodSTLContainer<ElemType, ContainerType>
{
public:
    // Types

    /**
     * This type
     */
    using this_container_type = JeodSequenceContainer<ElemType, ContainerType>;

    /**
     * The JeodSTLContainer
     */
    using base_container_type = JeodSTLContainer<ElemType, ContainerType>;

    // Member functions

    // Constructors and destructor.
    // NOTE: The constructors are not here in the public section.
    // They are protected. This class and its direct descendants JeodList and
    // JeodVector do not provide add any functionality to the STL sequences
    // except that the destructor is now virtual. Making the constructors
    // protected here forces a further derivation by some other class template
    // that does provide distinguishing characteristics.

    /**
     * Destructor.
     */
    virtual ~JeodSequenceContainer() = default;

    // Element access

    /**
     * Get the element at the tail of the contents.
     */
    typename base_container_type::reference back()
    {
        return this->contents.back();
    }

    /**
     * Get the element at the tail of the contents.
     */
    typename base_container_type::const_reference back() const
    {
        return this->contents.back();
    }

    /**
     * Get the element at the head of the contents.
     */
    typename base_container_type::reference front()
    {
        return this->contents.front();
    }

    /**
     * Get the element at the head of the contents.
     */
    typename base_container_type::const_reference front() const
    {
        return this->contents.front();
    }

    // Modifiers

    /**
     * Replace the container's contents with that specified by the iterators.
     *  @param  first  Input iterator.
     *  @param  last   Input iterator.
     */
    template<class InputIterator> void assign(InputIterator first, InputIterator last)
    {
        this->contents.assign(first, last);
    }

    /**
     * Replace the container's contents with @a new_size copies of @a new_elem.
     *  @param  new_size  New size of the container.
     *  @param  new_elem  Element to be replicated to fill the container.
     */
    void assign(typename base_container_type::size_type new_size, const ElemType & new_elem)
    {
        this->contents.assign(new_size, new_elem);
    }

    /**
     * Erase one item.
     * @param position Position to be erased
     */
    typename base_container_type::iterator erase(typename base_container_type::iterator position)
    {
        return this->contents.erase(position);
    }

    /**
     * Erase a sequence of items.
     * @param first First element to be erased
     * @param last  One past last element to be erased
     */
    typename base_container_type::iterator erase(typename base_container_type::iterator first,
                                                 typename base_container_type::iterator last)
    {
        return this->contents.erase(first, last);
    }

    // insert: Note that one of the three insert methods is common to all
    // JEOD containers and hence is implemented in the base case.
    using base_container_type::insert;

    /**
     * Insert elements before iterator @a position, initializing the
     * inserted elements from the values pointed to by an iterator.
     * @param position  Insertion position
     * @param first     Input iterator
     * @param last      Input iterator
     */
    template<class InputIterator>
    void insert(typename base_container_type::iterator position, InputIterator first, InputIterator last)
    {
        this->contents.insert(position, first, last);
    }

    /**
     * Extends the list by @a ncopies elements before the iterator @a position,
     * initializing each newly created element with @a new_elem.
     * @param position  Insertion position
     * @param ncopies   Number of elements to be inserted
     * @param new_elem  Element value to be inserted
     */
    void insert(typename base_container_type::iterator position,
                typename base_container_type::size_type ncopies,
                const ElemType & new_elem)
    {
        this->contents.insert(position, ncopies, new_elem);
    }

    /**
     * Resizes the container, adding or deleting items as needed.
     * @param new_size  New size
     * @param new_elem  Element to be added repetively if object is to grow.
     */
    void resize(typename base_container_type::size_type new_size, ElemType new_elem = ElemType())
    {
        return this->contents.resize(new_size, new_elem);
    }

    /**
     * Add an element to the end of the contents.
     * @param elem Element to be added.
     */
    void push_back(const ElemType & elem)
    {
        this->contents.push_back(elem);
    }

    /**
     * Deletes the element at the end of the contents.
     */
    void pop_back()
    {
        this->contents.pop_back();
    }

protected:
    // Member functions

    // Constructors

    /**
     * Default constructor.
     * Note: Making this protected precludes someone from declaring an object
     * to be of type JEODSTLContainer. Access is via some other class that
     * inherits from this class.
     */
    JeodSequenceContainer() = default;

    /**
     * Copy constructor.
     * @param src Source container to be copied
     */
    explicit JeodSequenceContainer(const this_container_type & src)
        : base_container_type(src)
    {
    }

    /**
     * Copy constructor from STL container.
     * @param src Source container to be copied
     */
    explicit JeodSequenceContainer(const ContainerType & src)
        : base_container_type(src)
    {
    }
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
