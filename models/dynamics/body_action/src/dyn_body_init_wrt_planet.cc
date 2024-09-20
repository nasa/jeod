/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/src/dyn_body_init_wrt_planet.cc
 * Define methods for the DynBodyInitWrtPlanet class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((dyn_body_init_wrt_planet.cc)
   (dyn_body_init.cc)
   (dynamics/mass/src/mass_point_state.cc))



*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "environment/planet/include/planet.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/ref_frames/include/ref_frame_items.hh"

// Model includes
#include "../include/body_action_messages.hh"
#include "../include/dyn_body_init_wrt_planet.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Initialize the initializer.
 * \param[in,out] dyn_manager Dynamics manager
 */
void DynBodyInitWrtPlanet::initialize(DynManager & dyn_manager)
{
    // Find the planet, with error checking.
    planet = find_planet(dyn_manager, planet_name, "planet_name");

    // Initially set the reference ref_frame to the planet-centered inertial
    // frame to ensure that the planet's state is computed.
    reference_ref_frame = &planet->inertial;

    // Pass the message up the chain. This will initialize the base
    // characteristics of the instance.
    DynBodyInit::initialize(dyn_manager);
}

/**
 * Indicate what parts of the vehicle state this object initializes.
 * @return States initialized
 */
RefFrameItems::Items DynBodyInitWrtPlanet::initializes_what()
{
    return (set_items);
}

/**
 * Indicate whether the initializer is ready to run.
 * This particular implementation is just a pass-through.
 * @return Is initializer ready?
 */
bool DynBodyInitWrtPlanet::is_ready()
{
    // Defer the query to the parent class, DynBodyInit.
    return (DynBodyInit::is_ready());
}

/**
 * Apply the initializer.
 * This is just a pass-through.
 * Some derived class must do the actual work.
 * \param[in,out] dyn_manager Dynamics manager
 */
void DynBodyInitWrtPlanet::apply(DynManager & dyn_manager)
{
    // Pass the message up the chain.
    DynBodyInit::apply(dyn_manager);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
