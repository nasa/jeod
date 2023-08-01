/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup SurfaceModel
 * @{
 *
 * @file models/utils/surface_model/src/interaction_facet_factory.cc
 * Factory that creates an interaction facet, for a specific
 * enviornment interaction model, from a facet model
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((None))

Library dependencies:
    ((interaction_facet_factory.cc)
     (facet.cc)
     (facet_params.cc))

 
*******************************************************************************/

#include "../include/interaction_facet_factory.hh"
#include "../include/interaction_facet.hh"


//! Namespace jeod
namespace jeod {

/**
 * Default Constructor
 */

InteractionFacetFactory::InteractionFacetFactory (
   void)
:
   trick_bool(false)
{

   // empty for now

}

/**
 * Destructor
 */

InteractionFacetFactory::~InteractionFacetFactory (
   void)
{

   // empty for now

}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
