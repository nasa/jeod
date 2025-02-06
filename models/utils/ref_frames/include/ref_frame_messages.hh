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
 * @addtogroup Utils
 * @{
 * @addtogroup RefFrames
 * @{
 *
 * @file models/utils/ref_frames/include/ref_frame_messages.hh
 * Define the class RefFrameMessages, the class that specifies the message IDs
 * used in the reference frames model.
 */

/*******************************************************************************

Purpose:
  ()

Assumptions and limitations:
  ((This is a complete catalog of all the messages sent by the reference frames
    model.)
   (This is not an exhaustive list of all the things that can go awry.))

Library dependencies:
  ((../src/ref_frame_messages.cc))

 

*******************************************************************************/


#ifndef JEOD_REF_FRAMES_MESSAGES_HH
#define JEOD_REF_FRAMES_MESSAGES_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"


//! Namespace jeod
namespace jeod {

/**
 * Declares messages associated with the reference frames model.
 */
class RefFrameMessages {


 JEOD_MAKE_SIM_INTERFACES(RefFrameMessages)


 // Static member data
 public:
   /**
    * Issued to provide information regarding an attachment.
    */
   static char const * attach_info; //!< trick_units(--)

   /**
    * Issued when a duplicate reference frame is detected (name or address).
    */
   static char const * duplicate_entry; //!< trick_units(--)

   /**
    * Issued when some inconsistency is detected.
    */
   static char const * inconsistent_setup; //!< trick_units(--)

   /**
    * Error issued when some internal error occurred.
    * These errors should never happen.
    */
   static char const * internal_error; //!< trick_units(--)

   /**
    * Issued when an attachment cannot be performed as requested.
    */
   static char const * invalid_attach; //!< trick_units(--)

   /**
    * Issued when a detachment cannot be performed as requested.
    */
   static char const * invalid_detach; //!< trick_units(--)

   /**
    * Issued when a enum value is not one of the enumerated values.
    */
   static char const * invalid_enum; //!< trick_units(--)

   /**
    * Issued when something other than an enum, name, or node is invalid.
    */
   static char const * invalid_item; //!< trick_units(--)

   /**
    * Issued when a name is invalid -- NULL, empty, a duplicate, ...
    */
   static char const * invalid_name; //!< trick_units(--)

   /**
    * Issued when a node does not have expected linkages.
    */
   static char const * invalid_node; //!< trick_units(--)

   /**
    * Issued when a pointer that is null should be non-null.
    */
   static char const * null_pointer; //!< trick_units(--)

   /**
    * Error issued when a problem is detected in the subscription model.
    */
   static char const * subscription_error; //!< trick_units(--)

   /**
    * Error issued when a removal cannot be performed because the frame is
    * not registered.
    */
   static char const * removal_failed; //!< trick_units(--)


 // Member functions
 // This class is not instantiable.
 // The constructors and assignment operator for this class are declared
 // private and are not implemented.
 private:
   RefFrameMessages (void);
   RefFrameMessages (const RefFrameMessages &);
   RefFrameMessages & operator= (const RefFrameMessages &);

};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
