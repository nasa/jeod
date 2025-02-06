/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DerivedState
 * @{
 *
 * @file models/dynamics/derived_state/src/solar_beta_derived_state.cc
 * This function calculates the angle between a spacecraft's
 * orbital plane and the vector from the relevant planet to the sun.
 */

/******************************************************************************
PURPOSE:
    ()

ASSUMPTIONS AND LIMITATIONS:
    ((The vehicle must be in orbit about the named planet))

LIBRARY DEPENDENCY:
    ((solar_beta_derived_state.cc)
     (derived_state.cc)
     (derived_state_messages.cc)
     (dynamics/mass/src/mass_point_state.cc)
     (utils/sim_interface/src/memory_interface.cc)
     (utils/message/src/message_handler.cc)
     (utils/ref_frames/src/ref_frame.cc)
     (utils/ref_frames/src/ref_frame_compute_relative_state.cc))


******************************************************************************/

#define EPSILON 0.0000001

// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "environment/planet/include/planet.hh"
#include "utils/math/include/vector3.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/named_item/include/named_item.hh"

// Model includes
#include "../include/solar_beta_derived_state.hh"
#include "../include/derived_state_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a SolarBetaDerivedState object.
 */
SolarBetaDerivedState::SolarBetaDerivedState (
   void)
:
   planet (nullptr),
   sun (nullptr),
   solar_beta (0.0),
   active (true)
{
   Vector3::initialize(sun_wrt_planet);
}


/**
 * Begin initialization of a SolarBetaDerivedState.
 * The initialize method for all subclasses of DerivedState *must* pass
 * the initialize call to their immediate parent class, which in turn
 * must do the same, eventually invoking this method.
 * \param[in,out] subject_body Subject body
 * \param[in,out] dyn_manager Dynamics manager
 */
void
SolarBetaDerivedState::initialize (
   DynBody & subject_body,
   DynManager & dyn_manager)
{

   // Initialize as a DerivedState.
   DerivedState::initialize (subject_body, dyn_manager);

   // Setting the active flag to false prior to initialization time
   // permanently disables model.
   if (! active) {
      return;
   }

   // Find the planet and the Sun.
   // Note that find_planet doesn't return if the named planet is not found.
   planet = find_planet (dyn_manager, reference_name, "reference_name");
   sun = find_planet (dyn_manager, "Sun", "<none>");

   // Subscribe to the planet-centered and sun-centered inertial frames.
   planet->inertial.subscribe();
   sun->inertial.subscribe();
}


/**
 * Update the state.
 */
void
SolarBetaDerivedState::update (
   void)
{

   // Return if the model is currently inactive or permanently disabled.
   if ((! active) || (sun == nullptr)) {
      return;
   }

   // Find the position of the sun with respect ot the planet.
   sun->inertial.compute_position_from(planet->inertial, sun_wrt_planet);

   // Compute the relative state of the vehicle with respect to the planet.
   subject->composite_body.compute_relative_state(planet->inertial,
                                                  veh_wrt_planet);

   // Store the relative translational state.
   RefFrameTrans* trans_state = &veh_wrt_planet.trans;
   double ang_momentum[3];

   // Compute the angular momentum vector, which is a handy thing
   // to have when calculating solar beta.
   Vector3::cross(trans_state->position, trans_state->velocity, ang_momentum);

   // h = ang_momentum
   // s = sun_wrt_planet
   // |h| * |s| * sin(solar_beta) = h dot s
   double dot_product = Vector3::dot(ang_momentum, sun_wrt_planet);
   double h_mag_sq = Vector3::vmagsq(ang_momentum);
   double s_mag_sq = Vector3::vmagsq(sun_wrt_planet);

   if(h_mag_sq < EPSILON) {

      MessageHandler::fail (
         __FILE__, __LINE__, DerivedStateMessages::divide_by_zero,
         "The magnitude of the momentum of the vehicle tracked by solar "
         "beta was too small, resulting in a divide by zero. The function "
         "has failed.\n");
      return;

   }

   if(s_mag_sq < EPSILON) {

      MessageHandler::fail (
         __FILE__, __LINE__, DerivedStateMessages::divide_by_zero,
         "The magnitude of the position of the sun with respect to the planet "
         "referenced in solar beta was too small, resulting in a divide by "
         "zero. The function has failed.\n");
      return;

   }

   solar_beta = dot_product / (sqrt(h_mag_sq * s_mag_sq));
   solar_beta = asin(solar_beta);
}


/**
 * Destruct a SolarBetaDerivedState.
 */
SolarBetaDerivedState::~SolarBetaDerivedState (
   void)
{
   // Remove initialization-time subscriptions.
   if (sun != nullptr) {
      planet->inertial.unsubscribe();
      sun->inertial.unsubscribe();
   }
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
