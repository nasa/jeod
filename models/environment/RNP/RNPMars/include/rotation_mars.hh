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
 * @addtogroup Environment
 * @{
 * @addtogroup RNP
 * @{
 * @addtogroup RNPMars
 * @{
 *
 * @file models/environment/RNP/RNPMars/include/rotation_mars.hh
 * Represent the axial rotation portion of the JPL-developed "Pathfinder" Mars
 * orientation model.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((Konopliv, Alex S., et al)
    (A global solution for the Mars static and seasonal gravity, Mars
     orientation, Phobos and Deimos masses, and Mars ephemeris)
    (Icarus, Volume 182, Issue 1, Pages 23-50) (May 2006))
   ((Konopliv, Alex S., et al)
    (Mars high resolution gravity fields from MRO, Mars season gravity, and
     other dynamical parameters)
    (Icarus, Volume 211, Issue 1, Pages 401-428) (January 2011)))

Assumptions and limitations:
  ((Mars specific))

Library dependencies:
  ((../src/rotation_mars.cc))

 

*******************************************************************************/

#ifndef JEOD_ROTATION_MARS_HH
#define JEOD_ROTATION_MARS_HH


// System includes

// JEOD includes
#include "environment/RNP/GenericRNP/include/planet_rotation.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "nutation_mars.hh"


//! Namespace jeod
namespace jeod {

/**
 * Implements the axial rotation portion of the "Pathfinder" Mars RNP model.
 */
class RotationMars : public PlanetRotation {

   JEOD_MAKE_SIM_INTERFACES(RotationMars)

// Public data members
public:
   /**
    * The Mars average axial rotational velocity.
    */
   double planet_rotational_velocity; //!< trick_units(rad/s)

   /**
    * Pointer to the Mars nutation object, used to access nutation in longitude
    * information. Will be NULL (automatically) if anything but Full_Term_RNP
    * is set in the RNPMars object.
    */
   NutationMars* nutation; //!< trick_units(--)

   /**
    * Tells the rotation object if it should use a full blown rotation
    * formulation, or if it should just use the time passed multiplied by the
    * rotational velocity.  Used with the different intialization options for
    * the main RNP class.
    */
   bool use_full_rnp; //!< trick_units(--)

   /**
    * The (constant) rotated angle of Mars at the J2000 epoch.
    */
   double phi_at_j2000; //!< trick_units(rad)

   /**
    * The most recent calculated value of the rotation angle for Mars, measured
    * since J2000 epoch.
    */
   double phi_spin; //!< trick_units(rad)


// Private data members
private:


// Public member functions
public:
   RotationMars ();

   ~RotationMars () override;

   // RotationMars specific implementation of update rotation
   // Before this is called, the current_time parameter must be set to
   // Julian days since standard epoch J2000, per Konopliv references.
   void update_rotation () override;

// Private member functions
private:
   // Lock away the copy constructor and operator = by making them private.
   RotationMars& operator = (const RotationMars& rhs);
   RotationMars (const RotationMars& rhs);

};

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
