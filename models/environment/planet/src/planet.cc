/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Planet
 * @{
 *
 * @file models/environment/planet/src/planet.cc
 * Planet modeling class methods
 */

/********************************* TRICK HEADER ********************************
Purpose:
   ()

Library Dependency:
   ((planet.cc)
    (base_planet.cc)
    (planet_messages.cc)
    (environment/gravity/src/gravity_source.cc)
    (utils/message/src/message_handler.cc))


*******************************************************************************/

// System includes
#include <cmath>
#include <cstring>
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_manager/include/base_dyn_manager.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/planet.hh"
#include "../include/planet_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a Planet object.
 */
Planet::Planet(
   void)
:
   BasePlanet (),
   r_eq (0.0),
   r_pol (0.0),
   e_ellipsoid (0.0),
   e_ellip_sq (0.0),
   flat_coeff (0.0),
   flat_inv (0.0)
{
}


/**
 * Destruct a Planet object.
 */
Planet::~Planet(
   void)
{
}


/**
 * Register a Planet object with the Dynamics Manager.
 * \param[in,out] grav_source_in GravitySource object
 * \param[in,out] dyn_manager Dynamics manager
 */
void
Planet::register_model (
   GravitySource & grav_source_in,
   BaseDynManager & dyn_manager)
{

   // Register the planet with the dynamics manager.
   register_planet (dyn_manager);


   // Sanity check: The gravity body and the planet must share the same name.
   if ( name.compare( grav_source_in.name ) != 0 ) {
      MessageHandler::fail (
         __FILE__, __LINE__, PlanetMessages::name_error,
         "GravitySource '%s' cannot be linked to Planet '%s'.\n"
         "Names must match exactly.",
         grav_source_in.name.c_str(), name.c_str());
      return;
   }


   // Set the links between the planet and the grav body.
   grav_source = &grav_source_in;
   grav_source->inertial = &this->inertial;
   grav_source->pfix = &this->pfix;

   return;
}


/**
 * Initialize a Planet object.
 *
 * \par Assumptions and Limitations
 *  - Planet::register_model has already been invoked.
 */
void
Planet::initialize (
   void)
{


   // Sanity check: The name must be valid.
   // Note that this is a redundant check (the redundancy is necessary).
   if (name.empty()) {
      MessageHandler::fail (
         __FILE__, __LINE__, PlanetMessages::name_error,
         "Planet object has a null or empty name.");
      return;
   }

   // Sanity check: The model must have been registered.
   if (grav_source == nullptr) {
      MessageHandler::fail (
         __FILE__, __LINE__, PlanetMessages::registration_error,
         "Gravity Source has not been registered.");
      return;
   }

   // Require equatorial radius value to be provided
   if (std::fpclassify(r_eq) == FP_ZERO) {
      MessageHandler::fail (
         __FILE__, __LINE__, PlanetMessages::registration_error,
         "Equatorial radius for planet %s not provided.\n"
         "Please specify value greater than 0.",
         name.c_str());
      return;
   }

   // Set other data fields based on what was given by the user.

   // Flattening coefficient must be between 0 and 1 (exclusive).
   // Flattening is related to the eccentricity via e^2 = f*(2-f).
   if (std::fpclassify(flat_coeff) != FP_ZERO) {
      if ((flat_coeff < 0.0) || (flat_coeff >= 1.0)) {
         MessageHandler::fail (
            __FILE__, __LINE__, PlanetMessages::domain_error,
            "Flattening coefficient %g for planet %s is out of range.\n"
            "The value must be between 0 (inclusive) and 1 (exclusive).",
            flat_coeff, name.c_str());
         return;
      }

      flat_inv = 1.0 / flat_coeff;
      e_ellip_sq = (2.0 * flat_coeff) - (flat_coeff * flat_coeff);
      e_ellipsoid = std::sqrt (e_ellip_sq);
      r_pol = r_eq * (1.0 - flat_coeff);
   }

   // Inverse flattening must be between 1 and infinity.
   else if (std::fpclassify(flat_inv) != FP_ZERO) {
      if (flat_inv <= 1.0) {
         MessageHandler::fail (
            __FILE__, __LINE__, PlanetMessages::domain_error,
            "Inverse flattening coefficient %g for planet %s is out of range.\n"
            "The value must more than 1.",
            flat_inv, name.c_str());
         return;
      }
      flat_coeff = 1.0 / flat_inv;
      e_ellip_sq = (2.0 * flat_coeff) - (flat_coeff * flat_coeff);
      e_ellipsoid = std::sqrt (e_ellip_sq);
      r_pol = r_eq * (1.0 - flat_coeff);
   }

   // Eccentricity must be between 0 and 1 (exclusive).
   else if (std::fpclassify(e_ellipsoid) != FP_ZERO) {
      if ((e_ellipsoid < 0.0) || (e_ellipsoid >= 1.0)) {
         MessageHandler::fail (
            __FILE__, __LINE__, PlanetMessages::domain_error,
            "Ellipsoid eccentricity %g for planet %s is out of range.\n"
            "The value must be between 0 (inclusive) and 1 (exclusive).",
            e_ellipsoid, name.c_str());
         return;
      }

      e_ellip_sq = e_ellipsoid * e_ellipsoid;
      flat_coeff = 1.0 - std::sqrt (1.0 - e_ellip_sq);
      flat_inv = 1.0 / flat_coeff;
      r_pol = r_eq * (1.0 - flat_coeff);
   }

   // Eccentricity squared must be between 0 and 1 (exclusive).
   else if (std::fpclassify(e_ellip_sq) != FP_ZERO) {
      if ((e_ellip_sq < 0.0) || (e_ellip_sq >= 1.0)) {
         MessageHandler::fail (
            __FILE__, __LINE__, PlanetMessages::domain_error,
            "Eccentricity squared %g for planet %s is out of range.\n"
            "The value must be between 0 (inclusive) and 1 (exclusive).",
            e_ellip_sq, name.c_str());
         return;
      }

      e_ellipsoid = std::sqrt (e_ellip_sq);
      flat_coeff = 1.0 - std::sqrt (1.0 - e_ellip_sq);
      flat_inv = 1.0 / flat_coeff;
      r_pol = r_eq * (1.0 - flat_coeff);
   }

   // Polar radius must be between 0 (exclusive) and the equatorial radius.
   else if (std::fpclassify(r_pol) != FP_ZERO) {
      if ((r_pol < 0.0) || (r_pol > r_eq)) {
         MessageHandler::fail (
            __FILE__, __LINE__, PlanetMessages::domain_error,
            "Polar radius %g for planet %s is out of range.\n"
            "The value must not exceed the equatorial radius.",
            name.c_str(), r_pol);
         return;
      }

      flat_coeff = 1.0 - r_pol / r_eq;
      flat_inv = 1.0 / flat_coeff;
      e_ellip_sq = (2.0 * flat_coeff) - (flat_coeff * flat_coeff);
      e_ellipsoid = std::sqrt (e_ellip_sq);
   }

   // Everything empty -- defaults to perfectly spherical planet
   else {
      flat_coeff = 0.0;
      flat_inv   = 0.0;
      e_ellipsoid = 0.0;
      e_ellip_sq = 0.0;
      r_pol = r_eq;
   }
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
