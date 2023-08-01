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
 * @file models/utils/ref_frames/include/ref_frame_interface.hh
 * Define the class RefFrameOwner, which identifies an object as an "owner"
 * of a reference frame.
 */

/********************************* TRICK HEADER ********************************

Purpose:
  ()

 
*******************************************************************************/


#ifndef JEOD_REF_FRAME_INTERFACE_HH
#define JEOD_REF_FRAME_INTERFACE_HH

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "subscription.hh"
#include "class_declarations.hh"


//! Namespace jeod
namespace jeod {

/**
 * Identify an object as an "owner" of a reference frame.
 * This class is an interface -- it has no member data. It instead defines
 * minimal capabilities common to all things that can "own" a reference frame.
 *
 * This interface class is one of the very few classes that JEOD uses in
 * the form of multiple inheritance.
 */
class RefFrameOwner {

 // Member data -- None.

 // Member functions
 public:

   /**
    * RefFrameOwner default constructor.
    */
   RefFrameOwner () {}

   /**
    * RefFrameOwner destructor.
    */
   virtual ~RefFrameOwner () {}


   /**
    * Note that a reference frame has changed its active/inactive status.
    * This default implementation does nothing.
    * @param frame  Frame whose status has changed
    */
   virtual void note_frame_status_change (RefFrame * frame JEOD_UNUSED) {}

};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
