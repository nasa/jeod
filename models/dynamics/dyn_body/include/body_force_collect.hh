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
 * @addtogroup DynBody
 * @{
 *
 * @file models/dynamics/dyn_body/include/body_force_collect.hh
 * Define the class BodyForceCollect.
 */

/********************************* TRICK HEADER ********************************
Purpose:
  ()

Library dependencies:
  ((aux_classes.o))



*******************************************************************************/


#ifndef JEOD_BODY_FORCE_COLLECT_HH
#define JEOD_BODY_FORCE_COLLECT_HH

// System includes

// JEOD includes
#include "utils/container/include/pointer_vector.hh"
#include "utils/memory/include/jeod_alloc.hh"

// Model includes
#include "force.hh"
#include "torque.hh"


//! Namespace jeod
namespace jeod {

/**
 * Release JEOD-allocated memory in the collect vector.
 * \param[in,out] vec Collected vectors
 */
template <class CollectType>
void
release_vector (
   CollectType & vec)
{
   unsigned int nitems = vec.size();

   for (unsigned int ii = 0; ii < nitems; ii++) {
      if (JEOD_IS_ALLOCATED (vec[ii])) {
         JEOD_DELETE_OBJECT (vec[ii]);
         vec[ii] = NULL;
      }
   }
   vec.clear();
}


/**
 * This is a derived version of the template class
 * JeodPointerVector<CollectForce>::type with an implementation of the method
 * perform_cleanup_action which frees and clears stale data following a restore.
 */
class JPVCollectForce: public JeodPointerVector<CollectForce>::type {

public:

   /**
    * Free stale data, typically following a restore from checkpoint.
    */
   virtual void perform_cleanup_action (
      const std::string &) {

#if ((TRICK_VER > 13) || (TRICK_VER == 13 && TRICK_MINOR >= 3))
// Not needed prior to Trick 13.3
      release_vector(*this);
#endif

   }

};


/**
 * This is a derived version of the template class
 * JeodPointerVector<CollectTorque>::type with an implementation of the method
 * perform_cleanup_action which frees and clears stale data following a restore.
 */
class JPVCollectTorque: public JeodPointerVector<CollectTorque>::type {

public:

   /**
    * Free stale data, typically following a restore from checkpoint.
    */
   virtual void perform_cleanup_action (
      const std::string &) {

#if ((TRICK_VER > 13) || (TRICK_VER == 13 && TRICK_MINOR >= 3))
// Not needed prior to Trick 13.3
      release_vector(*this);
#endif

   }

};



/**
 * Serves as the collection point for forces and torques that act on a vehicle.
 * This class is a simple class that is tightly coupled with the DynBody class.
 * The DynBody class contains (has-a) a BodyForceCollect member.
 *
 * The Trick vcollect mechanism (or a similar mechanism in a non-Trick sim)
 * pushes the individual forces and torques onto the various collect_XXX members
 * of a BodyForceCollect.  DynBody members cumulate these collected forces and
 * torques to form the total forces and torques acting on the vehicle.
 */
class BodyForceCollect {

 // Member functions

 // Make the copy constructor and assignment operator private
 // (and unimplemented) to avoid erroneous copies
 private:

   BodyForceCollect (BodyForceCollect&);
   BodyForceCollect & operator = (const BodyForceCollect&);


 public:

   // Constructor and destructor
   BodyForceCollect ();
   ~BodyForceCollect ();

 // Member data

   /**
    * Sum of effector forces, struct ref
    */
   double effector_forc[3]; //!< trick_units(N)

   /**
    * Sum of env forces, struct ref
    */
   double environ_forc[3]; //!< trick_units(N)

   /**
    * Sum of local forces, struct ref
    */
   double no_xmit_forc[3]; //!< trick_units(N)

   /**
    * Sum of external forces, struct ref
    */
   double extern_forc_struct[3]; //!< trick_units(N)

   /**
    * Sum of external forces, inertial
    */
   double extern_forc_inrtl[3]; //!< trick_units(N)

   /**
    * Sum of effector torques about body CoM, struct ref
    */
   double effector_torq[3]; //!< trick_units(N*m)

   /**
    * Sum of environment torqs about body CoM, struct ref
    */
   double environ_torq[3]; //!< trick_units(N*m)

   /**
    * Sum of torqs not transmitted to a parent about body CoM, struct ref
    */
   double no_xmit_torq[3]; //!< trick_units(N*m)

   /**
    * Induced inertial torques from second order rotational dynamics, w x Iw,
    * body ref
    */
   double inertial_torq[3]; //!< trick_units(N*m)

   /**
    * Sum of external torques, struct ref
    */
   double extern_torq_struct[3];  //!< trick_units(N*m)

   /**
    * Sum of external torques, body ref
    */
   double extern_torq_body[3]; //!< trick_units(N*m)

   /**
    * Vector of effector forces, (struct)
    */
   JPVCollectForce collect_effector_forc; //!< trick_io(**)

   /**
    * Vector of env forces, (struct)
    */
   JPVCollectForce collect_environ_forc; //!< trick_io(**)

   /**
    * Vector of local forces, (struct)
    */
   JPVCollectForce collect_no_xmit_forc; //!< trick_io(**)

   /**
    * Vector of effector torques, (struct)
    */
   JPVCollectTorque collect_effector_torq; //!< trick_io(**)

   /**
    * Vector of env torques, (struct)
    */
   JPVCollectTorque collect_environ_torq; //!< trick_io(**)

   /**
    * Vector of local torques, (struct)
    */
   JPVCollectTorque collect_no_xmit_torq; //!< trick_io(**)

};

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
