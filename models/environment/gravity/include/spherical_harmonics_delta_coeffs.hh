//=============================================================================
// Notices:
//
// Copyright © 2025 United States Government as represented by the Administrator
// of the National Aeronautics and Space Administration.  All Rights Reserved.
//
//
// Disclaimers:
//
// No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF
// ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED
// TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO SPECIFICATIONS, ANY
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
// FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL BE ERROR
// FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO THE
// SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN
// ENDORSEMENT BY GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS,
// RESULTING DESIGNS, HARDWARE, SOFTWARE PRODUCTS OR ANY OTHER APPLICATIONS
// RESULTING FROM USE OF THE SUBJECT SOFTWARE.  FURTHER, GOVERNMENT AGENCY
// DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING THIRD-PARTY SOFTWARE,
// IF PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT "AS IS."
//
// Waiver and Indemnity:  RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS AGAINST THE
// UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
// PRIOR RECIPIENT.  IF RECIPIENT'S USE OF THE SUBJECT SOFTWARE RESULTS IN ANY
// LIABILITIES, DEMANDS, DAMAGES, EXPENSES OR LOSSES ARISING FROM SUCH USE,
// INCLUDING ANY DAMAGES FROM PRODUCTS BASED ON, OR RESULTING FROM, RECIPIENT'S
// USE OF THE SUBJECT SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE
// UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
// PRIOR RECIPIENT, TO THE EXTENT PERMITTED BY LAW.  RECIPIENT'S SOLE REMEDY FOR
// ANY SUCH MATTER SHALL BE THE IMMEDIATE, UNILATERAL TERMINATION OF THIS
// AGREEMENT.
//
//=============================================================================
//
//
/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Gravity
 * @{
 *
 * @file models/environment/gravity/include/spherical_harmonics_delta_coeffs.hh
 * Define the class SphericalHarmonicsDeltaCoeffs, the base class
 * for tidal effects and temporal gravity sub-models.
 */

/*******************************************************************************

Purpose:
  ()

References:
  (((none)))

Assumptions and limitations:
  ((none))

Library dependencies:
  ((../src/spherical_harmonics_delta_coeffs.cc))



*******************************************************************************/

#ifndef JEOD_SPHERICAL_HARMONICS_DELTA_COEFFS_HH
#define JEOD_SPHERICAL_HARMONICS_DELTA_COEFFS_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "class_declarations.hh"

//! Namespace jeod
namespace jeod
{

class BaseDynManager;

/**
 * Base class for tidal and temporal gravity models.
 */
class SphericalHarmonicsDeltaCoeffs
{
    JEOD_MAKE_SIM_INTERFACES(jeod, SphericalHarmonicsDeltaCoeffs)

    // Member data
public:
    /**
     * Pointer to the gravity body associated with this effect
     */
    SphericalHarmonicsGravitySource * grav_source{}; //!< trick_units(--)

    /**
     * Normalized real (cosine) variational spherical harmonic coefficients.
     */
    double ** delta_Cnm{}; //!< trick_units(--)

    /**
     * Normalized imaginary (sine) variational spherical harmonic coeffs.
     */
    double ** delta_Snm{}; //!< trick_units(--)

    /**
     * Coefficient degree to be used for this SphericalHarmonicsDeltaCoeffs.
     */
    unsigned int degree{}; //!< trick_units(--)

    /**
     * Coefficient order to be used for this SphericalHarmonicsDeltaCoeffs.
     */
    unsigned int order{}; //!< trick_units(--)

    /**
     * delta C20 coefficient for first order effect
     */
    double dC20{}; //!< trick_units(--)

    // Member functions
public:
    SphericalHarmonicsDeltaCoeffs() = default;
    virtual ~SphericalHarmonicsDeltaCoeffs();

    virtual void initialize(SphericalHarmonicsDeltaCoeffsInit & var_init, BaseDynManager & dyn_manager);

    virtual void update(SphericalHarmonicsGravityControls & controls);
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
