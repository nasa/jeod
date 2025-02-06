/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Gravity
 * @{
 *
 * @file models/environment/gravity/src/spherical_harmonics_solid_body_tides_init.cc
 * Define member functions for the SphericalHarmonicsSolidBodyTidesInit class.
 */

/*******************************************************************************

Purpose:
  ()

References:
  (((none)))

Assumptions and limitations:
  ((none))

Library dependencies:
  ((spherical_harmonics_solid_body_tides_init.o)
   (spherical_harmonics_tidal_effects_init.o))



*******************************************************************************/

// System includes

// JEOD includes
#include "utils/memory/include/jeod_alloc.hh"

// Model includes
#include "../include/spherical_harmonics_solid_body_tides_init.hh"


//! Namespace jeod
namespace jeod {

/**
 * SphericalHarmonicsSolidBodyTidesInit constructor.
 */
SphericalHarmonicsSolidBodyTidesInit::SphericalHarmonicsSolidBodyTidesInit (
   void)
{
   ;  // Nothing to do
}


/**
 * SphericalHarmonicsSolidBodyTidesInit destructor.
 */
SphericalHarmonicsSolidBodyTidesInit::~SphericalHarmonicsSolidBodyTidesInit (
   void)
{
   if (num_tidal_bodies != 0) {
      for (unsigned int jj = 0; jj < num_tidal_bodies; ++jj) {
         if (JEOD_IS_ALLOCATED(tidal_body_names[jj])) {
            JEOD_DELETE_ARRAY(tidal_body_names[jj]);
         } // end if
      } // end for jj
      if (JEOD_IS_ALLOCATED(tidal_body_names)) {
         JEOD_DELETE_ARRAY(tidal_body_names);
      } // end if
   } // end outer if
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
