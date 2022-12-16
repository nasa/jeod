//=============================================================================
// Notices:
//
// Copyright © 2022 United States Government as represented by the Administrator
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
 * @addtogroup Environment
 * @{
 * @addtogroup Ephemerides
 * @{
 * @addtogroup EphemItem
 * @{
 *
 * @file models/environment/ephemerides/ephem_item/include/ephem_item_inline.hh
 * Define inline methods for the EphemerisItem class.
 */

/*******************************************************************************

Purpose:
  ()



*******************************************************************************/


#ifndef JEOD_EPHEM_ITEM_INLINE_HH
#define JEOD_EPHEM_ITEM_INLINE_HH


// JEOD includes
#ifndef JEOD_EPHEM_ITEM_HH
#include "ephem_item.hh"
#endif


//! Namespace jeod
namespace jeod {

/**
 * Return the name.
 * @return Void
 */
inline const char *
EphemerisItem::get_name (
   void)
const
{
   return name;
}


/**
 * Set the update time of this item.
 * \param[in] time Time\n Units: s
 */
inline void
EphemerisItem::set_timestamp (
   double time)
{
   update_time = time;
   return;
}


/**
 * Return the update time of this item.
 * @return Time of last update\n Units: s
 */
inline double
EphemerisItem::timestamp (
   void)
const
{
   return update_time;
}


/**
 * Return enabled status.
 * @return Is item enabled?
 */
inline bool
EphemerisItem::is_enabled (
   void)
const
{
   return enabled;
}


/**
 * Deactivate a EphemerisItem object.
 */
inline void
EphemerisItem::deactivate (
   void)
{
   active = false;
   return;
}


/**
 * Return activity status.
 * @return Is item active?
 */
inline bool
EphemerisItem::is_active (
   void)
const
{
   return active;
}


/**
 * Set the owner of this item.
 * \param[in] new_owner New owner
 */
inline void
EphemerisItem::set_owner (
   EphemerisInterface * new_owner)
{
   owner = new_owner;
}


/**
 * Return the owner of this item.
 * @return Frame owner
 */
inline EphemerisInterface *
EphemerisItem::get_owner (
   void)
const
{
   return owner;
}


/**
 * Set the manager of this item.
 * \param[in] new_manager New owner
 */
inline void
EphemerisItem::set_manager (
   BaseEphemeridesManager * new_manager)
{
   manager = new_manager;
   return;
}


/**
 * Return the manager of this item.
 * @return Object manager
 */
inline BaseEphemeridesManager *
EphemerisItem::get_manager (
   void)
const
{
   return manager;
}


/**
 * Set the head item.
 * \param[in,out] head_item Root item
 */
inline void
EphemerisItem::set_head (
   EphemerisItem * head_item)
{
   head = head_item;
   return;
}


/**
 * Get the head item.
 * @return Root item
 */
inline EphemerisItem *
EphemerisItem::get_head (
   void)
const
{
   return head;
}


/**
 * Set the next item.
 * \param[in,out] next_item Next item
 */
inline void
EphemerisItem::set_next (
   EphemerisItem * next_item)
{
   next = next_item;
   return;
}


/**
 * Get the next item.
 * @return Next item
 */
inline EphemerisItem *
EphemerisItem::get_next (
   void)
const
{
   return next;
}


/**
 * Get the target frame.
 * @return Target frame
 */
inline EphemerisRefFrame *
EphemerisItem::get_target_frame (
   void)
const
{
   return target_frame;
}


/**
 * Get the item marked as enabled, if any.
 * @return Enabled item
 */
inline EphemerisItem *
EphemerisItem::get_enabled_item (
   void)
const
{
   EphemerisItem * enabled_item;
   for (enabled_item = head;
        (enabled_item != NULL) && (! enabled_item->enabled);
        enabled_item = enabled_item->next) {
      ; // Null body
   }
   return enabled_item;
}


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
