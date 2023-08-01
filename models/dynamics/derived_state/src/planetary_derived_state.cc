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
  ((planetary_derived_state.cc)
   (derived_state.cc)
   (dynamics/mass/src/mass_point_state.cc)
   (utils/planet_fixed/planet_fixed_posn/src/planet_fixed_posn.cc)
   (utils/ref_frames/src/ref_frame.cc)
   (utils/ref_frames/src/ref_frame_compute_relative_state.cc))



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
   planet (nullptr),
   use_alt_pfix(false),
   pfix_ptr(nullptr)
{
}


/**
 * Destruct a PlanetaryDerivedState object.
 */
PlanetaryDerivedState::~PlanetaryDerivedState (
   void)
{
   if (pfix_ptr != nullptr) {
      pfix_ptr->unsubscribe();
   }
}


/**
 * Setter for use_alt_pfix
 */
void
PlanetaryDerivedState::set_use_alt_pfix (
   const bool use_alt_pfix_in)
{
   use_alt_pfix = use_alt_pfix_in;
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

   // Find the planet.
   // Note that find_planet doesn't return if the named planet is not found.
   planet = find_planet (dyn_manager, reference_name, "reference_name");

   // Choose the planet fixed frame to be used
   if (use_alt_pfix) {
      pfix_ptr = &(planet->alt_pfix);
   } else {
      pfix_ptr = &(planet->pfix);
   }
   pfix_ptr->subscribe();

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
   subject->composite_body.compute_position_from (*pfix_ptr, pfix_pos);
   state.update_from_cart (pfix_pos);
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
