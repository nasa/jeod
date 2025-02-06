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
 * @file models/dynamics/body_action/include/dyn_body_init_rot_state.hh
 * Define the class DynBodyInitRotState that initialize aspects of a vehicle's
 * rotational state.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/dyn_body_init_rot_state.cc))



*******************************************************************************/


#ifndef JEOD_DYN_BODY_INIT_ROT_STATE_HH
#define JEOD_DYN_BODY_INIT_ROT_STATE_HH


// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "class_declarations.hh"
#include "dyn_body_init.hh"


//! Namespace jeod
namespace jeod {

/**
 * Initialize aspects of a vehicle's rotational state.
 */
class DynBodyInitRotState : public DynBodyInit {

   JEOD_MAKE_SIM_INTERFACES(DynBodyInitRotState)


 // Enumerations

 public:

   /**
    * Identify which of attitude/rate is to be initialized.
    */
   enum StateItems {
      Both      = 0, ///< Initialize both attitude and rate
      Attitude  = 1, ///< Initialize attitude only
      Rate      = 2  ///< Initialize rate only
   };


 // Member data

 public:

   /**
    * State items to be initialized -- attitude, rate, or both.
    */
   StateItems state_items; //!< trick_units(--)


 // Member functions


 // The copy constructor and assignment operator for this class are
 // declared private and are not implemented.
 private:

   DynBodyInitRotState (const DynBodyInitRotState&);
   DynBodyInitRotState & operator = (const DynBodyInitRotState&);


 public:

   DynBodyInitRotState ();

   ~DynBodyInitRotState () override;

   // initialize: Initialize the initializer.
   void initialize (DynManager & dyn_manager) override;

   // apply: Apply the state to the subject body.
   void apply (DynManager & dyn_manager) override;

   // initializes_what: Indicate what aspect of the state is initialized.
   // The DynBodyInitRotSate initializes position and velocity.
   RefFrameItems::Items initializes_what (void) override;

   // is_ready: Indicate whether the initializer is ready to be applied.
   // The base DynBodyInit is always ready.
   bool is_ready (void) override;

};


/**
 * Destructor.
 */
inline
DynBodyInitRotState::~DynBodyInitRotState (
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
