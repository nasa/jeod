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
 * @file models/utils/container/include/jeod_stl_container.hh
 * Define checkpointable replacements for STL containers.
 * This file defines class template JeodSTLContainer, the starting point of this
 * concept. The ultimate goal is to provide the full functionality of the
 * ISO/IEC 14882:2003 STL containers as transparently as possible
 * in the form of checkpointable class templates.
 */

/*******************************************************************************

Purpose:
  ()



*******************************************************************************/

#ifndef JEOD_MEMORY_STL_CONTAINER_H
#define JEOD_MEMORY_STL_CONTAINER_H

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod
{

// What's missing:
// Some constructors (probably never)
// Operators not common to deque, vector, and list (implemented elsewhere).

/**
 * This is the base class for the JEOD replacements of the STL containers.
 *
 * A key goal of the JEOD STL container replacement effort is to
 * provide checkpointable replacements that transparently provide the full
 * functionality of the ISO/IEC 14882:2003 STL containers.
 * This class begins that effort by defining types and member functions
 * common to the STL deque, list, map, set, and vector class templates.
 * Non-common methods are the responsibility of derived class templates
 * specialized to a specific container types.
 *
 * @note Exceptions to full functionality goal:
 * The above goal is not and never will be fully achieved. Exceptions are:
 *  - JEOD doesn't supply a replacement for std::deque or std::map.
 *    JEOD doesn't use deques at all and its maps are not checkpointable.
 *  - The full panoply of STL container constructors is not supplied.
 *  - The swap method is supplied but it is protected.
 *    The intent is that this class be further derived to create a
 *    checkpointable class. Swapping the checkpointable content is a dubious
 *    concept. The swap method is eventually exposed as the swap_stl_contents
 *    method to make it clear that that method is not a true swap.
 */
template<typename ElemType, typename ContainerType> class JeodSTLContainer
{
public:
    // Types

    /**
     * This particular JeodSTLContainer type.
     */
    using this_container_type = JeodSTLContainer<ElemType, ContainerType>;

    /**
     * Import the ContainerType::allocator_type
     */
    using allocator_type = typename ContainerType::allocator_type;

    /**
     * Import the ContainerType::reference
     */
    using reference = typename ContainerType::reference;

    /**
     * Import the ContainerType::const_reference
     */
    using const_reference = typename ContainerType::const_reference;

    /**
     * Import the ContainerType::iterator.
     */
    using iterator = typename ContainerType::iterator;

    /**
     * Import the ContainerType::const_iterator.
     */
    using const_iterator = typename ContainerType::const_iterator;

    /**
     * Import the ContainerType::reverse_iterator.
     */
    using reverse_iterator = typename ContainerType::reverse_iterator;

    /**
     * Import the ContainerType::const_reverse_iterator.
     */
    using const_reverse_iterator = typename ContainerType::const_reverse_iterator;

    /**
     * Import the ContainerType::difference_type.
     */
    using difference_type = typename ContainerType::difference_type;

    /**
     * Import the ContainerType::size_type.
     */
    using size_type = typename ContainerType::size_type;

    /**
     * Import the ContainerType::value_type.
     */
    using value_type = typename ContainerType::value_type;

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
    virtual ~JeodSTLContainer() = default;

    // Conversion operators

    /**
     * Returns the contents as an lvalue.
     */
    operator ContainerType &()
    {
        return contents;
    }

    /**
     * Returns the contents as a const rvalue.
     */
    operator const ContainerType &() const
    {
        return contents;
    }

    // Assignment operator

    /**
     * Assignment operator.
     * @param src Source container to be copied
     */
    this_container_type & operator=(const this_container_type & src)
    {
        if(&src != this)
        {
            clear();
            contents = src.contents;
        }
        return *this;
    }

    /**
     * Assignment operator.
     * @param src Source container to be copied
     */
    this_container_type & operator=(const ContainerType & src)
    {
        if(&src != &contents)
        {
            clear();
            contents = src;
        }
        return *this;
    }

    // Allocator operators

    /**
     * Returns the allocator object used to construct the contents.
     */
    allocator_type get_allocator() const
    {
        return contents.get_allocator();
    }

    // Iterators

    /**
     * Returns an iterator that points to the first element.
     */
    iterator begin()
    {
        return contents.begin();
    }

    /**
     * Returns a const iterator that points to the first element.
     */
    const_iterator begin() const
    {
        return contents.begin();
    }

    /**
     * Returns an iterator that points past the last element.
     */
    iterator end()
    {
        return contents.end();
    }

    /**
     * Returns a const iterator that points past the last element.
     */
    const_iterator end() const
    {
        return contents.end();
    }

    /**
     * Returns a reverse iterator that points to the last element.
     */
    reverse_iterator rbegin()
    {
        return contents.rbegin();
    }

    /**
     * Returns a const reverse iterator that points to the last element.
     */
    const_reverse_iterator rbegin() const
    {
        return contents.rbegin();
    }

    /**
     * Returns a reverse iterator that points before the first element.
     */
    reverse_iterator rend()
    {
        return contents.rend();
    }

    /**
     * Returns a const reverse iterator that points before the first element.
     */
    const_reverse_iterator rend() const
    {
        return contents.rend();
    }

    // Capacity

    /**
     * Returns true if the contents are empty, false otherwise.
     */
    bool empty() const
    {
        return contents.empty();
    }

    /**
     * Returns the implementation's limit on the number of elements.
     */
    size_type max_size() const
    {
        return contents.max_size();
    }

    /**
     * Returns the number of elements.
     */
    size_type size() const
    {
        return contents.size();
    }

    // Modifiers

    /**
     * Clear the contents.
     */
    void clear()
    {
        contents.clear();
    }

    /**
     * Insert a new element initialized with @a new_elem before the
     * iterator @a position.
     * @param position  Insertion position
     * @param new_elem  Element value to be inserted
     * @return          Iterator that points to the newly-inserted element
     */
    iterator insert(iterator position, const value_type & new_elem)
    {
        return contents.insert(position, new_elem);
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
    JeodSTLContainer() = default;

    /**
     * Copy constructor.
     * @param src Source container to be copied
     */
    explicit JeodSTLContainer(const this_container_type & src)
        : contents(src.contents)
    {
    }

    /**
     * Copy constructor from STL container.
     * @param src Source container to be copied
     */
    explicit JeodSTLContainer(const ContainerType & src)
        : contents(src)
    {
    }

    // Swap. Protected because making these public would make it look
    // like the checkpointable extensions have a real swap.
    // (These are real swap operations.)

    /**
     * Swap contents.
     * @param other Other JEOD container with contents are to be swapped.
     */
    void swap(this_container_type & other)
    {
        contents.swap(other.contents);
    }

    /**
     * Swap contents.
     * @param other Other STL container with contents are to be swapped.
     */
    void swap(ContainerType & other)
    {
        contents.swap(other);
    }

    // Member data

    /**
     * The STL container.
     */
    ContainerType contents; //!< trick_io(**)
};

} // namespace jeod

// Delayed includes
#include "jeod_container_compare.hh"

#endif

/**
 * @}
 * @}
 * @}
 */
