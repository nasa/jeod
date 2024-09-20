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
 * @addtogroup SimInterface
 * @{
 *
 * @file models/utils/sim_interface/include/jeod_class.hh
 * Define the JEOD class declaration macros JEOD_MAKE_SIM_INTERFACES and
 * and JEOD_DECLARE_SIM_INTERFACES.
 * All JEOD class definitions must invoke JEOD_MAKE_SIM_INTERFACES within the
 * body of the class. Corresponding invocations of JEOD_DECLARE_SIM_INTERFACES
 * Are made at file scope and in the context of the global namespace.
 *
 * In a Trick environment, these macros gives the Trick input processor,
 * the Trick checkpoint / checkpoint-restart facility, and the ICG-generated
 * io_src file for the header full visibility of the class's contents.
 * The intent is to provide the same capability outside the Trick.
 */

/********************************* TRICK HEADER *******************************

Purpose:
  ()

Assumptions and Limitations:
  (((Inclusion of this file and use of the JEOD_MAKE_SIM_INTERFACES macro
     in each defined mandatory for all JEOD class definition files.)))


*******************************************************************************/

#ifndef JEOD_CLASS_HH
#define JEOD_CLASS_HH

// JEOD includes
#include "config.hh"

// Some installations may not be able to fit within the confines of the main
// body of this header. Include the alternate implementation if defined.
// The alternate implementation must define the macros JEOD_MAKE_SIM_INTERFACES
// and JEOD_DECLARE_SIM_INTERFACES.
#ifdef JEOD_CLASS_IMPLEMENTATION_HEADER
#include JEOD_CLASS_IMPLEMENTATION_HEADER // cppcheck-suppress preprocessorErrorDirective

// The main body of this header is within the body of this #else.
#else

// JEOD_MAKE_SIM_INTERFACES(class_name)
// Three alternate definition mechanisms are provided below:
// - As an alias for JEOD_CLASS_ESTABLISH_FRIENDS if defined,
// - As a wrapper around JEOD_CLASS_FRIEND_CLASS if defined,
// - A null implementation if neither of the above is defined.

/**
 * JEOD_MAKE_SIM_INTERFACES(class_name)
 * Defines friends of the given class. This macro is to be invoked in the body
 * of all JEOD classes. The intent is to make all parts of the class visible to
 * the designated simulation engine classes and functions.
 *
 * @param class_name Name of the class being defined.
 */
#if(defined JEOD_CLASS_ESTABLISH_FRIENDS)
#define JEOD_MAKE_SIM_INTERFACES(...) JEOD_CLASS_ESTABLISH_FRIENDS(__VA_ARGS__);

#elif defined(JEOD_CLASS_FRIEND_CLASS)
#define JEOD_MAKE_SIM_INTERFACES(...) friend class JEOD_CLASS_FRIEND_CLASS;

#else
#define JEOD_MAKE_SIM_INTERFACES(...)

#endif // JEOD_MAKE_SIM_INTERFACES

// JEOD_DECLARE_SIM_INTERFACES(class_name)
// Three alternate definition mechanisms are provided below:
// - As an alias for JEOD_CLASS_DECLARE_FRIENDS if defined,
// - As a wrapper around JEOD_CLASS_FRIEND_CLASS_FORWARD if defined,
// - A null implementation if neither of the above is defined.

/**
 * JEOD_DECLARE_SIM_INTERFACES(class_name)
 * Forward declare classes and external functions needed to make the
 * JEOD_MAKE_SIM_INTERFACES(class_name) expansion compilable.
 * All JEOD files that use JEOD_MAKE_SIM_INTERFACES within classes (will) make a
 * parallel call to this macro at file scope in the global namespace.
 * @param class_name Name of the class defined later in the header in question.
 */
#if(defined JEOD_CLASS_DECLARE_FRIENDS)
#define JEOD_DECLARE_SIM_INTERFACES(class_name) JEOD_CLASS_DECLARE_FRIENDS(class_name)

#elif defined(JEOD_CLASS_FRIEND_CLASS_FORWARD)
#define JEOD_DECLARE_SIM_INTERFACES(class_name) class JEOD_CLASS_FRIEND_CLASS_FORWARD;

#else
#define JEOD_DECLARE_SIM_INTERFACES(class_name)

#endif // JEOD_DECLARE_SIM_INTERFACES

#endif // JEOD_CLASS_IMPLEMENTATION_HEADER

#endif // One-time include

/**
 * @}
 * @}
 * @}
 */
