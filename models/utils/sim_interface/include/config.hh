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
 * @addtogroup SimInterface
 * @{
 *
 * @file models/utils/sim_interface/include/config.hh
 * Configure JEOD for use by some simulation engine.
 */

/*******************************************************************************

Purpose:
  ()



*******************************************************************************/

#ifndef JEOD_CONFIG_HH
#define JEOD_CONFIG_HH

// For Trick-based applications,
// Use the version-specific configuration for Trick-based builds.
#if(defined TRICK_VER)
#include "config_trick10.hh"

// Standalone JEOD unit tests use the test harness configuration.
#elif(defined JEOD_UNIT_TEST)
#include "config_test_harness.hh"

#define JEOD_THROW(x)

// FUTURE, if needed: configure for the non-Trick demo capability.

// Non-Trick installations should consolidate requisite configuration
// information in a single header file and compile JEOD with
// JEOD_CONFIG_HEADER defined to be this config file.
#elif(defined JEOD_CONFIG_HEADER)
#include JEOD_CONFIG_HEADER

#else
// We're out of options. Compilation will be hosed.
#endif

// Define macros that enable the package to take advantage of extensions
// to c++, if possible.

#if(!defined SWIG) && ((defined __GNUC__) || (defined __llvm__) || (defined __clang__))

/**
 * @def JEOD_UNUSED
 * Arguments that aren't used should either have no name or
 * be marked as JEOD_UNUSED.
 */
#define JEOD_UNUSED __attribute__((unused))
#ifndef JEOD_THROW
#define JEOD_THROW(x) throw x
#endif

#ifndef JEOD_DEPRECATED
#if defined(__GNUC__) || defined(__clang__)
#define JEOD_DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define JEOD_DEPRECATED __declspec(deprecated)
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define JEOD_DEPRECATED
#endif
#endif

#ifndef JEOD_DEPRECATED
#if defined(__GNUC__) || defined(__clang__)
#define JEOD_DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define JEOD_DEPRECATED __declspec(deprecated)
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define JEOD_DEPRECATED
#endif
#endif

#else
#define JEOD_UNUSED

#ifdef ER7_UTILS_UNUSED
#undef ER7_UTILS_UNUSED
#endif
#define ER7_UTILS_UNUSED

#ifdef ER7_UTILS_RESTRICT
#undef ER7_UTILS_RESTRICT
#endif
#define ER7_UTILS_RESTRICT

#ifdef ER7_UTILS_ALWAYS_INLINE
#undef ER7_UTILS_ALWAYS_INLINE
#endif
#define ER7_UTILS_ALWAYS_INLINE

#endif

// Provide standard defaults for integral types
#ifndef JEOD_SIZE_T
#define JEOD_SIZE_T std::size_t
#endif

#ifndef JEOD_PTRDIFF_T
#define JEOD_PTRDIFF_T ptrdiff_t
#endif

#ifndef JEOD_INTPTR_T
#define JEOD_INTPTR_T intptr_t
#endif

#ifndef JEOD_UINTPTR_T
#define JEOD_UINTPTR_T uintptr_t
#endif

#endif

/**
 * @}
 * @}
 * @}
 */
