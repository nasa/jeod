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
 * @file models/dynamics/body_action/include/body_attach_aligned.hh
 * Define the class MassBodyAttachAligned, which causes one MassBody to be
 * attached to another at a pair of MassPoints.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/body_attach_aligned.cc))



*******************************************************************************/


#ifndef JEOD_MASS_BODY_ATTACH_ALIGNED_HH
#define JEOD_MASS_BODY_ATTACH_ALIGNED_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "class_declarations.hh"
#include "body_attach.hh"


//! Namespace jeod
namespace jeod {

/**
 * Attaches a pair of MassBody objects at a pair of MassPoints.
 * When the action is ready, the attachment proceeds as follows:
 * - The points indicated by the subject and parent mass point names will
 *   be coincident after attachment is complete.
 * - The orientation between the two reference frames associated with the
 *   two attach points is a 180 degree yaw.
 */
class BodyAttachAligned : public BodyAttach {

 JEOD_MAKE_SIM_INTERFACES(BodyAttachAligned)


 // Member data

 public:

   /**
    * The name of the mass point on the subject mass body to be attached to
    * to the parent_point_name mass point on the parent mass body. The supplied
    * name can omit the subject mass body name dot prefix if desired.
    */
   std::string subject_point_name; //!< trick_units(--)

   /**
    * The name of the mass point on the parent mass body to be attached to
    * to the mass pointed named subject_point_name on the subject mass body. The
    * supplied name can omit the parent mass body name dot prefix if desired.
    */
   std::string parent_point_name; //!< trick_units(--)


 // Methods

 public:

   // Default constructor.
   BodyAttachAligned ();

   // Destructor.
   ~BodyAttachAligned () override;

   // initialize: Initialize the initializer.
   void initialize (DynManager & dyn_manager) override;

   // apply: Attach the specified mass bodies.
   void apply (DynManager & dyn_manager) override;


 private:

   BodyAttachAligned (const BodyAttachAligned&);
   BodyAttachAligned & operator = (const BodyAttachAligned&);

};


/**
 * Destructor
 */
inline
BodyAttachAligned::~BodyAttachAligned (
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
