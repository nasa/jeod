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
 * @addtogroup Memory
 * @{
 *
 * @file models/utils/memory/include/jeod_alloc.hh
 * Define JEOD memory allocation macros.
 *
 * The jeod_alloc.hh memory macros can be viewed as
 *  - Being externally-usable or for internal use only.@n
 *    The supported use of the JEOD memory model is via those macros
 *    advertised as externally-usable. These externally-usable macros expand
 *    into invocations of internal macros, which in turn expand into calls to
 *    methods of classes defined in the memory model.
 *    Those macros marked as internal are for internal use only by this file.
 *  - Supporting allocation versus deletion.@n
 *    Some of the jeod_alloc.hh memory macros allocate memory while others
 *    delete it. With one exception, the allocation/delete nature of a macro
 *    is explicit in the macro name. Allocation macros start with JEOD_ALLOC.
 *    Macros that address deleting memory start with JEOD_DELETE.
 *  - Operating on objects versus arrays.@n
 *    The memory management macros come in two basic forms: ARRAY and OBJECT.
 *    Memory allocated with an ARRAY allocator macro must be freed with
 *    JEOD_DELETE_ARRAY. Memory allocated with an OBJECT allocator macro
 *    must be freed with JEOD_DELETE_OBJECT. This corresponds to the C++
 *    distiction between operator new[], delete[], new, and delete.
 *  - Operating on structured versus non-structured data.@n
 *    The JEOD memory model registers allocated memory with the underlying
 *    simulation engine (e.g., Trick). To make the data in a structured type
 *    visible to the engine, the user must declare an external reference to
 *    the engine's description of the type.
 *    For example, to allocate an instance of some class Foo using the
 *    default constructor use@code
 *      JEOD_DECLARE_ATTRIBUTES (Foo)
 *      ...
 *       Foo * foo_obj = JEOD_ALLOC_CLASS (Foo, ());@endcode
 *    See \ref JEOD_DECLARE_ATTRIBUTES.
 *
 * Two compile -D options affect the behavior of these macros. These are
 *  - JEOD_MEMORY_DEBUG - The memory model debugging level.
 *    The debugging level ranges from 0 (off) to 3 (all transactions).
 *    If this is not set in the compile flags the value is set to 0 (off).
 *  - JEOD_MEMORY_GUARD - Guards will be added around allocated memory
 *    if this option is defined and has a non-zero value.
 */

/*******************************************************************************

Purpose:
  ()



*******************************************************************************/

#ifndef JEOD_ALLOC_HH
#define JEOD_ALLOC_HH

// System includes
#include <cstddef>
#include <new>

// JEOD includes
#include "utils/sim_interface/include/memory_attributes.hh"

// Model includes
#include "jeod_alloc_get_allocated_pointer.hh"
#include "memory_manager.hh"

/**
 * \defgroup external Externally-usable macros
 *    The supported use of the JEOD memory model is via those macros
 *    advertised as externally-usable. These externally-usable macros expand
 *    into invocations of internal macros, which in turn expand into calls to
 *    methods of classes defined in the memory model.
 *
 * \defgroup internal Internal macros
 *    The internal macros act as the bridge between the externally-usable
 *    memory model macros and the publicly-visible memory model class methods.
 *
 * \defgroup classes  Support classes
 *    The memory model classes are the workhorses of the JEOD memory model.
 */

/**
 * \ingroup external
 * \def JEOD_MEMORY_DEBUG
 *   Specifies the level of checking performed by the JEOD memory model.\n
 *   0 - JEOD memory management off\n
 *   1 - Error checking only\n
 *   2 - Summary checking\n
 *   3 - Blow-by-block account of allocation, deallocation.
 */
#ifndef JEOD_MEMORY_DEBUG
#define JEOD_MEMORY_DEBUG 2
#endif

/*******************************************************************************
 * Internal memory management macros
 ******************************************************************************/

/**
 * \addtogroup internal
 * @{
 */

/**
 * \def JEOD_ALLOC_OBJECT_FILL
 *   Fill pattern for non-primitive types.
 * \details
 *   This is a nasty fill pattern that forces JEOD developers to write
 *   constructors that initialize every element of a class.
 */
#define JEOD_ALLOC_OBJECT_FILL 0xdf

/**
 * \def JEOD_ALLOC_PRIMITIVE_FILL
 *   Fill pattern for primitive types.
 * \details
 *   Primitive types are initialized to all zero.
 */
#define JEOD_ALLOC_PRIMITIVE_FILL 0

/**
 * \def JEOD_ALLOC_POINTER_FILL
 *   Fill pattern for pointer types.
 * \details
 *   Pointer types are initialized to null pointers.
 *   Note that this may change in the future. JEOD developers are strongly
 *   encouraged to initialize pointer arrays after allocating them.
 */
#define JEOD_ALLOC_POINTER_FILL 0

/**
 * \def JEOD_CREATE_MEMORY (is_array,nelem,fill,tentry)
 *   Allocate and register memory to be populated via placement new.
 * \param is_array Allocated as an array?
 * \param nelem    Size of the array.
 * \param fill     Fill pattern.
 * \param tentry   JEOD type descriptor entry.
 */
#define JEOD_CREATE_MEMORY(is_array, nelem, fill, tentry)                                                              \
    jeod::JeodMemoryManager::create_memory(is_array, nelem, fill, tentry, __FILE__, __LINE__)

/**
 * \def JEOD_ALLOC_ARRAY_INTERNAL(type,nelem,fill,tentry)
 *   Allocate nelem elements of pointers to the specified structured type.
 * \param type     Data type.
 * \param nelem    Size of the array.
 * \param fill     Fill pattern.
 * \param fill     Fill pattern.
 * \param tentry   JEOD type descriptor entry.
 */
#define JEOD_ALLOC_ARRAY_INTERNAL(type, nelem, fill, tentry)                                                           \
    new(JEOD_CREATE_MEMORY(true, nelem, fill, tentry)) type[nelem]

/**
 * \def JEOD_ALLOC_OBJECT_INTERNAL(type,fill,constr,tentry)
 *   Allocate an instance of the specified class using the specified constructor
 *   arguments.
 * \param type     Data type.
 * \param fill     Fill pattern.
 * \param constr   Constructor arguments, enclosed in parentheses.
 * \param tentry   JEOD type descriptor entry.
 */
#define JEOD_ALLOC_OBJECT_INTERNAL(type, fill, constr, tentry)                                                         \
    new(JEOD_CREATE_MEMORY(false, 1, fill, tentry)) type constr

/**
 * \def JEOD_DELETE_INTERNAL(ptr,is_array)
 *   Free memory allocated with some JEOD_ALLOC macro.
 * \details
 *   Depends on @verbatim dynamic_cast<void*>(ptr) @endverbatim yielding
 *   a pointer to the most derived object pointed to by @a ptr.
 *   See ISO/IEC 14882:2003 section 5.2.7.7.
 * \param ptr      Memory to be released.
 * \param is_array True for DELETE_ARRAY, false for DELETE_OBJECT.
 */
#define JEOD_DELETE_INTERNAL(ptr, is_array)                                                                            \
    do                                                                                                                 \
    {                                                                                                                  \
        if(ptr != nullptr && JEOD_IS_ALLOCATED(ptr))                                                                   \
        {                                                                                                              \
            jeod::JeodMemoryManager::destroy_memory(jeod::jeod_alloc_get_allocated_pointer(ptr),                       \
                                                    is_array,                                                          \
                                                    __FILE__,                                                          \
                                                    __LINE__);                                                         \
            ptr = nullptr;                                                                                             \
        }                                                                                                              \
    } while(0)

/**
 * @}
 */

/******************************************************************************
 * JEOD externally-usable memory management macros
 *****************************************************************************/

/**
 * \addtogroup external
 * @{
 */

/**
 * \def JEOD_REGISTER_CLASS (type)
 *   Register the type @a type with the memory manager.
 * \param type      Data type (C token, not a string).
 */
#define JEOD_REGISTER_CLASS(type)                                                                                      \
    jeod::JeodMemoryManager::register_class(jeod::JeodMemoryTypePreDescriptorDerived<type>(true).get_ref())

// Deprecated.
/**
 * \def JEOD_REGISTER_INCOMPLETE_CLASS (type)
 *   Register the incomplete class @a type with the memory manager.
 *   The type mechanism now does the "right thing" with types.
 *   This macro is deprecated.
 * \param type      Data type (C token, not a string).
 */
#define JEOD_REGISTER_INCOMPLETE_CLASS(type) JEOD_REGISTER_CLASS(type)

/**
 * \def JEOD_REGISTER_NONEXPORTED_CLASS (type)
 *   Register the type @a type with the memory manager,
 *   but with the class marked as not exportable to the simulation engine.
 *   Instances of a non-exported class allocated with JEOD_ALLOC_xxx
 *   will not be registered with the simulation engine.
 * \param type      Data type (C token, not a string).
 */
#define JEOD_REGISTER_NONEXPORTED_CLASS(type)                                                                          \
    jeod::JeodMemoryManager::register_class(jeod::JeodMemoryTypePreDescriptorDerived<type>(false).get_ref())

/**
 * \def JEOD_REGISTER_CHECKPOINTABLE (owner,elem_name)
 *   Register the data member @a elem_name of the @a owner
 *   as a Checkpointable object.
 *
 *   <b>Assumptions and Limitations:</b>
 *    - The @a owner must be a pointer, typically this.
 *    - The @a owner class must have been registered with the memory manager
 *      via JEOD_REGISTER_CLASS.
 *    - The @a elem_name must identify a data member of the @a owner
 *      whose type derives from Checkpointable.
 *
 * \param owner      The object that contains the Checkpointable object.
 * \param elem_name  The Checkpointable object.
 */
#define JEOD_REGISTER_CHECKPOINTABLE(owner, elem_name)                                                                 \
    jeod::JeodMemoryManager::register_container(jeod::jeod_alloc_get_allocated_pointer(owner),                         \
                                                typeid(*owner),                                                        \
                                                #elem_name,                                                            \
                                                elem_name)

/**
 * \def JEOD_DEREGISTER_CHECKPOINTABLE (owner,elem_name)
 *   Register the data member @a elem_name of the @a owner
 *   as a Checkpointable object.
 *
 *   <b>Assumptions and Limitations:</b>
 *    - The @a owner must be a pointer, typically this.
 *    - The @a owner class must have been registered with the memory manager
 *      via JEOD_REGISTER_CLASS.
 *    - The @a elem_name must identify a data member of the @a owner
 *      whose type derives from Checkpointable.
 *
 * \param owner      The object that contains the Checkpointable object.
 * \param elem_name  The Checkpointable object.
 */
#define JEOD_DEREGISTER_CHECKPOINTABLE(owner, elem_name)                                                               \
    jeod::JeodMemoryManager::deregister_container(jeod::jeod_alloc_get_allocated_pointer(owner),                       \
                                                  typeid(*owner),                                                      \
                                                  #elem_name,                                                          \
                                                  elem_name)

// Not needed yet.
#if 0
/**
 * \def JEOD_REGISTER_FUNCTION_POINTER (owner,elem_name)
 *   Register the data member @a elem_name of the @a owner
 *   as a function pointer.
 *
 *   <b>Assumptions and Limitations:</b>
 *    - The @a owner must be a pointer, typically just this.
 *    - The @a owner class must have been registered with the memory manager
 *      via JEOD_REGISTER_CLASS.
 *    - The @a elem_name must identify a data member of the @a owner
 *      that is a globally-visible function pointer.
 *    - Casting a function pointer to void * is perfectly safe.
 *      Actually, it's not safe at all. It is undefined behavior.
 *      This works fine in any POSIX-compliant machine but
 *       probably won't work on a Harvard architecture machine.
 *
 * \param owner      The object that contains the function pointer.
 * \param elem_name  The function pointer.
 */
#define JEOD_REGISTER_FUNCTION_POINTER(owner, elem_name)                                                               \
    jeod::JeodMemoryManager::register_function_pointer(reinterpret_cast<void *>(owner),                                \
                                                       typeid(*owner),                                                 \
                                                       #elem_name,                                                     \
                                                       reinterpret_cast<char **>(&elem_name),                          \
                                                       reinterpret_cast<void *>(elem_name))
#endif

/**
 * \def JEOD_ALLOC_CLASS_MULTI_POINTER_ARRAY(nelem,type,asters)
 *   Allocate an array of @a nelem multi-level pointers to the specified
 *   @a type. The @a asters are asterisks that specify the pointer level.
 *   The allocated memory is initialized via @c new.
 * \return         Allocated array of specified type.
 * \param nelem    Size of the array.
 * \param type     The underlying type, which must be a structured type.
 * \param asters   A bunch of asterisks.
 * \par Example:
 *   <tt>
 *   Foo *** foo_array = JEOD_ALLOC_CLASS_MULTI_POINTER_ARRAY(2,Foo,**);
 *   </tt>\n\n
 *   This allocates two pointers-to-pointers to the class Foo.
 *   Note that this does not allocate either the Foo objects or pointers to
 *   the Foo objects.
 */
#define JEOD_ALLOC_CLASS_MULTI_POINTER_ARRAY(nelem, type, asters)                                                      \
    JEOD_ALLOC_ARRAY_INTERNAL(type asters, nelem, JEOD_ALLOC_POINTER_FILL, JEOD_REGISTER_CLASS(type asters))

/**
 * \def JEOD_ALLOC_CLASS_POINTER_ARRAY(nelem,type)
 *   Allocate an array of @a nelem pointers to the specified @a type.
 *   The allocated memory is initialized via @c new.
 * \return         Allocated array of specified type.
 * \param nelem    Size of the array.
 * \param type     The underlying type, which must be a structured type.
 * \par Example:
 *   <tt>
 *   Foo ** foo_array = JEOD_ALLOC_CLASS_POINTER_ARRAY(2,Foo);
 *   </tt>\n\n
 *   This allocates two pointers to the class Foo.
 *   Note that this does not allocate the Foo objects themselves.
 */
#define JEOD_ALLOC_CLASS_POINTER_ARRAY(nelem, type) JEOD_ALLOC_CLASS_MULTI_POINTER_ARRAY(nelem, type, *)

/**
 * \def JEOD_ALLOC_CLASS_ARRAY(nelem,type)
 *   Allocate an array of @a nelem instances of the specified structured
 *   @a type. The default constructor is invoked to initialize each
 *   allocated object.
 * \return         Allocated array of specified type.
 * \param nelem    Size of the array.
 * \param type     The underlying type, which must be a structured type.
 * \par Example:
 *   <tt>
 *   Foo ** foo_array = JEOD_ALLOC_CLASS_ARRAY(2,Foo);
 *   </tt>\n\n
 *   This allocates two objects of the class Foo.
 */
#define JEOD_ALLOC_CLASS_ARRAY(nelem, type)                                                                            \
    JEOD_ALLOC_ARRAY_INTERNAL(type, nelem, JEOD_ALLOC_OBJECT_FILL, JEOD_REGISTER_CLASS(type))

/**
 * \def JEOD_ALLOC_PRIM_ARRAY(nelem,type)
 *   Allocate @a nelem elements of the specified primitive @a type.
 *   The allocated array is zero-filled.
 * \return         Allocated array of specified type.
 * \param nelem    Size of the array.
 * \param type     The underlying type, which must be a C++ primitive type.
 * \par Example:
 *   <tt>
 *   double * double_array = JEOD_ALLOC_PRIM_ARRAY(2,double);
 *   </tt>\n\n
 *   This allocates an array of two doubles.
 */
#define JEOD_ALLOC_PRIM_ARRAY(nelem, type)                                                                             \
    JEOD_ALLOC_ARRAY_INTERNAL(type, nelem, JEOD_ALLOC_PRIMITIVE_FILL, JEOD_REGISTER_CLASS(type))

/**
 * \def JEOD_ALLOC_CLASS_OBJECT(type,constr)
 *   Allocate @b one instance of the specified class. The supplied
 *   constructor arguments, @a constr, are used as arguments to
 *   @c new.
 *   The default constructor will be invoked if the @a constr
 *   argument is the empty list; a non-default constructor will be
 *   invoked for a non-empty list.
 * \return         Pointer to allocated object.
 * \param type     The underlying type, which must be a structured type.
 * \param constr   Constructor arguments, enclosed in parentheses.
 * \par Example:
 *   <tt>
 *   Foo * foo = JEOD_ALLOC_CLASS_OBJECT(Foo,(bar,baz));
 *   </tt>\n\n
 *   This allocates a new object of type Foo, invoking the
 *   <tt>Foo::Foo(bar,baz)</tt> constructor.
 */
#define JEOD_ALLOC_CLASS_OBJECT(type, constr)                                                                          \
    JEOD_ALLOC_OBJECT_INTERNAL(type, JEOD_ALLOC_OBJECT_FILL, constr, JEOD_REGISTER_CLASS(type))

/**
 * \def JEOD_ALLOC_PRIM_OBJECT(type, initial)
 *    Allocate @b one instance of the specified @a type.
 *    The object is initialized with the supplied @a initial value.
 * \return         Pointer to allocated primitive.
 * \param type     The underlying type, which must be a C++ primitive type.
 * \param initial  Initial value.
 * \par Example:
 *   <tt>
 *   double * foo = JEOD_ALLOC_PRIM_OBJECT(double, 3.14159265358979323846);
 *   </tt>\n\n
 *   This allocates a double and initializes it to pi.
 */
#define JEOD_ALLOC_PRIM_OBJECT(type, initial)                                                                          \
    JEOD_ALLOC_OBJECT_INTERNAL(type, JEOD_ALLOC_PRIMITIVE_FILL, (initial), JEOD_REGISTER_CLASS(type))

/**
 * \def JEOD_IS_ALLOCATED(ptr)
 *   Determine if @a ptr was allocated by some
 *   <tt>JEOD_ALLOC_xxx_ARRAY</tt> macro.
 * \return        true if @a ptr was allocated by this module, false otherwise.
 * \param ptr     Memory to be checked.
 * \par Example:
 *   <tt>
 *   char * name;\n
 *   ...\n
 *   if (JEOD_IS_ALLOCATED(name)) {\n
 *      JEOD_DELETE_ARRAY(name);\n
 *      name = NULL;\n
 *   }
 *   </tt>\n\n
 *   The above deletes the memory at @a name, but only if that memory was
 *   allocated by the JEOD memory management module.
 */
#define JEOD_IS_ALLOCATED(ptr)                                                                                         \
    jeod::JeodMemoryManager::is_allocated(jeod::jeod_alloc_get_allocated_pointer(ptr), __FILE__, __LINE__)

/**
 * \def JEOD_DELETE_ARRAY(ptr)
 *   Free memory at @a ptr that was earlier allocated with some
 *   <tt>JEOD_ALLOC_xxx_ARRAY</tt> macro.
 * \param ptr    Memory to be released.
 * \par Example:
 *   <tt>
 *   Foo * foo_array = JEOD_ALLOC_CLASS_ARRAY(2,Foo);\n
 *   ...\n
 *   JEOD_DELETE_ARRAY(foo_array);
 *   </tt>\n\n
 *   The above allocates a chunk of memory and then frees it.
 */
#define JEOD_DELETE_ARRAY(ptr) JEOD_DELETE_INTERNAL(ptr, true)

/**
 * \def JEOD_DELETE_OBJECT(ptr)
 *   Free memory at @a ptr that was earlier allocated with some
 *   <tt>JEOD_ALLOC_xxx_OBJECT</tt> macro.
 * \param ptr    Memory to be released.
 * \par Example:
 *   <tt>
 *   Foo * foo1 = JEOD_ALLOC_CLASS_OBJECT(Foo,());\n
 *   ...\n
 *   JEOD_DELETE_OBJECT(foo1);
 *   </tt>\n\n
 *   The above allocates a chunk of memory and then frees it.
 */
#define JEOD_DELETE_OBJECT(ptr) JEOD_DELETE_INTERNAL(ptr, false)

#define JEOD_DELETE_2D(ptr, size, is_array)                                                                            \
    if(ptr != nullptr)                                                                                                 \
    {                                                                                                                  \
        for(unsigned int ii = 0; ii < size; ++ii)                                                                      \
        {                                                                                                              \
            if(is_array)                                                                                               \
            {                                                                                                          \
                JEOD_DELETE_ARRAY(ptr[ii]);                                                                            \
            }                                                                                                          \
            else                                                                                                       \
            {                                                                                                          \
                JEOD_DELETE_OBJECT(ptr[ii]);                                                                           \
            }                                                                                                          \
        }                                                                                                              \
    }                                                                                                                  \
    JEOD_DELETE_ARRAY(ptr);

/**
 * @}
 */

#endif

/**
 * @}
 * @}
 * @}
 */
