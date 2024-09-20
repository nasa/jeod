/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DerivedState
 * @{
 *
 * @file models/dynamics/derived_state/src/lvlh_derived_state.cc
 * Define methods for the base body initialization class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((lvlh_derived_state.cc)
   (derived_state.cc)
   (dynamics/mass/src/mass_point_state.cc)
   (utils/quaternion/src/quat_from_mat.cc)
   (utils/lvlh_frame/src/lvlh_frame.cc)
   (utils/ref_frames/src/ref_frame.cc)
   (utils/ref_frames/src/ref_frame_compute_relative_state.cc))



*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "environment/planet/include/planet.hh"
#include "utils/math/include/vector3.hh"

// Model includes
#include "../include/lvlh_derived_state.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Destruct a LvlhDerivedState object.
 */
LvlhDerivedState::~LvlhDerivedState()
{
    // Disconnect the LVLH frame from the planet-center inertial frame.
    lvlh_frame.remove_from_parent();

    // Recant the registration done by the initialize() method.
    if(local_dm != nullptr)
    {
        local_dm->remove_ref_frame(lvlh_frame);
    }
}

/**
 * Begin initialization of a LvlhDerivedState.
 * The initialize method for all subclasses of DerivedState *nust* pass
 * the initialize call to their immediate parent class, which in turn
 * must do the same, eventually invoking this method.
 * \param[in,out] subject_body Subject body
 * \param[in,out] dyn_manager Dynamics manager
 */
void LvlhDerivedState::initialize(DynBody & subject_body, DynManager & dyn_manager)
{
    // First perform general derived state initializations.
    DerivedState::initialize(subject_body, dyn_manager);
    lvlh_state.set_subject_frame(subject->composite_body);
    lvlh_state.set_planet_name(reference_name);
    lvlh_state.initialize(dyn_manager);

    lvlh_frame.set_name(subject_body.name.get_name(), reference_name, "lvlh");
    planet_centered_inertial = const_cast<RefFrame *>(lvlh_state.frame.get_parent());
    planet_centered_inertial->add_child(lvlh_frame);

    // If requested, register the frame with the dynamics manager.
    if(register_frame)
    {
        dyn_manager.add_ref_frame(lvlh_frame);
        local_dm = &dyn_manager;
    }
}

/**
 * Update the state.
 */
void LvlhDerivedState::update()
{
    lvlh_state.update();
    lvlh_frame.state = lvlh_state.frame.state;
    lvlh_frame.set_timestamp(lvlh_state.frame.timestamp());
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
