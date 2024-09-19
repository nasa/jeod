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
 * @file models/utils/memory/include/memory_manager_hide_from_trick.hh
 * Trick doesn't understand these.
 * This file is included from the private part of memory_manager.hh.
 * The types are private and the corresponding members hidden from Trick.
 * These will be folded into memory_manager.hh when Trick ICG, both
 * Trick 7 and Trick 10, understands these or provides a common mechanism
 * for telling ICG to ignore content.
 */

/*******************************************************************************

Purpose:
  ()

*******************************************************************************/

#ifndef JEOD_MEMORY_MANAGER_HIDE_FROM_TRICK_HH
#define JEOD_MEMORY_MANAGER_HIDE_FROM_TRICK_HH

#include "memory_item.hh"
#include "memory_table.hh"
#include "memory_type.hh"
#include <map>

//! Namespace jeod
namespace jeod
{

/**
 * An AllocTable maps memory addresses to memory descriptions.
 */
using AllocTable = std::map<const void *, JeodMemoryItem>;

/**
 * The type type itself is a memory table with copy implemented by clone().
 */
using TypeTable = JeodMemoryTableClonable<JeodMemoryTypeDescriptor>;

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
