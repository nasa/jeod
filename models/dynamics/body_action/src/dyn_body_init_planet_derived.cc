/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/src/dyn_body_init_planet_derived.cc
 * Define methods for the DynBodyInitPlanetDerived class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((dyn_body_init_planet_derived.cc)
   (dyn_body_init.cc)
   (dyn_body_init_wrt_planet.cc)
   (dynamics/mass/src/mass_point_state.cc)
   (utils/ref_frames/src/ref_frame.cc)
   (utils/ref_frames/src/ref_frame_compute_relative_state.cc))



*******************************************************************************/


// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/ref_frames/include/ref_frame_items.hh"

// Model includes
#include "../include/body_action_messages.hh"
#include "../include/dyn_body_init_planet_derived.hh"


//! Namespace jeod
namespace jeod {

/**
 * DynBodyInitPlanetDerived default constructor.
 */
DynBodyInitPlanetDerived::DynBodyInitPlanetDerived (
   void)
:
   DynBodyInitWrtPlanet(),
   ref_body_name(),
   ref_body(nullptr),
   required_items(RefFrameItems::Pos_Vel_Att_Rate),
   body_is_required(true)
{
   return;
}


/**
 * DynBodyInitPlanetDerived destructor.
 */
DynBodyInitPlanetDerived::~DynBodyInitPlanetDerived (
   void)
{
   return;
}


/**
 * Initialize the initializer.
 * \param[in,out] dyn_manager Dynamics manager
 */
void
DynBodyInitPlanetDerived::initialize (
   DynManager & dyn_manager)
{

   // Pass the message up the chain. This will initialize the base
   // characteristics of the instance.
   DynBodyInitWrtPlanet::initialize (dyn_manager);


   // Find the reference body that will be used to determine the derived frame.
   // Bypass if some derived class already set this or if the
   // ref_body is not truly needed.
   if ((ref_body == nullptr) && body_is_required) {
      ref_body = find_dyn_body (dyn_manager, ref_body_name, "ref_body_name");
   }

   return;
}


/**
 * Indicate whether the initializer is ready to run.
 * When the state is based on some reference body, that reference
 * vehicle's composite body frame must contain the specified required
 * items before the initializer can run.
 * @return Is initializer ready?
 */
bool
DynBodyInitPlanetDerived::is_ready (
   void)
{

   // The initializer is ready if all internal and external dependencies have
   // been satisfied.
   // The parent class handles the internal dependencies.
   // Externally, the reference body (if any) must have the requisite
   // required_items set before the initializer can run.
   return (DynBodyInitWrtPlanet::is_ready () &&
           ((ref_body == nullptr) ||
            ref_body->composite_body.initialized_items.contains (
               required_items)));
}


/**
 * Apply the initializer: This is just a pass through.
 * A derived class is responsible for setting the state that the
 * DynBodyInit uses to initialize the state.
 * \param[in,out] dyn_manager Dynamics manager
 */
void
DynBodyInitPlanetDerived::apply (
   DynManager & dyn_manager)
{

   // Pass the message up the chain.
   DynBodyInitWrtPlanet::apply (dyn_manager);

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
