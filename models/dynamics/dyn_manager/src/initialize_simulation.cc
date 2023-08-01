/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynManager
 * @{
 *
 * @file models/dynamics/dyn_manager/src/initialize_simulation.cc
 * Define DynManager::initialize_simulation, which completes the
 * initialization of the JEOD dynamics manager.
 */

/*****************************************************************************
Purpose:
  ()

Library dependencies:
  ((initialize_simulation.cc)
   (initialize_dyn_bodies.cc)
   (dynamics/dyn_body/src/dyn_body.cc)
   (environment/gravity/src/gravity_manager.cc))


******************************************************************************/


// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "environment/gravity/include/gravity_manager.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/dyn_manager.hh"
#include "../include/dyn_manager_messages.hh"



//! Namespace jeod
namespace jeod {

/**
 * Complete initialization of the JEOD manager model.
 */
void
DynManager::initialize_simulation (
   void)
{

   // Sanity check:
   // There should be an overarching Gravity Manager for the simulation for all
   // but non-empty space simulations and planet-based sims with no gravity.
   // (That there is only one Gravity Manager is enforced elsewhere.)
   if ((mode != DynManagerInit::EphemerisMode_EmptySpace) &&
       (gravity_manager == nullptr) &&
       (! gravity_off)) {
      MessageHandler::warn (
         __FILE__, __LINE__, DynManagerMessages::inconsistent_setup,
         "A Gravity Manager has not been registered with the dynamics manager\n"
         "for a simulation that involves planets and dynamic bodies.\n"
         "Gravity will not be calculated for this simulation.\n"
         "Please check the order in which initialization jobs are called.");
   }


   // Initialize the ephemerides.
   initialize_ephemerides ();

   // Initialize the gravity controls for each dynamic body.
   if (gravity_manager != nullptr) {
      initialize_gravity_controls ();
   }

   // Check that each subscribed frame has an owner.
   check_ref_frame_ownership ();

   // Compute ephemerides for active planets.
   activate_ephemerides ();

   // Update the ephemerides.
   update_ephemerides ();

   // Initialize the gravitational bodies.
   if (gravity_manager != nullptr) {
      gravity_manager->initialize_state (*this);
   }

   // Initialize the integration groups.
   initialize_integ_groups ();

   // Initialize the dynamic bodies.
   initialize_dyn_bodies ();

   // Indicate that initialization has been completed.
   initialized = true;

   return;
}


/**
 * Complete initialization of the initialization groups.
 */
void
DynManager::initialize_integ_groups (
   void)
{
   // Initialize the integration groups.
   // Monolithic mode (no external groups registered):
   // Initialize the default group.
   if (integ_groups.empty()) {
      default_integ_group->initialize_group (*this);
   }

   // Multiple integrators mode:
   // Initialize each integration group.
   // The default group is left uninitialized.
   else {
      for (std::vector<DynamicsIntegrationGroup *>::iterator it =
              integ_groups.begin();
           it != integ_groups.end();
           ++it) {
         DynamicsIntegrationGroup * group = *it;
         group->initialize_group (*this);
      }
   }
}


/**
 * Add DynBody objects to the default integration group.
 * @param[in,out] group  Group to be updated
*/
void
DynManager::update_integration_group (
   JeodIntegrationGroup & group)
{
   // The group should be the default integration group.
   // This is one of those "this should never happen" kind of errors.
   if (&group != default_integ_group) {
      MessageHandler::fail (
         __FILE__, __LINE__, DynManagerMessages::inconsistent_setup,
         "Internal error.");
   }

   // Add each body that doesn't have an integrator to the default group.
   for (std::vector<DynBody *>::const_iterator it = dyn_bodies.begin();
        it != dyn_bodies.end();
        ++it) {
      DynBody * body = *it;
      if (body->get_integration_group() == nullptr) {
         default_integ_group->add_dyn_body (*body);
      }
   }
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
