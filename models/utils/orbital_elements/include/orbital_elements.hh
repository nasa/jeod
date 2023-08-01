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
 * @addtogroup Utils
 * @{
 * @addtogroup OrbitalElements
 * @{
 *
 * @file models/utils/orbital_elements/include/orbital_elements.hh
 * Orbital elements class definition.
 */

/*****************************************************************************
PURPOSE:
    ()

REFERENCE:
    (((Vallado, David A.) (Fundamentals of Astrodynamics and Applications)
      (McGraw-Hill) (New York) (1997)))

ASSUMPTIONS AND LIMITATIONS:
    ((none))
 Library dependencies:
  ((../src/orbital_elements.cc))

 
******************************************************************************/

#ifndef ORBITAL_ELEMENTS_HH
#define ORBITAL_ELEMENTS_HH


// System includes
#include <string>

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod {

/**
 * Represents state in terms of Keplerian orbital elements.
 */
class OrbitalElements {

 JEOD_MAKE_SIM_INTERFACES(OrbitalElements)


  // Member data
  public:
   // Orbit definition parameters
   /**
    * Semi-major-axis (a)
    */
   double semi_major_axis; //!< trick_units(m)
   /**
    * Semiparameter (p)
    */
   double semiparam; //!< trick_units(m)
   /**
    * Magnitude of eccentricity (e)
    */
   double e_mag; //!< trick_units(--)
   /**
    * Orbit inclination (i)
    */
   double inclination; //!< trick_units(rad)
   /**
    * Argument of periapsis (w)
    */
   double arg_periapsis; //!< trick_units(rad)
   /**
    * Longitude of ascending node (Omega)
    */
   double long_asc_node; //!< trick_units(rad)

   // Orbital position parameters
   /**
    * Magnitude of orbital radius
    */
   double r_mag; //!< trick_units(m)
   /**
    * Magnitude of orbital velocity
    */
   double vel_mag; //!< trick_units(m/s)
   /**
    * True Anomaly (v)
    */
   double true_anom; //!< trick_units(rad)
   /**
    * Mean Anomaly (M)
    */
   double mean_anom; //!< trick_units(rad)
   /**
    * Mean motion of orbit (n)
    */
   double mean_motion; //!< trick_units(rad/s)
   /**
    * Eccentric (E), Hyperbolic (H), or Parabolic (B) anomaly
    */
   double orbital_anom; //!< trick_units(rad)
   // "Global" working parameters
   /**
    * Sine of the true anomaly
    */
   double sin_v; //!< trick_units(--)
   /**
    * Cosine of the true anomaly
    */
   double cos_v; //!< trick_units(--)
   /**
    * Specific orbital energy
    */
   double orb_energy; //!< trick_units(m2/s2)
   /**
    * Specific orbital angular momentum
    */
   double orb_ang_momentum; //!< trick_units(m2/s)

  protected:
   /**
    * Name of orbital object
    */
   std::string object_name; //!< trick_units(--)
   /**
    * Name of planet about which the object orbits
    */
   std::string planet_name; //!< trick_units(--)


  // Member functions
  // Make the copy constructor and assignment operator private
  // (and unimplemented) to avoid erroneous copies
  private:
   OrbitalElements (const OrbitalElements &);
   OrbitalElements & operator= (const OrbitalElements &);

  public:

   // Constructor & Destructor
   OrbitalElements();
   virtual ~OrbitalElements();

   // Get the names
   const char * get_object_name ( void ) const;
   const char * get_planet_name ( void ) const;

   // Set the names
   void set_object_name (const char * name);
   void set_planet_name (const char * name);

   // Transformation routines
   int from_cartesian (double mu, const double pos[3], const double vel[3]);
   int to_cartesian (double mu, double pos[3], double vel[3]);

   // Utility routines
   int nu_to_anomalies();
   int mean_anom_to_nu();

  protected:

   int KepEqtnE (double M, double e, double * E);

   int KepEqtnH (double M, double e, double * H);

   int KepEqtnB (double M, double * B);

};

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
