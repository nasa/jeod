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
 * @file models/environment/gravity/include/spherical_harmonics_delta_controls.hh
 * Define the gravity controls for the variational gravity models such as
 * solid-body tides.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

*******************************************************************************/

#ifndef JEOD_SPHERICAL_HARMONICS_DELTA_CONTROLS_HH
#define JEOD_SPHERICAL_HARMONICS_DELTA_CONTROLS_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "class_declarations.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Provides controls for how a variational model affects a vehicle.
 */
class SphericalHarmonicsDeltaControls
{
    JEOD_MAKE_SIM_INTERFACES(jeod, SphericalHarmonicsDeltaControls)

    // Member data
public:
    /**
     * Pointer to associated coefficient-altering gravitational effect
     */
    SphericalHarmonicsDeltaCoeffs * grav_effect{}; //!< trick_units(--)

    /**
     * Pointer to the gravity body associated with this effect
     */
    SphericalHarmonicsGravitySource * grav_source{}; //!< trick_units(--)

    /**
     * Is this variational gravity effect active for this body?
     */
    bool active{}; //!< trick_units(--)

    /**
     * Calculate first-order term of this effect only; default to true for 2.0
     */
    bool first_order_only{true}; //!< trick_units(--)

    /**
     * Coefficient degree to be used for this gravity effect.
     */
    unsigned int degree{}; //!< trick_units(--)

    /**
     * Coefficient order to be used for this gravity effect.
     */
    unsigned int order{}; //!< trick_units(--)

    // Member functions
public:
    SphericalHarmonicsDeltaControls() = default;
    ~SphericalHarmonicsDeltaControls() = default;
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
