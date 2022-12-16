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
 * @file models/dynamics/dyn_body/include/torque.hh
 * Define the JEOD torque model.
 */

/*******************************************************************************
Purpose:
  ()

Library dependencies:
  ((torque.o))



*******************************************************************************/


#ifndef JEOD_TORQUE_HH
#define JEOD_TORQUE_HH


//! Namespace jeod
namespace jeod {

/**
 * A Torque represents a Newtonian torque that acts on a DynBody.
 * The class encapsulates an active flag and a 3-vector that contains the
 * torque components. Torques are collected in one of a DynBody object's
 * torque collection STL vectors. The torque vector is expressed in the
 * structural frame of that DynBody object.
 *
 * The Torque class is the recommended mechanism for representing torques in
 * JEOD. While 3-vectors can also be collected into a collect STL vector,
 * theee is is no way to turn off these collected 3-vectors. Even worse, there
 * is no way to tell whether a collected 3-vector does indeed represent a
 * torque, or even if it is a 3-vector. In comparison, Torque objects can be
 * turned on and off, and more importantly, they are type-safe.
 */
class Torque {

 public:

   // Member functions

   // Constructor and destructor.
   Torque ();
   virtual ~Torque ();

   // Member data

   /**
    * Is this torque active?
    */
   bool active; //!< trick_units(--)
   /**
    * Torque vector
    */
   double torque[3]; //!< trick_units(N*m)


   // Operators; listed out-of-order due to Trick07 ICG.

   // Subscript operator: Access torque elements as an lvalue.
   double & operator[] (const unsigned int index);
   double operator[] (const unsigned int index) const;


 private:

   // Make the copy constructor and assignment operator private
   // (and unimplemented) to avoid erroneous copies

   /**
    * Not implemented.
    */
   Torque (const Torque &);

   /**
    * Not implemented.
    */
   Torque & operator= (const Torque &);

};



/**
 * A CollectTorque represents a collected torque that acts on a vehicle.
 * The BodyTorqueCollect class contains STL vectors that in turn contain
 * CollectTorque pointers. These vectors are populated via the Trick vcollect
 * mechanism. A Trick simulation issues vcollect statements such as
 * \code
 * vcollect vehicle.body.collect.collect_XXX_forc CollectTorque::create {
 *    vehicle.torque_model1.torque,
 *    vehicle.torque_model2.torque
 * };
 * \endcode
 * This invokes the appropriate CollectTorque create method on each listed
 * element.
 *
 * CollectTorques should not be used in model code to represent torques.
 * Use the Torque class instead.
 */
class CollectTorque {

 public:

   // Static member functions

   /* Factory constructors */
   static CollectTorque * create (double * vec);
   static CollectTorque * create (Torque & torque);
   static CollectTorque * create (CollectTorque & torque);

#ifndef SWIG

   static CollectTorque * create (Torque * torque);
   static CollectTorque * create (CollectTorque * torque);

#endif

   // Constructors
   CollectTorque ();
   explicit CollectTorque (double vec[3]);
   explicit CollectTorque (Torque &);
   explicit CollectTorque (CollectTorque &);

   // Destructor
   virtual ~CollectTorque ();

   // Is the torque active?
   bool is_active () const;

   // Subscript operator: Access torque elements as an lvalue.
   double & operator[] (const unsigned int index);
   double operator[] (const unsigned int index) const;


   // Member data
   // That these are public is deprecated.

   /**
    * Is this torque active?
    */
   bool * active; //!< trick_units(--)

   /**
    * Torque vector
    */
   double * torque; //!< trick_units(N*m)


private:

   // Make the copy constructor and assignment operator private
   // (and unimplemented) to avoid erroneous copies

   /**
    * Not implemented.
    */
   CollectTorque (const CollectTorque &);

   /**
    * Not implemented.
    */
   CollectTorque & operator= (const CollectTorque &);

};



/**
 * This class is deprecated.
 */
class CInterfaceTorque : public CollectTorque {

 public:

   // Member functions.

   // Constructors.
   CInterfaceTorque ();
   CInterfaceTorque (double * vec);

   // Destructor
   virtual ~CInterfaceTorque ();

   // Member data: This class adds no data to CollectTorque.


 private:

   // Make the copy constructor and assignment operator private
   // (and unimplemented) to avoid erroneous copies

   /**
    * Not implemented.
    */
   CInterfaceTorque (const CInterfaceTorque &);

   /**
    * Not implemented.
    */
   CInterfaceTorque & operator= (const CInterfaceTorque &);

};

} // End JEOD namespace


#include "torque_inline.hh"


#endif

/**
 * @}
 * @}
 * @}
 */
