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
 * @addtogroup Dynamics
 * @{
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/include/body_detach_specific.hh
 * Define the class MassBodyDetachSpecific, the class used for detaching one
 * MassBody object from another specified MassBody.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/body_detach_specific.cc))



*******************************************************************************/


#ifndef JEOD_MASS_BODY_DETACH_SPECIFIC_HH
#define JEOD_MASS_BODY_DETACH_SPECIFIC_HH


// System includes

// JEOD includes
#include "dynamics/mass/include/class_declarations.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "class_declarations.hh"
#include "body_action.hh"


//! Namespace jeod
namespace jeod {

/**
 * Causes the subject body to detach from a specific body by severing the link
 * immediately spawning from the detach_from body. This method works between
 * two dynamic bodies (DynBody) or mass bodies (MassBody), but not mixtures of
 * the two classes.
 * The subject body itself is detached from its parent if and only if the
 * specified detach_from body is the subject body's immediate parent. In the
 * case that the detach_from body is some indirect parent, the body that
 * detaches is the the immediate child body of the detach_from body that is
 * along the connectivity path from the subject body to the detach_from * body.
 * Specifing a detach_from body that is not a parent (direct or indirect) body
 * of the subject body is an error.
 */
class BodyDetachSpecific : public BodyAction {

 JEOD_MAKE_SIM_INTERFACES(BodyDetachSpecific)


 // Member data

 public:

   /**
     * Set the subject mass body of this action. Resets dyn_subject to null
     */
   void set_detach_from_body(MassBody & mass_body_in);

   /**
     * Set the subject mass body of this action. Resets dyn_subject to null
     */
   void set_detach_from_body(DynBody & dyn_body_in);

 protected:


   /**
    * The mass body from the subject of this action is to detach.
    * This pointer must be supplied for pure MassBody detachments.
    * The initialize method will attempt to determine if this MassBody refers
    * to a DynBody. The detachment is performed between the mass_detach_from object
    * and the direct descendant of the detach_from object that is in the
    * parental lineage from the subject body to the detach_from body.
    */
   MassBody * mass_detach_from; //!< trick_units(--)

   /**
    * The dynamic body from the subject of this action is to detach.
    * This pointer or the detach_from member must be supplied for dynamic body
    * detachment.
    * The detachment is performed between the mass_detach_from object and the
    * direct descendant of the mass_detach_from object that is in the parental
    * lineage from the subject body to the mass_detach_from body.
    */
   DynBody * dyn_detach_from; //!< trick_units(--)


 // Member functions

 public:

   // Default constructor.
   BodyDetachSpecific ();

   // Destructor.
   ~BodyDetachSpecific () override;

   // initialize: Initialize the initializer.
   void initialize (DynManager & dyn_manager) override;

   // apply: Detach the body from its parent body.
   void apply (DynManager & dyn_manager) override;

   // is_ready: Is the action ready? (In this case, is the active flag set?)
   bool is_ready (void) override;

};


/**
 * Destructor
 */
inline
BodyDetachSpecific::~BodyDetachSpecific (
   void)
{
   ; // Empty
}

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
