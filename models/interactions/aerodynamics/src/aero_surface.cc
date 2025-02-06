/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup Aerodynamics
 * @{
 *
 * @file models/interactions/aerodynamics/src/aero_surface.cc
 * Vehicle surface model for the aerodyanmic interaction models
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((None))

Library dependencies:
    ((aero_surface.cc)
     (aero_facet.cc)
     (aerodynamics_messages.cc)
     (utils/sim_interface/src/memory_interface.cc)
     (utils/message/src/message_handler.cc)
     (utils/surface_model/src/facet.cc)
     (utils/surface_model/src/interaction_surface.cc)
     (utils/surface_model/src/interaction_facet_factory.cc))


*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/surface_model/include/interaction_facet_factory.hh"
#include "utils/surface_model/include/facet.hh"

// Model includes
#include "../include/aero_surface.hh"
#include "../include/aero_facet.hh"
#include "../include/aerodynamics_messages.hh"



//! Namespace jeod
namespace jeod {

/**
 * Default Constructor
 */

AeroSurface::AeroSurface (
   void)
: // Return: -- void
   aero_facets(nullptr),
   facets_size(0)
{
   JEOD_REGISTER_CLASS(AeroSurface);
   JEOD_REGISTER_INCOMPLETE_CLASS(AeroFacet);
}

/**
 * Destructor
 */

AeroSurface::~AeroSurface (
   void)
{

   if (aero_facets != nullptr) {

      for (unsigned int ii = 0; ii < facets_size; ++ii) {
         if (aero_facets[ii] != nullptr) {
            JEOD_DELETE_OBJECT (aero_facets[ii]);
         }
      }

      JEOD_DELETE_ARRAY (aero_facets);

   }

}

/**
 * Allocates an array of AeroFacet pointers, of the
 * size indicated by the input variable
 * \param[in] size The size of the needed array\n Units: cnt:
 */

void
AeroSurface::allocate_array (
   unsigned int size)
{

   if (aero_facets != nullptr) {

      MessageHandler::fail (
         __FILE__, __LINE__, AerodynamicsMessages::initialization_error,
         "AeroSurfade::allocate_array can not continue as the array "
         "aero_facets has already been allocated.");

      return;
   }

   // Allocate the array we want, and set the size
   if (size == 0) {
      MessageHandler::warn(
         __FILE__,__LINE__, AerodynamicsMessages::initialization_error,
         "AeroSurfade::allocate_array called for a surface with no declared\n"
         "facets.  Check configuration if not using a default surface.\n"
         "aero-facets array not allocated.\n");
      facets_size = 0;
      return;
   }

   aero_facets = JEOD_ALLOC_CLASS_POINTER_ARRAY (size, AeroFacet);
   facets_size = size;

   // Make sure all pointers are NULL so destructor never crashes
   for (unsigned int ii = 0; ii < facets_size; ++ii) {
      aero_facets[ii] = nullptr;
   }

   return;

}

/**
 * Allocates a particular interaction facet, from an inputted general
 * facet, using the inputted parameters and interaction facet
 * factory. This facet is then placed at the index given. If the
 * correct InteractionFacetFactory and Facet Params are not given
 * for the aerodynamic interaction or for the type of facet given,
 * a fail message will be sent
 * \param[in] facet The basic facet used to create the interaction facet
 * \param[in] factory The factory used to create the interaction facet
 * \param[in] params The aero params used to create the interaction facet
 * \param[in] index Where the new interaction facet will be placed in the aero_facets array\n Units: cnt
 */

void
AeroSurface::allocate_interaction_facet (
   Facet* facet,
   InteractionFacetFactory* factory,
   FacetParams* params,
   unsigned int index)
{
   if (facets_size <= index) {


      MessageHandler::fail (
         __FILE__, __LINE__, AerodynamicsMessages::initialization_error,
         "AeroSurface::allocate_interaction_facet was asked to allocate "
         "from the Facet at array index %d. This is out of bounds "
         "of the array of facets, which is %d long",
         index, facets_size);

      return;

   }

   /* need to temporarily save off the InteractionFacet returned before
      dynamic casting it. If the dynamic cast fails, we want to destroy
      the InteractionFacet so we don't get a memory leak */

   InteractionFacet* temp_facet = nullptr;

   // attempt to create the facet
   temp_facet = factory->create_facet (facet, params);

   // if the facet is NULL, then we have a problem
   if (temp_facet == nullptr) {

      MessageHandler::fail (
         __FILE__, __LINE__, AerodynamicsMessages::initialization_error,
         "There was an error when attempting to create an interaction "
         "facet for the facet found at array index %d",
         index);

   }

   // Facet is currently an interaction facet. Try to make it an AeroFacet

   AeroFacet* temp_aero_facet = dynamic_cast<AeroFacet*> (temp_facet);


   // If that fails, it doesn't belong in this surface so there is a problem
   if (temp_aero_facet == nullptr) {

      // temp_facet can NOT be NULL, since it was already checked for above
      JEOD_DELETE_OBJECT (temp_facet);

      MessageHandler::fail (
         __FILE__, __LINE__, AerodynamicsMessages::initialization_error,
         "The InteractionFacet created from the Facet found at array "
         "index %d was not of a type that inherits from AeroFacet. "
         "This is required",
         index);


      return;

   }

   // Store the aero_facet into the aero_facets array
   aero_facets[index] = temp_aero_facet;

   return;

}


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
