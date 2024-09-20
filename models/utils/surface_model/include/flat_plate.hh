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
 * @addtogroup Utils
 * @{
 * @addtogroup SurfaceModel
 * @{
 *
 * @file models/utils/surface_model/include/flat_plate.hh
 * Flat plates for use in the surface model
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((None))

Library dependencies:
    ((../src/flat_plate.cc))


*******************************************************************************/

#ifndef JEOD_FLAT_PLATE_HH
#define JEOD_FLAT_PLATE_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "facet.hh"

//! Namespace jeod
namespace jeod
{

/**
 * A FlatPlate implementation of Facet.
 */
class FlatPlate : public Facet
{
    JEOD_MAKE_SIM_INTERFACES(jeod, FlatPlate)

public:
    FlatPlate() = default;
    ~FlatPlate() override = default;
    FlatPlate & operator=(const FlatPlate &) = delete;
    FlatPlate(const FlatPlate &) = delete;

    /**
     * normal of the plate, pointing outward of the craft, with respect
     * to the vehicle structural frame. If the mass tying functionality
     * is turned on, this will be in the structural frame associated
     * with the mass body named in struct_mass_name. If mass tying
     * functionaity is not turned on, this parameter should be
     * set directly. If it is turned on, then the local_normal
     * should be set and this variable should be automatically
     * calculated.
     */
    double normal[3]{}; //!< trick_units(--)

    /**
     * The normal of the plate with respect to the structural frame
     * associated with the mass body named in mass_body_name.
     */
    double local_normal[3]{}; //!< trick_units(--)

protected:
    void update_articulation_internal() override;
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
