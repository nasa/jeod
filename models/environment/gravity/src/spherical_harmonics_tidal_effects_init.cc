/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Gravity
 * @{
 *
 * @file models/environment/gravity/src/spherical_harmonics_tidal_effects_init.cc
 * Define member functions for the SphericalHarmonicsTidalEffectsInit class.
 */

/*******************************************************************************

Purpose:
  ()

References:
  (((none)))

Assumptions and limitations:
  ((none))

Library dependencies:
  ((spherical_harmonics_tidal_effects_init.o)
   (spherical_harmonics_delta_coeffs_init.o))


*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "../include/spherical_harmonics_tidal_effects_init.hh"



//! Namespace jeod
namespace jeod {

/**
 * SphericalHarmonicsTidalEffectsInit constructor.
 */
SphericalHarmonicsTidalEffectsInit::SphericalHarmonicsTidalEffectsInit (
   void)
{
   xp               = yp = 0.0;
   k2               = 0.0;
   Knm              = NULL;
   tidal_body_names = NULL;
   num_tidal_bodies = 0;

}


/**
 * SphericalHarmonicsTidalEffectsInit destructor.
 */
SphericalHarmonicsTidalEffectsInit::~SphericalHarmonicsTidalEffectsInit (
   void)
{
   ;  // Nothing to do
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
