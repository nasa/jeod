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
 * @addtogroup DerivedState
 * @{
 *
 * @file models/dynamics/derived_state/include/solar_beta_derived_state.hh
 * A class for calculating the solar beta of a vehicle
 */

/******************************************************************************
PURPOSE:
    ()

LIBRARY DEPENDENCY:
   ((../src/solar_beta_derived_state.cc))

******************************************************************************/

#ifndef JEOD_SOLAR_BETA_DERIVED_STATE_HH
#define JEOD_SOLAR_BETA_DERIVED_STATE_HH

// System includes

// JEOD includes
#include "dynamics/dyn_manager/include/class_declarations.hh"
#include "environment/planet/include/class_declarations.hh"
#include "utils/ref_frames/include/ref_frame_state.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "class_declarations.hh"
#include "derived_state.hh"


//! Namespace jeod
namespace jeod {

/**
 * The class for calculating the solar beta of a vehicle
 *
 * \par Assumptions and Limitations
 *  - The vehicle must be in orbit about the named planet
 */
class SolarBetaDerivedState : public DerivedState {

JEOD_MAKE_SIM_INTERFACES(SolarBetaDerivedState)


 // Member data

 public:

   // Note:
   // That planet and sun have public visibility is deprecated.

   /**
    * The planet, the name of which is specified by the inherited
    * reference_name data member.
    */
   Planet * planet; //!< trick_units(--)

   /**
    * The sun, found by looking for the planetary object named "Sun".
    */
   Planet * sun; //!< trick_units(--)

   /**
    * The angle between the orbital plane and the sun position vector.
    * Positive solar beta will be on the side of the positive angular momentum
    * vector.
    */
   double solar_beta; //!< trick_units(rad)

   /**
    * Indicates whether the model is "active".
    *
    * This flag defaults to true. This default value results in
    * - Initialization-time subscriptions issued against the
    * planet-centered and sun-centered inertial frames to
    * ensure that the planet and the Sun are a part of the active
    * reference frame tree, and
    * - Run-time calculations of the solar beta angle,
    * updated by calls to SolarBetaDerivedState::update.
    *
    * Setting the active flag to false prior to the call to
    * SolarBetaDerivedState::initialize permanently disables the model.
    * Setting the active flag to true after having set it to false prior to
    * the call to initialize initially will have no effect; the model
    * is permanently disabled.
    *
    * If the value of the flag is true at the time of the call to
    * SolarBetaDerivedState::initialize, subsequently setting the flag to false
    * will temporarily disable the model (cause SolarBetaDerivedState::update
    * to immediately return). Updates will once again be performed when the
    * flag is set to true.
    */
   bool active; //!< trick_units(--)


 // Methods

 public:

   // Default constructor
   SolarBetaDerivedState ();
   ~SolarBetaDerivedState () override;

   // initialize(): Initialize the DerivedState (but not necessarily the
   // state itself.)
   // Rules for derived classes:
   // All derived classes must forward the initialize() call to the immediate
   // parent class and then perform class-dependent object initializations.
   void initialize (DynBody & subject_body, DynManager & dyn_manager) override;

   // update(): Update the DerivedState representation of the subject DynBody.
   // Rules for derived classes:
   // All derived classes must perform class-dependent actions and then
   // must forward the update() call to the immediate parent class.
   void update (void) override;

 protected:

   /**
    * The state of the vehicle with respect to the planet
    */
   RefFrameState veh_wrt_planet; //!< trick_units(--)

   /**
    * The position of the sun with respect to the planet
    */
   double sun_wrt_planet[3]; //!< trick_units(m)


 // The copy constructor and assignment operator for this class are
 // declared private and are not implemented.
 private:

   SolarBetaDerivedState (const SolarBetaDerivedState&);
   SolarBetaDerivedState & operator = (const SolarBetaDerivedState&);

};

} // End JEOD namespace

#ifdef TRICK_VER
#include "environment/planet/include/planet.hh"
#endif

#endif

/**
 * @}
 * @}
 * @}
 */
