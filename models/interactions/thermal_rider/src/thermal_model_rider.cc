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
   ((thermal_model_rider.cc)
    (thermal_facet_rider.cc))


*******************************************************************************/

/* System includes */
#include <cmath>

/*  JEOD includes */

/* Model structure includes */
#include "../include/thermal_facet_rider.hh"
#include "../include/thermal_model_rider.hh"
#include "utils/surface_model/include/interaction_surface.hh"

//! Namespace jeod
namespace jeod
{

/**
 * update the thermal aspects of all facets
 * \param[in] surface_ptr pointer the surface.
 */
void ThermalModelRider::update(InteractionSurface * surface_ptr)
{
    if(include_internal_thermal_effects)
    {
        surface_ptr->accumulate_thermal_sources();
    }

    if(active && (std::fpclassify(ThermalFacetRider::cycle_time) != FP_ZERO))
    {
        surface_ptr->thermal_integrator();
    }
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
