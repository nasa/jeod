/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup Contact
 * @{
 *
 * @file models/interactions/contact/src/contact_surface_factory.cc
 * Factory that creates an contact surface, from a surface model
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

Library dependencies:
    ((contact_surface_factory.cc)
     (contact_messages.cc)
     (contact_params.cc))


*******************************************************************************/

/* System includes */
#include <cstddef>
#include <cstring>

/* JEOD includes */
#include "dynamics/mass/include/mass.hh"
#include "utils/surface_model/include/facet.hh"
#include "utils/surface_model/include/facet_params.hh"
#include "utils/surface_model/include/interaction_surface.hh"
#include "utils/surface_model/include/surface_model.hh"

/* Model includes */
#include "../include/contact_messages.hh"
#include "../include/contact_params.hh"
#include "../include/contact_surface_factory.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Default Constructor
 */
ContactSurfaceFactory::ContactSurfaceFactory()
{
    // push the facet factories that JEOD knows about onto the
    // factories list
    factories.push_back(&point_contact_facet_factory);
    factories.push_back(&line_contact_facet_factory);
}

/**
 * Creates an interaction surface, in the inter_surface parameter,
 * from the given SurfaceModel. The InteractionSurfaceFactory
 * should contain all necessary InteractionFacetFactories and
 * FacetParams already
 * \param[in] surface The surface model used to create the interaction surface
 * \param[out] inter_surface Where the interaction surface will be produced
 */

void ContactSurfaceFactory::create_surface(SurfaceModel * surface, InteractionSurface * inter_surface)
{
    // can't hand this un-allocated things. check for that.
    if(surface == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             ContactMessages::initialization_error,
                             "The SurfaceModel pointer supplied to "
                             "InteractionSurfaceFactory::create_surface was NULL. Please "
                             "provide a SurfaceModel pointer.");
        return;
    }

    if(inter_surface == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             ContactMessages::initialization_error,
                             "The InteractionSurface pointer supplied to "
                             "InteractionSurfaceFactory::create_surface was NULL. Please "
                             "provide an InteractionSurface pointer.");
        return;
    }

    // Allocate the array, through this virtual function, in the
    // interaction surface
    inter_surface->allocate_array(surface->facets.size());

    // For all facets in the surface, match up an interaction facet factory
    for(unsigned int ii = 0; ii < surface->facets.size(); ++ii)
    {
        InteractionFacetFactory * facet_factory = nullptr;

        for(unsigned int jj = 0; jj < factories.size(); ++jj)
        {
            if(factories[jj]->is_correct_factory(surface->facets[ii]))
            {
                facet_factory = factories[jj];
                break;
            }
        }

        if(facet_factory == nullptr)
        {
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 ContactMessages::initialization_error,
                                 "No InteractionFacetFactory was found for the facet at index "
                                 "%d. Please provide an appropriate InteractionFacetFactory",
                                 ii);

            return;
        }

        // For each facet, try to match up a facet_params that matches
        // them.

        FacetParams * facet_params = nullptr;

        for(unsigned int jj = 0; jj < params.size(); ++jj)
        {
            if(surface->facets[ii]->param_name.empty())
            {
                MessageHandler::fail(__FILE__,
                                     __LINE__,
                                     ContactMessages::initialization_error,
                                     "A Facet was found with no param_name set.");
            }

            if(params[jj]->name == surface->facets[ii]->param_name)
            {
                facet_params = params[jj];
                break;
            }
        }

        if(facet_params == nullptr)
        {
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 ContactMessages::initialization_error,
                                 "No facet_params with name (%s) was found.",
                                 surface->facets[ii]->param_name.c_str());
        }

        if((ii == 0) ||
           (surface->facets[ii - 1]->get_mass_body_ptr()->name == surface->facets[ii]->get_mass_body_ptr()->name))
        {
            inter_surface->allocate_interaction_facet(surface->facets[ii], facet_factory, facet_params, ii);
        }
        else
        {
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 ContactMessages::initialization_error,
                                 "An attempt was made to add facets to a single surface that are "
                                 "on different dyn_bodies. This is not allowed.",
                                 surface->facets[ii]->param_name.c_str());
        }
    } // for(unsigned int ii = 0)
}

/**
 * Add a named set of facet params to the surface factory. Intended
 * to be used when an contact specific surface is created, to
 * convert a basic facet to an contact interaction facet. This
 * MUST be a parameter inheriting from ContactParam, or the function
 * will fail and send a failure message
 * \param[in] to_add The facet parameters to add
 */
void ContactSurfaceFactory::add_facet_params(FacetParams * to_add)
{
    if(to_add->name.size() == 0)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             ContactMessages::pre_initialization_error,
                             "A FacetParams object was sent to "
                             "ContactSurfaceFactory::add_facet_params without a set name. "
                             "A set name is required to be added to the "
                             "ContactSurfaceFactory");
    }

    // The param MUST be an
    ContactParams * temp_ptr = nullptr;

    temp_ptr = dynamic_cast<ContactParams *>(to_add);

    if(temp_ptr == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             ContactMessages::pre_initialization_error,
                             "The FacetParams with name (%s) was not of a type that inherits "
                             "from ContactParams. This is a requirement to be added to an "
                             "ContactSurfaceFactory.",
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
