//=============================================================================
// Notices:
//
// Copyright © 2023 United States Government as represented by the Administrator
// of the National Aeronautics and Space Administration.  All Rights Reserved.
//
//
// Disclaimers:
//
// No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF
// ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED
// TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO SPECIFICATIONS, ANY
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
// FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL BE ERROR
// FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO THE
// SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN
// ENDORSEMENT BY GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS,
// RESULTING DESIGNS, HARDWARE, SOFTWARE PRODUCTS OR ANY OTHER APPLICATIONS
// RESULTING FROM USE OF THE SUBJECT SOFTWARE.  FURTHER, GOVERNMENT AGENCY
// DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING THIRD-PARTY SOFTWARE,
// IF PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT "AS IS."
//
// Waiver and Indemnity:  RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS AGAINST THE
// UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
// PRIOR RECIPIENT.  IF RECIPIENT'S USE OF THE SUBJECT SOFTWARE RESULTS IN ANY
// LIABILITIES, DEMANDS, DAMAGES, EXPENSES OR LOSSES ARISING FROM SUCH USE,
// INCLUDING ANY DAMAGES FROM PRODUCTS BASED ON, OR RESULTING FROM, RECIPIENT'S
// USE OF THE SUBJECT SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE
// UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
// PRIOR RECIPIENT, TO THE EXTENT PERMITTED BY LAW.  RECIPIENT'S SOLE REMEDY FOR
// ANY SUCH MATTER SHALL BE THE IMMEDIATE, UNILATERAL TERMINATION OF THIS
// AGREEMENT.
//
//=============================================================================
//
//
/**
 * @addtogroup Models
 * @{
 * @addtogroup Interactions
 * @{
 * @addtogroup Contact
 * @{
 *
 * @file models/interactions/contact/include/contact.hh
 * (Base class to for the contact manager for use with contact interaction model)
 */

/*****************************************************************************

 Purpose:
    ()

 Reference:
   (((Derived from OTCM model developed by Hung Nguyen L3-COM)))

 Assumptions and Limitations:
     ((N/A))

 Library dependencies:
    ((../src/contact.cc))

 

*****************************************************************************/

#ifndef CONTACT_HH
#define CONTACT_HH

// System includes
#include <list>

/* JEOD includes */
#include "dynamics/dyn_manager/include/class_declarations.hh"
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/container/include/pointer_list.hh"

// Model includes
#include "class_declarations.hh"
#include "contact_facet.hh"
#include "contact_pair.hh"
#include "pair_interaction.hh"

//! Namespace jeod
namespace jeod {

/**
 * An base contact class for use in the surface model.
 */
class Contact {

   JEOD_MAKE_SIM_INTERFACES(Contact)

public:
   /**
    * toggles contact on and off, true=on false=off
    */
   bool active; //!< trick_units(--)

   /**
    * factor determines if contact is limited by a muliple of the
    * maximum dimensions of the facets in a pair.
    */
   double contact_limit_factor; //!< trick_units(--)

   // constructor
   Contact ();

   // destructor
   virtual ~Contact ();

    /*
    The registration functions.
    Registrations accepts only the base class and type casts as necessary
    to create maps for the possible interactions.
    */
   // Single facet with no stipulations as to what it interacts with.
   void register_contact (ContactFacet * facet);
   // Array of ContactFacets with no stipulation as to what they interact with.
   void register_contact (ContactFacet ** facets, unsigned int n_facets);
   // Pair of facets each will only interact with the other.
   void register_contact (ContactFacet * facet1, ContactFacet * facet2);
   // Two arrays of ContactFacets that will only interact with each other.
   void register_contact (
      ContactFacet ** facets1,
      unsigned int n_facets1,
      ContactFacet ** facets2,
      unsigned int n_facets2);

   // place a pair interaction on the list.
   void register_interaction (PairInteraction * interaction);

   // find the correct pair interaction based on a set of contact params.
   virtual PairInteraction * find_interaction(
      ContactParams *params_1,
      ContactParams *params_2);

   /*
    Clean up registrations and create all necessary lists.
    */
   void initialize_contact (DynManager * manager);

   /*
    Check contact pair list for duplicates before making a new pair.
    */
   bool unique_pair ( const ContactFacet * facet_1, const ContactFacet * facet_2);

   /*
    Function to check for contact.  Loops through all the created pairs.
    */
   void check_contact ();

protected:
   /**
    * Pointer to the dyn_manager so relstates and be successfully initialized.
    */
   DynManager * dyn_manager; //!< trick_units(--)

   /**
    * list of all possible pairings of contact facets registered with this contact
    * class or derived class
    */
  JeodPointerList<ContactPair>::type contact_pairs; //!< trick_io(**)

   /**
    * list of all possible pair interaction types
    */
   JeodPointerList<PairInteraction>::type pair_interactions; //!< trick_io(**)


private:
   /* Operator = and copy constructor hidden from use by being private */

   Contact& operator = (const Contact& rhs);
   Contact (const Contact& rhs);

};

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
