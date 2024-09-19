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
 * @addtogroup Container
 * @{
 *
 * @file models/utils/container/include/jeod_vector.hh
 * Define class template JeodVector.
 */

/*******************************************************************************

Purpose:
  ()



*******************************************************************************/

#ifndef JEOD_MEMORY_VECTOR_H
#define JEOD_MEMORY_VECTOR_H

// Model includes
#include "jeod_sequence_container.hh"

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// System includes
#include <cstddef>
#include <vector>

//! Namespace jeod
namespace jeod
{

/*
Members unique to std::vector:
capacity
reserve
Members common to std::vector and std::deque but not to std::list:
operator[]
at
*/

/**
 * The JEOD replacement for std::vector.
 */
template<typename ElemType> class JeodVector : public JeodSequenceContainer<ElemType, std::vector<ElemType>>
{
public:
    // Types

    /**
     * This particular JeodVector type.
     */
    using this_container_type = JeodVector<ElemType>;

    /**
     * The JeodSequenceContainer type.
     */
    using jeod_sequence_container_type = JeodSequenceContainer<ElemType, std::vector<ElemType>>;

    /**
     * The JeodSTLContainer type.
     */
    using jeod_stl_container_type = JeodSTLContainer<ElemType, std::vector<ElemType>>;

    /**
     * The std::vector itself.
     */
    using stl_container_type = std::vector<ElemType>;

    // Member functions

    // Constructors and destructors
    // NOTE: The constructors are protected. See jeod_stl_container.hh.

    /**
     * Destructor.
     */
    virtual ~JeodVector() = default;

    // Assignment operators

    /**
     * Copy contents from the given source.
     */
    JeodVector & operator=(const this_container_type & src)
    {
        jeod_stl_container_type::operator=(src);
        return *this;
    }

    /**
     * Copy contents from the given source.
     */
    JeodVector & operator=(const stl_container_type & src)
    {
        jeod_stl_container_type::operator=(src);
        return *this;
    }

    // Capacity

    /**
     * Returns the size of the allocated storage space for the vector.
     */
    typename jeod_stl_container_type::size_type capacity() const
    {
        return this->contents.capacity();
    }

    /**
     * Requests that the capacity of the allocated storage space
     * be made large enough to hold at least @a n elements.
     */
    void reserve(typename jeod_stl_container_type::size_type n)
    {
        this->contents.reserve(n);
    }

    // Element access

    /**
     * Get the nth element of the vector.
     * @return Nth element of the vector.
     */
    typename stl_container_type::reference operator[](std::size_t n)
    {
        return this->contents[n];
    }

    /**
     * Get the nth element of the vector.
     * @return Nth element of the vector.
     */
    typename stl_container_type::const_reference operator[](std::size_t n) const
    {
        return this->contents[n];
    }

    /**
     * Get the nth element of the vector, throwing exception if out of range.
     * @return Nth element of the vector.
     */
    typename stl_container_type::reference at(std::size_t n)
    {
        return this->contents.at(n);
    }

    /**
     * Get the nth element of the vector, throwing exception if out of range.
     * @return Nth element of the vector.
     */
    typename stl_container_type::const_reference at(std::size_t n) const
    {
        return this->contents.at(n);
    }

protected:
    /**
     * Default constructor.
     */
    JeodVector() = default;

    /**
     * Copy constructor.
     */
    explicit JeodVector(const this_container_type & src)
        : jeod_sequence_container_type(src)
    {
    }

    /**
     * Copy constructor from STL container.
     * @param src Source container to be copied
     */
    explicit JeodVector(const stl_container_type & src)
        : jeod_sequence_container_type(src)
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
