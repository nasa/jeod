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
 * @file models/environment/ephemerides/ephem_item/src/ephem_point.cc
 * Define member functions for the EphemPoint class.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((ephem_point.cc)
   (ephem_item.cc)
   (utils/message/src/message_handler.cc))



*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "environment/ephemerides/ephem_interface/include/ephem_interface.hh"
#include "environment/ephemerides/ephem_interface/include/ephem_messages.hh"
#include "environment/ephemerides/ephem_manager/include/ephem_manager.hh"
#include "environment/planet/include/base_planet.hh"
#include "utils/math/include/vector3.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/named_item/include/named_item.hh"

// Model includes
#include "../include/ephem_point.hh"



//! Namespace jeod
namespace jeod {

/**
 * Construct an ephemeris point.
 */
EphemerisPoint::EphemerisPoint (
   void)
:
   EphemerisItem()
{
   ; // Empty
}


/**
 * Destroy an ephemeris point.
 */
EphemerisPoint::~EphemerisPoint (
   void)
{
   ; // Empty; object does not allocate memory
}


/**
 * Set active status to correspond with that of the inertial frame.
 * \param[in] frame Frame whose status has changed
 */
void
EphemerisPoint::note_frame_status_change (
   RefFrame * frame)
{

   // The frame had better be the inertial frame.
   if (frame != target_frame) {
      MessageHandler::error (
         __FILE__, __LINE__, EphemeridesMessages::internal_error,
         "Unexpected frame passed to note_frame_status_change");
      return;
   }

   // Make this point have the same activity level as the inertial frame
   // associated with the point.
   if (target_frame->is_active()) {
      activate ();
   } else {
      deactivate ();
   }
}


/**
 * Return the default suffix for this item class, i.e., "inertial".
 * @return Default suffix
 */
const char *
EphemerisPoint::default_suffix (
   void)
const
{
   return "inertial";
}


/**
 * Disconnect the associated inertial frame from the tree.
 */
void
EphemerisPoint::disconnect_from_tree (
   void)
{
   // Disconnect the inertial frame from the reference frame tree.
   // The frame will be reinserted if needed in a (possibly different) tree.
   if (active && (target_frame != nullptr)) {
      target_frame->remove_from_parent();
   }
}


/**
 * Zero-out the inertial frame's translational state.
 */
inline void
EphemerisPoint::initialize_state (
   void)
{
   RefFrameTrans & trans = target_frame->state.trans;

   Vector3::initialize (trans.position);
   Vector3::initialize (trans.velocity);
}


/**
 * Update the inertial frame's translational state.
 * \param[in] position Position wrt parent\n Units: M
 * \param[in] velocity Velocity wrt parent\n Units: M/s
 * \param[in] time Timestamp\n Units: s
 */
void
EphemerisPoint::update (
   const double * position,
   const double * velocity,
   double time)
{
   RefFrameTrans & trans (target_frame->state.trans);

   Vector3::copy (position, trans.position);
   Vector3::copy (velocity, trans.velocity);
   update_time = time;
   target_frame->set_timestamp (time);
}


/**
 * Update the inertial frame's translational state.
 * \param[in] position Position wrt parent\n Units: M
 * \param[in] velocity Velocity wrt parent\n Units: M/s
 * \param[in] scale Scale factor
 * \param[in] time Timestamp\n Units: s
 */
void
EphemerisPoint::update_scaled (
   const double * position,
   const double * velocity,
   double scale,
   double time)
{
   RefFrameTrans & trans (target_frame->state.trans);

   Vector3::scale (position, scale, trans.position);
   Vector3::scale (velocity, scale, trans.velocity);
   update_time = time;
   target_frame->set_timestamp (time);
}


/**
 * Specify the aspect of the target frame updated by the object.
 * EphemerisPoint objects update the translational state.
 * @return Target of object
 */
EphemerisItem::TargetAspect
EphemerisPoint::updates_what (
   void)
const
{
   return EphemerisItem::Translation;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 * @}
 */
