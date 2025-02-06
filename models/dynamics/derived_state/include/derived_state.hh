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
 * @file models/dynamics/derived_state/include/derived_state.hh
 * Define the class DerivedState, the base class used for deriving a state
 * representation of some subject DynBody.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/derived_state.cc))



*******************************************************************************/


#ifndef JEOD_DERIVED_STATE_HH
#define JEOD_DERIVED_STATE_HH

// System includes

// JEOD includes
#include "dynamics/dyn_body/include/class_declarations.hh"
#include "dynamics/dyn_manager/include/class_declarations.hh"
#include "environment/planet/include/class_declarations.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "class_declarations.hh"


//! Namespace jeod
namespace jeod {

/**
 * The base class used for deriving a state representation
 * of some subject DynBody.
 */
class DerivedState {

   JEOD_MAKE_SIM_INTERFACES(DerivedState)


 // Member data

 public:

   /**
    * The body that is the subject of the derived state.
    */
   DynBody * subject; //!< trick_units(--)

   /**
    * The name of the object with respect to which the subject state is
    * assessed.
    */
   char * reference_name; //!< trick_units(--)


 protected:

   /**
    * An identifier for this derived state, constructed at initialization time
    * from the class name, the subject body name, and the reference name.
    * This is used for generating error and debug messages.
    */
   char * state_identifier; //!< trick_units(--)


 // Methods

 // The copy constructor and assignment operator for this class are
 // declared private and are not implemented.
 private:

   DerivedState (const DerivedState&);
   DerivedState & operator = (const DerivedState&);


 public:

   // Default constructor
   DerivedState ();

   // Destructor
   virtual ~DerivedState ();

   // set_reference_name(): Set the reference name to the supplied name (copy).
   void set_reference_name (const char * new_name);

   // initialize(): Initialize the DerivedState (but not necessarily the
   // state itself.)
   // Rules for derived classes:
   // All derived classes must forward the initialize() call to the immediate
   // parent class and then perform class-dependent object initializations.
   virtual void initialize (DynBody & subject_body, DynManager & dyn_manager);

   // update(): Update the DerivedState representation of the subject DynBody.
   // Rules for derived classes:
   // All derived classes must perform class-dependent actions and then
   // must forward the update() call to the immediate parent class.
   virtual void update (void);


 protected:

   // find_planet: Find specified Planet, failing if not found.
   Planet * find_planet (
      const DynManager & dyn_manager,
      const char * planet_name,
      const char * variable_name);

};

} // End JEOD namespace

#ifdef TRICK_VER
#include "dynamics/dyn_body/include/dyn_body.hh"
#endif


#endif

/**
 * @}
 * @}
 * @}
 */
