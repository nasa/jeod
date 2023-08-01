/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup Aerodynamics
 * @{
 *
 * @file models/interactions/aerodynamics/src/aero_facet.cc
 * Individual facets for use with aero environment interaction models
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((None))

Library dependencies:
    ((aero_facet.cc)
     (utils/surface_model/src/interaction_facet.cc))


*******************************************************************************/

// JEOD includes
#include "utils/math/include/vector3.hh"

// Model includes
#include "../include/aero_facet.hh"
#include "../include/aero_drag.hh"

//! Namespace jeod
namespace jeod {

/**
 * Default constructor
 */

AeroFacet::AeroFacet (
   void)
{

   // empty for now

}

/**
 * Destructor
 */

AeroFacet::~AeroFacet (
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
