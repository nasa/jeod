/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DerivedState
 * @{
 *
 * @file models/dynamics/derived_state/src/ned_derived_state.cc
 * Define methods for NedDerivedState.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((ned_derived_state.o)
   (derived_state.o)
   (dynamics/mass/mass_point_state.o)
   (utils/planet_fixed/north_east_down/north_east_down.o)
   (utils/planet_fixed/planet_fixed_posn/planet_fixed_posn.o)
   (utils/ref_frames/ref_frame.o)
   (utils/ref_frames/ref_frame_compute_relative_state.o)
   (utils/ref_frames/ref_frame_set_name.o))



*******************************************************************************/


// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "environment/planet/include/planet.hh"
#include "utils/planet_fixed/north_east_down/include/north_east_down.hh"
#include "utils/ref_frames/include/ref_frame_state.hh"

// Model includes
#include "../include/ned_derived_state.hh"


//! Namespace jeod
namespace jeod {

/**
 * NedDerivedState default constructor.
 */
NedDerivedState::NedDerivedState (
   void)
:
   DerivedState(),
   register_frame(true),
   ned_state(),
   planet_centered_planet_fixed(NULL),
   pfix_rel_state()
{
   return;
}


/**
 * NedDerivedState destructor.
 */
NedDerivedState::~NedDerivedState (
   void)
{

   // Disconnect the NED frame from the planet-center planet-fixed frame.
   ned_state.ned_frame.remove_from_parent();

   // Remove the initialization-time subscription to the planet-centered frame.
   if (planet_centered_planet_fixed != NULL) {
      planet_centered_planet_fixed->unsubscribe();
   }

   // Recant the registration done by the initialize() method.
   if (register_frame) {
      // FIXME: There is no way to do this yet.
   }

   return;
}


/**
 * Begin initialization of a LvlhDerivedState.
 * The initialize method for all subclasses of DerivedState *must* pass
 * the initialize call to their immediate parent class, which in turn
 * must do the same, eventually invoking this method.
 * \param[in,out] subject_body Subject body
 * \param[in,out] dyn_manager Dynamics manager
 */
void
NedDerivedState::initialize (
   DynBody & subject_body,
   DynManager & dyn_manager)
{
   Planet * planet;

   DerivedState::initialize (subject_body, dyn_manager);

   // Find the planet.
   // Note that find_planet doesn't return if the named planet is not found.
   planet = find_planet (dyn_manager, reference_name, "reference_name");

   // Name the frame as <vehicle>.<planet>.ned
   ned_state.ned_frame.set_name (subject_body.name.c_str(), reference_name, "ned");

   // Connect the frame to the planet's planet-fixed frame
   // and subscribe to that frame.
   planet_centered_planet_fixed = &(planet->pfix);
   planet_centered_planet_fixed->add_child (ned_state.ned_frame);
   planet_centered_planet_fixed->subscribe ();

   // If requested, register the frame with the dynamics manager.
   if (register_frame) {
      dyn_manager.add_ref_frame (ned_state.ned_frame);
   }

   // Initialize the NED state.
   ned_state.initialize (planet);
}


/**
 * Update the state.
 */
void
NedDerivedState::update (
   void)
{
   // Update the NED frame based on the vehicle's state relative to the planet.
   subject->composite_body.compute_relative_state (
      *planet_centered_planet_fixed, pfix_rel_state);
   compute_ned_frame (pfix_rel_state.trans);

   // Timestamp the frame per the vehicle timestamp.
   ned_state.ned_frame.set_timestamp (subject->composite_body.timestamp());
}


/**
 * Update the state.
 * \param[in] rel_trans Planet relative state
 */
void
NedDerivedState::compute_ned_frame (
   const RefFrameTrans & rel_trans)
{
   ned_state.set_ned_trans_states (rel_trans.position, rel_trans.velocity);
   ned_state.build_ned_orientation();
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
