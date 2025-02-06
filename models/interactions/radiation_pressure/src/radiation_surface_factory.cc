/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup RadiationPressure
 * @{
 *
 * @file models/interactions/radiation_pressure/src/radiation_surface_factory.cc
 * Factory that creates an interaction surface, for a specific
 * enviornment interaction model, from a surface model
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((None))

Library dependencies:
    ((radiation_surface_factory.cc)
     (flat_plate_radiation_factory.cc)
     (radiation_messages.cc)
     (utils/message/src/message_handler.cc)
     (utils/surface_model/src/interaction_surface_factory.cc))


*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/radiation_messages.hh"
#include "../include/radiation_params.hh"
#include "../include/radiation_surface_factory.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Constructor
 */
RadiationSurfaceFactory::RadiationSurfaceFactory()
{
    // push the facet factories that JEOD knows about onto the
    // factories list
    factories.push_back(&flat_plate_radiation_factory);
}

/**
 * Add a set of radiation parameters to those available to Radiation
 * Surface.
 * \param[in] to_add pointer to the parameter list to add.
 */
void RadiationSurfaceFactory::add_facet_params(FacetParams * to_add)
{
    RadiationParams * temp_ptr = nullptr;

    temp_ptr = dynamic_cast<RadiationParams *>(to_add);

    if(temp_ptr == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             RadiationMessages::invalid_setup_error,
                             "\n"
                             "The parameter list sent to the Radiation Surface Factory is\n"
                             "not a set of radiation parameters.  Radiation parameters\n"
                             "(class RadiationParams) should have the following elements:\n"
                             "name, albedo, diffuse, thermal.emissivity, either \n"
                             "thermal.heat_capacity or thermal.heat_capacity_per_area, \n"
                             "and (optional, default to 0.0) thermal.thermal_power_dump .\n");
        return;
    }

    InteractionSurfaceFactory::add_facet_params(to_add);
}

/**
 * Destructor
 */
RadiationSurfaceFactory::~RadiationSurfaceFactory()
{
    factories.clear();
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
