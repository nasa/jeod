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
  ((orb_elem_derived_state.cc)
   (derived_state.cc)
   (dynamics/mass/src/mass_point_state.cc)
   (utils/orbital_elements/src/orbital_elements.cc)
   (utils/ref_frames/src/ref_frame.cc)
   (utils/ref_frames/src/ref_frame_compute_relative_state.cc))



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
namespace jeod
{
/**
 * Destruct a OrbElemDerivedState object.
 */
OrbElemDerivedState::~OrbElemDerivedState()
{
    if(inertial_ptr != nullptr)
    {
        inertial_ptr->unsubscribe();
    }
}

/**
 * Setter for use_alt_inertial
 */
void OrbElemDerivedState::set_use_alt_inertial(const bool use_alt_inertial_in)
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
void OrbElemDerivedState::initialize(DynBody & subject_body, DynManager & dyn_manager)
{
    // Initialize as a DerivedState.
    DerivedState::initialize(subject_body, dyn_manager);

    // Find the planet.
    // Note that find_planet doesn't return if the named planet is not found.
    planet = find_planet(dyn_manager, reference_name, "reference_name");

    // Choose the planet inertial frame to be used
    if(use_alt_inertial)
    {
        inertial_ptr = &(planet->alt_inertial);
    }
    else
    {
        inertial_ptr = &(planet->inertial);
    }
    inertial_ptr->subscribe();

    // Copy the orbital object body name.
    elements.set_object_name(subject_body.name.c_str());

    // Set the planet name about which the object orbits.
    elements.set_planet_name(planet->name.c_str());
}

/**
 * Update the state.
 */
void OrbElemDerivedState::update()
{
    // Invoke the parent class update method.
    DerivedState::update(); // This really doesn't do anything!

    // Check to see if the integration state is planet centered inertial.
    if(subject->composite_body.get_parent() == inertial_ptr)
    {
        compute_orbital_elements(subject->composite_body.state.trans);
    }
    else
    {
        // If not planet centered inertial, compute it.
        subject->composite_body.compute_relative_state(*inertial_ptr, rel_state);
        compute_orbital_elements(rel_state.trans);
    }
}

/**
 * Compute the orbital elements for the current state.
 * \param[in] rel_trans Planet relative state.
 */
void OrbElemDerivedState::compute_orbital_elements(const RefFrameTrans & rel_trans)
{
    // Compute the orbital elements from the planet centered inertial state.
    elements.from_cartesian(planet->grav_source->mu, rel_trans.position, rel_trans.velocity);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
