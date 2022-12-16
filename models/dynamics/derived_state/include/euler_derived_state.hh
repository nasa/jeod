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
 * @addtogroup DerivedState
 * @{
 *
 * @file models/dynamics/derived_state/include/euler_derived_state.hh
 * Define the class EulerDerivedState, the class used for deriving the
 * Euler angle representation of a subject DynBody's attitude.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((euler_derived_state.o))



*******************************************************************************/


#ifndef JEOD_EULER_DERIVED_STATE_HH
#define JEOD_EULER_DERIVED_STATE_HH

// System includes

// Trick includes

// JEOD includes
#include "dynamics/dyn_body/include/class_declarations.hh"
#include "dynamics/dyn_manager/include/class_declarations.hh"
#include "utils/orientation/include/orientation.hh"
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/ref_frames/include/class_declarations.hh"
#include "utils/ref_frames/include/ref_frame_state.hh"

// Model includes
#include "derived_state.hh"


//! Namespace jeod
namespace jeod {

/**
 * The class used for deriving the Euler angle representation
 * of a subject DynBody's attitude.
 */
class EulerDerivedState : public DerivedState {

 JEOD_MAKE_SIM_INTERFACES(EulerDerivedState)


 // Member data

 public:

   /**
    * Euler angle sequence specification.
    */
   Orientation::EulerSequence sequence; //!< trick_units(--)

   /**
    * Euler angles from reference frame.
    */
   double ref_body_angles[3];           //!< trick_units(rad)

   /**
    * Euler angles to reference frame.
    */
   double body_ref_angles[3];           //!< trick_units(rad)

   /**
    * The relative state of the body. This is just a copy of the body's
    * state when the relative frame is the parent frame. This is a computed
    * relative state when the relative frame is not the parent frame.
    */
   RefFrameState rel_state; //!< trick_units(--)


 protected:

   /**
    * Reference frame from which to compute the Euler angle attitude.  If
    * this is NULL then the body's parent frame is used.
    */
   RefFrame * rel_frame; //!< trick_units(--)


 // Methods

 public:

   // Default constructor and destructor
   EulerDerivedState ();
   ~EulerDerivedState ();

   // initialize(): Initialize the DerivedState (but not necessarily the
   // state itself.)
   // Rules for derived classes:
   // All derived classes must forward the initialize() call to the immediate
   // parent class and then perform class-dependent object initializations.
   virtual void initialize (DynBody & subject_body, DynManager & dyn_manager);

   virtual void initialize (
      RefFrame & ref_frame, DynBody & subject_body, DynManager & dyn_manager);

   // update(): Update the DerivedState representation of the subject DynBody.
   // Rules for derived classes:
   // All derived classes must perform class-dependent actions and then
   // must forward the update() call to the immediate parent class.
   virtual void update (void);


 // The copy constructor and assignment operator for this class are
 // declared private and are not implemented.
 private:

   EulerDerivedState (const EulerDerivedState&);
   EulerDerivedState & operator = (const EulerDerivedState&);

};

} // End JEOD namespace

#ifdef TRICK_VER
#endif

#endif

/**
 * @}
 * @}
 * @}
 */
