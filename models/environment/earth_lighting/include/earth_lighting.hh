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
 * @addtogroup EarthLighting
 * @{
 *
 * @file models/environment/earth_lighting/include/earth_lighting.hh
 * Calculates lighting information from the Earth, Sun and Moon for a vehicle
 * in low Earth orbit
 */

/******************************************************************************
PURPOSE:
    ()
REFERENCES:
    (((None)))
ASSUMPTIONS AND LIMITATIONS:
    ((User specifies phase of the moon)
     (Earth Albedo is crudely calculated using a perfect reflector assumption)
     (This function is only good for a vehicle in low earth orbit)
     (The DynManager that the EarthLighting object is initialized from must
      include ephemeris objects named specifically "Earth", "Moon" and "Sun"))
LIBRARY DEPENDENCY:
    ((earth_lighting.o))

******************************************************************************/

#ifndef JEOD_EARTH_LIGHTING_HH
#define JEOD_EARTH_LIGHTING_HH


// JEOD includes
#include "dynamics/dyn_manager/include/class_declarations.hh"
#include "environment/planet/include/class_declarations.hh"
#include "utils/ref_frames/include/class_declarations.hh"
#include "utils/sim_interface/include/jeod_class.hh"




//! Namespace jeod
namespace jeod {

/**
 * Represents a major source of light in a space environment, such as the sun, the Earth, the moon, etc.
 */
class LightingBody {

  JEOD_MAKE_SIM_INTERFACES(LightingBody)

public:

   // constructor
   LightingBody ();

   // destructor
   ~LightingBody ();

   /**
    * Celestial body mean equitorial radius.
    */
   double radius;      //!< trick_units(m)
   /**
    * Inertial position relative to observer.
    */
   double position[3]; //!< trick_units(m)
   /**
    * Distance from observer to light body.
    */
   double distance;    //!< trick_units(m)
   /**
    * Apparent half angle of body disk.
    */
   double half_angle;  //!< trick_units(rad)


protected:

private:

   // operator = and copy constructor locked from use because they are private
   LightingBody& operator = (const LightingBody& rhs);
   LightingBody (const LightingBody& rhs);

};

/**
 * Contains important parameters for lighting information.
 */
class LightingParams {

  JEOD_MAKE_SIM_INTERFACES(LightingParams)

public:

   // constructor
   LightingParams ();

   // destructor
   ~LightingParams ();

   /**
    * Apparent observation angle from light source.
    */
   double obs_angle; //!< trick_units(rad)

   /**
    * Apparent lighting phase of planet.
    */
   double phase;     //!< trick_units(--)

   /**
    * Fraction of planetary surface occlusion.
    */
   double occlusion; //!< trick_units(--)
   /**
    * Fraction of planetary surface visible.
    */
   double visible;   //!< trick_units(--)
   /**
    * Fraction of lighting (phase * visible).
    */
   double lighting;  //!< trick_units(--)

protected:

private:

   // operator = and copy constructor locked from use because they are private
   LightingParams& operator = (const LightingParams& rhs);
   LightingParams (const LightingParams& rhs);

};

/**
 * A class for calculating lighting effects in low Earth orbit.
 */
class EarthLighting {

  JEOD_MAKE_SIM_INTERFACES(EarthLighting)

public:

   // constructor
   EarthLighting ();

   // destructor
   ~EarthLighting ();

   void initialize (DynManager& manager);

   // note: units were not specified in JEOD 2.0, and should not matter
   // as long as you stay self consistent.
   int circle_intersect (
      double r_bottom, double r_top, double d_centers, double* area);

   void calc_lighting (double pos_veh[3]);

   /**
    * flag for if the model is active or not
    */
   bool active; //!< trick_units(--)

   /**
    * Pointer to the Earth planet from the DynManager
    */
   Planet* earth; //!< trick_units(--)
   /**
    * Pointer to the Moon planet from the DynManager
    */
   Planet* moon; //!< trick_units(--)
   /**
    * Pointer to the Sun planet from the DynManager
    */
   Planet* sun; //!< trick_units(--)

   /**
    * Pointer to the translation information for Earth inertial
    */
   const RefFrame* earth_frame; //!< trick_units(--)
   /**
    * Pointer to the translation information for Moon inertial
    */
   const RefFrame* moon_frame; //!< trick_units(--)
   /**
    * Pointer to the translation information for Sun inertial
    */
   const RefFrame* sun_frame; //!< trick_units(--)

   /**
    * Sun stellar parameters.
    */
   LightingBody sun_body; //!< trick_units(--)
   /**
    * Earth planetary parameters.
    */
   LightingBody earth_body; //!< trick_units(--)
   /**
    * Lunar planetary parameters.
    */
   LightingBody moon_body;  //!< trick_units(--)

   /**
    * Lighting of sun w.r.t. vehicle.
    */
   LightingParams sun_earth;    //!< trick_units(--)
   /**
    * Lighting of moon w.r.t. vehicle.
    */
   LightingParams moon_earth;   //!< trick_units(--)
   /**
    * Earth albedo lighting.
    */
   LightingParams earth_albedo; //!< trick_units(--)

protected:


   // These values are calculated when calc_lighting is called. Should
   // not be filled in, and filled in values will not be used
   /**
    * Moon position wrt Earth inertial
    */
   double pos_moon[3]; //!< trick_units(m)
   /**
    * Sun position wrt Earth inertial
    */
   double pos_sun[3]; //!< trick_units(m)

private:

   // copy constructor and operator = locked from use
   EarthLighting& operator = (const EarthLighting& rhs);
   EarthLighting (const EarthLighting& rhs);

};


} // End JEOD namespace

#ifdef TRICK_VER
#include "environment/planet/include/planet.hh"
#include "utils/ref_frames/include/ref_frame.hh"
#endif

#endif

/**
 * @}
 * @}
 * @}
 */
