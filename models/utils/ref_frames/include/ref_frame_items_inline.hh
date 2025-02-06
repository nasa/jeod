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
 * @addtogroup RefFrames
 * @{
 *
 * @file models/utils/ref_frames/include/ref_frame_items_inline.hh
 * Define inline functions for the RefFrameItems::Items.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))



*******************************************************************************/

#ifndef JEOD_REF_FRAME_ITEMS_INLINE_HH
#define JEOD_REF_FRAME_ITEMS_INLINE_HH

#include "ref_frame_items.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Get the value of a RefFrameItems.
 * @return Current value
 */
inline RefFrameItems::Items RefFrameItems::get() const
{
    return (value);
}

/**
 * Determine if specified aspects of a RefFrameItems are set.
 * @return Are specified items set?
 * \param[in] test_items Test items
 */
inline bool RefFrameItems::contains(RefFrameItems::Items test_items) const
{
    return ((static_cast<unsigned int>(value) & static_cast<unsigned int>(test_items)) ==
            static_cast<unsigned int>(test_items));
}

/**
 * Determine whether a RefFrameItems equals the specified aspects.
 * @return Exact equality?
 * \param[in] test_items Test items
 */
inline bool RefFrameItems::equals(RefFrameItems::Items test_items) const
{
    return (value == test_items);
}

/**
 * Determine whether a RefFrameItems has nothing set.
 * @return Nothing set?
 */
inline bool RefFrameItems::is_empty() const
{
    return (value == No_Items);
}

/**
 * Determine whether a RefFrameItems has all bits set.
 * @return Fully set?
 */
inline bool RefFrameItems::is_full() const
{
    return (value == Pos_Vel_Att_Rate);
}

/**
 * Set the value of a RefFrameItems.
 * @return Updated value
 * \param[in] new_value New value
 */
inline RefFrameItems::Items RefFrameItems::set(RefFrameItems::Items new_value)
{
    value = new_value;
    return (value);
}

/**
 * Set aspects of a RefFrameItems.
 * @return Updated value
 * \param[in] new_items Items to add
 */
inline RefFrameItems::Items RefFrameItems::add(RefFrameItems::Items new_items)
{
    value = static_cast<RefFrameItems::Items>(static_cast<unsigned int>(value) | static_cast<unsigned int>(new_items));
    return (value);
}

/**
 * Clear aspects of a RefFrameItems.
 * @return Updated value
 * \param[in] old_items Items to remove
 */
inline RefFrameItems::Items RefFrameItems::remove(RefFrameItems::Items old_items)
{
    value = static_cast<RefFrameItems::Items>(
        static_cast<unsigned int>(value) &
        ((~static_cast<unsigned int>(old_items)) & static_cast<unsigned int>(Pos_Vel_Att_Rate)));
    return (value);
}

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
