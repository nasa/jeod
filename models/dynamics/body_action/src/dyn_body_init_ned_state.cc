/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/src/dyn_body_init_ned_state.cc
 * Define methods for DynBodyInitNedState.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((dyn_body_init_ned_state.cc)
   (body_action_messages.cc)
   (dyn_body_init.cc)
   (dyn_body_init_planet_derived.cc)
   (dyn_body_init_wrt_planet.cc)
   (dynamics/mass/src/mass_point_state.cc)
   (utils/message/src/message_handler.cc)
   (utils/planet_fixed/north_east_down/src/north_east_down.cc)
   (utils/planet_fixed/planet_fixed_posn/src/alt_lat_long_state.cc)
   (utils/planet_fixed/planet_fixed_posn/src/planet_fixed_posn.cc)
   (utils/ref_frames/src/ref_frame.cc)
   (utils/ref_frames/src/ref_frame_compute_relative_state.cc))



*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "environment/planet/include/planet.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/planet_fixed/north_east_down/include/north_east_down.hh"
#include "utils/ref_frames/include/ref_frame_items.hh"
#include "utils/ref_frames/include/ref_frame_state.hh"

// Model includes
#include "../include/body_action_messages.hh"
#include "../include/dyn_body_init_ned_state.hh"

//! Namespace jeod
namespace jeod
{

/**
 * DynBodyInitNedState default constructor.
 */
DynBodyInitNedState::DynBodyInitNedState()
{
    required_items = RefFrameItems::Pos;
    body_is_required = false;
}

/**
 * Setter for use_alt_pfix
 */
void DynBodyInitNedState::set_use_alt_pfix(const bool use_alt_pfix_in)
{
    use_alt_pfix = use_alt_pfix_in;
}

/**
 * Initialize the initializer.
 * \param[in,out] dyn_manager Dynamics manager
 */
void DynBodyInitNedState::initialize(DynManager & dyn_manager)
{
    if(!ref_body_name.empty())
    {
        body_is_required = true;
    }

    // Pass the message up the chain. This will initialize the base
    // characteristics of the instance.
    DynBodyInitPlanetDerived::initialize(dyn_manager);

    // Choose the planet fixed frame to be used
    if(use_alt_pfix)
    {
        planet->calculate_alt_pfix();
        pfix_ptr = &(planet->alt_pfix);
    }
    else
    {
        pfix_ptr = &(planet->pfix);
    }
}

/**
 * Apply the initializer.
 * \param[in,out] dyn_manager Dynamics manager
 */
void DynBodyInitNedState::apply(DynManager & dyn_manager)
{
    NorthEastDown ned_state;             // --  North-East-Down frame vehicle state
    RefFrameItems test_items(set_items); // -- The state components to be set

    // Initialize the N-E-D's planet reference to the specified planet.
    ned_state.initialize(planet);

    // Populate the planet-fixed state.
    // No reference body: Use the reference point as the basis for the state.
    if(ref_body == nullptr)
    {
        // The point is stationary wrt the rotating planet.
        Vector3::initialize(ned_state.ned_frame.state.trans.velocity);

        // Set the state with the supplied point, interpreted as geocentric
        // or geodetic per the altlatlong_type enum.
        switch(altlatlong_type)
        {
            case NorthEastDown::spherical:
                ned_state.update_from_spher(ref_point);
                break;

            case NorthEastDown::elliptical:
                ned_state.update_from_ellip(ref_point);
                break;

            case NorthEastDown::undefined:
            default:
                MessageHandler::fail(__FILE__,
                                     __LINE__,
                                     BodyActionMessages::illegal_value,
                                     "%s failed:\n"
                                     "AltLatLong type option not recognized",
                                     action_identifier.c_str());

                // Not reached
                return;
        }

        // Set the frame name based on the reference body.
        ned_state.ned_frame.set_name("ref_point", planet_name, "ned");
    }

    // Reference body supplied: Use its translational state as the basis for
    // the planet-fixed state.
    else
    {
        RefFrameState rel_state;

        // Compute the state of the reference body to the planet fixed frame.
        ref_body->composite_body.compute_relative_state(*pfix_ptr, rel_state);

        // Set the north-east-down frame's translational states.
        ned_state.set_ned_trans_states(rel_state.trans.position, rel_state.trans.velocity);

        // Set the frame name based on the reference body.
        ned_state.ned_frame.set_name(ref_body->name.get_name(), planet_name, "ned");
    }

    // Construct the N-E-D frame's orientation.
    ned_state.altlatlong_type = altlatlong_type;
    ned_state.build_ned_orientation();

    /* (Temporarily) attach the north-east-down frame to the planet-centered,
       planet-fixed frame. */
    pfix_ptr->add_child(ned_state.ned_frame);

    // The position/velocity/attitude/rate data pertain to this newly
    // constructed N-E-D frame.
    reference_ref_frame = &ned_state.ned_frame;

    // Compute the state relative to the parent given the user inputs
    // relative to the reference reference frame.
    apply_user_inputs();

    /* Pass the message up the chain, with the reference frame for the
       state set to this newly-constructed N-E-D frame */
    DynBodyInitPlanetDerived::apply(dyn_manager);

    // Remove the temporary connection established above.
    ned_state.ned_frame.remove_from_parent();

    // The N-E-D frame will go out of scope on returning from this function.
    // The reference_ref_frame should no longer refer to this frame.
    reference_ref_frame = nullptr;
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
