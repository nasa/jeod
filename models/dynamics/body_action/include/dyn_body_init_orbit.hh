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
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/include/dyn_body_init_orbit.hh
 * Define the class DynBodyInitOrbit, which initializes a vehicle in
 * in some orbit.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/dyn_body_init_orbit.cc))



*******************************************************************************/


#ifndef JEOD_DYN_BODY_INIT_ORBIT_HH
#define JEOD_DYN_BODY_INIT_ORBIT_HH

// System includes

// JEOD includes
#include "environment/ephemerides/ephem_interface/include/class_declarations.hh"
#include "environment/planet/include/class_declarations.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "dyn_body_init_trans_state.hh"


//! Namespace jeod
namespace jeod {

/**
 * Initialize a vehicle's translational state given an orbital specification.
 */
class DynBodyInitOrbit : public DynBodyInitTransState {

 JEOD_MAKE_SIM_INTERFACES(DynBodyInitOrbit)


 // Enumerations

 public:

   /**
    * Identifies which orbital elements define the orbit.
    * The goofy numbering scheme here is intentional.
    * The numbers map directly to the corresponding orbital_set number in
    * JEOD 1.4 / 1.5.
    * NOTE: Orbital sets 4 and 11 are the same options.
    */
   enum OrbitalSet {
      InvalidSet =  0, /*
         Not a valid choice; the default */

      SmaEccIncAscnodeArgperTimeperi =  1, /*
          Semi-major axis,
          Eccentricity,
          Inclination,
          Asc. node (right ascension or longitude),
          Argument of periapsis,
          Time since periapsis passage */

      SmaEccIncAscnodeArgperManom =  2, /*
          Semi-major axis,
          Eccentricity,
          Inclination,
          Asc. node (right ascension or longitude),
          Argument of periapsis,
          Mean anomaly */

      SlrEccIncAscnodeArgperTanom =  3, /*
          Semi-latus rectum,
          Eccentricity,
          Inclination,
          Asc. node (right ascension or longitude),
          Argument of periapsis,
          True anomaly */

      IncAscnodeAltperAltapoArgperTanom =  4, /*
          Inclination,
          Asc. node (right ascension or longitude),
          Perifocal altitude,
          Apofocal altitude,
          Argument of periapsis,
          True anomaly */

      IncAscnodeAltperAltapoArgperTimeperi =  5, /*
          Inclination,
          Asc. node (right ascension or longitude),
          Perifocal altitude,
          Apofocal altitude,
          Argument of periapsis,
          Time since periapsis passage */

      SmaIncAscnodeArglatRadRadvel =  6, /*
          Semi-major axis,
          Inclination,
          Asc. node (right ascension or longitude),
          Argument of latitude,
          Radial distance,
          Radial component of velocity */

      SmaEccIncAscnodeArgperTanom =  10, /*
          Semi-major axis,
          Eccentricity,
          Inclination,
          Asc. node (right ascension or longitude),
          Argument of periapsis,
          True anomaly */

      CaseEleven =  11, /*
          Inclination,
          Asc. node (right ascension or longitude),
          Perifocal altitude,
          Apofocal altitude,
          Argument of periapsis,
          True anomaly */
   };


 // Member data

 public:

   /**
    * The name of the planet around which the orbit is to be established.
    * This must be supplied, must name a planet, and the planet must have
    * a gravity model.
    */
   std::string planet_name; //!< trick_units(--)

   /**
    * Planet reference frame name, optionally dot-prefixed with the planet name.
    * If this specifies a rotating frame, a non-rotating frame instantaneously
    * co-aligned with the rotating frame is assumed.
    */
   std::string orbit_frame_name; //!< trick_units(--)

   /**
    * Specifies which set of orbital elements specify the orbit.
    */
   OrbitalSet set; //!< trick_units(--)

   /**
    * Semi-major axis
    */
   double semi_major_axis; //!< trick_units(m)

   /**
    * Semi-latus rectum
    */
   double semi_latus_rectum; //!< trick_units(m)

   /**
    * Periapsis altitude
    */
   double alt_periapsis; //!< trick_units(m)

   /**
    * Apoapsis altitude
    */
   double alt_apoapsis; //!< trick_units(m)

   /**
    * Distance from center of planet
    */
   double orb_radius; //!< trick_units(m)

   /**
    * Time derivative of the orbital radius
    */
   double radial_vel; //!< trick_units(m/s)

   /**
    * Eccentricity
    */
   double eccentricity; //!< trick_units(--)

   /**
    * Inclination
    */
   double inclination; //!< trick_units(rad)

   /**
    * Longitude (or right ascension) of ascending node
    */
   double ascending_node; //!< trick_units(rad)

   /**
    * Argument of periapsis
    */
   double arg_periapsis; //!< trick_units(rad)

   /**
    * Argument of latitude
    */
   double arg_latitude; //!< trick_units(rad)

   /**
    * Time since periapsis passage
    */
   double time_periapsis; //!< trick_units(s)

   /**
    * Mean anomaly
    */
   double mean_anomaly; //!< trick_units(rad)

   /**
    * True anomaly
    */
   double true_anomaly; //!< trick_units(rad)


 protected:

   /**
    * The planet
    */
   Planet * planet; //!< trick_io(**)

   /**
    * The orbit reference frame (ignoring rotation)
    */
   EphemerisRefFrame * orbit_frame; //!< trick_io(**)


 // Member functions

 public:

   DynBodyInitOrbit ();

   ~DynBodyInitOrbit () override;

   void initialize (DynManager & dyn_manager) override;

   void apply (DynManager & dyn_manager) override;
};

} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 */
