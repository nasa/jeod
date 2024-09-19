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
 * @addtogroup Interactions
 * @{
 * @addtogroup Aerodynamics
 * @{
 *
 * @file models/interactions/aerodynamics/include/flat_plate_aero_params.hh
 * The set of parameters used to create an aerodynamic interaction
 * specific flat plate facet from a general flat plate facet
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((None))

*******************************************************************************/

#ifndef FLAT_PLATE_AERO_PARAMS_HH
#define FLAT_PLATE_AERO_PARAMS_HH

#include "aero_drag.hh"
#include "aero_params.hh"
#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod
{

/**
 * used in the FlatPlateAeroFactory to create a FlatPlateAeroFacet.
 */
class FlatPlateAeroParams : public AeroParams
{
    JEOD_MAKE_SIM_INTERFACES(jeod, FlatPlateAeroParams)

public:
    FlatPlateAeroParams() = default;
    ~FlatPlateAeroParams() override = default;
    FlatPlateAeroParams & operator=(const FlatPlateAeroParams &) = delete;
    FlatPlateAeroParams(const FlatPlateAeroParams &) = delete;

    /**
     * Enum indicating which method of coef calculation to use:
     * specular, diffuse, calculated, mixed
     */
    AeroDragEnum::CoefCalcMethod coef_method{AeroDragEnum::Specular}; //!< trick_units(--)

    /**
     * Whether to calculate the drag coefficient
     */
    bool calculate_drag_coef{}; //!< trick_units(--)

    /**
     * Fraction of molecules that "bounce"
     */
    double epsilon{}; //!< trick_units(--)

    /**
     * Temperature of reflected molecules
     */
    double temp_reflect{}; //!< trick_units(K)

    /**
     * The coefficient for calculating drag normal to the plate
     */
    double drag_coef_norm{}; //!< trick_units(--)

    /**
     * The coefficient for calculating drag tangential to the plate
     */
    double drag_coef_tang{}; //!< trick_units(--)

    /**
     * The coefficient for calculating drag resulting only from molecules bouncing
     * off the surface
     */
    double drag_coef_spec{}; //!< trick_units(--)

    /**
     * The coefficient for calculating drag resulting only from molecules sticking
     * to the surface
     */
    double drag_coef_diff{}; //!< trick_units(--)
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
