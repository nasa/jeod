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
 * @addtogroup DerivedState
 * @{
 *
 * @file models/dynamics/derived_state/include/relative_derived_state.hh
 * Define the class RelativeDerivedState, the class used for deriving the
 * state of some frame associated with the subject DynBody relative to some
 * other target frame.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/relative_derived_state.cc))



*******************************************************************************/


#ifndef JEOD_RELATIVE_DERIVED_STATE_HH
#define JEOD_RELATIVE_DERIVED_STATE_HH


// Model includes
#include "derived_state.hh"

// JEOD includes
#include "dynamics/dyn_body/include/class_declarations.hh"
#include "dynamics/dyn_manager/include/class_declarations.hh"
#include "utils/ref_frames/include/class_declarations.hh"
#include "utils/ref_frames/include/ref_frame_state.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// System includes
#include <string>
#include <utility>


//! Namespace jeod
namespace jeod {

/**
 * The class used for deriving the state of some frame associated
 * with the subject DynBody relative to some other target frame.
 */
class RelativeDerivedState : public DerivedState {

 JEOD_MAKE_SIM_INTERFACES(RelativeDerivedState)


 // Enumerations

 public:

   /**
    * an enumeration to specify the direction and sense, and frame
    * representation intended for output from the RelativeDerivedState
    * calculations.
    */
   enum DirectionSense {
      undefined = -1, /*
         Default value; model does not assume a setting for the user. */

      ComputeSubjectStateinTarget = 0, /*
         Compute relative state as subject frame w.r.t. target in target. */

      ComputeTargetStateinSubject = 2 /*
         Compute relative state as target frame w.r.t. subject in subject. */
   };


 // Member data

 public:

   /**
    * The name of this relative derived state
    */
   std::string name; //!< trick_units(--)

   /**
    * The name of the frame on the subject vehicle.
    * This can specify one of the vehicle's three primary reference frames (core
    * body, composite body, or structure) or one of the vehicle's vehicle point
    * frames.
    * The vehicle name can be included in or omitted from the subject frame
    * name. A vehicle name prefix is assumed if it is omitted.
    */
   char * subject_frame_name; //!< trick_units(--)

   /**
    * The name of the target reference frame.
    */
   char * target_frame_name; //!< trick_units(--)

   /**
    * Indicates direction in which relative state is to be computed.
    */
   DirectionSense direction_sense; //!< trick_units(--)

   /**
    * Computed relative state.
    */
   RefFrameState rel_state; //!< trick_units(--)

   /**
    * Bool flag used by the RelKin model to turn on/off which
    * Relative Derived State needs to be managed
    */
   bool active; //!< trick_units(--)


 protected:

   /**
    * The reference frame corresponding to the user-input subject_frame_name.
    */
   BodyRefFrame * subject_frame; //!< trick_units(--)

   /**
    * The reference frame corresponding to the user-input target_frame_name.
    */
   RefFrame * target_frame; //!< trick_units(--)


 // Methods

 public:

   // Default constructor and destructor
   RelativeDerivedState ();
   ~RelativeDerivedState () override;

   /**
    * Setter for the name.
    */
   void set_name (std::string name_in)
   {
      name = std::move(name_in);
   }

   /**
    * Quick shortcut allowing use of conversion routines without requiring
    * initialization.
    * \param tf  New target frame.
    */
   void set_target_frame (RefFrame &tf) {target_frame = &tf; }

   /**
    * Quick shortcut allowing use of conversion routines without requiring
    * initialization.
    * \param sf  New subject frame.
    */
   void set_subject_frame (BodyRefFrame &sf) {subject_frame = &sf; }

   // initialize(): Initialize the RelativeDerivedState instance
   void initialize (DynBody & subject_body, DynManager & dyn_manager) override;

   // initialize(): Initialize the RelativeDerivedState instance
   virtual void initialize (DynManager & dyn_manager);

   // update(): Compute the relative state
   void update (void) override;

   /* set_activation_flag(): Set the activation_flag to true or false
    * /param raf  RelativeDerivedState activation flag for RelKin manager
    */
   void set_activation_flag (bool raf);

 // The copy constructor and assignment operator for this class are
 // declared private and are not implemented.
 private:

   RelativeDerivedState (const RelativeDerivedState&);
   RelativeDerivedState & operator = (const RelativeDerivedState&);
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
