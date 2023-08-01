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
 * @file models/dynamics/derived_state/include/orb_elem_derived_state.hh
 * Define the class OrbElemDerivedState, the class used for deriving the
 * orbital elements representation of a subject DynBody's position.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/orb_elem_derived_state.cc))



*******************************************************************************/


#ifndef JEOD_ORB_ELEM_DERIVED_STATE_HH
#define JEOD_ORB_ELEM_DERIVED_STATE_HH

// System includes

// JEOD includes
#include "dynamics/dyn_manager/include/class_declarations.hh"
#include "dynamics/dyn_body/include/class_declarations.hh"
#include "environment/planet/include/class_declarations.hh"
#include "utils/orbital_elements/include/orbital_elements.hh"
#include "utils/ref_frames/include/ref_frame_state.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "derived_state.hh"


//! Namespace jeod
namespace jeod {

/**
 * The class used for deriving the orbital elements representation
 * of a subject DynBody's position.
 */
class OrbElemDerivedState : public DerivedState {

 JEOD_MAKE_SIM_INTERFACES(OrbElemDerivedState)


 // Member data

 public:

   /**
    * The orbital elements of the subject body with respect to the planet
    * specified by the reference name.
    */
   OrbitalElements elements; //!< trick_units(--)

   /**
    * The planet, the name of which is specified by the inherited
    * reference_name data member.
    */
   Planet * planet; //!< trick_units(--)

 protected:

   /**
    * Use inertial or alt_inertial flag
    */
   bool use_alt_inertial;

   /**
    * Pointer to planet inertial frame to be used, either 
    * inertial or alt_inertial
    */
   EphemerisRefFrame * inertial_ptr;

   /**
    * Relative state; only used when the vehicle integration from is not the
    * planet-centered inertial frame.
    */
   RefFrameState rel_state; //!< trick_units(--)


 // Methods

 public:

   // Default constructor and destructor
   OrbElemDerivedState ();
   ~OrbElemDerivedState () override;

   // Setter for use_alt_inertial
   void set_use_alt_inertial (const bool use_alt_inertial_in);

   // initialize(): Initialize the DerivedState (but not necessarily the
   // state itself.)
   // Rules for derived classes:
   // All derived classes must forward the initialize() call to the immediate
   // parent class and then perform class-dependent object initializations.
   void initialize (DynBody & subject_body, DynManager & dyn_manager) override;

   // update(): Update the DerivedState representation of the subject DynBody.
   // Rules for derived classes:
   // All derived classes must perform class-dependent actions and then
   // must forward the update() call to the immediate parent class.
   void update (void) override;


 protected:

   void compute_orbital_elements (const RefFrameTrans & rel_trans);


 // The copy constructor and assignment operator for this class are
 // declared private and are not implemented.
 private:

   OrbElemDerivedState (const OrbElemDerivedState&);
   OrbElemDerivedState & operator = (const OrbElemDerivedState&);

};

} // End JEOD namespace

#ifdef TRICK_VER
#include "environment/planet/include/planet.hh"
#endif

#endif

/**
 * @}
 * @}
 * @}
 */
