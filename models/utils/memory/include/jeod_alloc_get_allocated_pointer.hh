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
 * @addtogroup Memory
 * @{
 *
 * @file models/utils/memory/include/jeod_alloc_get_allocated_pointer.hh
 * Define function template jeod_alloc_get_allocated_pointer.
 */

/*******************************************************************************

Purpose:
  ()

 

*******************************************************************************/

#ifndef JEOD_ALLOC_GET_ALLOCATED_POINTER_HH
#define JEOD_ALLOC_GET_ALLOCATED_POINTER_HH

// Model headers

// System headers
#include <cstddef>
#include <cstring>
#include <type_traits>

//! Namespace jeod
namespace jeod {

/*
 * Internal definitions
 * Do not use any of these outside of this class.
 * Future implementations are free to modify or delete any of these items.
 */


/**
 * Class template that provides a static function @a cast that casts a pointer
 * to an object of type @a T to a void* pointer.
 * @tparam T        Type
 * @tparam is_poly  True if the type T is a polymorphic class.
 *
 * This class template is intended for used by jeod_alloc_get_allocated_pointer.
 * Use in any other context is caveat emptor.
 *
 * This template provides a default implementation for non-polymorphic classes
 * (<tt>is_poly == false</tt>) that uses implicit cast.
 * The partial template instantiation that immediately follows provides n
 * an implementation that uses dynamic_cast when @a is_poly is true.
 */
template <typename T, bool is_poly>
class JeodAllocHelperAllocatedPointer {
public:

   /**
    * Cast a pointer to a non-polymorphic class via an implicit cast.
    * @return         Input pointer cast to void* via implicit cast.
    * @param  pointer Pointer
    */
   static void * cast (
      T * pointer)
   {
      return pointer;
   }
};

/**
 * Partial instantiation of JeodAllocHelperAllocatedPointer for polymorphic
 * classes. This class uses the fact that <tt>dynamic_cast<void*>(ptr)</tt>
 * results in a pointer to the most derived object pointed to by <tt>ptr</tt>.
 * See ISO/IEC 14882:2003 section 5.2.7.7 for details.
 */
template <typename T>
class JeodAllocHelperAllocatedPointer<T, true> {
public:

   /**
    * Cast a pointer to a non-class object via dynamic_cast.
    * @return         Input pointer cast to void* via dynamic_cast.
    * @param  pointer Pointer
    */
   static void * cast (
      T * pointer)
   {
      return dynamic_cast<void*>(pointer);
   }
};


/******************************************************************************/

/*
 * Externally-usable definitions
 * The remaining items can be used in derivative products.
 */


/**
 * Cast a pointer to some object to a pointer to void* such that a pointer to a
 * polymorphic object, downcast to a base class pointer, becomes a pointer to
 * the original  object, but also such that a pointer to an instance of a
 * non-polymorphic class or a pointer to a non-class type is handled correctly.
 * @tparam T       Pointed-to type.
 * @param  pointer Pointer to be cast to void*.
 *
 * <b>Usage:</b>@n
 *   @verbatim jeod_alloc_get_allocated_pointer(pointer) @endverbatim@n
 * Note that the template parameter does not need to be specified.
 * The compiler automagically determines the type.
 *
 * <b>Assumptions and limitations:</b>@n
 *  - The function argument @a pointer is a pointer.
 *  - The pointer is not @a cv qualified (i.e., neither const nor volatile).
 *  - Inheritance is public for polymorphic classes.
 *  - @verbatim jeod_alloc_get_allocated_pointer(&array[1]) @endverbatim
 *    does not return a pointer to @verbatim &array[0] @endverbatim.
 */
template<typename T>
inline void *
jeod_alloc_get_allocated_pointer (
   T* pointer)
{
   const bool is_poly = std::is_polymorphic<T>::value;
   return JeodAllocHelperAllocatedPointer<T, is_poly>::cast (pointer);
}


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
