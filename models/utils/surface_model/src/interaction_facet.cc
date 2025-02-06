/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup SurfaceModel
 * @{
 *
 * @file models/utils/surface_model/src/interaction_facet.cc
 * Individual facets for use with environment interaction models
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((None))

Library dependencies:
    ((interaction_facet.cc)
     (facet.cc))

 
*******************************************************************************/

// System includes
#include <cstddef>

// Jeod includes
#include "utils/math/include/vector3.hh"

// Model includes
#include "../include/interaction_facet.hh"
#include "../include/facet.hh"


//! Namespace jeod
namespace jeod {

/**
 * Default constructor
 */

InteractionFacet::InteractionFacet (
   void)
:
   base_facet(nullptr)
{

   Vector3::initialize (force);
   Vector3::initialize (torque);

}

/**
 * Destructor
 */

InteractionFacet::~InteractionFacet (
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
