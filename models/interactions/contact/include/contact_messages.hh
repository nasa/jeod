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
 * @file models/interactions/contact/include/contact_messages.hh
 * Contact message for message handling
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((None)))

Assumptions and limitations:
  ((This is a complete catalog of all the messages sent by the contact
    model.)
   (This is not an exhaustive list of all the things that can go awry.))

Class:
   (None)

Library dependencies:
  ((contact_messages.o))


*****************************************************************************/

#ifndef JEOD_CONTACT_MESSAGES_HH
#define JEOD_CONTACT_MESSAGES_HH

/* JEOD includes */
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/message/include/message_handler.hh"

//! Namespace jeod
namespace jeod {

/**
 * Messages associated with use of the contact model.
 */
class ContactMessages {


   JEOD_MAKE_SIM_INTERFACES (ContactMessages)

   // Static member data

public:

   // Errors

   /**
    * Associated with errors during initialization of the contact model
    */
   static char const * initialization_error; //!< trick_units(--)
   /**
    * Associated with errors during the runtime of the contact model
    */
   static char const * runtime_error; //!< trick_units(--)
   /**
    * Associated with errors during the setup of the system, before runtime
    */
   static char const * pre_initialization_error; //!< trick_units(--)

   // Warnings

   /**
    * Associated with warning during initialization of the contact model
    */
   static char const * initialization_warns; //!< trick_units(--)
   /**
    * Associated with warnings given at runtime
    */
   static char const * runtime_warns;     //!< trick_units(--)

   // Information

   /**
    * Associated with information given at runtime
    */
   static char const * runtime_inform;     //!< trick_units(--)

private:

   // Class is not instantiable, operator = and copy constructor are
   // hidden from use.
   ContactMessages (void);
   ContactMessages (const ContactMessages & rhs);
   ContactMessages& operator = (const ContactMessages & rhs);

};

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
