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
    ((radiation_surface_factory.o)
     (flat_plate_radiation_factory.o)
     (radiation_messages.o)
     (radiation_params.o)
     (utils/message/message_handler.o)
     (utils/surface_model/facet_params.o)
     (utils/surface_model/interaction_surface_factory.o))


*******************************************************************************/


// System includes
#include <cstddef>

// JEOD includes
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/radiation_surface_factory.hh"
#include "../include/radiation_params.hh"
#include "../include/radiation_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Constructor
 */
RadiationSurfaceFactory::RadiationSurfaceFactory (
   void)
{
   // push the facet factories that JEOD knows about onto the
   // factories list
   factories.push_back (&flat_plate_radiation_factory);
}

/**
 * Add a set of radiation parameters to those available to Radiation
 * Surface.
 * \param[in] to_add pointer to the parameter list to add.
 */
void
RadiationSurfaceFactory::add_facet_params (
   FacetParams* to_add)
{
   RadiationParams* temp_ptr = NULL;

   temp_ptr = dynamic_cast<RadiationParams*> (to_add);

   if (temp_ptr == NULL) {

      MessageHandler::fail (
         __FILE__, __LINE__, RadiationMessages::invalid_setup_error, "\n"
         "The parameter list sent to the Radiation Surface Factory is\n"
         "not a set of radiation parameters.  Radiation parameters\n"
         "(class RadiationParams) should have the following elements:\n"
         "name, albedo, diffuse, thermal.emissivity, either \n"
         "thermal.heat_capacity or thermal.heat_capacity_per_area, \n"
         "and (optional, default to 0.0) thermal.thermal_power_dump .\n");
      return;
   }

   InteractionSurfaceFactory::add_facet_params (to_add);

   return;

}


/**
 * Destructor
 */
RadiationSurfaceFactory::~RadiationSurfaceFactory (
   void)
{
   factories.clear();
}


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
