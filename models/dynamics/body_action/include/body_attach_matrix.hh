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
 * @file models/dynamics/body_action/include/body_attach_matrix.hh
 * Define the class MassBodyAttachMatrix, which causes one MassBody to be
 * attached given a transformation.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/body_attach_matrix.cc))



*******************************************************************************/


#ifndef JEOD_MASS_BODY_ATTACH_MATRIX_HH
#define JEOD_MASS_BODY_ATTACH_MATRIX_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/orientation/include/orientation.hh"

// Model includes
#include "body_attach.hh"


//! Namespace jeod
namespace jeod {

/**
 * Attaches a pair of MassBody objects using the offset+matrix attach mechanism.
 * When the action is ready, the attachment is made such that:
 * - The displacement between the origins of the parent and subject bodies'
 *   structural frames is that given by the offset_pstr_cstr_pstr data member.
 * - The orientation between these two reference frames's axes is that given by
 *   the pstr_cstr data member.
 */
class BodyAttachMatrix : public BodyAttach {

 JEOD_MAKE_SIM_INTERFACES(BodyAttachMatrix)


 // Member data

 public:

   /**
    * Location of this body's structural origin with respect to the new parent
    * body's structural origin (or generic reference frame),
    * specified in structural coordinates of the new parent body.
    */
   double offset_pstr_cstr_pstr[3];   //!< trick_units(m)

   /**
    * Orientation of child's structural frame with respect to that of the
    * new parent; sense is parent-to-child.
    */
   Orientation pstr_cstr;             //!< trick_units(--)


 // Methods

 public:

   // Default constructor.
   BodyAttachMatrix ();

   // Destructor.
   ~BodyAttachMatrix () override;

   // apply: Attach the specified mass bodies.
   void apply (DynManager & dyn_manager) override;

};


/**
 * Destructor
 */
inline
BodyAttachMatrix::~BodyAttachMatrix (
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
