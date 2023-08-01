/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Gravity
 * @{
 *
 * @file models/environment/gravity/src/spherical_harmonics_tidal_effects.cc
 * Define member functions for the SphericalHarmonicsTidalEffects class.
 */

/*******************************************************************************

Purpose:
  ()

References:
  (((none)))

Assumptions and limitations:
  ((none))

Library dependencies:
  ((spherical_harmonics_tidal_effects.cc)
   (spherical_harmonics_delta_coeffs.cc)
   (spherical_harmonics_delta_coeffs_init.cc)
   (spherical_harmonics_tidal_effects_init.cc)
   (spherical_harmonics_gravity_source.cc)
   (gravity_messages.cc)
   (environment/planet/src/planet.cc)
   (utils/message/src/message_handler.cc)
   (utils/ref_frames/src/ref_frame.cc))



*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_manager/include/base_dyn_manager.hh"
#include "environment/planet/include/planet.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/ref_frames/include/ref_frame.hh"

// Model includes
#include "../include/spherical_harmonics_tidal_effects.hh"
#include "../include/spherical_harmonics_delta_coeffs_init.hh"
#include "../include/spherical_harmonics_tidal_effects_init.hh"
#include "../include/gravity_messages.hh"
#include "../include/spherical_harmonics_gravity_source.hh"


//! Namespace jeod
namespace jeod {

// Attributes used in allocations
JEOD_DECLARE_ATTRIBUTES (Planet)
JEOD_DECLARE_ATTRIBUTES (RefFrame)



/**
 * SphericalHarmonicsTidalEffects constructor.
 */
SphericalHarmonicsTidalEffects::SphericalHarmonicsTidalEffects (
   void)
{
   xp               = 0.0;
   yp               = 0.0;
   k2               = 0.0;
   Knm              = nullptr;
   num_tidal_bodies = 0;
   tidal_bodies          = nullptr;
   tidal_bodies_inertial = nullptr;
   pfix                  = nullptr;
}


/**
 * SphericalHarmonicsTidalEffects destructor.
 */
SphericalHarmonicsTidalEffects::~SphericalHarmonicsTidalEffects (
   void)
{

   if (tidal_bodies != nullptr) {
      JEOD_DELETE_ARRAY (tidal_bodies);
      tidal_bodies = nullptr;
   }

   if (tidal_bodies_inertial != nullptr) {
      JEOD_DELETE_ARRAY (tidal_bodies_inertial);
      tidal_bodies_inertial = nullptr;
   }

   if (Knm != nullptr) {
      for (unsigned int ii = 0; ii <= degree; ++ii) {
         JEOD_DELETE_ARRAY (Knm[ii]);
      }
      JEOD_DELETE_ARRAY (Knm);
      Knm = nullptr;
   }
}


/**
 * Initialize a SphericalHarmonicsTidalEffects object.
 * This method overrides and calls the base class initialize method.
 * \param[in] gen_var_init Effect init structure
 * \param[in] dyn_manager Dynamics manager
 */
void
SphericalHarmonicsTidalEffects::initialize (
   SphericalHarmonicsDeltaCoeffsInit & gen_var_init,
   BaseDynManager & dyn_manager)
{
   SphericalHarmonicsTidalEffectsInit * var_init;

   // This initializer operates on tidal effects init.
   // Cast the supplied generic delta coeffs init object to a tidal effects.
   var_init = dynamic_cast<SphericalHarmonicsTidalEffectsInit*> (&gen_var_init);

   // Failure to cast is a non-fatal error.
   if (var_init == nullptr) {
      MessageHandler::error (
         __FILE__, __LINE__, GravityMessages::invalid_object,
         "Invalid SphericalHarmonicsDeltaCoeffsInit object.\n"
         "The object must be a reference to a\n"
         "SphericalHarmonicsTidalEffectsInit object.");
      return;
   }


   // Set variables from init structure
   xp               = var_init->xp;
   yp               = var_init->yp;
   k2               = var_init->k2;
   num_tidal_bodies = var_init->num_tidal_bodies;

   // A null body names array for a non-empty list of tidal bodies is fatal.
   if ((var_init->tidal_body_names == nullptr) && (num_tidal_bodies != 0)) {
      MessageHandler::fail (
         __FILE__, __LINE__, GravityMessages::invalid_object,
         "Invalid SphericalHarmonicsTidalEffectsInit object.\n"
         "The array of tidal body names is null.");
      return;
   }


   // Allocate arrays for this tidal effects object.
   tidal_bodies =
      JEOD_ALLOC_CLASS_POINTER_ARRAY (num_tidal_bodies, Planet);
   tidal_bodies_inertial =
      JEOD_ALLOC_CLASS_POINTER_ARRAY (num_tidal_bodies, RefFrame);

   for (unsigned int ii = 0; ii < num_tidal_bodies; ++ii) {

      // Check for an invalid name.
      if ((var_init->tidal_body_names[ii] == nullptr) ||
          (var_init->tidal_body_names[ii][0] == '\0')) {
         MessageHandler::fail (
            __FILE__, __LINE__, GravityMessages::invalid_name,
            "tidal_body_name[%d] is a null or empty string.",
            ii);
         return;
      }

      // Find the Planet whose name matches the supplied name.
      tidal_bodies[ii] = dyn_manager.find_planet (
         var_init->tidal_body_names[ii]);

      // The supplied name must specify a registered planet.
      if (tidal_bodies[ii] == nullptr) {
         MessageHandler::fail (
            __FILE__, __LINE__, GravityMessages::invalid_name,
            "tidal_body_name[%d]=\"%s\" is not a valid planet name.",
            ii, var_init->tidal_body_names[ii]);
         return;
      }

      // Cache the body-centered inertial frame.
      tidal_bodies_inertial[ii] = &(tidal_bodies[ii]->inertial);

      // Let the dynamics manager know this is a frame of interest.
      dyn_manager.subscribe_to_frame (*(tidal_bodies_inertial[ii]));
   }

   // Cache the planet fixed frame associated with the primary body.
   pfix = grav_source->pfix;


   // Perform base class initializations.
   SphericalHarmonicsDeltaCoeffs::initialize (gen_var_init, dyn_manager);


   // Allocate and load up Knm
   if (degree > 0) {
      Knm = JEOD_ALLOC_PRIM_ARRAY (degree + 1, double *);

      for (unsigned int ii = 0; ii <= degree; ++ii) {
         Knm[ii] = JEOD_ALLOC_PRIM_ARRAY (ii + 1, double);
      }

      for (unsigned int ii = 0; ii < degree; ++ii) {
         for (unsigned int jj = 0; jj < order; ++jj) {
            Knm[ii][jj] = var_init->Knm[ii][jj];
         }
      }
   }

   return;
}


/**
 * Pure virtual update method.
 * \param[in] controls Gravity controls for planet
 */
void
SphericalHarmonicsTidalEffects::update (
   SphericalHarmonicsGravityControls&)
{

   // double moon_earth_pos[3];
   // tidal_bodies_inertial[0] is the moon inertial frame
   // tidal_bodies_inertial[0]->compute_position_from(*pfix, moon_earth_pos);
   // moon_earth_pos will now be the position of moon, in the earth fixed frame

}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
