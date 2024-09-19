/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Ephemerides
 * @{
 * @addtogroup PropagatedPlanet
 * @{
 *
 * @file models/environment/ephemerides/propagated_planet/src/propagated_planet.cc
 * Define the methods of the classes defined in propagated_planet.hh.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

LIBRARY DEPENDENCY:
  ((propagated_planet.cc)
   (dynamics/dyn_manager/src/initialize_dyn_bodies.cc)
   (dynamics/dyn_body/src/dyn_body_propagate_state.cc)
   (dynamics/dyn_body/src/dyn_body_set_state.cc)
   (dynamics/mass/src/mass_point_state.cc)
   (environment/ephemerides/ephem_interface/src/ephem_messages.cc)
   (environment/ephemerides/ephem_item/src/ephem_item.cc)
   (environment/ephemerides/ephem_item/src/ephem_orient.cc)
   (environment/ephemerides/ephem_item/src/ephem_point.cc)
   (environment/ephemerides/ephem_manager/src/ephem_manager.cc)
   (environment/time/src/time.cc)
   (environment/time/src/time_dyn.cc)
   (environment/time/src/time_manager.cc)
   (utils/memory/src/memory_manager_static.cc)
   (utils/message/src/message_handler.cc)
   (utils/named_item/src/named_item.cc)
   (utils/orientation/src/orientation.cc)
   (utils/ref_frames/src/ref_frame.cc)
   (utils/ref_frames/src/ref_frame_compute_relative_state.cc)
   (utils/ref_frames/src/ref_frame_manager.cc)
   (utils/ref_frames/src/ref_frame_state.cc)
   (utils/ref_frames/src/subscription.cc))



*******************************************************************************/

// System includes
#include <climits>
#include <cstddef>
#include <cstdio>

// JEOD includes
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "environment/ephemerides/ephem_interface/include/ephem_messages.hh"
#include "environment/ephemerides/ephem_manager/include/ephem_manager.hh"
#include "environment/planet/include/base_planet.hh"
#include "environment/time/include/time_dyn.hh"
#include "environment/time/include/time_manager.hh"
#include "environment/time/include/time_tt.hh"
#include "utils/math/include/vector3.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/named_item/include/named_item.hh"

// Model includes
#include "../include/propagated_planet.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Copy rotational state from/to the body reference frame.
 * \param[in] dyn_time Dynamic time seconds\n Units: s
 */
void PropagatedEphemerisPlanet::update(double dyn_time)
{
    RefFrameState rel_state;

    // Item is enabled =>
    // The body reference frame state is the source of the planetary state.
    if(enabled)
    {
        body_ref_frame.compute_relative_state(*(target_frame->get_parent()), rel_state);
        target_frame->state.trans = rel_state.trans;
        target_frame->set_timestamp(dyn_time);
    }

    // This object is still 'active' when the ephemeris item is disabled.
    // Enabled/disabled dictates which item updates the planet state.
    // In this case, that the object is disabled means that the body ref frame
    // state is driven by the planet state.
    else
    {
        target_frame->compute_relative_state(*(body_ref_frame.get_parent()), rel_state);
        body.set_state(RefFrameItems::Pos_Vel, rel_state, body_ref_frame);
        body_ref_frame.set_timestamp(dyn_time);
    }

    update_time = dyn_time;
}

/**
 * PropagatedEphemerisOrientation non-default constructor.
 * \param[in,out] dyn_body The DynBody that represents the planet
 * \param[in,out] frame The body reference frame
 */
PropagatedEphemerisOrientation::PropagatedEphemerisOrientation(DynBody & dyn_body, BodyRefFrame & frame)
    : EphemerisOrientation(),
      body(dyn_body),
      body_ref_frame(frame)
{
}

/**
 * Copy rotational state from/to the body reference frame.
 * \param[in] dyn_time Dynamic time seconds\n Units: s
 */
void PropagatedEphemerisOrientation::update(double dyn_time)
{
    RefFrameState rel_state;

    // Item is enabled =>
    // The body reference frame state is the source of the planetary state.
    if(enabled)
    {
        body_ref_frame.compute_relative_state(*(target_frame->get_parent()), rel_state);
        target_frame->state.rot = rel_state.rot;
        target_frame->set_timestamp(dyn_time);
    }

    // This object is still 'active' when the ephemeris item is disabled.
    // Enabled/disabled dictates which item updates the planet state.
    // In this case, that the object is disabled means that the body ref frame
    // state is driven by the planet state.
    else
    {
        target_frame->compute_relative_state(*(body_ref_frame.get_parent()), rel_state);
        body.set_state(RefFrameItems::Att_Rate, rel_state, body_ref_frame);
        body_ref_frame.set_timestamp(dyn_time);
    }

    update_time = dyn_time;
}

/**
 * PropagatedEphemerisPlanet non-default constructor.
 * \param[in,out] dyn_body The DynBody that represents the planet
 * \param[in,out] frame The body reference frame
 */
PropagatedEphemerisPlanet::PropagatedEphemerisPlanet(DynBody & dyn_body, BodyRefFrame & frame)
    : EphemerisPoint(),
      body(dyn_body),
      body_ref_frame(frame)
{
}

/**
 * PropagatedPlanet default constructor.
 */
PropagatedPlanet::PropagatedPlanet()
    : body(),
      ephem_planet(body, body.composite_body),
      ephem_orient(body, body.composite_body)
{
    ephem_planet.set_owner(this);
    ephem_orient.set_owner(this);
}

/**
 * Nominally, activate the object.
 * In the case of a PropagatedPlanet object,
 * an inactive object cannot be activated.
 */
void PropagatedPlanet::activate()
{
    if(!active)
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              EphemeridesMessages::internal_error,
                              "A previously inactive PropagatedPlanet cannot be re-activated.");
    }
}

