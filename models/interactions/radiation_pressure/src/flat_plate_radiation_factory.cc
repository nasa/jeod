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
   ((flat_plate_radiation_factory.o)
    (flat_plate_radiation_facet.o)
    (radiation_facet.o)
    (radiation_messages.o)
    (radiation_params.o)
    (utils/message/message_handler.o)
    (utils/surface_model/facet.o)
    (utils/surface_model/facet_params.o)
    (utils/surface_model/flat_plate_thermal.o)
    (utils/surface_model/interaction_facet_factory.o))


*******************************************************************************/

// System includes
#include <cstddef>
#include <typeinfo>

// JEOD includes
#include "utils/surface_model/include/flat_plate_thermal.hh" //includes Facet
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/flat_plate_radiation_factory.hh"
#include "../include/radiation_params.hh"  // includes FacetParams
#include "../include/flat_plate_radiation_facet.hh"
#include "../include/radiation_messages.hh"

//! Namespace jeod
namespace jeod {

// Attributes used in allocations
JEOD_DECLARE_ATTRIBUTES (FlatPlateRadiationFacet)


/**
 * Constructor for FlatPlateRadiationFactory
 */
FlatPlateRadiationFactory::FlatPlateRadiationFactory (
   void)
{
   JEOD_REGISTER_CLASS(FlatPlateRadiationFacet);
}


/**
 * Records the data for the Flat Plate Radiation Facet.
 * @return pointer to the interaction facet that this function creates.
 * \param[in] facet pointer to the facet
 * \param[in] params pointer to the set of parameters for the facet.
 */
InteractionFacet*
FlatPlateRadiationFactory::create_facet (
   Facet* facet,
   FacetParams* params)
{
   RadiationParams* radiation_params = NULL;
   FlatPlateThermal* flat_plate      = NULL;

   radiation_params = dynamic_cast<RadiationParams*> (params);
   flat_plate       = dynamic_cast<FlatPlateThermal*> (facet);

   if (radiation_params == NULL) {
      MessageHandler::fail (
         __FILE__, __LINE__, RadiationMessages::invalid_setup_error, "\n"
         "The parameter list sent to the Flat Plate Radiation Factory is\n"
         "not a set of radiation parameters.  Radiation parameters\n"
         "(class RadiationParams) should have the following elements:\n"
         "name, albedo, diffuse, thermal.emissivity, "
         "thermal.heat_capacity_per_area, \n"
         "and (optional, default to 0.0) thermal.thermal_power_dump .\n");
   }
   if (flat_plate == NULL) {
      MessageHandler::fail (
         __FILE__, __LINE__, RadiationMessages::invalid_setup_error, "\n"
         "The Facet sent to the Flat Plate Radiation Factory is\n"
         "not a Thermal Flat Plate.  All Radiation facets must be \n"
         "Thermal Facets (consider changing from FlatPlate to \n"
         "FlatPlateThermal in the data file that defines the radiation\n"
         "surface).  This particular factory can only handle Flat\n"
         "Plates, a different factory must be used for any other\n"
         "geometry.\n");
   }

   FlatPlateRadiationFacet* fpr_facet =
      JEOD_ALLOC_CLASS_OBJECT (FlatPlateRadiationFacet, ());

   fpr_facet->base_facet = facet;

   //  Note the redundant variables being passed:
   //  "define_facet" must receive a Facet* (the only facet type common to all
   //  factories), and a ThermalFacetRider to account for the thermal side of
   //  flat_plate.  This call is common to all factories.
   fpr_facet->RadiationFacet::define_facet_core (flat_plate,
                                                 flat_plate->thermal,
                                                 radiation_params);

   // This call is specific to the facet geometry.
   fpr_facet->define_facet (flat_plate);

   return fpr_facet;

}

/**
 * Tests to ensure that the factory can function on the facet as
 * intended.
 * @return Boolean, is this the correct factory?
 * \param[in] facet pointer to the facet being manipulated by the factory
 */
bool
FlatPlateRadiationFactory::is_correct_factory (
   Facet* facet)
{

   if (typeid(*facet) == typeid(FlatPlateThermal)) {
      return true;
   }
   else {
      return false;
   }

}



/**
 * Destructor for FlatPlateRadiationFactory
 */
FlatPlateRadiationFactory::~FlatPlateRadiationFactory (
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
