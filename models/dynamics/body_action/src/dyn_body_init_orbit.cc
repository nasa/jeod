/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/src/dyn_body_init_orbit.cc
 * Define classes for items represented in some ephemeris model.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((dyn_body_init_orbit.cc)
   (body_action.cc)
   (body_action_messages.cc)
   (dyn_body_init.cc)
   (dyn_body_init_trans_state.cc)
   (dynamics/mass/src/mass_point_state.cc)
   (utils/message/src/message_handler.cc)
   (utils/named_item/src/named_item.cc)
   (utils/orbital_elements/src/orbital_elements.cc)
   (utils/ref_frames/src/ref_frame.cc))



*******************************************************************************/

// System includes
#include <cmath>
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "environment/ephemerides/ephem_interface/include/ephem_ref_frame.hh"
#include "environment/planet/include/planet.hh"
#include "utils/math/include/vector3.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/named_item/include/named_item.hh"
#include "utils/orbital_elements/include/orbital_elements.hh"

// Model includes
#include "../include/body_action_messages.hh"
#include "../include/dyn_body_init_orbit.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Initialize the initializer.
 * \param[in,out] dyn_manager Dynamics manager
 */
void DynBodyInitOrbit::initialize(DynManager & dyn_manager)
{
    std::string frame_name;
    RefFrame * found_frame;

    // Pass the message up the chain. This will initialize the base
    // characteristics of the instance.
    DynBodyInitTransState::initialize(dyn_manager);

    // Sanity check: The orbital set must be valid.
    switch(set)
    {
        case SmaEccIncAscnodeArgperTimeperi:
        case SmaEccIncAscnodeArgperManom:
        case SlrEccIncAscnodeArgperTanom:
        case IncAscnodeAltperAltapoArgperTanom:
        case IncAscnodeAltperAltapoArgperTimeperi:
        case SmaIncAscnodeArglatRadRadvel:
        case SmaEccIncAscnodeArgperTanom:
        case CaseEleven:
            break;

        case InvalidSet:
        default:
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 BodyActionMessages::illegal_value,
                                 "%s error:\n"
                                 "Invalid orbital set",
                                 action_identifier.c_str());

            // Not reached
            return;
    }

    // Find the planet with the given name.
    planet = find_planet(dyn_manager, planet_name, "planet_name");

    // Sanity check: The planet must have a gravity model.
    if(planet->grav_source == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             BodyActionMessages::invalid_object,
                             "%s error:\n"
                             "Planet '%s' refers to no gravity source",
                             action_identifier.c_str(),
                             planet_name.c_str());

        // Not reached
        return;
    }

    // Sanity check: The orbital frame name must be provided.
    validate_name(orbit_frame_name, "orbit_frame_name", "RefFrame");

    // Find the orbital reference frame.
    frame_name = NamedItem::construct_name(planet_name, NamedItem::suffix(planet_name, orbit_frame_name));
    found_frame = dyn_manager.find_ref_frame(frame_name);
    orbit_frame = dynamic_cast<EphemerisRefFrame *>(found_frame);

    // Sanity checks: The frame must exist and must be an ephemeris frame.
    if(found_frame == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             BodyActionMessages::invalid_name,
                             "%s error:\n"
                             "Planet '%s' does not have a '%s' frame",
                             action_identifier.c_str(),
                             planet_name.c_str(),
                             orbit_frame_name.c_str());
        return;
    }

    if(orbit_frame == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             BodyActionMessages::invalid_object,
                             "%s error:\n"
                             "Planet '%s' frame '%s' is not an ephemeris reference frame",
                             action_identifier.c_str(),
                             planet_name.c_str(),
                             orbit_frame_name.c_str());
        return;
    }

    // Set the reference ref_frame to the planet-centered inertial frame to
    // ensure that the planet's state is computed.
    reference_ref_frame = &planet->inertial;
}

/**
 * Apply the initializer.
 * \param[in,out] dyn_manager Dynamics manager
 */
