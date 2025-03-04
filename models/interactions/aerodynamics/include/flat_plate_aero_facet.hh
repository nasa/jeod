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
 * @addtogroup Interactions
 * @{
 * @addtogroup Aerodynamics
 * @{
 *
 * @file models/interactions/aerodynamics/include/flat_plate_aero_facet.hh
 * The aerodynamic specific implementation of flat plate
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((None))

Library dependencies:
    ((../src/flat_plate_aero_facet.cc))


*******************************************************************************/

#ifndef FLAT_PLATE_AERO_FACET_HH
#define FLAT_PLATE_AERO_FACET_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "aero_drag.hh"
#include "aero_facet.hh"

//! Namespace jeod
namespace jeod
{

/**
 * The aerdynamic specific version of a flat plate.
 */
class FlatPlateAeroFacet : public AeroFacet
{
    JEOD_MAKE_SIM_INTERFACES(jeod, FlatPlateAeroFacet)

public:
    FlatPlateAeroFacet() = default;
    ~FlatPlateAeroFacet() override = default;
    FlatPlateAeroFacet & operator=(const FlatPlateAeroFacet &) = delete;
    FlatPlateAeroFacet(const FlatPlateAeroFacet &) = delete;

    // The flat plate specific implementation of aerodrag_force
    void aerodrag_force(const double velocity_mag,
                        const double rel_vel_hat[3],
                        AeroDragParameters * aero_drag_param_ptr,
                        double center_grav[3]) override;

    /**
     * Flat plate center of pressure (in structural frame). Once the
     * aero surface is initialized, it points to the position
     * found in FlatPlate
     */
    double * center_pressure{}; //!< trick_units(m)

    /**
     * Unit vector normal to the plate surface, pointing
     * outward (structural frame). Once the aero surface is initialized,
     * it points to the normal found in FlatPlate
     */
    double * normal{}; //!< trick_units(--)

    /**
     * Magnitude of the force normal to the plate
     */
    double force_n{}; //!< trick_units(N)

    /**
     * Magnitude of the force tangential to the plate, or
     * parallel to the velocity vector, depending on application
     */
    double force_t{}; //!< trick_units(N)

    /**
     * Enum indicating which method of coef calculation to use:
     * specular, diffuse, calculated, mixed
     */
    AeroDragEnum::CoefCalcMethod coef_method{AeroDragEnum::Specular}; //!< trick_units(--)

    /**
     * whether to calculate the drag coefficient
     */
    bool calculate_drag_coef{}; //!< trick_units(--)

    /**
     * fraction of molecules that "bounce"
     */
    double epsilon{}; //!< trick_units(--)

    /**
     * temperature of reflected molecules
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

    /**
     * Temperature of the plate
     */
    double temperature{}; //!< trick_units(K)

    /**
     * area of the plate
     */
    double area{}; //!< trick_units(m2)
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
