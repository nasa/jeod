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
 * @file models/environment/ephemerides/ephem_item/src/ephem_orient.cc
 * Define member functions for the EphemItem class and subclasses.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((ephem_orient.cc)
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
#include "../include/ephem_orient.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Specify the aspect of the target frame updated by the object.
 * EphemerisOrientation objects update the rotational state.
 * @return Target of object
 */
EphemerisItem::TargetAspect EphemerisOrientation::updates_what() const
{
    return EphemerisItem::Rotation;
}

/**
 * Enable a EphemerisItem object.
 */
void EphemerisOrientation::enable()
{
    if(!enabled)
    {
        auto * enabled_item = dynamic_cast<EphemerisOrientation *>(get_enabled_item());

        if((enabled_item != nullptr) && enabled_item->subscribed_to_inertial)
        {
            enabled_item->subscribed_to_inertial = false;
            subscribed_to_inertial = true;
        }
        else
        {
            subscribed_to_inertial = false;
        }

        EphemerisItem::enable();
    }
}

/**
 * Null implementation.
 * \param[in] frame Frame whose status has changed
 */
void EphemerisOrientation::note_frame_status_change(RefFrame * frame)
{
    auto * inertial = const_cast<RefFrame *>(frame->get_parent());

    // The frame had better be the planet-fixed frame.
    if(frame != target_frame)
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              EphemeridesMessages::internal_error,
                              "Unexpected frame passed to note_frame_status_change");
        return;
    }

    // Planet-centered frame is active:
    // Activate this object and issue a subscription if needed.
    if(target_frame->is_active())
    {
        activate();

        if(!subscribed_to_inertial)
        {
            subscribed_to_inertial = true;
            inertial->subscribe();
        }
    }

    // Planet-centered frame is not active:
    // Deactivate and retract subscription.
    else
    {
        deactivate();

        if(subscribed_to_inertial)
        {
            subscribed_to_inertial = false;
            inertial->unsubscribe();
        }
    }
}

/**
 * Return the default suffix for this item class, i.e., "pfix".
 * @return Default suffix
 */
std::string EphemerisOrientation::default_suffix() const
{
    return "pfix";
}

/**
 * Disconnect the item from the tree; this is a no-op for an
 * EphemerisOrientation.
 */
void EphemerisOrientation::disconnect_from_tree()
{
    ; // No-op
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 * @}
 */
