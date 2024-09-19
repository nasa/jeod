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
 * @file models/interactions/aerodynamics/include/flat_plate_aero_factory.hh
 * Creates a flat plate aero facet from a basic flat plate facet
 */

/************************** TRICK HEADER***************************************
PURPOSE:
   ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((None))

Library dependencies:
    ((../src/flat_plate_aero_factory.cc))


*******************************************************************************/

#ifndef FLAT_PLATE_AERO_FACTORY_HH
#define FLAT_PLATE_AERO_FACTORY_HH

// JEOD Includes
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/surface_model/include/interaction_facet_factory.hh"

//! Namespace jeod
namespace jeod
{

class Facet;
class InteractionFacet;
class FacetParams;

/**
 * Creates a FlatPlateAeroFacet from a FlatPlate.
 */
class FlatPlateAeroFactory : public InteractionFacetFactory
{
    JEOD_MAKE_SIM_INTERFACES(jeod, FlatPlateAeroFactory)

public:
    FlatPlateAeroFactory();
    ~FlatPlateAeroFactory() override = default;
    FlatPlateAeroFactory & operator=(const FlatPlateAeroFactory &) = delete;
    FlatPlateAeroFactory(const FlatPlateAeroFactory &) = delete;

    InteractionFacet * create_facet(Facet * facet, FacetParams * params) override;

    // 'true' if this factory is meant to be used on the type of facet
    // sent in through the 'facet' pointer. 'false' otherwise
    bool is_correct_factory(Facet * facet) override;
};

} // namespace jeod

#ifdef TRICK_VER
#include "flat_plate_aero_facet.hh"
#include "utils/surface_model/include/facet.hh"
#endif

#endif

/**
 * @}
 * @}
 * @}
 */
