/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup Aerodynamics
 * @{
 *
 * @file models/interactions/aerodynamics/src/flat_plate_aero_factory.cc
 * Factory that creates a FlatPlateAeroFacet from a FlatPlate,
 * using a FlatPlateAeroParams object
 */

/************************** TRICK HEADER***************************************
PURPOSE:
   ()

Library dependencies:
   ((flat_plate_aero_factory.cc)
    (aerodynamics_messages.cc)
    (flat_plate_aero_facet.cc)
    (utils/message/src/message_handler.cc)
    (utils/surface_model/src/facet.cc)
    (utils/surface_model/src/flat_plate.cc))


*******************************************************************************/

// System includes
#include <cstddef>
#include <typeinfo>

// JEOD includes
#include "utils/math/include/vector3.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/surface_model/include/facet.hh"
#include "utils/surface_model/include/flat_plate.hh"
#include "utils/surface_model/include/interaction_facet.hh"

// Model includes
#include "../include/aerodynamics_messages.hh"
#include "../include/flat_plate_aero_facet.hh"
#include "../include/flat_plate_aero_factory.hh"
#include "../include/flat_plate_aero_params.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Default Constructor
 */
FlatPlateAeroFactory::FlatPlateAeroFactory()
{
    JEOD_REGISTER_CLASS(FlatPlateAeroFactory);
    JEOD_REGISTER_CLASS(FlatPlateAeroFacet);
}

/**
 * Create a FlatPlateAeroFacet from a flat plate facet and a
 * FlatePlateAeroParams object
 * @return The new FlatPlateAeroFacet. Note that this is allocated and YOU are responsible for destroying it at the end!
 * \param[in] facet The FlatPlate. This MUST be a flat plate or the algorithm will send a failure message
 * \param[in] params FlatPlateAeroParams. This MUST be of the type FlatPlateAeroParams, or the algorithm will send a
 * failture message\n Units: The
 */

InteractionFacet * FlatPlateAeroFactory::create_facet(Facet * facet, FacetParams * params)
{
    FlatPlateAeroParams * aero_params = nullptr;
    FlatPlate * flat_plate = nullptr;

    aero_params = dynamic_cast<FlatPlateAeroParams *>(params);
    flat_plate = dynamic_cast<FlatPlate *>(facet);

    // We have tried casting the facet and params to the correct types.
    // if they were not the correct type, send out an error message

    if(aero_params == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             AerodynamicsMessages::initialization_error,
                             "The FacetParams supplied to "
                             "FlatPlateAeroFactory::create_facet, named (%s), "
                             "was not of type "
                             "FlatPlateAeroParams.",
                             params->name.c_str());
        return nullptr;
    }
    if(flat_plate == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             AerodynamicsMessages::initialization_error,
                             "the Facet supplied to "
                             "FlatPlateAeroFactory::create_facet was not of type "
                             "FlatPlate, as is required");
        return nullptr;
    }

    // Create the interaction facet
    FlatPlateAeroFacet * inter_facet = JEOD_ALLOC_CLASS_OBJECT(FlatPlateAeroFacet, ());

    // Fill it out from the parameters and from the facet itself
    inter_facet->base_facet = facet;

    inter_facet->coef_method = aero_params->coef_method;
    inter_facet->calculate_drag_coef = aero_params->calculate_drag_coef;
    inter_facet->epsilon = aero_params->epsilon;
    inter_facet->temp_reflect = aero_params->temp_reflect;
    inter_facet->drag_coef_norm = aero_params->drag_coef_norm;
    inter_facet->drag_coef_tang = aero_params->drag_coef_tang;
    inter_facet->drag_coef_spec = aero_params->drag_coef_spec;
    inter_facet->drag_coef_diff = aero_params->drag_coef_diff;

    inter_facet->temperature = facet->temperature;

    inter_facet->normal = flat_plate->normal;
    inter_facet->center_pressure = flat_plate->position;

    inter_facet->area = flat_plate->area;

    return inter_facet;
}

/**
 * FlatPlateAeroFactory specific implementation of this function.
 * If the Facet is of type FlatPlate, returns true. False otherwise
 * @return true if facet is a FlatPlate, false otherwise
 * \param[in] facet The facet to check
 */

bool FlatPlateAeroFactory::is_correct_factory(Facet * facet)
{
    if(typeid(*facet) == typeid(FlatPlate))
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
