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
 * @file models/utils/container/include/jeod_container_compare.hh
 * Define comparison operators for JEOD STL container.
 * The comparisons are the same as those for the underlying STL containers
 * and are implemented using the underlying STL container comparison operators.
 * There are three template functions to define for each comparison operator:
 *   - JEOD container to STL container
 *   - STL container to JEOD container
 *   - JEOD container to JEOD container.
 * With 6 comparison operators this means 18 function templates need to
 * be defined.
 */

/*******************************************************************************

Purpose:
  ()



*******************************************************************************/

#ifndef JEOD_MEMORY_CONTAINER_COMPARATORS_H
#define JEOD_MEMORY_CONTAINER_COMPARATORS_H

// Model includes
#include "jeod_stl_container.hh"

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// The 18 functions can be defined a bit more easily with macros.
// In fact, the 18 functions *were* defined with macros.
// The code that follows is the macro expansion.
// The macros follow, commented out.
#if 0

// Comparison operators.
// There are three template functions to define for each comparison operator:
//   - JEOD container to STL container
//   - STL container to JEOD container
//   - JEOD container to JEOD container.
// With 6 comparison operators this means 18 function templates.
// That is tedious and error prone.
// Macros help with both problems.


// First define some shortcuts.

/**
 * Cast a JEOD container to its STL equivalent.
 * @param x Argument.
 */
#define JEOD_CONTAINER_CAST_TO_STL(x) static_cast<const ContainerType &>(x)


// Next define three macros that generate function templates to compare
//  - A JEOD container to another JEOD container,
//  - An STL container to a JEOD container, and
//  - A JEOD container to an STL container.

/**
 * Generate a comparison operator function template that compares
 * a JEOD container to an STL container.
 * @param op Comparison operator
 */
#define JEOD_CONTAINER_GENERATE_JEOD_TO_STL_COMPARATOR(op)                                                             \
    template<typename ElemType, typename ContainerType>                                                                \
    inline bool operator op(const jeod::JeodSTLContainer<ElemType, ContainerType> & x, const ContainerType & y)        \
    {                                                                                                                  \
        return JEOD_CONTAINER_CAST_TO_STL(x) op y;                                                                     \
    }

/**
 * Generate a comparison operator function template that compares
 * an STL container to a JEOD container.
 * @param op Comparison operator
 */
#define JEOD_CONTAINER_GENERATE_STL_TO_JEOD_COMPARATOR(op)                                                             \
    template<typename ElemType, typename ContainerType>                                                                \
    inline bool operator op(const ContainerType & x, const jeod::JeodSTLContainer<ElemType, ContainerType> & y)        \
    {                                                                                                                  \
        return x op JEOD_CONTAINER_CAST_TO_STL(y);                                                                     \
    }

/**
 * Generate a comparison operator function template that compares
 * a JEOD container to a JEOD container.
 * @param op Comparison operator
 */
#define JEOD_CONTAINER_GENERATE_JEOD_TO_JEOD_COMPARATOR(op)                                                            \
    template<typename ElemType, typename ContainerType>                                                                \
    inline bool operator op(const jeod::JeodSTLContainer<ElemType, ContainerType> & x,                                 \
                            const jeod::JeodSTLContainer<ElemType, ContainerType> & y)                                 \
    {                                                                                                                  \
        return JEOD_CONTAINER_CAST_TO_STL(x) op JEOD_CONTAINER_CAST_TO_STL(y);                                         \
    }


// Next define a macro that simply invokes the above three macros.

/**
 * Generate all three comparison operator function template
 * for a given operator.
 * @param op Comparison operator
 */
#define JEOD_CONTAINER_GENERATE_THREE_COMPARATORS(op)                                                                  \
    JEOD_CONTAINER_GENERATE_JEOD_TO_STL_COMPARATOR(op)                                                                 \
    JEOD_CONTAINER_GENERATE_STL_TO_JEOD_COMPARATOR(op)                                                                 \
    JEOD_CONTAINER_GENERATE_JEOD_TO_JEOD_COMPARATOR(op)


