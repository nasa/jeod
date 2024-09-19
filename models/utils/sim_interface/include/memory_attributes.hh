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
 * @file models/utils/sim_interface/include/memory_attributes.hh
 * Define JEOD memory interface macros.
 * - Most of the memory interface between JEOD and the simulation engine is
 *   handled by the JeodMemoryInterface.
 * - The macros defined in this file represent the functionality that cannot be
 *   solved using c++ classes.
 * - The macros prefixed with JEOD_DECLARE are used in model files that use the
 *   memory model to allocate memory.
 * - The remaining macros are used internally by the JEOD memory model and
 *   should not be used in model files.
 */

/*******************************************************************************

Purpose:
  ()



*******************************************************************************/

#ifndef JEOD_MEMORY_ATTRIBUTES_HH
#define JEOD_MEMORY_ATTRIBUTES_HH

// Get the JEOD configuration.
#include "config.hh"

// Some installations may not be able to fit within the confines of the main
// body of this header. Include the alternate implementation if defined.
#ifdef JEOD_ATTRIBUTES_IMPLEMENTATION_HEADER
#include JEOD_ATTRIBUTES_IMPLEMENTATION_HEADER // cppcheck-suppress preprocessorErrorDirective

// The the main body of this header is within this #else.
#else

// Simulation engine attributes definition file
#ifdef JEOD_ATTRIBUTES_SIM_ENGINE_HEADER
#include JEOD_ATTRIBUTES_SIM_ENGINE_HEADER
#endif

// JEOD includes
// #include "simulation_interface.hh"
// #include "memory_interface.hh"

//! Namespace jeod
namespace jeod
{

class JeodSimulationInterface;

/**
 * JEOD_DECLARE_ATTRIBUTES(class_name)
 * This macro is obsolete.
 */
#define JEOD_DECLARE_ATTRIBUTES(class_name)

/**
 * \def JEOD_ATTRIBUTES(type)
 *   Get a pointer to or construct the name of the attributes for the type.
 *   @note This is a primitive macro. Do not use it in model files.
 * \param type    Data type.
 * \return        Pointer to or symbolic name of the attributes for the type.
 */
#define JEOD_ATTRIBUTES(type) JeodSimulationInterface::get_memory_interface().find_attributes(#type)

} // namespace jeod

#endif

#endif

/**
 * @}
 * @}
 * @}
 */
