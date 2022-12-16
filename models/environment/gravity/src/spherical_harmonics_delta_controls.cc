/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Gravity
 * @{
 *
 * @file models/environment/gravity/src/spherical_harmonics_delta_controls.cc
 * Define member functions for the SphericalHarmonicsDeltaControls class.
 */

/*******************************************************************************

Purpose:
  ()

References:
  (((none)))

Assumptions and limitations:
  ((none))

Library dependencies:
  ((spherical_harmonics_delta_controls.o)
   (spherical_harmonics_delta_coeffs.o)
   (spherical_harmonics_gravity_source.o))



*******************************************************************************/

// System includes
#include <cstddef>

// model includes
#include "../include/spherical_harmonics_delta_controls.hh"
#include "../include/spherical_harmonics_delta_coeffs.hh"
#include "../include/spherical_harmonics_gravity_source.hh"



//! Namespace jeod
namespace jeod {

/**
 * SphericalHarmonicsDeltaControls constructor.
 */
SphericalHarmonicsDeltaControls::SphericalHarmonicsDeltaControls (
   void)
:
   grav_effect(NULL),
   grav_source(NULL),
   active(false),
   first_order_only(true),
   degree(0),
   order(0)
{
   return;
}


/**
 * SphericalHarmonicsDeltaControls destructor.
 */
SphericalHarmonicsDeltaControls::~SphericalHarmonicsDeltaControls (
   void)
{
   ; // nothing to do; model does not allocate resources
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
