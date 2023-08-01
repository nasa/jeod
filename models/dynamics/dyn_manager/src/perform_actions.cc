/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynManager
 * @{
 *
 * @file models/dynamics/dyn_manager/src/perform_actions.cc
 * Define DynManager::perform_actions.
 */

/*****************************************************************************
Purpose:
  ()

Library dependencies:
  ((perform_actions.cc)
   (dynamics/mass/src/mass_point_state.cc))


******************************************************************************/


// System includes
#include <cstdio>
#include <cstring>

// JEOD includes
#include "dynamics/body_action/include/body_action.hh"

// Model includes
#include "../include/dyn_manager.hh"



//! Namespace jeod
namespace jeod {

/**
 * Perform dynamic body actions that are ready to be applied.
 */
void
DynManager::perform_actions (
   void)
{

   // Walk over all of the queued actions, performing any actions that are
   // ready to be performed.
   for (std::list<BodyAction *>::iterator it = body_actions.begin();
        it != body_actions.end();
        /* No increment -- increment is in loop body */ ) {
      BodyAction * action = *it;

      // Action is ready:
      // Apply the action and delete it from the queue.
      if (action->is_ready()) {
         action->apply (*this);
         body_actions.erase (it++);
      }

      // Action is not ready: Advance to next, leaving this action in the queue.
      else {
         ++it;
      }
   }

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
