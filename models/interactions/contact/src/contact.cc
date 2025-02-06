/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup Contact
 * @{
 *
 * @file models/interactions/contact/src/contact.cc
 * Base Contact for use with contact interaction model
 */

/*****************************************************************************

 Purpose:
    ()

 Reference:
   (((Derived from OTCM model developed by Hung Nguyen L3-COM)))

 Assumptions and Limitations:
     ((N/A))

 Library dependencies:
    ((contact.o)
     (contact_pair.o))

 
*****************************************************************************/

/* JEOD includes */
#include "dynamics/mass/include/mass.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/surface_model/include/facet.hh"

/* Model includes */
#include "../include/contact.hh"
#include "../include/contact_facet.hh"
#include "../include/contact_pair.hh"
#include "../include/pair_interaction.hh"

//! Namespace jeod
namespace jeod {

/**
 * Default Constructor
 */

Contact::Contact (
   void)
   : // Return: -- None
   active (true),
   contact_limit_factor(0.0)
{
   JEOD_REGISTER_CLASS(Contact);
   JEOD_REGISTER_CLASS(ContactPair);
   JEOD_REGISTER_CHECKPOINTABLE(this, contact_pairs);
   JEOD_REGISTER_CHECKPOINTABLE(this, pair_interactions);
}


/**
 * Destructor
 */

Contact::~Contact (
   void)
{
   std::list<ContactPair *>::iterator cp;

   for (cp = contact_pairs.begin (); cp != contact_pairs.end (); ++cp) {
      JEOD_DELETE_OBJECT((*cp));
   }

   std::list<PairInteraction *>::iterator pint;

   for (pint = pair_interactions.begin (); pint != pair_interactions.end (); ++pint) {
      if(JEOD_IS_ALLOCATED((*pint))) {
         JEOD_DELETE_OBJECT((*pint));
      }
   }

   JEOD_DEREGISTER_CHECKPOINTABLE(this, contact_pairs);
   JEOD_DEREGISTER_CHECKPOINTABLE(this, pair_interactions);

}


/**
 * iterate through contact pairs list then call the
 * appropriate contact resolution functions
 */
void
Contact::check_contact (
   void)
{

   if (active) {
      std::list<ContactPair *>::iterator cp;

      for (cp = contact_pairs.begin (); cp != contact_pairs.end (); ++cp) {
         if ((*cp)->is_complete() && (*cp)->is_active() && (*cp)->in_range()) {
            (*cp)->in_contact();
         }
      }
   }

   return;
}

/**
 * Initialize ContactFacets and the mananger by cleaning up the pair list
 * \param[in,out] manager Dynamics Manager
 */
void
Contact::initialize_contact (
   DynManager * manager)
{
   ContactPair * pair;

   dyn_manager = manager;

   std::list<ContactPair *>::iterator sub;
   std::list<ContactPair *>::iterator tar;

   for (sub = contact_pairs.begin (); sub != contact_pairs.end (); ++sub) {
      if (!(*sub)->is_complete()) {
         for (tar = contact_pairs.begin (); tar != contact_pairs.end (); ++tar) {
            if ((*sub)->get_subject() != (*tar)->get_subject() && unique_pair((*sub)->get_subject(), (*tar)->get_subject())) {
               pair = (*sub)->get_subject()->create_pair((*tar)->get_subject(), this);
               if (pair == NULL) {
                  pair = (*tar)->get_subject()->create_pair((*sub)->get_subject(), this);
               }
               if (pair != NULL) {
                  pair->initialize_relstate(dyn_manager);
                  contact_pairs.push_back(pair);
               }
            }
         }
      }
   }
   return;
}

/**
 * Check to see if a pair of facets already exists.
 * @return bool
 * \param[in,out] facet_1 ContactFacet
 * \param[in,out] facet_2 ContactFacet
 */
bool
Contact::unique_pair (
   ContactFacet * facet_1,
   ContactFacet * facet_2)
{
   std::list<ContactPair *>::iterator cp;

   for (cp = contact_pairs.begin (); cp != contact_pairs.end (); ++cp) {
      if(((*cp)->get_subject() == facet_1 && (*cp)->get_target() == facet_2)
        || ((*cp)->get_subject() == facet_2 && (*cp)->get_target() == facet_1)) {
         return false;
      }
   }

   return true;
}


/**
 * Register one ContactFacet with all inclusive interactions
 * with other registered ContactFacets.
 * \param[in,out] facet ContactFacet
 */
void
Contact::register_contact (
   ContactFacet * facet)
{
   ContactPair * pair;

   pair = facet->create_pair();
   if(pair != NULL) {
      contact_pairs.push_back (pair);
   }

   return;
}


/**
 * Register an array of ContactFacets with all inclusive
 * interactions with other registered ContactFacets.
 * \param[in,out] facets array of ContactFacets
 * \param[in] nFacets number of ContactFacets in array
 */
void
Contact::register_contact (
   ContactFacet ** facets,
   unsigned int nFacets)
{
   for (unsigned int i = 0; i < nFacets; i++) {
      register_contact (facets[i]);
   }

   return;
}


/**
 * Register two facets as a pair.
 * \param[in,out] facet1 Contact Facet 1
 * \param[in,out] facet2 Contact Facet 2
 */
void
Contact::register_contact (
   ContactFacet * facet1,
   ContactFacet * facet2)
{
   ContactPair * pair;
   pair = facet1->create_pair(facet2, this);
   if (pair == NULL) {
      pair = facet2->create_pair(facet1, this);
   }
   if(pair != NULL) {
      (pair)->initialize_relstate(dyn_manager);
      contact_pairs.push_back (pair);
   }

   return;
}


/**
 * Regiser to arrays of facets and create specific pairs between all of
 * them.
 * \param[in,out] facets1 array of ContactFacets
 * \param[in] nFacets1 number of ContactFacets in array
 * \param[in,out] facets2 array of ContactFacets
 * \param[in] nFacets2 number of ContactFacets in array
 */
void
Contact::register_contact (
   ContactFacet ** facets1,
   unsigned int nFacets1,
   ContactFacet ** facets2,
   unsigned int nFacets2)
{
   unsigned int i, j;

   for (i = 0; i < nFacets1; i++) {
      for (j = 0; j < nFacets2; j++) {
         register_contact (facets1[i], facets2[j]);
      }
   }

   return;
}

/**
 * Register a pair interaction.
 * \param[in] interaction PairInteraction to add to list
 */
void
Contact::register_interaction (
   PairInteraction * interaction)
{
   pair_interactions.push_back (interaction);

   return;
}

/**
 * find a PairInteraction baced on a set of ContactParams.
 * @return pointer to a PairInteraction
 * \param[in] params_1 ContactParams from a ContactFacet
 * \param[in] params_2 ContactParams from a ContactFacet
 */
PairInteraction *
Contact::find_interaction (
   ContactParams * params_1,
   ContactParams * params_2)
{
   std::list<PairInteraction *>::iterator pint;

   for (pint = pair_interactions.begin (); pint != pair_interactions.end (); ++pint) {
      if ((*pint)->is_correct_interaction(params_1, params_2)) {
         return (*pint);
      }
   }

   return NULL;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
