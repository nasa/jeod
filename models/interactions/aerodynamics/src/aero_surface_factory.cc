/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup Aerodynamics
 * @{
 *
 * @file models/interactions/aerodynamics/src/aero_surface_factory.cc
 * Factory that creates an aerodynamics surface, from a surface model
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

Library dependencies:
    ((aero_surface_factory.cc)
     (aerodynamics_messages.cc)
     (flat_plate_aero_factory.cc)
     (flat_plate_thermal_aero_factory.cc)
     (utils/surface_model/src/interaction_surface_factory.cc)
     (utils/message/src/message_handler.cc))


*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "utils/surface_model/include/facet_params.hh"

// Model includes
#include "../include/aero_params.hh"
#include "../include/aero_surface_factory.hh"
#include "../include/aerodynamics_messages.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Default Constructor
 */

AeroSurfaceFactory::AeroSurfaceFactory()
{
    // push the facet factories that JEOD knows about onto the
    // factories list
    factories.push_back(&flat_plate_aero_factory);
    factories.push_back(&flat_plate_thermal_aero_factory);
}

/**
 * Add a named set of facet params to the surface factory. Intended
 * to be used when an aerodynamic specific surface is created, to
 * convert a basic facet to an aerodynamic interaction facet. This
 * MUST be a parameter inheriting from AeroParam, or the function
 * will fail and send a failure message
 * \param[in] to_add The facet parameters to add
 */

void AeroSurfaceFactory::add_facet_params(FacetParams * to_add)
{
    if(to_add->name.empty())
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             AerodynamicsMessages::pre_initialization_error,
                             "A FacetParams object was sent to "
                             "AeroSurfaceFactory::add_facet_params without a set name. "
                             "A set name is required to be added to the "
                             "AeroSurfaceFactory");
    }

    // The param MUST be an
    AeroParams * temp_ptr = nullptr;

    temp_ptr = dynamic_cast<AeroParams *>(to_add);

    if(temp_ptr == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             AerodynamicsMessages::pre_initialization_error,
                             "The FacetParams with name (%s) was not of a type that inherits "
                             "from AeroParams. This is a requirement to be added to an "
                             "AeroSurfaceFactory.",
                             to_add->name.c_str());

        return;
    }

    // Add the parameter through the inherited function
    InteractionSurfaceFactory::add_facet_params(to_add);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
