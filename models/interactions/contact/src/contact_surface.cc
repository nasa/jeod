/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup Contact
 * @{
 *
 * @file models/interactions/contact/src/contact_surface.cc
 * Vehicle surface model for the contact interaction models
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((None))

Library dependencies:
    ((contact_surface.cc)
     (contact_messages.cc)
     (contact_facet.cc))


*******************************************************************************/

/* System includes */
#include <typeinfo>
#include <cstddef>

/* JEOD includes */
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "utils/math/include/vector3.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/surface_model/include/facet.hh"
#include "utils/surface_model/include/interaction_facet_factory.hh"

/* Model includes */
#include "../include/contact_surface.hh"
#include "../include/contact_messages.hh"
#include "../include/contact_facet.hh"

//! Namespace jeod
namespace jeod {

JEOD_DECLARE_ATTRIBUTES (ContactFacet)


/**
 * Default Constructor
 */

ContactSurface::ContactSurface (
   void)
   : // Return: -- void
   contact_facets (nullptr),
   facets_size (0)
{
   JEOD_REGISTER_CLASS(ContactSurface);
}


/**
 * Destructor
 */

ContactSurface::~ContactSurface (
   void)
{

   if (contact_facets != nullptr) {

      for (unsigned int ii = 0; ii < facets_size; ++ii) {
         if (contact_facets[ii] != nullptr) {
            JEOD_DELETE_OBJECT (contact_facets[ii]);
         }
      }

      JEOD_DELETE_ARRAY (contact_facets);

   }

}


/**
 * Allocates an array of ContactFacet pointers, of the
 * size indicated by the input variable
 * \param[in] size The size of the needed array\n Units: cnt:
 */

void
ContactSurface::allocate_array (
   unsigned int size)
{

   if (contact_facets != nullptr) {

      MessageHandler::fail (
         __FILE__, __LINE__, ContactMessages::initialization_error,
         "ContactSurfade::allocate_array can not continue as the array "
         "contact_facets has already been allocated.");

      return;
   }

   // Allocate the array we want, and set the size
   contact_facets = JEOD_ALLOC_CLASS_POINTER_ARRAY (size, ContactFacet);
   facets_size    = size;

   // Make sure all pointers are NULL so destructor never crashes
   for (unsigned int ii = 0; ii < facets_size; ++ii) {
      contact_facets[ii] = nullptr;
   }

   return;

}


/**
 * Creates an interaction facet or more accurately a contact
 * facet from a basic facet and set of parameters
 * \param[in] facet The basic facet used to create the interaction facet
 * \param[in] factory The factory used to create the interaction facet
 * \param[in] params The contact params used to create the interaction facet
 * \param[in] index Where the new interaction facet will be placed in the contact_facets array\n Units: cnt
 */

void
ContactSurface::allocate_interaction_facet (
   Facet* facet,
   InteractionFacetFactory* factory,
   FacetParams* params,
   unsigned int index)
{
   if (facets_size <= index) {


      MessageHandler::fail (
         __FILE__, __LINE__, ContactMessages::initialization_error,
         "ContactSurface::allocate_interaction_facet was asked to allocate "
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
         __FILE__, __LINE__, ContactMessages::initialization_error,
         "There was an error when attempting to create an interaction "
         "facet for the facet found at array index %d",
         index);

   }

   // Facet is currently an interaction facet. Try to make it an ContactFacet

   ContactFacet* temp_contact_facet = dynamic_cast<ContactFacet*> (temp_facet);


   // If that fails, it doesn't belong in this surface so there is a problem
   if (temp_contact_facet == nullptr) {

      // temp_facet can NOT be NULL, since it was already checked for above
      JEOD_DELETE_OBJECT (temp_facet);

      MessageHandler::fail (
         __FILE__, __LINE__, ContactMessages::initialization_error,
         "The InteractionFacet created from the Facet found at array "
         "index %d was not of a type that inherits from ContactFacet. "
         "This is required",
         index);


      return;

   }

   // Store the contact_facet into the contact_facets array
   contact_facets[index] = temp_contact_facet;

   return;

}


/**
 * collect the forces and torques from all the facets in this contact
 * surface
 */

void
ContactSurface::collect_forces_torques ( void )
{
   Vector3::initialize (contact_force);
   Vector3::initialize (contact_torque);


   for (unsigned int i = 0; i < facets_size; i++) {
      Vector3::sum (contact_facets[i]->force, contact_force, contact_force);

      Vector3::sum (contact_facets[i]->torque, contact_torque, contact_torque);

      Vector3::initialize (contact_facets[i]->force);
      Vector3::initialize (contact_facets[i]->torque);

   }

   Vector3::zero_small (1e-10, contact_force);
   Vector3::zero_small (1e-10, contact_torque);
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
