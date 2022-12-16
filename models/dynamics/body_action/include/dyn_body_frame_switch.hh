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
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/include/dyn_body_frame_switch.hh
 * Define the class DynBodyFrameSwitch, the BodyAction derived class used for
 * switch a DynBody's integration frame.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((dyn_body_frame_switch.o))



*******************************************************************************/


#ifndef JEOD_DYN_BODY_FRAME_SWITCH_HH
#define JEOD_DYN_BODY_FRAME_SWITCH_HH


// System includes

// JEOD includes
#include "dynamics/dyn_body/include/class_declarations.hh"
#include "utils/ref_frames/include/class_declarations.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "class_declarations.hh"
#include "body_action.hh"


//! Namespace jeod
namespace jeod {

class EphemerisRefFrame;

/**
 * Switch a DynBody's integration frame to a specified frame when
 * the body switches to that integration frame's sphere of influence.
 */
class DynBodyFrameSwitch : public BodyAction {

 JEOD_MAKE_SIM_INTERFACES(DynBodyFrameSwitch)


 // Enumerations

 public:

   /**
    * Specifies whether the is_ready() method is to look for the vehicle
    * entering (SwitchOnApproach) the new integration frame's sphere of
    * influence versus leaving (SwitchOnDeparture) the current integration
    * frame's sphere of influence.
    */
   enum SwitchSense {
      SwitchOnApproach  = 0, /*
         The subject DynBody's integration frame is to be switched to the
         specified integration frame when the distance between the DynBody
         and the origin of the new integration frame becomes less than
         the specified switch distance. */

      SwitchOnDeparture = 1  /*
         The subject DynBody's integration frame is to be switched to the
         specified integration frame when the distance between the DynBody
         and the origin of the current integration frame becomes greater than
         the specified switch distance. */
   };


 // Member data

 public:

   /**
    * The name of the new integration frame. This name must specify a valid
    * valid integration frame. Failure to do so constitutes a fatal error.
    */
   char * integ_frame_name; //!< trick_units(--)

   /**
    * Indicates whether the switch occurs when the subject DynBody enters
    * a sphere of influence around the new integration frame or leaves a sphere
    * sphere of influence around of the current integration frame.
    */
   SwitchSense switch_sense; //!< trick_units(--)

   /**
    * If set, the body's gravitational controls are sorted in ascending
    * acceleration magnitude.
    */
   bool sort_grav_controls; //!< trick_units(--)

   /**
    * The radius of the sphere of influence.
    */
   double switch_distance; //!< trick_units(m)


 protected:
   /**
    * The reference frame corresponding to the input integ_frame_name.
    */
   EphemerisRefFrame * integ_frame; //!< trick_io(**)


 // Member functions

 public:

   // Default constructor.
   DynBodyFrameSwitch ();

   // Destructor.
   virtual ~DynBodyFrameSwitch ();

   // initialize: Initialize the initializer.
   virtual void initialize (DynManager & dyn_manager);

   // apply: Forward the apply call up the class heirarchy.
   virtual void apply (DynManager & dyn_manager);

   // is_ready: Is the action ready?
   // In this case, is the active flag set and has the body entered/left
   // the appropriate sphere of influence?
   virtual bool is_ready (void);

};

} // End JEOD namespace

#ifdef TRICK_VER
#include "environment/ephemerides/ephem_interface/include/ephem_ref_frame.hh"
#endif

#endif

/**
 * @}
 * @}
 * @}
 */
