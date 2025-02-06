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
  ((dyn_body_init_orbit.o)
   (body_action.o)
   (body_action_messages.o)
   (dyn_body_init.o)
   (dyn_body_init_trans_state.o)
   (dynamics/mass/mass_point_state.o)
   (utils/message/message_handler.o)
   (utils/named_item/named_item.o)
   (utils/orbital_elements/orbital_elements.o)
   (utils/ref_frames/ref_frame.o))



*******************************************************************************/

// System includes
#include <cstddef>
#include <math.h>

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
namespace jeod {

/**
 * DynBodyInitOrbit default constructor.
 */
DynBodyInitOrbit::DynBodyInitOrbit (
   void)
{
   set = InvalidSet;

   semi_major_axis    = 0.0;
   semi_latus_rectum  = 0.0;
   alt_periapsis      = 0.0;
   alt_apoapsis       = 0.0;
   orb_radius         = 0.0;
   radial_vel         = 0.0;
   eccentricity       = 0.0;
   inclination        = 0.0;
   ascending_node     = 0.0;
   arg_periapsis      = 0.0;
   arg_latitude       = 0.0;
   time_periapsis     = 0.0;
   mean_anomaly       = 0.0;
   true_anomaly       = 0.0;

   planet      = nullptr;
   orbit_frame = nullptr;
}


/**
 * DynBodyInitOrbit destructor.
 */
DynBodyInitOrbit::~DynBodyInitOrbit (
   void)
{
}


/**
 * Initialize the initializer.
 * \param[in,out] dyn_manager Dynamics manager
 */
void
DynBodyInitOrbit::initialize (
   DynManager & dyn_manager)
{
   char * frame_name;
   RefFrame * found_frame;

   // Pass the message up the chain. This will initialize the base
   // characteristics of the instance.
   DynBodyInitTransState::initialize (dyn_manager);

   // Sanity check: The orbital set must be valid.
   switch (set) {
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
      MessageHandler::fail (
         __FILE__, __LINE__, BodyActionMessages::illegal_value,
         "%s error:\n"
         "Invalid orbital set",
         action_identifier.c_str());

      // Not reached
      return;
   }

   // Find the planet with the given name.
   planet = find_planet (dyn_manager, planet_name, "planet_name");

   // Sanity check: The planet must have a gravity model.
   if (planet->grav_source == nullptr) {
      MessageHandler::fail (
         __FILE__, __LINE__, BodyActionMessages::invalid_object,
         "%s error:\n"
         "Planet '%s' refers to no gravity source",
         action_identifier.c_str(), planet_name.c_str());

      // Not reached
      return;
   }

   // Sanity check: The orbital frame name must be provided.
   validate_name (orbit_frame_name, "orbit_frame_name", "RefFrame");

   // Find the orbital reference frame.
   frame_name = NamedItem::construct_name (
                   planet_name.c_str(),
                   NamedItem::suffix (planet_name.c_str(), orbit_frame_name.c_str()));
   found_frame = dyn_manager.find_ref_frame (frame_name);
   orbit_frame = dynamic_cast<EphemerisRefFrame *>(found_frame);

   // Sanity checks: The frame must exist and must be an ephemeris frame.
   if (found_frame == nullptr) {
      MessageHandler::fail (
         __FILE__, __LINE__, BodyActionMessages::invalid_name,
         "%s error:\n"
         "Planet '%s' does not have a '%s' frame",
         action_identifier.c_str(), planet_name.c_str(), orbit_frame_name.c_str());
   }

   if (orbit_frame == nullptr) {
      MessageHandler::fail (
         __FILE__, __LINE__, BodyActionMessages::invalid_object,
         "%s error:\n"
         "Planet '%s' frame '%s' is not an ephemeris reference frame",
         action_identifier.c_str(), planet_name.c_str(), orbit_frame_name.c_str());
   }

   JEOD_DELETE_ARRAY (frame_name);


   // The reverse_sense setting makes no sense here.
   // FIXME: Sanity check? Warn about override.
   reverse_sense = false;

   // Set the reference ref_frame to the planet-centered inertial frame to
   // ensure that the planet's state is computed.
   reference_ref_frame = &planet->inertial;
}


/**
 * Apply the initializer.
 * \param[in,out] dyn_manager Dynamics manager
 */
void
DynBodyInitOrbit::apply (
   DynManager & dyn_manager)
{
   OrbitalElements elem;
   enum {
      ShapeAltitudes,
      ShapeSemiMajorAxis,
      ShapeSemiLatusRectum
   } shape;
   enum {
      LocationTimePeri,
      LocationMeanAnom,
      LocationTrueAnom
   } location;

   // Convert the input elements to inclination, ascending node, arg. peri PLUS
   // - altitudes at periapsis, apoapsis, OR
   //   semi-major axis and eccentricity  OR
   //   semi-latus rectum and eccentricity
   // - time since periapsis, mean anomaly, or true anomaly.
   switch (set) {

   // SmaEccIncAscnodeArgperTimeperi:
   // Have a,e,i,Omega,omega,T.
   case SmaEccIncAscnodeArgperTimeperi:
      shape    = ShapeSemiMajorAxis;
      location = LocationTimePeri;
      break;

   // SmaEccIncAscnodeArgperManom:
   // Have a,e,i,Omega,omega,M.
   case SmaEccIncAscnodeArgperManom:
      shape    = ShapeSemiMajorAxis;
      location = LocationMeanAnom;
      break;

   // SlrEccIncAscnodeArgperTanom:
   // Have alpha,e,i,Omega,omega,nu.
   case SlrEccIncAscnodeArgperTanom:
      shape    = ShapeSemiLatusRectum;
      location = LocationTrueAnom;
      break;

   // IncAscnodeAltperAltapoArgperTanom
   // Have i,Omega,perialt,apoalt,omega,nu.
   case IncAscnodeAltperAltapoArgperTanom:
   case CaseEleven:
      shape    = ShapeAltitudes;
      location = LocationTrueAnom;
      break;

   // IncAscnodeAltperAltapoArgperTimeperi:
   // Have i,Omega,perialt,apoalt,omega,T.
   case IncAscnodeAltperAltapoArgperTimeperi:
      shape    = ShapeAltitudes;
      location = LocationTimePeri;
      break;

   // SmaIncAscnodeArglatRadRadvel:
   // Have a,i,Omega,arglat,r,rdot.
   // Compute omega,e,nu from the above.
   case SmaIncAscnodeArglatRadRadvel:
   {
      double ecosE, esinE, ecc_sq, kcost, ksint;

      // Compute the eccentricity times the
      // sine and cosine of the eccentric anomaly.
      ecosE = (semi_major_axis - orb_radius) / semi_major_axis;
      esinE = (radial_vel * orb_radius) /
              std::sqrt (planet->grav_source->mu * semi_major_axis);

      // Compute the eccentricity.
      ecc_sq = ecosE * ecosE + esinE * esinE ;
      eccentricity = std::sqrt (ecc_sq);

      // Non-circular orbit: Compute the true anomaly.
      if (eccentricity >= 1.0e-14) {
         kcost = ecosE - ecc_sq;
         ksint = std::sqrt (1.0 - ecc_sq) * esinE ;
         true_anomaly = std::atan2 (ksint, kcost);
      }

      // Circular orbit: Set the true anomaly to zero.
      else {
         true_anomaly = 0.0 ;
      }

      // Compute the argument of periapsis.
      arg_periapsis = arg_latitude - true_anomaly;

      shape    = ShapeSemiMajorAxis;
      location = LocationTrueAnom;
      break;
   }

   // SmaEccIncAscnodeArgperTanom:
   // Have a,e,i,Omega,omega,nu.
   case SmaEccIncAscnodeArgperTanom:
      shape    = ShapeSemiMajorAxis;
      location = LocationTrueAnom;
      break;

   // All others are illegal.
   case InvalidSet:
   default:
      MessageHandler::fail (
         __FILE__, __LINE__, BodyActionMessages::illegal_value,
         "%s error:\n"
         "Invalid orbital set",
         action_identifier.c_str());

      // Not reached
      return;
   }


   // Compute the semi-major axis and eccentricity from altitudes.
   if (shape == ShapeAltitudes) {
      semi_major_axis = planet->r_eq +
                        0.5 * (alt_apoapsis + alt_periapsis);
      eccentricity    = (alt_apoapsis - alt_periapsis) /
                        (2.0 * semi_major_axis);
      shape = ShapeSemiMajorAxis;
   }

   // Compute the semi-latus rectum from the semi-major axis and eccentricity.
   if (shape == ShapeSemiMajorAxis) {
      semi_latus_rectum = semi_major_axis *
                          ( 1.0 - eccentricity * eccentricity);
      shape = ShapeSemiLatusRectum;
   }

   // Compute the mean anomaly from the time since periapsis passage.
   if (location == LocationTimePeri) {
      mean_anomaly = time_periapsis *
                     std::sqrt (planet->grav_source->mu / semi_major_axis) / semi_major_axis;
      location     = LocationMeanAnom;
   }

   // At this point, have alpha,e,i,Omega,omega and either nu or M.
   // All that remains to be resolved is mean versus true anomaly.
   // Populate the orbital elements structure.
   elem.semiparam     = semi_latus_rectum;
   elem.e_mag         = eccentricity;
   elem.inclination   = inclination;
   elem.arg_periapsis = arg_periapsis;
   elem.long_asc_node = ascending_node;

   // Compute true anomaly if needed.
   if (location == LocationMeanAnom) {
      elem.mean_anom = mean_anomaly;
      elem.mean_anom_to_nu();
   }
   else {
      elem.true_anom = true_anomaly;
      elem.nu_to_anomalies();
   }

   // Compute cartesian coordinates.
   elem.to_cartesian (planet->grav_source->mu, position, velocity);

   // Transform to planet-centered inertial.
   if (orbit_frame != &(planet->inertial)) {
      RefFrameState rel_state;
      orbit_frame->compute_relative_state (planet->inertial, rel_state);
      // Sanity check? Should have null position, velocity from the
      // planet inertial to the orbit frame.

      Vector3::transform_transpose (rel_state.rot.T_parent_this, position);
      Vector3::transform_transpose (rel_state.rot.T_parent_this, velocity);
   }

   // Pass the message up the chain.
   DynBodyInitTransState::apply (dyn_manager);

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
