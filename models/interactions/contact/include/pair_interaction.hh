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
 * @file models/interactions/contact/include/pair_interaction.hh
 * A class to define the interaction type for a pair of contact facets.
 * This is a base class and derived classes define the force generation
 * function when contact between facets occurs.
 */

/*****************************************************************************

 Purpose:
     ()

 Reference:
   (((Derived from OTCM model developed by Hung Nguyen L3-COM)))

 Assumptions and Limitations:
     ((N/A))

Library dependencies:
    ((pair_interaction.o))

 

*****************************************************************************/

#ifndef PAIR_INTERACTION_HH
#define PAIR_INTERACTION_HH

/* JEOD includes */
#include "utils/sim_interface/include/jeod_class.hh"
#include "dynamics/derived_state/include/class_declarations.hh"

/* Model includes */
#include "../include/class_declarations.hh"

//! Namespace jeod
namespace jeod {

/**
 * Simple spring contact parameters
 */
class PairInteraction {

   JEOD_MAKE_SIM_INTERFACES (PairInteraction)

public:

   /**
    * contact param type that defines this pair interaction.
    */
   char *params_1; //!< trick_units(--)
   /**
    * contact param type that defines this pair interaction.
    */
   char *params_2; //!< trick_units(--)

   /**
    * magnitude of the friction force on the contact surfaces.
    */
   double friction_mag;  //!< trick_units(N)

   // constructor
   PairInteraction ();

   // destructor
   virtual ~PairInteraction ();

   // check a pair of contact params for a match to stored ones.
   bool is_correct_interaction(ContactParams *subject_params, ContactParams *target_params);

      /**
    * Pure virtual function that is defined to calculate forces on facets in
    * contact.
    * \param[in,out] subject subject of the relative state
    * \param[in,out] target target of the relative state
    * \param[in] rel_state relative state between subject and target in subject frame
    * \param[in] penetration_vector vector that characterises the interpenetration of the subject and the target
    * \param[in] rel_velocity relative velocity of the subject and the target in the subject frame
    */
   virtual void calculate_forces (
      ContactFacet * subject,
      ContactFacet * target,
      RelativeDerivedState * rel_state,
      double* penetration_vector,
      double* rel_velocity)
   = 0;



private:

   // The operator = and copy constructor locked away from use by being private

   PairInteraction& operator = (const PairInteraction & rhs);
   PairInteraction (const PairInteraction & rhs);

};

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
