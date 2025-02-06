/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup Aerodynamics
 * @{
 *
 * @file models/interactions/aerodynamics/src/aero_params.cc
 * A virtual base class for aero facet parameters, used to create interaction
 * facets for aero in the InteractionSurfaceFactorys
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((None))

Library dependencies:
    ((aero_params.o)
     (utils/surface_model/facet_params.o))


*******************************************************************************/

#include "../include/aero_params.hh"

//! Namespace jeod
namespace jeod {

/**
 * Default Constructor
 */

AeroParams::AeroParams (
   void)
{

   // empty for now

}

/**
 * Destructor
 */

AeroParams::~AeroParams (
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
