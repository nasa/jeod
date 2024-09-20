/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup RadiationPressure
 * @{
 *
 * @file models/interactions/radiation_pressure/src/radiation_surface.cc
 * Vehicle surface model for Radiation Pressure model
 */

/************************** TRICK HEADER***************************************
PURPOSE:
()

REFERENCE:
(((None)))

ASSUMPTIONS AND LIMITATIONS:
((None))

Library dependencies:
((radiation_surface.cc)
(radiation_facet.cc)
(radiation_messages.cc)
(interactions/thermal_rider/src/thermal_facet_rider.cc)
(utils/message/src/message_handler.cc))


*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "utils/math/include/vector3.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/surface_model/include/facet.hh"
#include "utils/surface_model/include/interaction_facet.hh"
#include "utils/surface_model/include/interaction_facet_factory.hh"

// Model includes
#include "../include/radiation_facet.hh"
#include "../include/radiation_messages.hh"
#include "../include/radiation_surface.hh"

//! Namespace jeod
namespace jeod
{

// Attributes used in allocations
JEOD_DECLARE_ATTRIBUTES(RadiationFacet)

/**
 * Constructor for RadiationSurface
 */
RadiationSurface::RadiationSurface()
{
    JEOD_REGISTER_INCOMPLETE_CLASS(RadiationFacet);
}

/**
 * Initializes the radiation surface.
 * \param[in] center_grav position of center of mass\n Units: M
 */
void RadiationSurface::initialize(double center_grav[3])
{
    if(num_facets == 0)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             RadiationMessages::operational_setup_error,
                             "\n"
                             "The initialization routine for Radiation Surface was called with no "
                             "facets.  \n"
                             "This is likely an error, possibly caused by initializing the surface "
                             "before creating it. \n"
                             "If there are no facets in the radiation surface, eliminate the "
                             "radiation pressure model from the simulation. \n");
    }

    for(ii_facet = 0; ii_facet < num_facets; ++ii_facet)
    {
        facets[ii_facet]->initialize_geom(center_grav);
    }

    for(ii_facet = 0; ii_facet < num_facets; ++ii_facet)
    {
        if((facets[ii_facet]->albedo > 1) || (facets[ii_facet]->albedo < 0))
        {
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 RadiationMessages::invalid_setup_error,
                                 "\n"
                                 "The albedo from one of the facet %i is out of range.  \n"
                                 "Albedo represents a fraction of incident light, and must \n"
                                 "be between 0 and 1.\n",
                                 ii_facet);
        }
        if((facets[ii_facet]->diffuse > 1) || (facets[ii_facet]->diffuse < 0))
        {
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 RadiationMessages::invalid_setup_error,
                                 "\n"
                                 "The diffuse parameter from facet %i is out of range.  \n"
                                 "Diffuse represents a fraction of reflected light, and must \n"
                                 "be between 0 and 1.\n",
                                 ii_facet);
        }
        if((facets[ii_facet]->thermal.emissivity > 1) || (facets[ii_facet]->thermal.emissivity < 0))
        {
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 RadiationMessages::invalid_setup_error,
                                 "\n"
                                 "The emissivity from facet %i is out of range.  \n"
                                 "Emissivity represents the tendency towards a perfect black surface,\n"
                                 "0 representing a perfectly insulating surface (no emission), \n"
                                 "and 1 a perfect black body.  \n"
                                 "The value of thermal.emissivity must be between 0 and 1.\n",
                                 ii_facet);
        }
    }
}

/**
 * Allocates memory for an array of radiation facets
 * \param[in] size Size of array
 */
void RadiationSurface::allocate_array(unsigned int size)
{
    if(facets != nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             RadiationMessages::operational_setup_error,
                             "\n"
                             "Before setting up the array of RadiationFacet pointers (facets),\n"
                             "it was found to be non-NULL, indicating that it already contains data."
                             "\nIt should not contain any data until it has been populated with\n"
                             "the interaction facets.\n");
        return;
    }

    facets = JEOD_ALLOC_CLASS_POINTER_ARRAY(size, RadiationFacet);
    num_facets = size;

    // Make sure all pointers are NULL so destructor never crashes
    for(ii_facet = 0; ii_facet < num_facets; ++ii_facet)
    {
        facets[ii_facet] = nullptr;
    }
}

/**
 * Turns facet memory into radiaiton facet memory.
 * \param[in] facet pointer to the facet
 * \param[in] factory pointer to the facet factory
 * \param[in] params pointer to the generic facet parameter.
 * \param[in] index index value in the facet list.
 */