// Now we can generate all eighteen comparison function templates.
JEOD_CONTAINER_GENERATE_THREE_COMPARATORS (<)
JEOD_CONTAINER_GENERATE_THREE_COMPARATORS (==)
JEOD_CONTAINER_GENERATE_THREE_COMPARATORS (>)
JEOD_CONTAINER_GENERATE_THREE_COMPARATORS (>=)
JEOD_CONTAINER_GENERATE_THREE_COMPARATORS (!=)
JEOD_CONTAINER_GENERATE_THREE_COMPARATORS (<=)


// Last act: Get rid of the macros. We're done with them.
#undef JEOD_CONTAINER_GENERATE_THREE_COMPARATORS
#undef JEOD_CONTAINER_GENERATE_JEOD_TO_JEOD_COMPARATOR
#undef JEOD_CONTAINER_GENERATE_STL_TO_JEOD_COMPARATOR
#undef JEOD_CONTAINER_GENERATE_JEOD_TO_STL_COMPARATOR
#undef JEOD_CONTAINER_CAST_TO_STL

#endif

/**
 * Test if x is less than y.
 * @param x Comparand
 * @param y Comparand
 * @return x < y
 */
template<typename ElemType, typename ContainerType>
inline bool operator<(const jeod::JeodSTLContainer<ElemType, ContainerType> & x, const ContainerType & y)
{
    return static_cast<const ContainerType &>(x) < y;
}

/**
 * Test if x is less than y.
 * @param x Comparand
 * @param y Comparand
 * @return x < y
 */
template<typename ElemType, typename ContainerType>
inline bool operator<(const ContainerType & x, const jeod::JeodSTLContainer<ElemType, ContainerType> & y)
{
    return x < static_cast<const ContainerType &>(y);
}

/**
 * Test if x is less than y.
 * @param x Comparand
 * @param y Comparand
 * @return x < y
 */
template<typename ElemType, typename ContainerType>
inline bool operator<(const jeod::JeodSTLContainer<ElemType, ContainerType> & x,
                      const jeod::JeodSTLContainer<ElemType, ContainerType> & y)
{
    return static_cast<const ContainerType &>(x) < static_cast<const ContainerType &>(y);
}

/**
 * Test if x is equal to y.
 * @param x Comparand
 * @param y Comparand
 * @return x == y
 */
template<typename ElemType, typename ContainerType>
inline bool operator==(const jeod::JeodSTLContainer<ElemType, ContainerType> & x, const ContainerType & y)
{
    return static_cast<const ContainerType &>(x) == y;
}

/**
 * Test if x is equal to y.
 * @param x Comparand
 * @param y Comparand
 * @return x == y
 */
template<typename ElemType, typename ContainerType>
inline bool operator==(const ContainerType & x, const jeod::JeodSTLContainer<ElemType, ContainerType> & y)
{
    return x == static_cast<const ContainerType &>(y);
}

/**
 * Test if x is equal to y.
 * @param x Comparand
 * @param y Comparand
 * @return x == y
 */
template<typename ElemType, typename ContainerType>
inline bool operator==(const jeod::JeodSTLContainer<ElemType, ContainerType> & x,
                       const jeod::JeodSTLContainer<ElemType, ContainerType> & y)
{
    return static_cast<const ContainerType &>(x) == static_cast<const ContainerType &>(y);
}

/**
 * Test if x is greater than y.
 * @param x Comparand
 * @param y Comparand
 * @return x > y
 */
template<typename ElemType, typename ContainerType>
inline bool operator>(const jeod::JeodSTLContainer<ElemType, ContainerType> & x, const ContainerType & y)
{
    return static_cast<const ContainerType &>(x) > y;
}

/**
 * Test if x is greater than y.
 * @param x Comparand
 * @param y Comparand
 * @return x > y
 */
template<typename ElemType, typename ContainerType>
inline bool operator>(const ContainerType & x, const jeod::JeodSTLContainer<ElemType, ContainerType> & y)
{
    return x > static_cast<const ContainerType &>(y);
}

