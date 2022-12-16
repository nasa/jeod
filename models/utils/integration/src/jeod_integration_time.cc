/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup Integration
 * @{
 *
 * @file models/utils/integration/src/jeod_integration_time.cc
 * Define JeodIntegrationTime methods.
 */

/*****************************************************************************
Purpose:
  ()

Library dependencies:
  ((jeod_integration_time.o)
   (integration_messages.o)
   (utils/message/message_handler.o))

 
******************************************************************************/


// System includes
#include <algorithm>
#include <cstddef>

// JEOD includes
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"

// Local includes
#include "../include/integration_messages.hh"
#include "../include/jeod_integration_time.hh"
#include "../include/time_change_subscriber.hh"


//! Namespace jeod
namespace jeod {

/**
 * JeodIntegrationTime constructor.
 */
JeodIntegrationTime::JeodIntegrationTime ()
:
   TimeInterface (),
   time_change_subscribers ()
{
   // Tell the JEOD memory manager about the time change subscribers
   // so that it can be checkpointed and restarted.
   JEOD_REGISTER_CLASS (JeodIntegrationTime);
   JEOD_REGISTER_CLASS (TimeChangeSubscriber);
   JEOD_REGISTER_CHECKPOINTABLE (this, time_change_subscribers);
}

/**
 * JeodIntegrationTime destructor.
 */
JeodIntegrationTime::~JeodIntegrationTime ()
{
   // Tell the JEOD memory manager that the time change subscribers
   // is no longer viable.
   JEOD_DEREGISTER_CHECKPOINTABLE (this, time_change_subscribers);
}


/**
 * Add a time change subscriber.
 * @param subscriber  Object to be added to list of subscribers.
 */
void
JeodIntegrationTime::add_time_change_subscriber (
   TimeChangeSubscriber & subscriber)
{
   if (std::find (time_change_subscribers.begin(),
                  time_change_subscribers.end(),
                  &subscriber) !=
       time_change_subscribers.end()) {
      MessageHandler::error (
         __FILE__, __LINE__,
         IntegrationMessages::invalid_request,
         "Object requested to be added to the list of time change\n"
         "subscribers is already in the list.");
      return;
   }

   time_change_subscribers.push_back (&subscriber);
}


/**
 * Remove a time change subscriber.
 * @param subscriber  Object to be removed from list of subscribers.
 */
void
JeodIntegrationTime::remove_time_change_subscriber (
   TimeChangeSubscriber & subscriber)
{
   std::vector<TimeChangeSubscriber*>::iterator iter =
      std::find (time_change_subscribers.begin(),
                 time_change_subscribers.end(),
                 &subscriber);

   if (iter == time_change_subscribers.end()) {
      MessageHandler::warn (
         __FILE__, __LINE__,
         IntegrationMessages::invalid_request,
         "Object requested to be removed from the list of time change\n"
         "subscribers was not found in the list.");
      return;
   }

   time_change_subscribers.erase (iter);
}

/**
 * Notify subscribers that the nature of time has changed.
 */
void
JeodIntegrationTime::notify_time_change_subscribers ()
{
   // Walk over the list of registered time change subscribers,
   // notifying each that time has changed rate / direction.
   for (std::vector<TimeChangeSubscriber*>::const_iterator iter =
           time_change_subscribers.begin();
        iter != time_change_subscribers.end();
        ++iter) {
      (*iter)->respond_to_time_change();
   }
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