/**
 * Deactivate the PropagatedPlanet object.
 */
void PropagatedPlanet::deactivate()
{
    active = false;
}

/**
 * Return time of last update.
 * @return Timestamp\n Units: day
 */
double PropagatedPlanet::timestamp() const
{
    return update_time;
}

/**
 * Return model name.
 * @return Name
 */
std::string PropagatedPlanet::get_name() const
{
    return ident;
}

/**
 * Initialize a PropagatedPlanet model.
 *
 * \par Assumptions and Limitations
 *  - This method must be called after the ephemeris model that defines
 *      the parent frame has been initialized.
 * \param[in] time_manager Time manager
 * \param[in,out] dyn_manager_ref Dynamics manager
 */
void PropagatedPlanet::initialize_model(const TimeManager & time_manager, DynManager & dyn_manager_ref)
{
    // Nothing to do if the model is inactive.
    if(!active)
    {
        return;
    }

    // Construct the identifier for this model.
    ident = NamedItem::construct_name("PropagatedPlanet", planet_name);

    // Save the dyn_manager object pointer.
    dyn_manager = &dyn_manager_ref;

    // Get pointers to the ephemeris time and dynamic time time objects.
    time_dyn = dynamic_cast<const TimeDyn *>(time_manager.get_time_ptr("Dyn"));
    if(time_dyn == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             EphemeridesMessages::inconsistent_setup,
                             "Could not find the Dynamic Time time object.");

        // Not reached
        return;
    }

    // Find the parent frame, which must exist.
    parent_frame = dyn_manager->find_integ_frame(parent_name);

    if(parent_frame == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             EphemeridesMessages::inconsistent_setup,
                             "Frame '%s' has not been registered with the manager.",
                             parent_name.c_str());
        // Not reached
        return;
    }

    // Tell the dynamics manager that this is an active ephemeris model.
    dyn_manager->add_ephemeris(*this);

    // Tell the EphemeridesManager about the ephemeris items that manage
    // the planet's inertial and planet-fixed frames.
    ephem_planet.set_name(planet_name, "inertial");
    ephem_orient.set_name(planet_name, "pfix");
    dyn_manager->add_ephem_item(ephem_planet);
    dyn_manager->add_ephem_item(ephem_orient);

    // Name the body and its parent frame if not already done by the user.
    if(body.name.size() == 0)
    {
        body.name.set_name(planet_name);
    }
    if(body.integ_frame_name.empty())
    {
        body.integ_frame_name = parent_name;
    }

    // Initialize the body and register it with the dynamics manager.
    body.initialize_model(*dyn_manager);
}

/**
 * Mark appropriate items in the model as active.
 * \param[in,out] ephem_manager Ephemerides manager
 */
