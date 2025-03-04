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
 * @addtogroup Contact
 * @{
 *
 * @file models/interactions/contact/include/point_contact_facet_factory.hh
 * Creates a point contact facet from an circular flat plate facet
 */

/*****************************************************************************

 Purpose:
   ()

 Reference:
   (((Derived from OTCM model developed by Hung Nguyen L3-COM)))

 Assumptions and Limitations:
     ((N/A))

Library dependencies:
    ((../src/point_contact_facet_factory.cc))



*****************************************************************************/

#ifndef POINT_CONTACT_FACET_FACTORY_HH
#define POINT_CONTACT_FACET_FACTORY_HH

/* JEOD includes */
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/surface_model/include/class_declarations.hh"
#include "utils/surface_model/include/interaction_facet.hh"
#include "utils/surface_model/include/interaction_facet_factory.hh"

/* Model includes */
#include "class_declarations.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Creates a PointContactFacet from an InteractionFacet.
 */
class PointContactFacetFactory : public InteractionFacetFactory
{
    JEOD_MAKE_SIM_INTERFACES(jeod, PointContactFacetFactory)

public:
    PointContactFacetFactory();
    ~PointContactFacetFactory() override = default;
    PointContactFacetFactory & operator=(const PointContactFacetFactory &) = delete;
    PointContactFacetFactory(const PointContactFacetFactory &) = delete;

    InteractionFacet * create_facet(Facet * facet, FacetParams * params) override;

    // 'true' if this factory is meant to be used on the type of facet
    // sent in through the 'facet' pointer. 'false' otherwise
    bool is_correct_factory(Facet * facet) override;
};

} // namespace jeod

#ifdef TRICK_VER
#include "point_contact_facet.hh"
#endif

#endif

/**
 * @}
 * @}
 * @}
 */
