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
 * @file models/utils/memory/include/jeod_alloc_construct_destruct.hh
 * Define templates for use by jeod_alloc.hh.
 * These are isolated from jeod_alloc.hh because
 *  - They are templates; everything in jeod_alloc.hh is a macro.
 *  - Some of the templates might have wider interest than JEOD.
 *  - Some of this stuff can go away with C++11.
 *  .
 * The externally-usable items defined in this file are
 *  - Function template jeod_alloc_construct_array, and
 *  - Function template jeod_alloc_destruct_array.
 */

/*******************************************************************************

Purpose:
  ()

 

*******************************************************************************/

#ifndef JEOD_ALLOC_CONSTRUCT_DESTRUCT_HH
#define JEOD_ALLOC_CONSTRUCT_DESTRUCT_HH

// Model headers

// JEOD headers
#include "utils/sim_interface/include/jeod_class.hh"

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
 * Class template that provides static functions @a construct and @a destruct
 * that construct an array of objects.
 * @tparam T           Type
 * @tparam is_class    True if the type T is a class.
 * @tparam is_abstract True if the type T is an abstract class.
 *
 * This class template is intended for used by jeod_alloc_construct_array and
 * jeod_alloc_destruct_array. Use in any other context is caveat emptor.
 *
 * This template provides do-nothing implementations, which is about all one
 * can do for arrays of abstract objects (which can't exist).
 */
template <typename T, bool is_class, bool is_abstract>
class JeodAllocHelperConstructDestruct {
public:

   /**
    * Construct an array of objects.
    * @return         Constructed array.
    * @param  nelem   Number of elements in the array
    * @param  addr    Address to be constructed
    */
   static void * construct (
      std::size_t nelem JEOD_UNUSED,
      void * addr)
   {
      return addr;
   }

   /**
    * Destruct an array of objects.
    * @param  nelem   Number of elements in the array
    * @param  addr    Address to be destructed
    */
   static void destruct (
      std::size_t nelem JEOD_UNUSED,
      void * addr JEOD_UNUSED)
   {
      ; // Nothing to do.
   }
};


/**
 * Partial instantiation for non-classes.
 */
template <typename T, bool is_abstract>
class JeodAllocHelperConstructDestruct<T, false, is_abstract> {
public:

   /**
    * Construct an array of objects.
    * @return         Constructed array.
    * @param  nelem   Number of elements in the array
    * @param  addr    Address to be constructed
    */
   static void * construct (
      std::size_t nelem,
      void * addr)
   {
      std::memset (addr, 0, sizeof(T) * nelem);
      return addr;
   }

   /**
    * Destruct an array of objects.
    * @param  nelem   Number of elements in the array
    * @param  addr    Address to be destructed
    */
   static void destruct (
      std::size_t nelem JEOD_UNUSED,
      void * addr JEOD_UNUSED)
   {
      ; // Nothing to do for non-class objects.
   }
};


/**
 * Partial instantiation for non-abstract classes.
 */
template <typename T>
class JeodAllocHelperConstructDestruct<T, true, false> {
public:

   /**
    * Construct an array of objects.
    * @return         Constructed array.
    * @param  nelem   Number of elements in the array
    * @param  addr    Address to be constructed
    */
   static void * construct (
      std::size_t nelem,
      void * addr)
   {
      return new (addr) T[nelem];
   }

   /**
    * Destruct an array of objects.
    * @param  nelem   Number of elements in the array
    * @param  addr    Address to be destructed
    */
   static void destruct (
      std::size_t nelem,
      void * addr)
   {
      T * array = reinterpret_cast<T *>(addr);
      for (unsigned int ii = 0; ii < nelem; ++ii) {
         array[ii].~T();
      }
   }
};


/******************************************************************************/

/*
 * Externally-usable definitions
 * The remaining items can be used in derivative products.
 */


/**
 * Construct an array of objects of type @a T.
 * @tparam T       Pointed-to type.
 * @param  nelem   Number of elements in the array
 * @param  addr    Address to be constructed
 * @return         Constructed array.
 */
template<typename T>
inline void *
jeod_alloc_construct_array (
   std::size_t nelem,
   void * addr)
{
   typedef JeodAllocHelperConstructDestruct <
              T,
              std::is_class<T>::value,
              std::is_abstract<T>::value>
      ConstructDestruct;
   return ConstructDestruct::construct (nelem, addr);
}


/**
 * Destruct an array of objects of type @a T.
 * @tparam T       Pointed-to type.
 * @param  nelem   Number of elements in the array
 * @param  addr    Address to be destructed
 */
template<typename T>
inline void
jeod_alloc_destruct_array (
   std::size_t nelem,
   void * addr)
{
   typedef JeodAllocHelperConstructDestruct <
              T,
              std::is_class<T>::value,
              std::is_abstract<T>::value>
      ConstructDestruct;
   ConstructDestruct::destruct (nelem, addr);
}


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
