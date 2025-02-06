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
 * @addtogroup Gravity
 * @{
 *
 * @file models/environment/gravity/include/gravity_source.hh
 * Define the gravity body base (pure virtual) class.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((none)))

Assumptions and limitations:
  ((none))

Library dependencies:
  ((gravity_source.o))


*******************************************************************************/


#ifndef JEOD_GRAVITY_BODY_HH
#define JEOD_GRAVITY_BODY_HH


// System includes
#include <vector>
#include <string>

// JEOD includes
#include "environment/ephemerides/ephem_interface/include/class_declarations.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "class_declarations.hh"


//! Namespace jeod
namespace jeod {

class EphemerisRefFrame;

/**
 * Models the gravity for a specific planet; pure virtual.
 */
class GravitySource {

 JEOD_MAKE_SIM_INTERFACES(GravitySource)

 // Member data
 public:
   /**
    * The name of the source (i.e. associated planet or planetoid)
    * The GravitySource object, the BasePlanet derived object that points to the
    * GravitySource object, and the EphemerisPlanet that enables populating the
    * planetoid's inertial RefFrame object must all have the exact same name.
    */
   std::string name; //!< trick_units(--)

   /**
    * The pseudo-inertial frame associated with this gravity source. Used for
    * most basic gravity calculations
    * planet represented by this
    */
   EphemerisRefFrame* inertial; //!< trick_units(--)

   /**
    * Planetoid fixed frame. The Cartesian reference frame centered and fixed
    * on the associated gravity source. Used for advanced (e.g. nonspherical
    * gravity effects.
    */
   EphemerisRefFrame* pfix; //!< trick_units(--)

   /**
    * The planet's standard gravitational parameter, G times planet mass.
    */
   double mu; //!< trick_units(m3/s2)

   /**
    * Relative states with respect to this body, for each integration frame
    */
   GravityIntegFrame * frames;   //!< trick_units(--)


 // Make the copy constructor and assignment operator private
 // (and unimplemented) to avoid erroneous copies
 private:

   /**
    * Not implemented.
    */
   GravitySource (const GravitySource &);

   /**
    * Not implemented.
    */
   GravitySource & operator= (const GravitySource &);


 public:

   // Default constructor
   GravitySource ();

   // Destructor
   virtual ~GravitySource ();


   // State initializer function
   virtual void initialize_state (
         const std::vector<EphemerisRefFrame *> & integ_frames,  /* In: --
            All possible integration frames */
         const GravityManager & gravity_manager);    /* In: -- Gravity Manager */

};


} // End JEOD namespace

#ifdef TRICK_VER
#include "gravity_integ_frame.hh"
#endif


#endif

/**
 * @}
 * @}
 * @}
 */
