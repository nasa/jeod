/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup Contact
 * @{
 *
 * @file models/interactions/contact/src/line_contact_facet_factory.cc
 * Factory that creates a LineContactFacetFactory from a Cylinder facet and
 * a ContactParams object
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

Library dependencies:
   ((line_contact_facet_factory.cc)
    (line_contact_facet.cc)
    (utils/surface_model/src/cylinder.cc)
    (dynamics/dyn_body/src/dyn_body.cc)
    (contact_messages.cc))


*******************************************************************************/

/* System includes */
#include <typeinfo>
#include <cstddef>

// JEOD includes
#include "utils/surface_model/include/cylinder.hh"
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "utils/memory/include/jeod_alloc.hh"

// Model includes
#include "../include/line_contact_facet_factory.hh"
#include "../include/line_contact_facet.hh"
#include "../include/contact_messages.hh"
#include "../include/contact_params.hh"


//! Namespace jeod
namespace jeod {

JEOD_DECLARE_ATTRIBUTES (LineContactFacet)

/**
 * Default Constructor
 */

LineContactFacetFactory::LineContactFacetFactory (
   void)
{
   JEOD_REGISTER_CLASS(LineContactFacet);
}


/**
 * Destructor
 */

LineContactFacetFactory::~LineContactFacetFactory (
   void)
{

   // empty for now

}


/**
 * Create a PointContactFacet from a CircularFlatPlate facet and a
 * ContactParams object
 * @return The new EllipsoidContactFacet. Note that this is allocated and YOU are responsible for destroying it at the end!
 * \param[in] facet The CircularFlatPlate. This MUST be a circular flat plate or the algorithm will send a failure message
 * \param[in] params ContactParams
 */

InteractionFacet*
LineContactFacetFactory::create_facet (
   Facet* facet,
   FacetParams* params)
{
   ContactParams* contact_params = nullptr;
   Cylinder* cylinder          = nullptr;

   contact_params = dynamic_cast<ContactParams*> (params);
   cylinder      = dynamic_cast<Cylinder*> (facet);


   // We have tried casting the facet and params to the correct types.
   // if they were not the correct type, send out an error message
   if (contact_params == nullptr) {

      MessageHandler::fail (
         __FILE__, __LINE__,
         ContactMessages::initialization_error,
         "the params supplied to "
         "PointContactFacetFactory::create_facet was not of type "
         "ContactParams, as is required");
      return nullptr;
   }
   if (cylinder == nullptr) {

      MessageHandler::fail (
         __FILE__, __LINE__,
         ContactMessages::initialization_error,
         "the Facet supplied to "
         "PointContactFacetFactory::create_facet was not of type "
         "CircularFlatPlate, as is required");
      return nullptr;
   }

   // Create the interaction facet
   LineContactFacet* inter_facet =
      JEOD_ALLOC_CLASS_OBJECT (LineContactFacet, ());


   // Fill it out from the parameters and from the facet itself
   inter_facet->base_facet = facet;

   inter_facet->radius  = cylinder->radius;
   inter_facet->length = cylinder->length;
   inter_facet->set_max_dimension();

   inter_facet->surface_type = contact_params;

   Vector3::copy (cylinder->position, inter_facet->position);
   Vector3::copy (cylinder->normal, inter_facet->normal);


   inter_facet->vehicle_body = facet->get_mass_body_ptr()->dyn_owner;
   if (inter_facet->vehicle_body == nullptr) {

      MessageHandler::fail (
         __FILE__, __LINE__,
         ContactMessages::initialization_error,
         "mass_body_ptr contained in the facet supplied to"
         "PointContactFacetFactory::create_facet was not of type "
         "DynBody, as is required");
      return nullptr;
   }

   inter_facet->create_vehicle_point();

   return inter_facet;

}


/**
 * PointContactFacetFactory specific implementation of this function.
 * If the Facet is of type CircularFlatPlate, returns true. False otherwise
 * @return true if facet is a FlatPlateCircular, false otherwise
 * \param[in] facet The facet to check
 */

bool
LineContactFacetFactory::is_correct_factory (
   Facet* facet)
{
   if (typeid(*facet) == typeid(Cylinder)) {
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
