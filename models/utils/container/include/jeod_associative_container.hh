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
 * @file models/utils/container/include/jeod_associative_container.hh
 * Define checkpointable replacements for STL associative containers.
 * This file defines class template JeodAssociativeContainer, the basis for the
 * concept. The ultimate goal is to provide the full functionality of the
 * ISO/IEC 14882:2003 STL associative containers as transparently as possible
 * in the form of checkpointable class templates.
 */

/*******************************************************************************

Purpose:
  ()



*******************************************************************************/

#ifndef JEOD_MEMORY_ASSOCIATIVE_CONTAINER_H
#define JEOD_MEMORY_ASSOCIATIVE_CONTAINER_H

// Model includes
#include "jeod_stl_container.hh"

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// System includes
#include <utility>

//! Namespace jeod
namespace jeod
{

// What's missing:
// Some constructors (probably never)
// Methods common to sequence and associative containers, and methods unique
// to associative containers but not common all associative containers.

/**
 * This is the base class for the JEOD replacements of the STL associative
 * containers. The class derives from JeodSTLContainer, the base class for
 * the JEOD replacements of the STL containers.
 *
 * A key goal of the JEOD STL associative container replacement effort is to
 * provide checkpointable replacements that transparently provide the full
 * functionality of the ISO/IEC 14882:2003 STL associative containers.
 * This class begins that effort by defining types and member functions
 * common to the STL set and map class templates.
 * Non-common methods are the responsibility of derived class templates
 * specialized to a specific container types.
 *
 * @note Exceptions to full functionality goal:
 * The above goal is not and never will be fully achieved. Exceptions are:
 *  - The full set of STL associative container constructors is not supplied.
 */
template<typename ElemType, typename ContainerType>
class JeodAssociativeContainer : public JeodSTLContainer<ElemType, ContainerType>
{
public:
    // Types

    /**
     * This type
     */
    using this_container_type = JeodAssociativeContainer<ElemType, ContainerType>;

    /**
     * The JeodSTLContainer
     */
    using base_container_type = JeodSTLContainer<ElemType, ContainerType>;

    /**
     * Import the ContainerType::key_type.
     */
    using key_type = typename ContainerType::key_type;

    /**
     * Import the ContainerType::key_compare.
     */
    using key_compare = typename ContainerType::key_compare;

    /**
     * Import the ContainerType::value_compare.
     */
    using value_compare = typename ContainerType::value_compare;

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
    virtual ~JeodAssociativeContainer() = default;

    // Observers

    /**
     * Returns the key comparison object used to populate the contents.
     */
    key_compare key_comp() const
    {
        return this->contents.key_comp();
    }

    /**
     * Returns the value comparison object used to populate the contents.
     */
    value_compare value_comp() const
    {
        return this->contents.value_comp();
    }

    // Operations

    // Comment on the following:
    // The below is contrary to ISO/IEC 14882:2003 regarding std::set.
    // That's because the standard itself has bugs regarding std::set.
    // The implementation is consistent with g++ and many other compilers
    // that have implemented fixes to the bugs in the standard.

    /**
     * Find the number of occurrences of the specified element.
     */
    typename base_container_type::size_type count(const key_type & x) const
    {
        return this->contents.count(x);
    }

    /**
     * Find the element specified by the given key.
     */
    typename base_container_type::iterator find(const key_type & x)
    {
        return this->contents.find(x);
    }

    /**
     * Find the element specified by the given key.
     */
    typename base_container_type::const_iterator find(const key_type & x) const
    {
        return this->contents.find(x);
    }

    /**
     * Find the start of a sequence specified by the given key.
     */
    typename base_container_type::iterator lower_bound(const key_type & x)
    {
        return this->contents.lower_bound(x);
    }

    /**
     * Find the start of a sequence specified by the given key.
     */
    typename base_container_type::const_iterator lower_bound(const key_type & x) const
    {
        return this->contents.lower_bound(x);
    }

    /**
     * Find the end of a sequence specified by the given key.
     */
    typename base_container_type::iterator upper_bound(const key_type & x)
    {
        return this->contents.upper_bound(x);
    }

    /**
     * Find the end of a sequence specified by the given key.
     */
    typename base_container_type::const_iterator upper_bound(const key_type & x) const
    {
        return this->contents.upper_bound(x);
    }

    /**
     * Find the start and end of a sequence specified by the given key.
     */
    std::pair<typename base_container_type::iterator, typename base_container_type::iterator> equal_range(
        const key_type & x)
    {
        return this->contents.equal_range(x);
    }

    /**
     * Find the start and end of a sequence specified by the given key.
     */
    std::pair<typename base_container_type::const_iterator, typename base_container_type::const_iterator> equal_range(
        const key_type & x) const
    {
        return this->contents.equal_range(x);
    }

    // Modifiers

    // insert: Note that one of the three insert methods is common to all
    // JEOD containers and hence is implemented in the base case.
    using base_container_type::insert;

    /**
     * Insert elements, initializing the
     * inserted elements from the values pointed to by an iterator.
     * @param first     Input iterator
     * @param last      Input iterator
     */
    template<class InputIterator> void insert(InputIterator first, InputIterator last)
    {
        this->contents.insert(first, last);
    }

    /**
     * Inserts the provided value into the associative list.
     * @param new_elem  Element value to be inserted
     */
    std::pair<typename base_container_type::iterator, bool> insert(
        const typename base_container_type::value_type & new_elem)
    {
        return this->contents.insert(new_elem);
    }

    /**
     * Erase one item.
     * @param position Position to be erased
     */
    void erase(typename base_container_type::iterator position)
    {
        this->contents.erase(position);
    }

    /**
     * Erase a sequence of items.
     * @param first First element to be erased
     * @param last  One past last element to be erased
     */
    void erase(typename base_container_type::iterator first, typename base_container_type::iterator last)
    {
        this->contents.erase(first, last);
    }

    /**
     * Erases the item(s) specified by supplied key from the contents.
     * @param x         Key of item(s) to be erased
     */
    typename base_container_type::size_type erase(const key_type & x)
    {
        return this->contents.erase(x);
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
    JeodAssociativeContainer() = default;

    /**
     * Copy constructor.
     * @param src Source container to be copied
     */
    JeodAssociativeContainer(const this_container_type & src)
        : base_container_type(src)
    {
    }

    /**
     * Copy constructor from STL container.
     * @param src Source container to be copied
     */
    explicit JeodAssociativeContainer(const ContainerType & src)
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
