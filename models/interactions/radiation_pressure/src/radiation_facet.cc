/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup RadiationPressure
 * @{
 *
 * @file models/interactions/radiation_pressure/src/radiation_facet.cc
 * Individual facets for use with Radiation Pressure interaction models
 */

/************************** TRICK HEADER***************************************
PURPOSE:
()

REFERENCE:
(((None)))

ASSUMPTIONS AND LIMITATIONS:
((None))

Library dependencies:
((radiation_facet.cc)
(interactions/thermal_rider/src/thermal_facet_rider.cc)
(radiation_messages.cc)
(utils/message/src/message_handler.cc))


*******************************************************************************/

/* System includes */

/*  JEOD includes */
#include "interactions/thermal_rider/include/thermal_facet_rider.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/surface_model/include/facet.hh"

/* Model structure includes */
#include "../include/radiation_facet.hh"
// #include "../include/radiation_surface.hh"
#include "../include/radiation_messages.hh"
#include "../include/radiation_params.hh"

//! Namespace jeod
namespace jeod
{

// Static default values.

const double RadiationFacet::two_thirds = 2.0 / 3.0;

/**
 * Run sanity checks on input variables
 */
void RadiationFacet::initialize()
{
    RadiationBaseFacet::initialize();

    if((albedo < 0.0) || (albedo_vis < 0.0) || (albedo_IR < 0.0) || (diffuse < 0.0) || (albedo > 1.0) ||
       (albedo_vis > 1.0) || (albedo_IR > 1.0) || (diffuse > 1.0))
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             RadiationMessages::invalid_setup_error,
                             "\n"
                             "An illegal value has been set in the Radiation Pressure surface.\n"
                             "The following values should all have values between 0.0 and 1.0:\n"
                             "albedo,       albedo_vis,      albedo_IR,     diffuse.  \n"
                             "The values are:\n %f %f %f %f.\n",
                             albedo,
                             albedo_vis,
                             albedo_IR,
                             diffuse);
    }

    thermal.facet = this;
}

/**
 * Defines the inherent facet values
 * \param[in] facet pointer to the facet
 * \param[in] facet_thermal reference to the thermal components of the facet.
 * \param[in] params pointer to the list of material parameters for the facet.
 */
void RadiationFacet::define_facet_core(Facet *, const ThermalFacetRider & facet_thermal, RadiationParams * params)
{
    albedo = params->albedo;
    diffuse = params->diffuse;
    thermal.active = facet_thermal.active;
    thermal.thermal_power_dump = facet_thermal.thermal_power_dump;
    thermal.heat_capacity = params->thermal.heat_capacity_per_area * base_facet->area;
    thermal.emissivity = params->thermal.emissivity;
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
