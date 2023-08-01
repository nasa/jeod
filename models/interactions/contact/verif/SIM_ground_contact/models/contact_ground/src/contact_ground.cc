/*****************************************************************************

 Purpose:
    (Base Contact for use with contact interaction model)

 Reference:
   (((Derived from OTCM model developed by Hung Nguyen L3-COM)))

 Assumptions and Limitations:
     ((N/A))

 Library dependencies:
    ((contact_ground.cc)
     (ground_interaction.cc))

 

*****************************************************************************/

/* JEOD includes */
#include "dynamics/mass/include/mass.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/surface_model/include/facet.hh"
#include "interactions/contact/include/contact_pair.hh"
#include "interactions/contact/include/pair_interaction.hh"
#include "interactions/contact/include/contact_facet.hh"
#include "environment/planet/include/planet.hh"

/* Model includes */
#include "../include/contact_ground.hh"


//! Namespace jeod
namespace jeod {

/*******************************************************************************
  Function: ContactGround
  Purpose: (Default Constructor)
*******************************************************************************/

ContactGround::ContactGround (
   void)
   : // Return: -- None
   ground_active (true),
   planet (nullptr),
   terrain (nullptr)
{
   JEOD_REGISTER_CLASS(ContactGround);
   JEOD_REGISTER_CLASS(GroundInteraction);
   JEOD_REGISTER_CLASS(GroundFacet);
   JEOD_REGISTER_CHECKPOINTABLE(this, ground_interactions);
   JEOD_REGISTER_CHECKPOINTABLE(this, ground_facets);
}


/*******************************************************************************
  Function: ~ContactGround
  Purpose: (Destructor)
*******************************************************************************/

ContactGround::~ContactGround ( // Return: -- void
   void)
{
   std::list<GroundInteraction *>::iterator gi;

   for (gi = ground_interactions.begin (); gi != ground_interactions.end (); ++gi) {
      if (JEOD_IS_ALLOCATED (*gi)) {
         JEOD_DELETE_OBJECT((*gi));
      }
   }

   std::list<GroundFacet *>::iterator gf;

   for (gf = ground_facets.begin (); gf != ground_facets.end (); ++gf) {
      if (JEOD_IS_ALLOCATED (*gf)) {
         JEOD_DELETE_OBJECT((*gf));
      }
   }

   JEOD_DEREGISTER_CHECKPOINTABLE(this, ground_interactions);
   JEOD_DEREGISTER_CHECKPOINTABLE(this, ground_facets);
}


/*******************************************************************************
Function: check_contact_ground
Purpose: (iterate through ground interaction list then call the
   appropriate contact resolution functions)
*******************************************************************************/
void
ContactGround::check_contact_ground ( /* Return: -- Void */
   void)
{
   if (ground_active) {
      std::list<GroundInteraction *>::iterator gi;

      for (gi = ground_interactions.begin (); gi != ground_interactions.end (); ++gi) {
         if ((*gi)->is_active() && (*gi)->in_range()) {
            (*gi)->in_contact();
         }
      }
   }

   return;
}

/*******************************************************************************
 Function: get_ground_interaction
 Purpose: (Get the ground_interaction for a specific ContactFacet.)
 *******************************************************************************/
GroundInteraction *
ContactGround::get_ground_interaction ( /* Return: -- ground interaction */
   ContactFacet * facet)
{
   std::list<GroundInteraction *>::iterator gi;

   for (gi = ground_interactions.begin (); gi != ground_interactions.end (); ++gi) {
      if ((*gi)->get_subject() == facet) {
         return (*gi);
      }
   }

   return nullptr;
}

/*******************************************************************************
 Function: is_unique_ground_interaction
 Purpose: (Check if a contact facet already has an GroundInterface defined)
 *******************************************************************************/
bool
ContactGround::is_unique_ground_interaction ( /* Return: -- bool */
   ContactFacet * facet)
{
   std::list<GroundInteraction *>::iterator gi;

   if (facet == nullptr) {
      return false;
   }

   for (gi = ground_interactions.begin (); gi != ground_interactions.end (); ++gi) {
      if ((*gi)->get_subject() == facet) {
         return false;
      }
   }

   return true;
}

/*******************************************************************************
 Function: initialize_ground
 Purpose: (create the ground interfaces list)
 Class: (initialization)
 *******************************************************************************/
void
ContactGround::initialize_ground ( /* Return: -- Void */
   Planet * planet_in,     // In: -- planet
   Terrain * terrain_in)   // In: -- terrain
{
   GroundInteraction * ground_interaction;

   planet = planet_in;

   terrain = terrain_in;

   std::list<ContactPair *>::iterator cp;

   std::list<GroundFacet *>::iterator gf;

   for (gf = ground_facets.begin (); gf != ground_facets.end (); ++gf) {
      for (cp = contact_pairs.begin (); cp != contact_pairs.end (); ++cp) {
         if (is_unique_ground_interaction((*cp)->get_subject())) {
            ground_interaction = (*gf)->create_interaction((*cp)->get_subject(), this);
            if (ground_interaction != nullptr) {
               ground_interaction->initialize(terrain);
	            ground_interactions.push_back(ground_interaction);
            }
         }
         if ((*cp)->is_complete() && is_unique_ground_interaction((*cp)->get_target())) {
            ground_interaction = (*gf)->create_interaction((*cp)->get_target(), this);
            if (ground_interaction != nullptr) {
               ground_interaction->initialize(terrain);
	            ground_interactions.push_back(ground_interaction);
            }
         }
      }
   }

   return;
}

/*******************************************************************************
 Function: register_ground_facet
 Purpose: (Register a GroundFacet with the ContactGround class.)
 Class: (initialization)
 *******************************************************************************/
void
ContactGround::register_ground_facet(  /* Return: -- Void */
   GroundFacet * ground_facet)  // In: -- ground facet
{
   ground_facets.push_back(ground_facet);
}
} // End JEOD namespace