void PropagatedPlanet::ephem_initialize(EphemeridesManager & ephem_manager)
{
    // Nothing to do if the model is inactive.
    if(!active)
    {
        return;
    }

    // Find the planet, which must exist.
    planet = ephem_manager.find_base_planet(planet_name);

    if(planet == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             EphemeridesMessages::inconsistent_setup,
                             "Planet '%s' has not been registered with the manager.",
                             planet_name.c_str());
        // Not reached
        return;
    }

    ephem_manager.set_target_frame(planet->inertial);
    ephem_manager.set_target_frame(planet->pfix);

    // The ephemeris items must target the proper frames.
    if((ephem_planet.get_target_frame() != &planet->inertial) || (ephem_orient.get_target_frame() != &planet->pfix))
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             EphemeridesMessages::inconsistent_setup,
                             "Propagated planet '%s' does not target the planet frames.",
                             planet_name.c_str());
    }

    // Dynamic bodies are always active, so the planetary frames must also always
    // be active. Subscribe to the planet frames to ensure this is the case.
    planet->inertial.subscribe();
    planet->pfix.subscribe();

    // Enable/disable the ephemeris objects per the mode.
    // Enabled means that the planet frame depends on the body frame;
    // disabled means the body frame depends on the planet frame.
    set_mode();
}

/**
 * Setter for the commanded mode.
 * \param[in] new_mode New commanded mode
 */
void PropagatedPlanet::set_commanded_mode(PropagatedPlanet::Mode new_mode)
{
    commanded_mode = new_mode;
}

/**
 * Change the behavior of a PropagatedPlanet.
 */
void PropagatedPlanet::set_mode()
{
    // Switching from the body being the source to the planet is illegal.
    if((((mode == TransFromBody_RotFromPlanet) || (mode == TransFromBody_RotFromBody)) &&
        ((commanded_mode == TransFromPlanet_RotFromPlanet) || (commanded_mode == TransFromPlanet_RotFromBody))) ||
       (((mode == TransFromPlanet_RotFromBody) || (mode == TransFromBody_RotFromBody)) &&
        ((commanded_mode == TransFromPlanet_RotFromPlanet) || (commanded_mode == TransFromBody_RotFromPlanet))))
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             EphemeridesMessages::inconsistent_setup,
                             "Illegal attempt to switch PropagatedPlanet to ephemeris mode.");
        return;
    }

    // Enable/disable the ephemeris objects per the mode.
    // Enabled means that the planet frame depends on the body frame;
    // disabled means the body frame depends on the planet frame.
    if((commanded_mode == TransFromBody_RotFromPlanet) || (commanded_mode == TransFromBody_RotFromBody))
    {
        ephem_planet.enable();
    }
    else
    {
        ephem_planet.disable();
    }

    if((commanded_mode == TransFromPlanet_RotFromBody) || (commanded_mode == TransFromBody_RotFromBody))
    {
        ephem_orient.enable();
    }
    else
    {
        ephem_orient.disable();
    }

    mode = commanded_mode;
    dyn_manager->ephem_note_tree_status_change();
}

/**
 * Activate ephemerides.
 * \param[in,out] ephem_manager Ephemerides manager
 */
void PropagatedPlanet::ephem_activate(EphemeridesManager & ephem_manager JEOD_UNUSED)
{
    // Determine data flow directions.
    bool body_is_trans_source = (mode == TransFromBody_RotFromPlanet) || (mode == TransFromBody_RotFromBody);
    bool body_is_rot_source = (mode == TransFromPlanet_RotFromBody) || (mode == TransFromBody_RotFromBody);

    body.translational_dynamics = body_is_trans_source;
    body.rotational_dynamics = body_is_rot_source;
}

/**
 * Construct the ephemeris model portions of the reference frame tree.
 * \param[in,out] ephem_manager Ephemerides manager
 */
void PropagatedPlanet::ephem_build_tree(EphemeridesManager & ephem_manager)
{
    // Nothing to do if the model is inactive.
    if(!active)
    {
        return;
    }

    // Add the planet to the tree.
    if(planet->inertial.get_parent() != parent_frame)
    {
        planet->inertial.remove_from_parent();
        ephem_manager.add_frame_to_tree(planet->inertial, parent_frame);
    }
}

/**
 * Update ephemerides for subscribed items.
 */
void PropagatedPlanet::ephem_update()
{
    // Nothing to do if the model is inactive.
    if(!active)
    {
        return;
    }

    // Initialize the dyn body if this hasn't been done yet.
    if(!initialized)
    {
        dyn_manager->initialize_dyn_body(body);
        initialized = true;
    }

    // Get the time of this update.
    update_time = time_dyn->seconds;

    // Update the dependent translational and rotational states from the sources.
    ephem_planet.update(update_time);
    ephem_orient.update(update_time);

    // Not in full body mode => need to propagate state throughout the body.
    if(mode != TransFromBody_RotFromBody)
    {
        body.propagate_state();
    }

    // Finally, change the mode to the commanded mode if needed.
    // This must be done *after* the two update calls above to accommodate
    // a change from FromPlanet to FromBody at post-initialization time.
    if(commanded_mode != mode)
    {
        set_mode();
    }
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 * @}
 */