void DynBodyInitOrbit::apply(DynManager & dyn_manager)
{
    OrbitalElements elem;

    enum
    {
        ShapeAltitudes,
        ShapeSemiMajorAxis,
        ShapeSemiLatusRectum
    } shape;

    enum
    {
        LocationTimePeri,
        LocationMeanAnom,
        LocationTrueAnom
    } location;

    // Convert the input elements to inclination, ascending node, arg. peri PLUS
    // - altitudes at periapsis, apoapsis, OR
    //   semi-major axis and eccentricity  OR
    //   semi-latus rectum and eccentricity
    // - time since periapsis, mean anomaly, or true anomaly.
    switch(set)
    {
        // SmaEccIncAscnodeArgperTimeperi:
        // Have a,e,i,Omega,omega,T.
        case SmaEccIncAscnodeArgperTimeperi:
            shape = ShapeSemiMajorAxis;
            location = LocationTimePeri;
            break;

        // SmaEccIncAscnodeArgperManom:
        // Have a,e,i,Omega,omega,M.
        case SmaEccIncAscnodeArgperManom:
            shape = ShapeSemiMajorAxis;
            location = LocationMeanAnom;
            break;

        // SlrEccIncAscnodeArgperTanom:
        // Have alpha,e,i,Omega,omega,nu.
        case SlrEccIncAscnodeArgperTanom:
            shape = ShapeSemiLatusRectum;
            location = LocationTrueAnom;
            break;

        // IncAscnodeAltperAltapoArgperTanom
        // Have i,Omega,perialt,apoalt,omega,nu.
        case IncAscnodeAltperAltapoArgperTanom:
        case CaseEleven:
            shape = ShapeAltitudes;
            location = LocationTrueAnom;
            break;

        // IncAscnodeAltperAltapoArgperTimeperi:
        // Have i,Omega,perialt,apoalt,omega,T.
        case IncAscnodeAltperAltapoArgperTimeperi:
            shape = ShapeAltitudes;
            location = LocationTimePeri;
            break;

        // SmaIncAscnodeArglatRadRadvel:
        // Have a,i,Omega,arglat,r,rdot.
        // Compute omega,e,nu from the above.
        case SmaIncAscnodeArglatRadRadvel:
        {
            double ecosE, esinE, ecc_sq;

            // Compute the eccentricity times the
            // sine and cosine of the eccentric anomaly.
            ecosE = (semi_major_axis - orb_radius) / semi_major_axis;
            esinE = (radial_vel * orb_radius) / std::sqrt(planet->grav_source->mu * semi_major_axis);

            // Compute the eccentricity.
            ecc_sq = ecosE * ecosE + esinE * esinE;
            eccentricity = std::sqrt(ecc_sq);

            // Non-circular orbit: Compute the true anomaly.
            if(eccentricity >= 1.0e-14)
            {
                double kcost = ecosE - ecc_sq;
                double ksint = std::sqrt(1.0 - ecc_sq) * esinE;
                true_anomaly = std::atan2(ksint, kcost);
            }

            // Circular orbit: Set the true anomaly to zero.
            else
            {
                true_anomaly = 0.0;
            }

            // Compute the argument of periapsis.
            arg_periapsis = arg_latitude - true_anomaly;

            shape = ShapeSemiMajorAxis;
            location = LocationTrueAnom;
            break;
        }

        // SmaEccIncAscnodeArgperTanom:
        // Have a,e,i,Omega,omega,nu.
        case SmaEccIncAscnodeArgperTanom:
            shape = ShapeSemiMajorAxis;
            location = LocationTrueAnom;
            break;

        // All others are illegal.
        case InvalidSet:
        default:
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 BodyActionMessages::illegal_value,
                                 "%s error:\n"
                                 "Invalid orbital set",
                                 action_identifier.c_str());

            // Not reached
            return;
    }

    // Compute the semi-major axis and eccentricity from altitudes.
    if(shape == ShapeAltitudes)
    {
        semi_major_axis = planet->r_eq + 0.5 * (alt_apoapsis + alt_periapsis);
        eccentricity = (alt_apoapsis - alt_periapsis) / (2.0 * semi_major_axis);
        shape = ShapeSemiMajorAxis;
    }

    // Compute the semi-latus rectum from the semi-major axis and eccentricity.
    if(shape == ShapeSemiMajorAxis)
    {
        semi_latus_rectum = semi_major_axis * (1.0 - eccentricity * eccentricity);
        // shape = ShapeSemiLatusRectum;
    }

    // Compute the mean anomaly from the time since periapsis passage.
    if(location == LocationTimePeri)
    {
        mean_anomaly = time_periapsis * std::sqrt(planet->grav_source->mu / semi_major_axis) / semi_major_axis;
        location = LocationMeanAnom;
    }

    // At this point, have alpha,e,i,Omega,omega and either nu or M.
    // All that remains to be resolved is mean versus true anomaly.
    // Populate the orbital elements structure.
    elem.semiparam = semi_latus_rectum;
    elem.e_mag = eccentricity;
    elem.inclination = inclination;
    elem.arg_periapsis = arg_periapsis;
    elem.long_asc_node = ascending_node;

    // Compute true anomaly if needed.
    if(location == LocationMeanAnom)
    {
        elem.mean_anom = mean_anomaly;
        elem.mean_anom_to_nu();
    }
    else
    {
        elem.true_anom = true_anomaly;
        elem.nu_to_anomalies();
    }

    // Compute cartesian coordinates.
    elem.to_cartesian(planet->grav_source->mu, position, velocity);

    // Transform to planet-centered inertial.
    if(orbit_frame != &(planet->inertial))
    {
        RefFrameState rel_state;
        orbit_frame->compute_relative_state(planet->inertial, rel_state);
        // Sanity check? Should have null position, velocity from the
        // planet inertial to the orbit frame.

        Vector3::transform_transpose(rel_state.rot.T_parent_this, position);
        Vector3::transform_transpose(rel_state.rot.T_parent_this, velocity);
    }

    // Pass the message up the chain.
    DynBodyInitTransState::apply(dyn_manager);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
