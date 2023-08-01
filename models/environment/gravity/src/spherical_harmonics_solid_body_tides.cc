/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Gravity
 * @{
 *
 * @file models/environment/gravity/src/spherical_harmonics_solid_body_tides.cc
 * Define member functions for the SphericalHarmonicsSolidBodyTides class.
 */

/*******************************************************************************

Purpose:
  ()

References:
  (((none)))

Assumptions and limitations:
  ((none))

Library dependencies:
  ((spherical_harmonics_solid_body_tides.cc)
   (spherical_harmonics_delta_coeffs_init.cc)
   (spherical_harmonics_gravity_source.cc)
   (spherical_harmonics_gravity_controls.cc)
   (environment/planet/src/planet.cc)
   (utils/ref_frames/src/ref_frame.cc))


*******************************************************************************/

// System includes
#include <cmath>

// JEOD includes
#include "dynamics/dyn_manager/include/base_dyn_manager.hh"
#include "environment/planet/include/planet.hh"
#include "utils/math/include/vector3.hh"
#include "utils/ref_frames/include/ref_frame.hh"

// Model includes
#include "../include/spherical_harmonics_solid_body_tides.hh"
#include "../include/spherical_harmonics_delta_coeffs_init.hh"
#include "../include/spherical_harmonics_gravity_controls.hh"
#include "../include/spherical_harmonics_gravity_source.hh"



//! Namespace jeod
namespace jeod {

/**
 * SphericalHarmonicsSolidBodyTides constructor.
 */
SphericalHarmonicsSolidBodyTides::SphericalHarmonicsSolidBodyTides (
   void)
{
   ; // Nothing to do
}


/**
 * SphericalHarmonicsSolidBodyTides destructor.
 */
SphericalHarmonicsSolidBodyTides::~SphericalHarmonicsSolidBodyTides (
   void)
{
   ; // Nothing to do

}


/**
 * Initialize the solid body tidal model.
 * \param[in] var_init Effect init structure
 * \param[in] dyn_manager Dynamics manager
 */
void
SphericalHarmonicsSolidBodyTides::initialize (
   SphericalHarmonicsDeltaCoeffsInit & var_init,
   BaseDynManager & dyn_manager)
{

   // Pass initialization up the chain
   SphericalHarmonicsTidalEffects::initialize (var_init, dyn_manager);

}


/**
 * Update the solid-body tidal delta-coefficients.
 * \param[in] controls Gravity controls for planet
 */
void
SphericalHarmonicsSolidBodyTides::update (
   SphericalHarmonicsGravityControls & controls JEOD_UNUSED)

{
   double pfix_position[3];
   double F = 0.0;
   double deltaC20;

   for (unsigned int ii = 0; ii < num_tidal_bodies; ++ii) {
      tidal_bodies_inertial[ii]->compute_position_from (*pfix,
                                                        pfix_position);

      double tidal_body_mu = tidal_bodies[ii]->grav_source->mu;

      double r = Vector3::vmag (pfix_position);
      double phi    = asin (pfix_position[2] / r);
      double Rovr_3 = grav_source->radius / r;
      Rovr_3 *= Rovr_3 * Rovr_3;

      F += tidal_body_mu / grav_source->mu* Rovr_3* sqrt (5.0) *
           (1.5 * sin (phi) * sin (phi) - 0.5);
   }

   deltaC20 = k2 / 5.0 * F;
   dC20 = deltaC20;

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
