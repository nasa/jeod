/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynManager
 * @{
 *
 * @file models/dynamics/dyn_manager/src/initialize_model.cc
 * Define DynManager::initialize_model.
 */

/*****************************************************************************
Purpose:
  ()

Library dependencies:
  ((initialize_model.o)
   (dyn_manager_messages.o)
   (environment/ephemerides/ephem_item/ephem_item.o)
   (utils/message/message_handler.o)
   (utils/ref_frames/ref_frame.o)
  )



******************************************************************************/

// System includes
#include <cstddef>

// ER7 utilities include
#include "er7_utils/integration/core/include/integrator_constructor.hh"
#include "er7_utils/integration/core/include/integrator_constructor_factory.hh"

// JEOD includes
#include "environment/ephemerides/ephem_interface/include/simple_ephemerides.hh"
#include "environment/ephemerides/ephem_item/include/ephem_item.hh"
#include "environment/time/include/time_manager.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/sim_interface/include/jeod_integrator_interface.hh"

// Model includes
#include "../include/dyn_manager.hh"
#include "../include/dyn_manager_messages.hh"


//! Namespace jeod
namespace jeod {

// Attributes used in allocations
JEOD_DECLARE_ATTRIBUTES (EmptySpaceEphemeris)
JEOD_DECLARE_ATTRIBUTES (SinglePlanetEphemeris)


/**
 * Begin initialization of the JEOD manager model.
 * \param[in,out] init Initialization data
 * \param[in,out] time_mngr Time manager
 */
void
DynManager::initialize_model (
   DynManagerInit & init,
   TimeManager & time_mngr)
{
   integ_interface = JeodSimulationInterface::create_integrator_interface();
   sim_integrator = integ_interface->get_integrator();

   initialize_model_internal (init, time_mngr);
}


/**
 * Begin initialization of the JEOD manager model.
 * @param[in,out] integ_if   Integrator interface
 * @param[in,out] init       Initialization data
 * @param[in,out] time_mngr  Time manager

Class:
  (initialization)
******************************************************************************/
void
DynManager::initialize_model (
   JeodIntegratorInterface & integ_if,
   DynManagerInit & init,
   TimeManager & time_mngr)
{
   integ_interface = &integ_if;
   sim_integrator = integ_interface->get_integrator();

   initialize_model_internal (init, time_mngr);
}


/**
 * Begin initialization of the JEOD manager model.
 *
 * \par Assumptions and Limitations
 *  - The user-input item selection table must have at most one selection
 *     rule for a given name. This limitation is an enforced constraint.
 * \param[in,out] init Initialization data
 * \param[in,out] time_mngr Time manager
 */
void
DynManager::initialize_model_internal (
   DynManagerInit & init,
   TimeManager & time_mngr)
{

   // Set the mode.
   mode = init.mode;

  if (mode != DynManagerInit::EphemerisMode_Ephemerides) {
     if ((init.central_point_name == NULL) ||
         (init.central_point_name[0] == '\0')) {
         MessageHandler::fail (
            __FILE__, __LINE__, DynManagerMessages::invalid_name,
            "Invalid value (NULL or empty string) for "
            "DynManagerInit.central_point_name");
         return;
      }


      switch (mode) {
      // Empty space mode:
      // Make an empty space ephemeris model to represent all of (empty) space.
      case DynManagerInit::EphemerisMode_EmptySpace:
         simple_ephemeris =
            JEOD_ALLOC_CLASS_OBJECT (EmptySpaceEphemeris, ());
         break;

      // Single planet mode:
      // Make an single-planet ephemeris model to represent space.
      // Note: The simulation must define exactly one Planet object, give it the
      // same name as the central point, and register it with the manager.
      case DynManagerInit::EphemerisMode_SinglePlanet:
         simple_ephemeris =
            JEOD_ALLOC_CLASS_OBJECT (SinglePlanetEphemeris, ());
         break;
      // Anything else is an error.
      case DynManagerInit::EphemerisMode_Ephemerides:
      default:
         MessageHandler::fail (
            __FILE__, __LINE__, DynManagerMessages::inconsistent_setup,
            "Illegal mode value");
         return;
      }

      // There shouldn't be any ephemerides models registered with the
      // ephemerides manager, but just in case, disable them and empty the list.
      clear_added_ephemerides ();

      // Initialize the simple ephemeris model.
      simple_ephemeris->set_name (init.central_point_name);
      simple_ephemeris->initialize_model (*this);

      // Tell the ephemeris manager to reject subsequent ephemerides.
      disable_add_ephemeris ();
   }


   // Create the default integration group, if needed.
   if (integ_groups.empty()) {

      // Create the integrator constructor.
      if (init.integ_constructor != NULL) {
         integ_constructor = init.integ_constructor->create_copy();
      }
      else {
         if (init.jeod_integ_opt == er7_utils::Integration::Unspecified) {
            init.jeod_integ_opt =
               integ_interface->interpret_integration_type (init.sim_integ_opt);
            if (init.jeod_integ_opt == er7_utils::Integration::Unspecified) {
               // FIXME! This will die in the factory. Better to die here.
            }
         }
         integ_constructor =
            er7_utils::IntegratorConstructorFactory::create (
               init.jeod_integ_opt);
      }


      // Create the default integration group.
      if (init.integ_group_constructor != NULL) {
         default_integ_group =
            init.integ_group_constructor->create_group (
               *this,
               *integ_constructor,
               *integ_interface,
               time_mngr.get_jeod_integration_time());
      }
      else {
         default_integ_group =
            JEOD_ALLOC_CLASS_OBJECT (
               DynamicsIntegrationGroup,
               (*this,
                *integ_constructor,
                *integ_interface,
                time_mngr.get_jeod_integration_time()));
      }
   }
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
