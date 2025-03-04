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
 * @addtogroup Utils
 * @{
 * @addtogroup SurfaceModel
 * @{
 *
 * @file models/utils/surface_model/include/interaction_surface_factory.hh
 * Factory that creates an interaction surface, for a specific
 * environment interaction model, from a surface model
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((None))

Library dependencies:
    ((../src/interaction_surface_factory.cc))


*******************************************************************************/

#ifndef JEOD_INTERACTION_SURFACE_FACTORY_HH
#define JEOD_INTERACTION_SURFACE_FACTORY_HH

// JEOD includes
#include "utils/container/include/pointer_vector.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes

//! Namespace jeod
namespace jeod
{

class InteractionFacetFactory;
class FacetParams;
class SurfaceModel;
class InteractionSurface;

/**
 * A base class for creating specific interaction surfaces from
 * general surfaces.
 */
class InteractionSurfaceFactory
{
    JEOD_MAKE_SIM_INTERFACES(jeod, InteractionSurfaceFactory)

public:
    InteractionSurfaceFactory();
    virtual ~InteractionSurfaceFactory();
    InteractionSurfaceFactory & operator=(const InteractionSurfaceFactory &) = delete;
    InteractionSurfaceFactory(const InteractionSurfaceFactory &) = delete;

    // Creates the interaction surface from a base surface model
    virtual void create_surface(SurfaceModel * surface, InteractionSurface * inter_surface);

    /**
     * Convenience version of create_surface which can be called from the
     * input file.
     * @param surface       Surface model.
     * @param inter_surface Reference to the interaction surface.
     */
    void create_surface(SurfaceModel & surface, InteractionSurface & inter_surface)
    {
        create_surface(&surface, &inter_surface);
    }

    /**
     * A vector of interaction facet factories to be used.
     * Matched to facets by type
     */
    JeodPointerVector<InteractionFacetFactory>::type factories; //!< trick_io(**)

    /**
     * A vector of FacetParams to be used
     */
    JeodPointerVector<FacetParams>::type params; //!< trick_io(**)

    /* Allows for a new InteractionFacetFactory to be added to the
       InteractionSurfaceFactory */
    virtual void add_facet_factory(InteractionFacetFactory * to_add);

    /* Allows for new facet parameters to be added to the
       InteractionSurfaceFactory for use when creating the InteractionSurface */
    virtual void add_facet_params(FacetParams * to_add);
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
