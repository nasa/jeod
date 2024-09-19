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
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynBody
 * @{
 *
 * @file models/dynamics/dyn_body/include/torque_inline.hh
 * Define the JEOD torque model.
 */

/*******************************************************************************
Purpose:
  ()



*******************************************************************************/

#ifndef JEOD_TORQUE_INLINE_HH
#define JEOD_TORQUE_INLINE_HH

#include "torque.hh"

#include <cstddef>

//! Namespace jeod
namespace jeod
{

/**
 * Access a torque element, non-const version.
 * @return Torque component at specified index\n Units: NM
 * \param[in] index Index number
 */
inline double & Torque::operator[](const unsigned int index)
{
    return torque[index];
}

/**
 * Access a torque element, const version.
 * @return Torque component at specified index\n Units: NM
 * \param[in] index Index number
 */
inline double Torque::operator[](const unsigned int index) const
{
    return torque[index];
}

/**
 * A torque is active if it has a non-null torque vector and
 * the active pointer is null or the pointed-to boolean is true.
 * @return Is the torque active?
 */
inline bool CollectTorque::is_active() const
{
    return ((torque != nullptr) && ((active == nullptr) || *active));
}

/**
 * Access a torque element, non-const version.
 * @return Torque component at specified index\n Units: N
 * \param[in] index Index number
 */
inline double & CollectTorque::operator[](const unsigned int index)
{
    return torque[index];
}

/**
 * Access a torque element, const version.
 * @return Torque component at specified index\n Units: N
 * \param[in] index Index number
 */
inline double CollectTorque::operator[](const unsigned int index) const
{
    return torque[index];
}

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
