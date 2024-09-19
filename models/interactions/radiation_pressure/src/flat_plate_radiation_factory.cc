/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup RadiationPressure
 * @{
 *
 * @file models/interactions/radiation_pressure/src/flat_plate_radiation_factory.cc
 * Factory that creates a FlatPlateRadiationFacet, from a facet model
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((None))

Library dependencies:
   ((flat_plate_radiation_factory.cc)
    (flat_plate_radiation_facet.cc)
    (radiation_facet.cc)
    (radiation_messages.cc)
    (utils/message/src/message_handler.cc)
    (utils/surface_model/src/facet.cc))


*******************************************************************************/

// System includes
#include <cstddef>
#include <typeinfo>

// JEOD includes
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/surface_model/include/flat_plate_thermal.hh" //includes Facet

// Model includes
#include "../include/flat_plate_radiation_facet.hh"
#include "../include/flat_plate_radiation_factory.hh"
#include "../include/radiation_messages.hh"
#include "../include/radiation_params.hh" // includes FacetParams

//! Namespace jeod
namespace jeod
{

// Attributes used in allocations
JEOD_DECLARE_ATTRIBUTES(FlatPlateRadiationFacet)

/**
 * Constructor for FlatPlateRadiationFactory
 */
FlatPlateRadiationFactory::FlatPlateRadiationFactory()
{
    JEOD_REGISTER_CLASS(FlatPlateRadiationFacet);
}

/**
 * Records the data for the Flat Plate Radiation Facet.
 * @return pointer to the interaction facet that this function creates.
 * \param[in] facet pointer to the facet
 * \param[in] params pointer to the set of parameters for the facet.
 */
InteractionFacet * FlatPlateRadiationFactory::create_facet(Facet * facet, FacetParams * params)
{
    RadiationParams * radiation_params = nullptr;
    FlatPlateThermal * flat_plate = nullptr;

    radiation_params = dynamic_cast<RadiationParams *>(params);
    flat_plate = dynamic_cast<FlatPlateThermal *>(facet);

    if(radiation_params == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             RadiationMessages::invalid_setup_error,
                             "\n"
                             "The parameter list sent to the Flat Plate Radiation Factory is\n"
                             "not a set of radiation parameters.  Radiation parameters\n"
                             "(class RadiationParams) should have the following elements:\n"
                             "name, albedo, diffuse, thermal.emissivity, "
                             "thermal.heat_capacity_per_area, \n"
                             "and (optional, default to 0.0) thermal.thermal_power_dump .\n");
        return nullptr;
    }
    if(flat_plate == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             RadiationMessages::invalid_setup_error,
                             "\n"
                             "The Facet sent to the Flat Plate Radiation Factory is\n"
                             "not a Thermal Flat Plate.  All Radiation facets must be \n"
                             "Thermal Facets (consider changing from FlatPlate to \n"
                             "FlatPlateThermal in the data file that defines the radiation\n"
                             "surface).  This particular factory can only handle Flat\n"
                             "Plates, a different factory must be used for any other\n"
                             "geometry.\n");
        return nullptr;
    }

    FlatPlateRadiationFacet * fpr_facet = JEOD_ALLOC_CLASS_OBJECT(FlatPlateRadiationFacet, ());

    fpr_facet->base_facet = facet;

    //  Note the redundant variables being passed:
    //  "define_facet" must receive a Facet* (the only facet type common to all
    //  factories), and a ThermalFacetRider to account for the thermal side of
    //  flat_plate.  This call is common to all factories.
    fpr_facet->RadiationFacet::define_facet_core(flat_plate, flat_plate->thermal, radiation_params);

    // This call is specific to the facet geometry.
    fpr_facet->define_facet(flat_plate);

    return fpr_facet;
}

/**
 * Tests to ensure that the factory can function on the facet as
 * intended.
 * @return Boolean, is this the correct factory?
 * \param[in] facet pointer to the facet being manipulated by the factory
 */
bool FlatPlateRadiationFactory::is_correct_factory(Facet * facet)
{
    if(typeid(*facet) == typeid(FlatPlateThermal))
    {
        return true;
    }
    else
    {
        return false;
    }
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
