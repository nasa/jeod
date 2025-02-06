/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup RefFrames
 * @{
 *
 * @file models/utils/ref_frames/src/subscription.cc
 * Define non-inlined methods for the Subscription class.
 */

/*******************************************************************************
  Purpose:
    ()


*******************************************************************************/

// JEOD includes
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/ref_frame_messages.hh"
#include "../include/subscription.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Activate a Subscription object.
 *
 * \par Assumptions and Limitations
 *  - Activation is valid for this object.
 */
void Subscription::activate()
{
    if(mode == Subscribe)
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              RefFrameMessages::subscription_error,
                              "Object %p is in Subscription mode. activate() call ignored.",
                              this);
        return;
    }

    if(mode == Detect)
    {
        mode = Activate;
    }

    if(!active)
    {
        set_active_status(true);
    }
}

/**
 * Deactivate a Subscription object.
 *
 * \par Assumptions and Limitations
 *  - Activation is valid for this object.
 */
void Subscription::deactivate()
{
    if(mode == Subscribe)
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              RefFrameMessages::subscription_error,
                              "Object %p is in Subscription mode. deactivate() call ignored.",
                              this);
        return;
    }

    if(mode == Detect)
    {
        mode = Activate;
    }

    if(active)
    {
        set_active_status(false);
    }
}

/**
 * Add a subscription to a Subscription object.
 *
 * \par Assumptions and Limitations
 *  - Subscription is valid for this object.
 */
void Subscription::subscribe()
{
    if(mode == Activate)
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              RefFrameMessages::subscription_error,
                              "Object %p is in Activate mode. subscribe() call ignored.",
                              this);
        return;
    }

    if(mode == Detect)
    {
        mode = Subscribe;
    }

    ++subscribers;

    if(!active)
    {
        set_active_status(true);
    }
}

/**
 * Remove a subscription to a Subscription object.
 *
 * \par Assumptions and Limitations
 *  - Subscription is valid for this object.
 */
void Subscription::unsubscribe()
{
    if(mode == Activate)
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              RefFrameMessages::subscription_error,
                              "Object %p is in Activate mode. unsubscribe() call ignored.",
                              this);
        return;
    }

    if(mode == Detect)
    {
        mode = Subscribe;
    }

    if(subscribers > 0)
    {
        --subscribers;
    }
    else
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              RefFrameMessages::subscription_error,
                              "Object %p has no subscribers. Subscriptions may be invalid.",
                              this);
    }

    if((subscribers == 0) && active)
    {
        set_active_status(false);
    }
}

/**
 * Set the active data member to the provided value.
 * \param[in] value New active value
 */
void Subscription::set_active_status(bool value)
{
    active = value;
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
