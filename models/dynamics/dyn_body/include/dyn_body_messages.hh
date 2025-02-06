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
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynBody
 * @{
 *
 * @file models/dynamics/dyn_body/include/dyn_body_messages.hh
 * Define the class DynBodyMessages.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((dyn_body_messages.o))



*******************************************************************************/


#ifndef JEOD_DYNBODY_MESSAGES_HH
#define JEOD_DYNBODY_MESSAGES_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"


//! Namespace jeod
namespace jeod {

/**
 * Specify the message IDs used in the DynBody model.
 *
 * \par Assumptions and Limitations
 *  - This is a complete catalog of all the messages sent by the DynBody model.
 *  - This is not an exhaustive list of all the things that can go awry.
 */
class DynBodyMessages {

 JEOD_MAKE_SIM_INTERFACES(DynBodyMessages)


 // Static member data

 public:

   /**
    * Issued when a body is invalid such as not being initialized.
    */
   static char const * invalid_body; //!< trick_units(--)

   /**
    * Issued when a group is invalid such as not initialized or NULL.
    */
   static char const * invalid_group; //!< trick_units(--)

   /**
    * Issued when a name is invalid -- NULL, empty, a duplicate, ...
    */
   static char const * invalid_name; //!< trick_units(--)

   /**
    * Issued when a frame is invalid -- not an integ frame, ...
    */
   static char const * invalid_frame; //!< trick_units(--)

   /**
    * Issued when a attachment is invalid from a state point of view.
    */
   static char const * invalid_attachment; //!< trick_units(--)

   /**
    * Issued when an integration technique is invalid.
    */
   static char const * invalid_technique; //!< trick_units(--)

   /**
    * Issued when a MassBody is expected to be a DynBody but that is not
    * the case.
    */
   static char const * not_dyn_body; //!< trick_units(--)

   /**
    * Error issued when some internal error occurred.
    * These errors should never happen.
    */
   static char const * internal_error; //!< trick_units(--)


 // Member functions

 // This class is not instantiable.
 // The constructors and assignment operator for this class are declared
 // private and are not implemented.
 private:

   DynBodyMessages (void);
   DynBodyMessages (const DynBodyMessages &);
   DynBodyMessages & operator= (const DynBodyMessages &);

};

} // End JEOD namespace


#endif

/**
 * @}
 * @}
 * @}
 */
