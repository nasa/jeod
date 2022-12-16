//=============================================================================
// Notices:
//
// Copyright © 2022 United States Government as represented by the Administrator
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
 * @file models/interactions/contact/include/contact_pair.hh
 * Base class for pair of contact facets for use with contact interaction model
 */

/*****************************************************************************

 Purpose:
 ()

 Reference:
 (((Derived from OTCM model developed by Hung Nguyen L3-COM)))

 Assumptions and Limitations:
 ((N/A))

 Library dependencies:
 ((contact_pair.o))


 *****************************************************************************/

#ifndef CONTACT_PAIR_HH
#define CONTACT_PAIR_HH

/* JEOD includes */
#include "dynamics/derived_state/include/relative_derived_state.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "class_declarations.hh"
#include "contact_facet.hh"

//! Namespace jeod
namespace jeod {

/**
 * An base contact pair class for use in the contact model.
 */
class ContactPair {

   JEOD_MAKE_SIM_INTERFACES(ContactPair)

public:
   /**
    * Parameters that define the force calculation function between the subjec and target
    */
   PairInteraction *interaction; //!< trick_units(--)

   /**
    * rel_state distance at which in_contact should be called
    */
   double interaction_distance; //!< trick_units(m)

   // constructor
   ContactPair();

   // destructor
   virtual ~ContactPair();

   // test whether the pair is in range for interaction
   bool in_range();

   // check to make sure the pair is valid for contact.
   bool is_active();

   // determine if the pair is complete, target not NULL
   bool is_complete(void);

   // return the subject of the pair
   ContactFacet * get_subject(void);

   // return the target of the pair
   ContactFacet * get_target(void);

      /**
    * Virtual funtion to determine if the pair is in contact.  Contact
    * depends on specific geometry so implementation has to wait for a derived
    * class.
    */
   virtual void in_contact(
      void)
   = 0;


      /**
    * Initialize the contact pair by setting the subject, target, and creating
    * the relstate if possible.
    * \param[in,out] subject_facet subject ContactFacet
    * \param[in,out] target_facet target ContactFacet
    */
   virtual void initialize_pair (
      ContactFacet *subject_facet,
      ContactFacet *target_facet)
   = 0;

   /**
    Initialize the relative state between the facets and register with
    the dynamics manager.
    */
   virtual void initialize_relstate(DynManager * dyn_manager);

   // check the pair and make sure they are on different mass trees.
   virtual bool check_tree();

protected:
   /**
    * Current relative state between the subject and the target in the subject frame.
    */
   RelativeDerivedState rel_state;  //!< trick_units(--)

   /**
    * pointer to the contact facet that is the subject of the associated relative states.
    */
   ContactFacet * subject; //!< trick_units(--)

   /**
    * pointer to the contact facet that is the target of the associated relative states.
    */
   ContactFacet * target;  //!< trick_units(--)

private:

   /* Operator = and copy constructor hidden from use by being private */
   ContactPair& operator = (const ContactPair & rhs);
   ContactPair (const ContactPair & rhs);

};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
