/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DerivedState
 * @{
 *
 * @file models/dynamics/derived_state/src/planetary_derived_state.cc
 * Define methods for the base body initialization class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((planetary_derived_state.o)
   (derived_state.o)
   (dynamics/mass/mass_point_state.o)
   (utils/planet_fixed/planet_fixed_posn/planet_fixed_posn.o)
   (utils/ref_frames/ref_frame.o)
   (utils/ref_frames/ref_frame_compute_relative_state.o))



*******************************************************************************/


// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "environment/planet/include/planet.hh"

// Model includes
#include "../include/planetary_derived_state.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a PlanetaryDerivedState object.
 */
PlanetaryDerivedState::PlanetaryDerivedState (
   void)
:
   planet (NULL)
{
}


/**
 * Begin initialization of a PlanetaryDerivedState.
 * The initialize method for all subclasses of DerivedState *must* pass
 * the initialize call to their immediate parent class, which in turn
 * must do the same, eventually invoking this method.
 * \param[in,out] subject_body Subject body
 * \param[in,out] dyn_manager Dynamics manager
 */
void
PlanetaryDerivedState::initialize (
   DynBody & subject_body,
   DynManager & dyn_manager)
{

   // Initialize as a DerivedState.
   DerivedState::initialize (subject_body, dyn_manager);

   // Find the planet. Note that this fails if the planet is not found.
   planet = find_planet (dyn_manager, reference_name, "reference_name");

   // Issue a subscription to the planet-fixed frame.
   planet->pfix.subscribe();

   // Initialize the planet-fixed position.
   state.initialize (planet);

   return;
}


/**
 * Update the state.
 */
void
PlanetaryDerivedState::update (
   void)
{
   double pfix_pos[3];

   // Compute the cartesian coordinates relative to the planet fixed frame and
   // update the planet fixed position from these cartesian coordinates.
   subject->composite_body.compute_position_from (planet->pfix, pfix_pos);
   state.update_from_cart (pfix_pos);
}


/**
 * Destruct a PlanetaryDerivedState object.
 */
PlanetaryDerivedState::~PlanetaryDerivedState (
   void)
{
   // Remove the initialization-time subscription to the planet-centered frame.
   if (planet != NULL) {
      planet->pfix.unsubscribe();
   }
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
