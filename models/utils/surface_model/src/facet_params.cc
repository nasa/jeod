/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup SurfaceModel
 * @{
 *
 * @file models/utils/surface_model/src/facet_params.cc
 * A pure virtual base class for facet parameters, used to create interation
 * facets in the InteractionSurfaceFactorys
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((None))

Library dependencies:
    ((facet_params.o))

 
*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes

// Model includes
#include "../include/facet_params.hh"


//! Namespace jeod
namespace jeod {

/**
 * Default Constructor
 */

FacetParams::FacetParams (
   void)
:  // Return: -- void
   name()
{

   // empty for now

}

/**
 * Destructor
 */

FacetParams::~FacetParams (
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
