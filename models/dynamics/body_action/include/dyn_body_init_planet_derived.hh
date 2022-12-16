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
 * @file models/dynamics/body_action/include/dyn_body_init_planet_derived.hh
 * Define the class DynBodyInitPlanetDerived, the base class for initializing
 * selected aspects of a vehicle's state with respect to some state that is
 * derived from some vehicle's state in conjunction with a planet.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((dyn_body_init_planet_derived.o))



*******************************************************************************/


#ifndef JEOD_DYN_BODY_INIT_PLANET_DERIVED_HH
#define JEOD_DYN_BODY_INIT_PLANET_DERIVED_HH

// System includes

// JEOD includes
#include "dynamics/dyn_body/include/class_declarations.hh"
#include "dynamics/dyn_manager/include/class_declarations.hh"
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/ref_frames/include/ref_frame_items.hh"

// Model includes
#include "dyn_body_init_wrt_planet.hh"



//! Namespace jeod
namespace jeod {

/**
 * (Initialize selected aspects of a vehicle's state with respect to some state
 *  that is derived from some vehicle's state in conjunction with a planet.
 */
class DynBodyInitPlanetDerived : public DynBodyInitWrtPlanet {

   JEOD_MAKE_SIM_INTERFACES(DynBodyInitPlanetDerived)


 // Member data

 public:

   /**
    * The name of the vehicle whose composite body frame is used to build the
    * derived state with respect to which the vehicle initialization data
    * are referenced.
    */
   char * ref_body_name; //!< trick_units(--)


 protected:
   /**
    * The vehicle corresponding to the ref_body_name.
    * Note that this is not a user-inputtable item.
    */
   DynBody * ref_body; //!< trick_io(**)

   /**
    * The state elements in the reference body's composite body frame that must
    * be set before this initializer can proceed.
    * This is not user-inputtable; derived classes should set this item.
    * The default is to require the full state to be set.
    */
   RefFrameItems::Items required_items; //!< trick_io(**)

   /**
    * If true (default), the ref_body cannot be NULL.
    * If false, the derived class must provide some means other than using a
    * derived state to set the reference RefFrame.
    */
   bool body_is_required; //!< trick_io(**)


 // Member functions

 // The copy constructor and assignment operator for this class are
 // declared private and are not implemented.
 private:

   DynBodyInitPlanetDerived (const DynBodyInitPlanetDerived&);
   DynBodyInitPlanetDerived & operator = (const DynBodyInitPlanetDerived&);


 public:

   DynBodyInitPlanetDerived ();

   virtual ~DynBodyInitPlanetDerived ();

   // initialize: Initialize the initializer.
   virtual void initialize (DynManager & dyn_manager);

   // is_ready: Indicate whether the initializer is ready to be applied.
   // A DynBodyInitPlanetDerived state initializer is ready when the reference
   // body's required items have been set.
   virtual bool is_ready (void);

   // apply: Apply the state to the subject body.
   virtual void apply (DynManager & dyn_manager);

};

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
