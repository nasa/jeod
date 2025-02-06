/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup SurfaceModel
 * @{
 *
 * @file models/utils/surface_model/src/interaction_surface_factory.cc
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
    ((interaction_surface_factory.o)
     (surface_model_messages.o)
     (interaction_facet_factory.o)
     (surface_model.o)
     (interaction_surface.o)
     (utils/message/message_handler.o))

 
*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "utils/message/include/message_handler.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/named_item/include/named_item.hh"

// Model includes
#include "../include/interaction_surface_factory.hh"
#include "../include/interaction_facet_factory.hh"
#include "../include/interaction_surface.hh"
#include "../include/facet.hh"
#include "../include/facet_params.hh"
#include "../include/surface_model.hh"
#include "../include/surface_model_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Default Constructor
 */

InteractionSurfaceFactory::InteractionSurfaceFactory (
   void)
{
   JEOD_REGISTER_CLASS(InteractionSurfaceFactory);
   JEOD_REGISTER_INCOMPLETE_CLASS(InteractionFacetFactory);
   JEOD_REGISTER_CLASS(FacetParams);
   JEOD_REGISTER_CHECKPOINTABLE(this, factories);
   JEOD_REGISTER_CHECKPOINTABLE(this, params);
}

/**
 * Destructor
 */

InteractionSurfaceFactory::~InteractionSurfaceFactory (
   void)
{
   JEOD_DEREGISTER_CHECKPOINTABLE(this, factories);
   JEOD_DEREGISTER_CHECKPOINTABLE(this, params);

}

/**
 * Creates an interaction surface, in the inter_surface parameter,
 * from the given SurfaceModel. The InteractionSurfaceFactory
 * should contain all necessary InteractionFacetFactories and
 * FacetParams already
 * \param[in] surface The surface model used to create the interaction surface
 * \param[out] inter_surface Where the interaction surface will be produced
 */

void
InteractionSurfaceFactory::create_surface (
   SurfaceModel* surface,
   InteractionSurface* inter_surface)
{

   // can't hand this un-allocated things. check for that.
   if (surface == NULL) {

      MessageHandler::fail (
         __FILE__, __LINE__, SurfaceModelMessages::initialization_error,
         "The SurfaceModel pointer supplied to "
         "InteractionSurfaceFactory::create_surface was NULL. Please "
         "provide a SurfaceModel pointer.");

      return;
   }

   if (inter_surface == NULL) {

      MessageHandler::fail (
         __FILE__, __LINE__, SurfaceModelMessages::initialization_error,
         "The InteractionSurface pointer supplied to "
         "InteractionSurfaceFactory::create_surface was NULL. Please "
         "provide an InteractionSurface pointer.");

   }



   // Allocate the array, through this virtual function, in the
   // interaction surface
   inter_surface->allocate_array (surface->facets.size());



   // For all facets in the surface, match up an interaction facet factory
   for (unsigned int ii = 0; ii < surface->facets.size(); ++ii) {

      InteractionFacetFactory* facet_factory = NULL;

      for (unsigned int jj = 0; jj < factories.size(); ++jj) {

         if (factories[jj]->is_correct_factory (surface->facets[ii])) {
            facet_factory = factories[jj];
            break;
         }

      }

      if (facet_factory == NULL) {

         MessageHandler::fail (
            __FILE__, __LINE__, SurfaceModelMessages::initialization_error,
            "No InteractionFacetFactory was found for the facet at index "
            "%d. Please provide an appropriate InteractionFacetFactory",
            ii);

         return;
      }

      // For each facet, try to match up a facet_params that matches
      // them.

      FacetParams* facet_params = NULL;

      for (unsigned int jj = 0; jj < params.size(); ++jj) {

         if (surface->facets[ii]->param_name == NULL) {
            MessageHandler::fail (
               __FILE__, __LINE__,
               SurfaceModelMessages::initialization_error,
               "A Facet was found with no param_name set.");
         }

         if (strcmp (params[jj]->name.c_str(), surface->facets[ii]->param_name)
             == 0) {
            facet_params = params[jj];
            break;
         }
      }

      if (facet_params == NULL) {

         MessageHandler::fail (
            __FILE__, __LINE__,
            SurfaceModelMessages::initialization_error,
            "No facet_params with name %s was found.",
            surface->facets[ii]->param_name);

      }


      inter_surface->allocate_interaction_facet (surface->facets[ii],
                                                 facet_factory,
                                                 facet_params,
                                                 ii);
   } // for(unsigned int ii = 0)
   return;
}

/**
 * Used to add an interaction facet factory for use in the
 * surface factory
 * \param[in] to_add The interaction facet factory to add
 */

void
InteractionSurfaceFactory::add_facet_factory (
   InteractionFacetFactory* to_add)
{
   if (to_add == NULL) {
      MessageHandler::fail (
         __FILE__, __LINE__, SurfaceModelMessages::setup_error,
         "A NULL pointer was given to the add_facet_factory function. "
         "This is not allowed, and a failure has happened.\n");
      return;
   }
   factories.push_back (to_add);
   return;

}

/**
 * Add a set of facet parameters for use in the interaction
 * surface factory
 * \param[in] to_add The facet parameters to add
 */

void
InteractionSurfaceFactory::add_facet_params (
   FacetParams* to_add)
{
   if (to_add == NULL) {
      MessageHandler::fail (
         __FILE__, __LINE__, SurfaceModelMessages::setup_error,
         "A NULL pointer was given to the add_facet_params function. "
         "This is not allowed, and a failure has happened.\n");
      return;
   }

   NamedItem::validate_name(__FILE__, __LINE__, to_add->name.c_str(),
                            "Name of the added FacetParams",
                            "to_add");

   params.push_back (to_add);

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
