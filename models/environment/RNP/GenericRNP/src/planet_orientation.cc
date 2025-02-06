/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup RNP
 * @{
 * @addtogroup GenericRNP
 * @{
 *
 * @file models/environment/RNP/GenericRNP/src/planet_orientation.cc
 * Implement PlanetOrientation
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  ((none))

Assumptions and limitations:
  ((none))

Library dependencies:
  ((planet_orientation.cc)
   (RNP_messages.cc)
   (dynamics/dyn_manager/src/dyn_manager.cc)
   (environment/planet/src/planet.cc)
   (utils/ref_frames/src/ref_frame_state.cc)
   (utils/message/src/message_handler.cc)
   (utils/quaternion/src/quat_from_mat.cc))

 

*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "environment/planet/include/planet.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/planet_orientation.hh"
#include "../include/RNP_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Default constructor; constructs a PlanetOrientation object.
 */
PlanetOrientation::PlanetOrientation (
   void)
:
   active(true),
   planet(nullptr),
   name(),
   planet_rot_state(nullptr),
   planet_omega(0.0)
{
   ;
}

/**
 * Class destructor
 */
PlanetOrientation::~PlanetOrientation (
   void)
{//empty
}


/**
 * Goes to the dyn manager given and searches for the planet
 * indicated by the user inputted name, in the given dyn manager.
 * Will send a fail message if the named planet is not found.
 * Additionally, this will register this ephemeris interface
 * with the given DynManager, and will set up the contained
 * EphemerisOrientation to control the named planet.
 * \param[in,out] dyn_manager DynManager where the attitude will be applied
 */
void
PlanetOrientation::initialize (
   DynManager& dyn_manager)
{

   if (name.empty()) {

      MessageHandler::fail (
         __FILE__, __LINE__, RNPMessages::initialization_error,
         "The planet name was not set in the PlanetOrientation"
         " object. This is required for initialization of the"
         " PlanetOrientation object\n");

   }

   planet = dyn_manager.find_planet (name.c_str());

   if (planet == nullptr) {

      MessageHandler::fail (
         __FILE__, __LINE__, RNPMessages::initialization_error,
         "Planet %s specified in PlanetOrientation::initialize"
         " was not found", name.c_str());
      return;
   }

   // initialize everything relevant in the
   planet->pfix.set_owner (this);
   planet_rot_state = & (planet->pfix.state.rot);

   planet_rot_state->ang_vel_unit[0] = 0;
   planet_rot_state->ang_vel_unit[1] = 0;
   planet_rot_state->ang_vel_unit[2] = 1;
   planet_rot_state->ang_vel_this[0] = 0;
   planet_rot_state->ang_vel_this[1] = 0;
   planet_rot_state->ang_vel_this[2] = planet_omega;
   planet_rot_state->ang_vel_mag     = planet_omega;

   // Initialize the lunar orientation
   orient_interface.set_name (name.c_str(), "pfix");
   orient_interface.set_owner (this);
   orient_interface.enable();

   dyn_manager.add_ephemeris(*this);
   dyn_manager.add_ephem_item(orient_interface);

   return;
}

/**
 * Activates the PlanetOrientation model
 */
void PlanetOrientation::activate() {

   active = true;

}

/**
 * Deactivates the PlanetOrientation model
 */
void PlanetOrientation::deactivate() {

   active = false;

}

/**
 * Implements the EphemInterface pure virtual function
 * ephem_initialize. Note that the stand alone "initialize"
 * function MUST be called before this function. Normal
 * operating procedure is the DynamicsManager calls this
 * function, not the user.
 * \param[in,out] manager ephemerides manager
 */
void
PlanetOrientation::ephem_initialize(
   EphemeridesManager& manager JEOD_UNUSED)
{
   ; // function intentionally left empty
}

/**
 * Mark the model as being activate or inactive.
 * \param[in,out] manager ephemerides manager
 */
void
PlanetOrientation::ephem_activate(
   EphemeridesManager& manager JEOD_UNUSED)
{
   ; // intentionally left blank
}

/**
 * Function, for a particular ephemeris model, nominally
 * builds the particular models portions of the reference
 * frame tree. This implementation, by default, does nothing
 * \param[in,out] manager ephemerides manager
 */

void
PlanetOrientation::ephem_build_tree(
   EphemeridesManager& manager JEOD_UNUSED)
{
   // purposefully do nothing. PlanetOrientation models do not,
   // by default, do not contribute to the ephemeris tree.
   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 * @}
 */
