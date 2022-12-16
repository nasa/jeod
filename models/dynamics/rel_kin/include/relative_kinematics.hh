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
 * @addtogroup RelKin
 * @{
 *
 * @file models/dynamics/rel_kin/include/relative_kinematics.hh
 * Define the class RelativeKinematics, the class used for calculating the
 * state of some point(s) of interest associated with the subject DynBody
 * relative to some other reference frame.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((relative_kinematics.o))


*******************************************************************************/


#ifndef JEOD_RELATIVE_KINEMATICS_HH
#define JEOD_RELATIVE_KINEMATICS_HH


// System includes

// JEOD includes
#include "dynamics/derived_state/include/class_declarations.hh"
#include "utils/container/include/pointer_vector.hh"
#include "utils/sim_interface/include/jeod_class.hh"


//! Namespace jeod
namespace jeod {

/**
 * Encapsulates functionality for computing relative states.
 */
class RelativeKinematics {

 JEOD_MAKE_SIM_INTERFACES(RelativeKinematics)


 // Member data
 public:

   /**
    * Length of above list of relative states being maintained by this.
    */
   unsigned int num_rel_states; //!< trick_units(--)

   /**
    * List of relative states to be computed and maintained by this model. Note
    * that this list is not restricted to be relative states associated with
    * only a single DynBody.
    */
   JeodPointerVector<RelativeDerivedState>::type relative_states;  //!< trick_io(**)


 // Member functions

 // Make the copy constructor and assignment operator private
 // (and unimplemented) to avoid erroneous copies
 private:
   RelativeKinematics (const RelativeKinematics &);
   RelativeKinematics & operator= (const RelativeKinematics &);


 public:

   // Default constructor and destructor
   RelativeKinematics ();
   ~RelativeKinematics ();

   // Add a RelativeDerivedState to the list of ones being maintained
   void add_relstate (RelativeDerivedState & relstate);

   // Remove a RelativeDerivedState from the list ones being maintained
   void remove_relstate (RelativeDerivedState & relstate);

   // Find a RelativeDerivedState by its given name
   RelativeDerivedState * find_relstate (const char * relstate_name);

   // Activate or deactivate given relstate to allow RelKin manager to update it
   void activate_relstate (RelativeDerivedState & relstate, bool raf);

   // Update a single RelativeDerivedState
   void update_single (const char * relstate_name);

   // Update all of the RelativeDerivedStates maintained by this model
   void update_all (void);

};

} // End JEOD namespace


#endif

/**
 * @}
 * @}
 * @}
 */