void RadiationSurface::allocate_interaction_facet(Facet * facet,
                                                  InteractionFacetFactory * factory,
                                                  FacetParams * params,
                                                  unsigned int index)
{
    if(num_facets <= index)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             RadiationMessages::invalid_setup_error,
                             "\n"
                             "Attempting to allocate data for facet number %i, but the model has\n"
                             "allocated space for only %i facets (num_facets = %i).\n",
                             index,
                             num_facets,
                             num_facets);

        return;
    }

    /* need to temporarily save off the InteractionFacet returned before
       dynamic casting it. If the dynamic cast fails, we want to destroy
       the InteractionFacet so we don't get a memory leak */

    InteractionFacet * temp_facet = nullptr;

    temp_facet = factory->create_facet(facet, params);

    if(temp_facet == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             RadiationMessages::operational_setup_error,
                             "\n"
                             "create_facet failed, returning a NULL pointer.\n");

        return;
    }

    auto * temp_radiation_facet = dynamic_cast<RadiationFacet *>(temp_facet);

    if(temp_radiation_facet == nullptr)
    {
        // temp_facet can NOT be NULL, since it was already checked for above
        JEOD_DELETE_OBJECT(temp_facet);

        MessageHandler::fail(__FILE__,
                             __LINE__,
                             RadiationMessages::operational_setup_error,
                             "\n"
                             "The InteractionFacet temp_facet exists, but it is not a\n"
                             "RadiationFacet; the cast to RadiationFacet failed.\n");
        return;
    }

    facets[index] = temp_radiation_facet;
}

/**
 * systematically calls the method to calculate the interaction on each facet.
 * \param[in] flux_mag Magnitude of incident flux
 * \param[in] flux_struc_hat unit vector of incident flux
 * \param[in] calculate_forces boolean indicating whether to calculate forces.
 */
void RadiationSurface::incident_radiation(double flux_mag, const double flux_struc_hat[3], bool calculate_forces)
{
    for(ii_facet = 0; ii_facet < num_facets; ++ii_facet)
    {
        facets[ii_facet]->incident_radiation(flux_mag, flux_struc_hat, calculate_forces);
    }
}

/**
 * systematically calls the method to calculate the interaction of each facet
 * with ThirdBody flux.
 * \param[in,out] third_body_ptr Third body that emits radiation.
 * \param[in] calculate_forces Calculate forces on the surface if true.
 */
void RadiationSurface::interact_with_third_body(RadiationThirdBody * third_body_ptr, const bool calculate_forces)
{
    for(ii_facet = 0; ii_facet < num_facets; ++ii_facet)
    {
        facets[ii_facet]->interact_with_third_body(third_body_ptr, calculate_forces);
    }
}

/**
 * systematically calls the method to accumulate thermal sources on each facet.
 */
void RadiationSurface::accumulate_thermal_sources()
{
    for(ii_facet = 0; ii_facet < num_facets; ++ii_facet)
    {
        facets[ii_facet]->thermal.accumulate_thermal_sources();
    }
}

/**
 * systematically calls the method to integrate the temperature variation for each facet.
 */
void RadiationSurface::thermal_integrator()
{
    for(ii_facet = 0; ii_facet < num_facets; ++ii_facet)
    {
        if(facets[ii_facet]->thermal.integrable_object.active)
        {
            facets[ii_facet]->base_facet->temperature = facets[ii_facet]->thermal.integrable_object.get_temp();
        }
        else
        {
            facets[ii_facet]->base_facet->temperature = facets[ii_facet]->thermal.integrate();
        }
    }
}

/**
 * systematically calls the method to ensure that the same for each facet.
 */
void RadiationSurface::equalize_absorption_emission()
{
    for(ii_facet = 0; ii_facet < num_facets; ++ii_facet)
    {
        facets[ii_facet]->thermal.power_emit = facets[ii_facet]->thermal.power_absorb;
    }
}

/**
 * systematically calls the method to wrap up the radiation pressure calculation on each facet and accumulate forces and
 * torques over all facets.
 */
void RadiationSurface::radiation_pressure()
{
    Vector3::initialize(force);
    Vector3::initialize(torque);

    for(ii_facet = 0; ii_facet < num_facets; ++ii_facet)
    {
        facets[ii_facet]->radiation_pressure();
        Vector3::incr(facets[ii_facet]->force, force);
        Vector3::incr(facets[ii_facet]->torque, torque);
    }
}

/**
 * To initialize the values during each update run
 */
void RadiationSurface::initialize_runtime_values()
{
    for(ii_facet = 0; ii_facet < num_facets; ++ii_facet)
    {
        facets[ii_facet]->initialize_runtime_values();
    }
}

/**
 * To attach thermal integrators from each facet to the integration
 * group of a DynBody
 * \param[in,out] dyn_body Body to which integrators are to be added.
 */
void RadiationSurface::add_thermal_integrators_to(DynBody * dyn_body)
{
    for(unsigned int ii = 0; ii < num_facets; ++ii)
    {
        facets[ii]->thermal.integrable_object.active = true;
        dyn_body->add_integrable_object(facets[ii]->thermal.integrable_object);
    }
}

/**
 * Destructor for RadiationSurface
 */
RadiationSurface::~RadiationSurface()
{
    JEOD_DELETE_2D(facets, num_facets, false);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
