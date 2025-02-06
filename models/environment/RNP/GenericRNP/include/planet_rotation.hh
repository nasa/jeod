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
 * @addtogroup Environment
 * @{
 * @addtogroup RNP
 * @{
 * @addtogroup GenericRNP
 * @{
 *
 * @file models/environment/RNP/GenericRNP/include/planet_rotation.hh
 * Pure virtual polymorphic base class for all forms of planet rotation in th
 * RNP model including precession, nutation, polar motion and axial rotation
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((None)))

Assumptions and limitations:
  ((Rotation outputted can only depend on time)
   (pure virtual class, all pure virtual functions must be implemented
    in an inheriting class, this class can't be instantiated))

Library dependencies:
  ((planet_rotation.o))

 

*******************************************************************************/


#ifndef PLANET_ROTATION_HH
#define PLANET_ROTATION_HH

// System includes
#include <math.h>

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod {

class PlanetRotationInit;


// Polymorphic, pure virtual base class for planet rotation models for rnp
/**
 * The generic base class for all planet transformations.
 */
class PlanetRotation {

   JEOD_MAKE_SIM_INTERFACES(PlanetRotation)

public: // public member variables

   /**
    * The transformation matrix outputted by the
    * model after update is called
    */
   double rotation[3][3]; //!< trick_units(--)
   /**
    * The current time the transformation matrix will
    * be calculated from. Unitless so that models with
    * different time scales can be used with one
    * underlying model
    */
   double current_time; //!< trick_units(--)

private: // private member variables

protected: // protected member variables
// constants commonly used in rotation model calculations

   /**
    * Convert from radians to degrees
    */
   double RADTODEG; //!< trick_units(degree/rad)
   /**
    * Inverse of RADTODEG
    */
   double DEGTORAD; //!< trick_units(rad/degree)
   /**
    * Convert from degrees to arcseconds
    */
   double DEGTOSEC; //!< trick_units(arcsecond/degree)
   /**
    * Inverse of DEGTOSEC
    */
   double SECTODEG; //!< trick_units(degree/arcsecond)
   /**
    * Convert from julian day to seconds
    */
   double JULIANDAYTOSEC; //!< trick_units(s/day)
   /**
    * Inverse of JULIANDAYTOSEC
    */
   double SECTOJULIANDAY; //!< trick_units(day/s)
   /**
    * Convert from julian centuries to julian days
    */
   double JULIANCENTTODAY; //!< trick_units(--)
   /**
    * Inverse of JULIANCENTTODAY
    */
   double DAYTOJULIANCENT; //!< trick_units(--)

public: // public member functions

   PlanetRotation ();

   virtual ~PlanetRotation ();

   // updates the rotation from the last time set through update_time
      /**
    * Updates the rotation of this particular object,
    * based on the last time set in "set_time".
    */
   /*
     Purpose:
       (Updates the rotation of this particular object,
        based on the last time set in "set_time".)
    */
   virtual void
   update_rotation (
      void)
   {return;}

   // Initialize the rotation model with the data and information contained
   // in the PlanetRotationInit object pointed to by init. PlanetRotationInit
   // is also a pure virtual, polymorphic class. Not all planet rotation models
   // will require this step (typically only those that require a large amount
   // of data that has no possibility of changing over time, nutation and
   // polar motion are the most common examples)
      /**
    * Initializes the invoking object from an initialization object.
    * \param[in] init The object that will initialize the invoking object.
    */
   // in the PlanetRotationInit object pointed to by init. PlanetRotationInit
   // is also a pure virtual, polymorphic class. Not all planet rotation models
   // will require this step (typically only those that require a large amount
   // of data that has no possibility of changing over time, nutation and
   // polar motion are the most common examples)
   /*
     Purpose:
       (Initializes the invoking object from an initialization object.)
    */
   virtual void
   initialize (
      PlanetRotationInit* init JEOD_UNUSED)
   {
      return;
   }

   // Update the time which the next update_rotation call will be based on
   virtual void update_time (double time);

   // Get the last calculated transformation matrix, based on the last time
   // set through update_time. rotation matrix returns through rot.
   void get_rotation (double rot[3][3]);

   // Same as get_rotation, but the returned matrix is the transpose of the
   // last calculated transformation matrix.
   void get_rotation_transpose (double rot[3][3]);

private: // private member functions

   // operator = and copy constructor locked from use by making them private
   PlanetRotation& operator = (const PlanetRotation& rhs);
   PlanetRotation (const PlanetRotation& rhs);
};

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
