/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Time
 * @{
 *
 * @file models/environment/time/src/time_manager__initialize.cc
 * Define TimeManager::initialize.
 * This method allocates resources and invokes TimeManagerInit functionality.
 * This method needs to be defined as a separate compilation unit.
 */

/******************************************************************************
PURPOSE:
  ()

REFERENCE:
  (((None)))

ASSUMPTIONS AND LIMITATIONS:
  ((None))

LIBRARY DEPENDENCY:
  ((time_manager.cc)
   (time.cc)
   (time_dyn.cc)
   (time_manager_init.cc)
   (time_messages.cc)
   (time_standard.cc)
   (utils/message/src/message_handler.cc)
   (utils/named_item/src/named_item.cc))


******************************************************************************/

// System includes

// JEOD includes
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/named_item/include/named_item.hh"

// Model includes
#include "../include/time.hh"
#include "../include/time_manager.hh"
#include "../include/time_manager_init.hh"

//! Namespace jeod
namespace jeod
{

// Attributes used in allocations
JEOD_DECLARE_ATTRIBUTES(JeodBaseTime)

/**
 * initializes the time manager
 * \param[in] time_manager_init Initialization parameters
 */
void TimeManager::initialize(TimeManagerInit * time_manager_init)
{
    JEOD_REGISTER_INCOMPLETE_CLASS(JeodBaseTime);

    register_time(dyn_time);
    num_types = time_vector.size();

    // Initialize from the time_manager_init object.
    time_manager_init->initialize_manager(this);

    // Deliberate redundancy here.  While each initial time value has already
    // been calculated as part of the initialization process, the update function
    // ensures that the dependency seen in the update tree is carried through.
    update(0.0);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
