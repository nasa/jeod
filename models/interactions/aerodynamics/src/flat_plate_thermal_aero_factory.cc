/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup Aerodynamics
 * @{
 *
 * @file models/interactions/aerodynamics/src/flat_plate_thermal_aero_factory.cc
 * Factory that creates a FlatPlateAeroFacet from a FlatPlateThermal,
 * using a FlatPlateAeroParams object
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

Library dependencies:
   ((flat_plate_thermal_aero_factory.cc)
    (flat_plate_aero_factory.cc)
    (utils/surface_model/src/flat_plate_thermal.cc))


*******************************************************************************/


// System includes
#include <typeinfo>

// JEOD includes
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/surface_model/include/flat_plate_thermal.hh"

// Model includes
#include "../include/flat_plate_thermal_aero_factory.hh"
#include "../include/flat_plate_aero_params.hh"
#include "../include/flat_plate_aero_facet.hh"
#include "../include/aerodynamics_messages.hh"

//! Namespace jeod
namespace jeod {

// Attributes used in allocations
JEOD_DECLARE_ATTRIBUTES (FlatPlateAeroFacet)


/**
 * Default Constructor
 */
FlatPlateThermalAeroFactory::FlatPlateThermalAeroFactory (
   void)
{
   JEOD_REGISTER_CLASS(FlatPlateThermalAeroFactory);
   JEOD_REGISTER_CLASS(FlatPlateAeroFacet);


}

/**
 * Destructor
 */

FlatPlateThermalAeroFactory::~FlatPlateThermalAeroFactory (
   void)
{

   // empty for now

}

/**
 * FlatPlateThermalAeroFactory specific implementation of this function.
 * If the Facet is of type FlatPlate, returns true. False otherwise
 * @return true if facet is a FlatPlate, false otherwise
 * \param[in] facet The facet to check
 */

bool
FlatPlateThermalAeroFactory::is_correct_factory (
   Facet* facet)
{

   if (typeid(*facet) == typeid(FlatPlateThermal)) {
      return true;
   }
   else {
      return false;
   }

}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
