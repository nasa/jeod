//=============================================================================
// Notices:
//
// Copyright © 2023 United States Government as represented by the Administrator
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
 * @file models/environment/gravity/include/spherical_harmonics_tidal_effects_init.hh
 * Define the SphericalHarmonicsTidalEffectsInit class, the initialization
 * class for tidal effects models.
 */

/*******************************************************************************

Purpose:
  ()

References:
  (((none)))

Assumptions and limitations:
  ((none))

*******************************************************************************/

#ifndef JEOD_SPHERICAL_HARMONICS_TIDAL_EFFECTS_INIT_HH
#define JEOD_SPHERICAL_HARMONICS_TIDAL_EFFECTS_INIT_HH

// System includes
#include <string>
#include <vector>

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "spherical_harmonics_delta_coeffs_init.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Initializes a tidal gravity model.
 */
class SphericalHarmonicsTidalEffectsInit : public SphericalHarmonicsDeltaCoeffsInit
{
    JEOD_MAKE_SIM_INTERFACES(jeod, SphericalHarmonicsTidalEffectsInit)

    // Member data
public:
    /**
     * Copy of polar motion coefficient xp (from polar motion class).
     */
    double xp{}; //!< trick_units(--)

    /**
     * Copy of polar motion coefficient yp (from polar motion class).
     */
    double yp{}; //!< trick_units(--)

    /**
     * The love number. Only used for a first order tidal effect model
     */
    double k2{}; //!< trick_units(--)

    /**
     * A matrix of love numbers. Used for higher order (not first) tidal effects
     */
    double ** Knm{}; //!< trick_units(--)

    /**
     * A named list of gravitational bodies contributing to this tidal efffect
     */
    std::vector<std::string> tidal_body_names; //!< trick_units(--)

    // Member functions
public:
    SphericalHarmonicsTidalEffectsInit() = default;
    ~SphericalHarmonicsTidalEffectsInit() override = default;
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
