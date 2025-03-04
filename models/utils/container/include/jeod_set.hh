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
 * @file models/utils/container/include/jeod_set.hh
 * Define the class template JeodSet.
 */

/*******************************************************************************

Purpose:
  ()



*******************************************************************************/

#ifndef JEOD_MEMORY_SET_H
#define JEOD_MEMORY_SET_H

// Model includes
#include "jeod_associative_container.hh"

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// System includes
#include <set>

//! Namespace jeod
namespace jeod
{

/*
Members unique to std::set: There are none.
*/

/**
 * The JEOD replacement for std::set.
 */
template<typename ElemType> class JeodSet : public JeodAssociativeContainer<ElemType, std::set<ElemType>>
{
public:
    // Types

    /**
     * This particular JeodSet type.
     */
    using this_container_type = JeodSet<ElemType>;

    /**
     * The JeodAssociativeContainer type.
     */
    using jeod_associative_container_type = JeodAssociativeContainer<ElemType, std::set<ElemType>>;

    /**
     * The JeodSTLContainer type.
     */
    using jeod_stl_container_type = JeodSTLContainer<ElemType, std::set<ElemType>>;

    /**
     * The std::set itself.
     */
    using stl_container_type = std::set<ElemType>;

    // Member functions

    // Constructors and destructors
    // NOTE: The constructors are protected. See jeod_stl_container.hh.

    /**
     * Destructor.
     */
    virtual ~JeodSet() = default;

    // Assignment operators

    /**
     * Copy contents from the given source.
     */
    JeodSet & operator=(const this_container_type & src)
    {
        jeod_stl_container_type::operator=(src);
        return *this;
    }

    /**
     * Copy contents from the given source.
     */
    JeodSet & operator=(const stl_container_type & src)
    {
        jeod_stl_container_type::operator=(src);
        return *this;
    }

protected:
    /**
     * Default constructor.
     */
    JeodSet() = default;

    /**
     * Copy constructor.
     */
    JeodSet(const this_container_type & src)
        : jeod_associative_container_type(src)
    {
    }

    /**
     * Copy constructor from STL container.
     * @param src Source container to be copied
     */
    explicit JeodSet(const stl_container_type & src)
        : jeod_associative_container_type(src)
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
