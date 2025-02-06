/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DerivedState
 * @{
 *
 * @file models/dynamics/derived_state/src/orb_elem_derived_state.cc
 * Define methods for the orbital elements derived state class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((orb_elem_derived_state.o)
   (derived_state.o)
   (dynamics/mass/mass_point_state.o)
   (utils/orbital_elements/orbital_elements.o)
   (utils/ref_frames/ref_frame.o)
   (utils/ref_frames/ref_frame_compute_relative_state.o))



*******************************************************************************/


// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "environment/planet/include/planet.hh"
#include "utils/orbital_elements/include/orbital_elements.hh"

// Model includes
#include "../include/orb_elem_derived_state.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a OrbElemDerivedState object.
 */
OrbElemDerivedState::OrbElemDerivedState (
   void)
:
   planet( NULL ),
   use_alt_inertial (false),
   ref_frame_ptr (NULL)
{
   return;
}



/**
 * Destruct a OrbElemDerivedState object.
 */
OrbElemDerivedState::~OrbElemDerivedState (
   void)
{
   // Remove the initialization-time subscription to the planet-centered frame.
   if (ref_frame_ptr != NULL) {
      ref_frame_ptr->unsubscribe();
   }
}



/**
 * Determine whether or not this instance uses the
 * planet's alternate inertial frame.
 */
void
OrbElemDerivedState::set_use_alt_inertial (
   const bool use_alt_inertial_in)
{
   use_alt_inertial = use_alt_inertial_in;
}



/**
 * Begin initialization of a OrbElemDerivedState.
 * The initialize method for all subclasses of DerivedState *must* pass
 * the initialize call to their immediate parent class, which in turn
 * must do the same, eventually invoking this method.
 * \param[in,out] subject_body Subject body
 * \param[in,out] dyn_manager Dynamics manager
 */
void
OrbElemDerivedState::initialize (
   DynBody    & subject_body,
   DynManager & dyn_manager)
{

   // Make sure to perform the base class initialization.
   DerivedState::initialize (subject_body, dyn_manager);

   // Copy the orbital object body name.
   elements.set_object_name (subject_body.name.c_str());

   // Must have a planetary body for orbital elements.
   planet = find_planet (dyn_manager, reference_name, "reference_name");

   // Set the planet name about which the object orbits.
   elements.set_planet_name (planet->name.c_str());

// Choose the planet inertial frame to be used
   if (use_alt_inertial) {
      ref_frame_ptr = &(planet->alt_inertial);
   } else {
      ref_frame_ptr = &(planet->inertial);
   }

   // Subscribe to the planet-centered inertial frame.
   ref_frame_ptr->subscribe ();

   return;

}


/**
 * Update the state.
 */
void
OrbElemDerivedState::update (
   void)
{
   // Invoke the parent class update method.
   DerivedState::update(); // This really doesn't do anything!

   // Check to see if the integration state is planet centered inertial.
   if (subject->composite_body.get_parent() == ref_frame_ptr) {
      compute_orbital_elements (subject->composite_body.state.trans);
   }
   else {
      // If not planet centered inertial, compute it.
      subject->composite_body.compute_relative_state (*ref_frame_ptr,
                                                      rel_state);
      compute_orbital_elements (rel_state.trans);
   }

   return;
}


/**
 * Compute the orbital elements for the current state.
 * \param[in] rel_trans Planet relative state.
 */
void
OrbElemDerivedState::compute_orbital_elements (
   const RefFrameTrans & rel_trans)
{

   // Compute the orbital elements from the planet centered inertial state.
   elements.from_cartesian (planet->grav_source->mu,
                            rel_trans.position,
                            rel_trans.velocity);

   return;

}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
