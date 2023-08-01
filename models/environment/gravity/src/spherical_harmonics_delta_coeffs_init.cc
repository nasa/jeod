/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Gravity
 * @{
 *
 * @file models/environment/gravity/src/spherical_harmonics_delta_coeffs_init.cc
 * Define member functions for the SphericalHarmonicsDeltaCoeffsInit class.
 */

/*******************************************************************************

Purpose:
  ()

References:
  (((none)))

Assumptions and limitations:
  ((none))

Library dependencies:
  ((spherical_harmonics_delta_coeffs_init.cc))



*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "../include/spherical_harmonics_delta_coeffs_init.hh"



//! Namespace jeod
namespace jeod {

/**
 * SphericalHarmonicsDeltaCoeffsInit constructor.
 */
SphericalHarmonicsDeltaCoeffsInit::SphericalHarmonicsDeltaCoeffsInit (
   void)
{
   delta_Cnm = nullptr;
   delta_Snm = nullptr;
   degree    = order = 0;

}


/**
 * SphericalHarmonicsDeltaCoeffsInit destructor.
 */
SphericalHarmonicsDeltaCoeffsInit::~SphericalHarmonicsDeltaCoeffsInit (
   void)
{
   ;  // Nothing to do; class does not allocate memory
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
