/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup ThermalRider
 * @{
 *
 * @file models/interactions/thermal_rider/src/thermal_model_rider.cc
 * ThermalModelRider interaction model
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((The thermal rider works with a surface interaction model.  Switching
      on the ThermalModelRider in the Interaction model allows access to dynamic
      temperature variations of the Interaction Facets that comprise the
      Interaction Surface that the Interaction Model uses.)
      (This is not a stand-alone interaction model))

Library dependencies:
   ((thermal_model_rider.o)
    (thermal_facet_rider.o))


*******************************************************************************/

/* System includes */

/*  JEOD includes */

/* Model structure includes */
#include "../include/thermal_model_rider.hh"
#include "utils/surface_model/include/interaction_surface.hh"
#include "../include/thermal_facet_rider.hh"




//! Namespace jeod
namespace jeod {

/**
 * Constructor
 */
ThermalModelRider::ThermalModelRider (
   void)
{
   active                           = false;
   include_internal_thermal_effects = false;
}

/**
 * update the thermal aspects of all facets
 * \param[in] surface_ptr pointer the surface.
 */
void
ThermalModelRider::update (
   InteractionSurface * surface_ptr)

{
   if (include_internal_thermal_effects) {
      surface_ptr->accumulate_thermal_sources();
   }

   if (active && (ThermalFacetRider::cycle_time != 0.0)) {
      surface_ptr->thermal_integrator();
   }
   return;
}


/**
 * Destructor
 */
ThermalModelRider::~ThermalModelRider (
   void)
{
}


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
