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
 * @addtogroup RNPMars
 * @{
 *
 * @file models/environment/RNP/RNPMars/include/nutation_mars.hh
 * Represent the nutation portion of the JPL-developed "Pathfinder" Mars
 * orientation model.
 */

/*****************************************************************************

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
((nutation_mars.o))

 
*****************************************************************************/

#ifndef JEOD_NUTATION_MARS_HH
#define JEOD_NUTATION_MARS_HH


// System includes

// JEOD includes
#include "environment/RNP/GenericRNP/include/planet_rotation.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes


//! Namespace jeod
namespace jeod {

/**
 * Implements the nutation portion of the "Pathfinder" Mars RNP model.
 */
class NutationMars : public PlanetRotation {

   JEOD_MAKE_SIM_INTERFACES(NutationMars)

   // Public data members
public:
   /**
    * The latest calculated nutation correction term in longitude
    */
   double nutation_in_longitude; //!< trick_units(rad)

   /**
    * The latest calculated nutation correction term in obliquity
    */
   double nutation_in_obliquity; //!< trick_units(rad)

   /**
    * The (constant) obliquity angle of Mars at the J2000 epoch.
    */
   double I_at_j2000; //!< trick_units(rad)

   /**
    * The (constant) simple secular change in Mars obliquity relative to
    * the Mars mean orbit.
    */
   double I_dot; //!< trick_units(rad/s)

   /**
    * The latest calculated angle of obliquity, that is, the current orbit
    * inclination angle as measured relative to the Mars mean orbit, measured
    * since the J2000 epoch.
    */
   double obliquity_angle; //!< trick_units(rad)

   /**
    * The mean motion of Mars, that is, the average orbit arc traversed.
    */
   double mean_motion; //!< trick_units(rad/s)

   /**
    * The Mars mean anomaly at the J2000 epoch.
    */
   double mean_anomaly_j2000; //!< trick_units(rad)

   /**
    * Parameter q is a slowly varying angle defined in Konopliv 2006 as
    * 2 times w, where w is the argument of perihelion of the Mars orbit
    * relative to the node of the Mars equator and Mars mean orbit. This
    * data member is the value of q at the J2000 epoch.
    */
   double q_angle_j2000; //!< trick_units(rad)

   /**
    * The obliquity nutation amplitude parameters.
    */
   double * I_m_orig; //!< trick_units(rad)

   /**
    * The longitude nutation amplitude parameters.
    */
   double * psi_m_orig; //!< trick_units(rad)

   /**
    * 0 to 9 cast as doubles for use in summation
    */
   double * int_to_double; //!< trick_units(--)


   // Private data members
private:


   // Public member functions
public:
   NutationMars ();

   virtual ~NutationMars ();

   // NutationMars specific implementation of update rotation
   // Before this is called, the current_time parameter must be set to
   // Julian days since standard epoch J2000, per Konopliv references.
   virtual void update_rotation ();


   // Private member functions
private:
   // Lock away the copy constructor and operator = by making them private.
   NutationMars& operator = (const NutationMars& rhs);
   NutationMars (const NutationMars& rhs);

};

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