/**
 * Test if x is greater than y.
 * @param x Comparand
 * @param y Comparand
 * @return x > y
 */
template<typename ElemType, typename ContainerType>
inline bool operator>(const jeod::JeodSTLContainer<ElemType, ContainerType> & x,
                      const jeod::JeodSTLContainer<ElemType, ContainerType> & y)
{
    return static_cast<const ContainerType &>(x) > static_cast<const ContainerType &>(y);
}

/**
 * Test if x is greater than or equal to y.
 * @param x Comparand
 * @param y Comparand
 * @return x >= y
 */
template<typename ElemType, typename ContainerType>
inline bool operator>=(const jeod::JeodSTLContainer<ElemType, ContainerType> & x, const ContainerType & y)
{
    return static_cast<const ContainerType &>(x) >= y;
}

/**
 * Test if x is greater than or equal to y.
 * @param x Comparand
 * @param y Comparand
 * @return x >= y
 */
template<typename ElemType, typename ContainerType>
inline bool operator>=(const ContainerType & x, const jeod::JeodSTLContainer<ElemType, ContainerType> & y)
{
    return x >= static_cast<const ContainerType &>(y);
}

/**
 * Test if x is greater than or equal to y.
 * @param x Comparand
 * @param y Comparand
 * @return x >= y
 */
template<typename ElemType, typename ContainerType>
inline bool operator>=(const jeod::JeodSTLContainer<ElemType, ContainerType> & x,
                       const jeod::JeodSTLContainer<ElemType, ContainerType> & y)
{
    return static_cast<const ContainerType &>(x) >= static_cast<const ContainerType &>(y);
}

/**
 * Test if x is not equal to y.
 * @param x Comparand
 * @param y Comparand
 * @return x != y
 */
template<typename ElemType, typename ContainerType>
inline bool operator!=(const jeod::JeodSTLContainer<ElemType, ContainerType> & x, const ContainerType & y)
{
    return static_cast<const ContainerType &>(x) != y;
}

/**
 * Test if x is not equal to y.
 * @param x Comparand
 * @param y Comparand
 * @return x != y
 */
template<typename ElemType, typename ContainerType>
inline bool operator!=(const ContainerType & x, const jeod::JeodSTLContainer<ElemType, ContainerType> & y)
{
    return x != static_cast<const ContainerType &>(y);
}

/**
 * Test if x is not equal to y.
 * @param x Comparand
 * @param y Comparand
 * @return x != y
 */
template<typename ElemType, typename ContainerType>
inline bool operator!=(const jeod::JeodSTLContainer<ElemType, ContainerType> & x,
                       const jeod::JeodSTLContainer<ElemType, ContainerType> & y)
{
    return static_cast<const ContainerType &>(x) != static_cast<const ContainerType &>(y);
}

/**
 * Test if x is less than or equal to y.
 * @param x Comparand
 * @param y Comparand
 * @return x <= y
 */
template<typename ElemType, typename ContainerType>
inline bool operator<=(const jeod::JeodSTLContainer<ElemType, ContainerType> & x, const ContainerType & y)
{
    return static_cast<const ContainerType &>(x) <= y;
}

/**
 * Test if x is less than or equal to y.
 * @param x Comparand
 * @param y Comparand
 * @return x <= y
 */
template<typename ElemType, typename ContainerType>
inline bool operator<=(const ContainerType & x, const jeod::JeodSTLContainer<ElemType, ContainerType> & y)
{
    return x <= static_cast<const ContainerType &>(y);
}

/**
 * Test if x is less than or equal to y.
 * @param x Comparand
 * @param y Comparand
 * @return x <= y
 */
template<typename ElemType, typename ContainerType>
inline bool operator<=(const jeod::JeodSTLContainer<ElemType, ContainerType> & x,
                       const jeod::JeodSTLContainer<ElemType, ContainerType> & y)
{
    return static_cast<const ContainerType &>(x) <= static_cast<const ContainerType &>(y);
}

#endif

/**
 * @}
 * @}
 * @}
 */
