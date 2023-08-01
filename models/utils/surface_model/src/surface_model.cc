/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup SurfaceModel
 * @{
 *
 * @file models/utils/surface_model/src/surface_model.cc
 * Vehicle surface model for general environment interaction models
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((None))

Library dependencies:
    ((surface_model.cc)
     (surface_model_messages.cc)
     (utils/message/src/message_handler.cc))

 
*******************************************************************************/

// System includes
#include <cstddef>
#include <algorithm>

// JEOD includes
#include "dynamics/dyn_manager/include/base_dyn_manager.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "dynamics/mass/include/mass.hh"

#include "utils/named_item/include/named_item.hh"

// Model includes
#include "../include/surface_model.hh"
#include "../include/surface_model_messages.hh"
#include "../include/facet.hh"


//! Namespace jeod
namespace jeod {

/**
 * Default constructor
 */

SurfaceModel::SurfaceModel (
   void)
:
   articulation_active(false),
   struct_body_name(nullptr),
   struct_body_ptr(nullptr)
{

   JEOD_REGISTER_CLASS(SurfaceModel);
   JEOD_REGISTER_CLASS(Facet);
   JEOD_REGISTER_CLASS(FacetStateInfo);
   JEOD_REGISTER_CHECKPOINTABLE(this, facets);
   JEOD_REGISTER_CHECKPOINTABLE(this, articulation_states);

}

/**
 * Destructor
 */

SurfaceModel::~SurfaceModel (
   void)
{
   JEOD_DEREGISTER_CHECKPOINTABLE(this, facets);
   JEOD_DEREGISTER_CHECKPOINTABLE(this, articulation_states);
}

/*******************************************************************************
  function: add_facets
  purpose: (copies an array of facet pointers into the surface model)
*******************************************************************************/

void
SurfaceModel::add_facets (
   Facet** new_facets,
   unsigned int num_new_facets)
{

   // check for a NULL pointer being sent in
   if (new_facets == nullptr) {
      MessageHandler::warn (
         __FILE__, __LINE__, SurfaceModelMessages::setup_error,
         "A NULL pointer was given to the add_facets function. "
         "No facets have been added.\n");
      return;
   }

   for (unsigned int ii = 0; ii < num_new_facets; ++ii) {
      if (new_facets[ii] == nullptr) {
         MessageHandler::warn (
            __FILE__, __LINE__, SurfaceModelMessages::setup_error,
            "A NULL pointer was given to the add_facets function "
            "at slot %d in new_facets. No facet was added "
            " for this slot.\n", ii);
      }
      facets.push_back (new_facets[ii]);
   }

   return;

}

/*******************************************************************************
  function: add_facet
  purpose: (copies a single facet pointer into the surface model.)
*******************************************************************************/

void
SurfaceModel::add_facet (
   Facet* new_facet)
{

   if(new_facet == nullptr) {
      MessageHandler::fail (
         __FILE__, __LINE__, SurfaceModelMessages::setup_error,
         "A NULL pointer was given to the add_facet function. "
         "This is not allowed, and a failure has happened.\n");
      return;
   }
   else {
      facets.push_back(new_facet);
   }

   return;

}

/*******************************************************************************
  function: initialize_mass_connections
  purpose: (Initialize mass connections in all contained facets)
*******************************************************************************/

void
SurfaceModel::initialize_mass_connections (
   BaseDynManager& manager)
{

   NamedItem::validate_name(__FILE__, __LINE__, struct_body_name,
                            "Structure Name", "struct_body_name");

   struct_body_ptr = manager.find_mass_body(struct_body_name);

   if(struct_body_ptr == nullptr) {
      MessageHandler::fail (
      __FILE__, __LINE__, SurfaceModelMessages::initialization_error,
      "In SurfaceModel::initialize_mass_connections, no mass body was found, "
      "in the given DynManager, with the name %s. "
      "Initialization "
      "has failed.\n", struct_body_name);
   }

   // Each facet is attached to a mass body. for articulation, we need the
   // relative position and orientation of that mass body w.r.t. the
   // user defined "main" mass body. If many facets are attached to
   // the same mass body, we don't want to re-calculate the same
   // relative states over and over again, so for each "attached to"
   // mass body we see, create a new FacetStateInfo for it.

   // create an iterator to search the vector of FacetStateInfos.
   std::list<FacetStateInfo>::iterator it;
   for(unsigned int ii = 0; ii < facets.size(); ++ii){
      facets[ii]->initialize_mass_connection(manager);

      // the facet should now have something in it's mass body ptr.
      // search for it in the vector of FacetStateInfos. If it doesn't
      // appear, create a new one for that mass body.

      it = find(articulation_states.begin(),
                articulation_states.end(),
                FacetStateInfo(facets[ii]->get_mass_body_ptr() ) );

      if(it == articulation_states.end()) {
         articulation_states.push_back(
            FacetStateInfo(facets[ii]->get_mass_body_ptr() ));
         facets[ii]->mass_rel_struct = &articulation_states.back().mass_state;
      }
      else {
         facets[ii]->mass_rel_struct = &(it->mass_state);
      }

   } // for(unsigned int ii)



   return;

}

/*******************************************************************************
  function: update_articulation
  purpose: (update the global zeroth order state, in the vehicle structural
            frame, of the facets in this surface model)
*******************************************************************************/

void
SurfaceModel::update_articulation (
   void)
{
   // check if the articulation is active. If not, automatically return
   if(!articulation_active){
      return;
   }

   // update all of the MassPointStates we have knowledge of.

   // for(unsigned int ii = 0; ii < articulation_states.size(); ++ii) {
   for(std::list<FacetStateInfo>::iterator ii = articulation_states.begin();
       ii != articulation_states.end();
       ++ii) {

      FacetStateInfo& current_state = *ii;

      if(struct_body_ptr->get_root_body() !=
         current_state.mass_body->get_root_body())
      {

         MessageHandler::fail (
            __FILE__, __LINE__, SurfaceModelMessages::runtime_error,
            "The function SurfaceModel::update_articulation was asked to take "
            "the relative state between MassPoints %s and %s. These "
            "points are not contained within the same mass tree, and "
            "thus update_articulation has failed\n",
struct_body_name, current_state.mass_body->name.c_str());

         return;
      }

      current_state.mass_body->structure_point.compute_relative_state(
         struct_body_ptr->structure_point, current_state.mass_state);

   }

   for(unsigned int ii = 0; ii < facets.size(); ++ii){
      facets[ii]->update_articulation();
   } // for(unsigned int ii)

   return;

}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
