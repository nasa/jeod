/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynManager
 * @{
 *
 * @file models/dynamics/dyn_manager/src/integ_group_primitives.cc
 * Define the DynManager member functions that search through and add elements
 * to the collection of DynamicsIntegrationGroup pointers.
 */

/*******************************************************************************
Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((N/A))

Library dependencies:
  ((integ_group_primitives.o)
   (dyn_manager.o)
   (dyn_manager_messages.o)
   (utils/message/message_handler.o))


*******************************************************************************/


// System includes
#include <algorithm>
#include <cstddef>

// JEOD includes
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/dyn_manager.hh"
#include "../include/dyn_manager_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Determine if the specified group has been registered with the DynManager.
 * @param integ_group  Integration group to be found.
 * @return True if integ_group has been registered, false otherwise.
 */
bool
DynManager::is_integ_group_registered (
   const DynamicsIntegrationGroup * integ_group)
const
{
   return std::find (integ_groups.begin(), integ_groups.end(), integ_group) !=
          integ_groups.end();
}


/**
 * Add an integration group to the integration group registry.
 * @param integ_group  Integration group to be added.
 */
void
DynManager::add_integ_group (
   DynamicsIntegrationGroup & integ_group)
{

   // Check if it is too late in the game for such a registration.
   if ((default_integ_group != NULL) || initialized) {
      MessageHandler::error (
         __FILE__, __LINE__, DynManagerMessages::inconsistent_setup,
         "Late call to add_integ_group()\n"
         "Integration groups should be registered as default_data jobs.\n"
         "Addition request ignored.");
      return;
   }

   // Ensure the supplied group is not already registered.
   if (is_integ_group_registered (&integ_group)) {
      MessageHandler::error (
         __FILE__, __LINE__, DynManagerMessages::duplicate_entry,
         "Duplicate entry passed to add_integ_group()\n"
         "Addition request ignored.");
      return;
   }

   // Add the group to the list of integration groups.
   integ_groups.push_back (&integ_group);
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
