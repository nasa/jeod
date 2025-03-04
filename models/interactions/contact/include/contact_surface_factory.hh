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
 * @file models/interactions/contact/include/contact_surface_factory.hh
 * Factory that creates an contact interaction surface
 * from a surface model
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
   (((Derived from OTCM model developed by Hung Nguyen L3-COM)))

ASSUMPTIONS AND LIMITATIONS:
      ((None))

Library dependencies:
    ((../src/contact_surface_factory.cc))


*****************************************************************************/

#ifndef JEOD_CONTACT_SURFACE_FACTORY_HH
#define JEOD_CONTACT_SURFACE_FACTORY_HH

/* JEOD includes */
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/surface_model/include/class_declarations.hh"
#include "utils/surface_model/include/interaction_surface_factory.hh"

/* Model includes */
#include "line_contact_facet_factory.hh"
#include "point_contact_facet_factory.hh"

//! Namespace jeod
namespace jeod
{

/**
 * The surface factory that creates an contact specific surface from a
 * general surface. Used with the surface model.
 */
class ContactSurfaceFactory : public InteractionSurfaceFactory
{
    JEOD_MAKE_SIM_INTERFACES(jeod, ContactSurfaceFactory)

public:
    ContactSurfaceFactory();
    ~ContactSurfaceFactory() override = default;
    ContactSurfaceFactory & operator=(const ContactSurfaceFactory &) = delete;
    ContactSurfaceFactory(const ContactSurfaceFactory &) = delete;

    // Creates the interaction surface from a base surface model
    void create_surface(SurfaceModel * surface, InteractionSurface * inter_surface) override;

    // Add a set of facet params to the ContactSurfaceFactory.
    // The type of FacetParams MUST be ContactParams, or the function
    // will fail.
    void add_facet_params(FacetParams * to_add) override;

protected:
    // Included so JEOD can include all default facet factories that
    // it knows about. For extensibility the user can add
    // factories with add_facet_factory

    /**
     * A factory that can create a point contact facet from a circular flat plate.
     */
    PointContactFacetFactory point_contact_facet_factory; //!< trick_units(--)

    /**
     * A factory that can create a line contact facet from a cylinder.
     */
    LineContactFacetFactory line_contact_facet_factory; //!< trick_units(--)
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
