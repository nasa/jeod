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
 * @file models/environment/gravity/include/spherical_harmonics_solid_body_tides_init.hh
 * Define the SphericalHarmonicsSolidBodyTidesInit class, which is the
 * initialization class for the solid body tides model.
 */

/*******************************************************************************

Purpose:
  ()

References:
  (((none)))

Assumptions and limitations:
  ((none))

*******************************************************************************/

#ifndef JEOD_SPHERICAL_HARMONICS_SOLID_BODY_TIDES_INIT_HH
#define JEOD_SPHERICAL_HARMONICS_SOLID_BODY_TIDES_INIT_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "spherical_harmonics_tidal_effects_init.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Initializes a solid body tides model.
 */
class SphericalHarmonicsSolidBodyTidesInit : public SphericalHarmonicsTidalEffectsInit
{
    JEOD_MAKE_SIM_INTERFACES(jeod, SphericalHarmonicsSolidBodyTidesInit)

public:
    SphericalHarmonicsSolidBodyTidesInit() = default;
    ~SphericalHarmonicsSolidBodyTidesInit() override = default;
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
