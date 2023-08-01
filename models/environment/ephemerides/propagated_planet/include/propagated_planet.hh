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
 * @addtogroup Ephemerides
 * @{
 * @addtogroup PropagatedPlanet
 * @{
 *
 * @file models/environment/ephemerides/propagated_planet/include/propagated_planet.hh
 * Define the classes needed to propagate a planet.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/propagated_planet.cc))


*******************************************************************************/

#ifndef JEOD_PROPAGATED_PLANET_HH
#define JEOD_PROPAGATED_PLANET_HH


// System includes

// JEOD includes
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "environment/ephemerides/ephem_interface/include/ephem_interface.hh"
#include "environment/ephemerides/ephem_item/include/ephem_item.hh"
#include "environment/ephemerides/ephem_item/include/ephem_orient.hh"
#include "environment/ephemerides/ephem_item/include/ephem_point.hh"
#include "utils/sim_interface/include/jeod_class.hh"


//! Namespace jeod
namespace jeod {

class BasePlanet;
class BodyRefFrame;
class DynManager;
class EphemeridesManager;
class TimeDyn;
class TimeManager;


/**
 * A PropagatedEphemerisPlanet is an EphemerisPoint whose state
 * is coupled with the translational state of a DynBody reference frame.
 *
 * This class is intended for use by the PropagatedPlanet class. Use outside of
 * the PropagatedPlanet is not sanctioned.
 *
 * The inherited enabled flag takes on an additional meaning in this derived
 * class. The base class meaning of this flag is that the ephemeris item, or the
 * ephemeris model that owns the item, is responsible for maintaining the
 * translational state of the target frame when the item is enabled but not when
 * the item is disabled.
 *
 * For this class, when the enabled flag is set still means that the item is
 * responsible for maintaining the  translational state of the target ephemeris
 * reference frame. The dynamic body serves as the source of this state.
 * When the enabled flag is clear, some other model is assumed to be responsible
 * for maintaining the target frame state. The target frame state serves as the
 * source of the dynamic body's state when the enabled flag is clear.
 */
/**
 * A PropagatedEphemerisPlanet is an EphemerisPoint whose state
 * is coupled with the translational state of a DynBody reference frame.
 *
 * This class is intended for use by the PropagatedPlanet class. Use outside of
 * the PropagatedPlanet is not sanctioned.
 *
 * The class acts analogously to the class PropagatedEphemerisOrientation, but
 * for translation rather than rotation. See PropagatedEphemerisOrientation
 * for a description of the behavior of the class.
 */
class PropagatedEphemerisPlanet : public EphemerisPoint {
JEOD_MAKE_SIM_INTERFACES(PropagatedEphemerisPlanet)

public:
   // Member functions
   // Note: The copy constructor and assignment operator are deleted.

   // Constructor and destructor

 // Member functions
   // Constructor (non-default) and destructor
   PropagatedEphemerisPlanet (DynBody & dyn_body, BodyRefFrame & frame);
   ~PropagatedEphemerisPlanet (void) override;

 // This class does not have copy constructor or assignment operator.
 private:
   /**
    * Not implemented.
    */
   PropagatedEphemerisPlanet (const PropagatedEphemerisPlanet &);
   /**
    * Not implemented.
    */
   PropagatedEphemerisPlanet & operator= (const PropagatedEphemerisPlanet &);

 public:

   // Update the state.
   using EphemerisPoint::update; /* ** */
   virtual void update (double time);


 // Member data
 protected:

   /**
    * The dynamic body whose state is tied to that of the planet.
    */
   DynBody & body; //!< trick_units(--)

   /**
    * The body reference frame whose translational state is coupled with that
    * of the planet's inertial frame.
    */
   BodyRefFrame & body_ref_frame; //!< trick_units(--)

};


/**
 * A PropagatedEphemerisOrientation is an EphemerisOrientation whose state is
 * coupled with the rotational state of a DynBody reference frame.
 *
 * This class is intended for use by the PropagatedPlanet class. Use outside of
 * the PropagatedPlanet is not sanctioned.
 *
 * The class acts analogously to the class PropagatedEphemerisPlanet, but
 * for rotation rather than translation. See PropagatedEphemerisPlanet
 * for a description of the behavior of the class.
 */
class PropagatedEphemerisOrientation : public EphemerisOrientation {
JEOD_MAKE_SIM_INTERFACES(PropagatedEphemerisOrientation)

public:

   // Member functions
   // Note: The copy constructor and assignment operator are deleted.

   // Constructor (non-default) and destructor
   PropagatedEphemerisOrientation (DynBody & dyn_body, BodyRefFrame & frame);
   ~PropagatedEphemerisOrientation (void) override;


 // Member functions
 public:
   // Constructor (non-default) and destructor

 public:

   // Update the state
   virtual void update (double time);


 // Member data
 protected:

   /**
    * The dynamic body whose state is tied to that of the planet.
    */
   DynBody & body; //!< trick_units(--)

   /**
    * The body reference frame whose rotational state is coupled with that
    * of the planet's planet-fixed frame.
    */
   BodyRefFrame & body_ref_frame; //!< trick_units(--)


private:
   ///
   /// Not implemented.
   PropagatedEphemerisOrientation (const PropagatedEphemerisOrientation &);
   ///
   /// Not implemented.
   PropagatedEphemerisOrientation & operator= (
      const PropagatedEphemerisOrientation &);
};


/**
 * The PropagatedPlanet ephemeris model provides planetary state via a DynBody
 * object whose state is propagated using the JEOD state integration techniques.
 * Scenarios in which a simulation will use a PropagatedPlanet object include:
 * - An object such as an asteroid for which an ephemeris model is not readily
 *   available.
 * - An object such as a planet that is represented in some other ephemeris
 *   model but the simulation developer wants the planet to be propagated to
 *   ensure that the planet and the vehicles operating in the vicinity of the
 *   planet obey the same laws of physics.
 *
 * The PropagatedPlanet model provides mechanisms that accommodate these
 * scenarios. The class defines multiple modes in which a propagated planet
 * planet object operates. In all modes, the model ensures consistency between
 * the translational states of the dynamic body's composite frame and the
 * planet's planet-centered frame and between the rotational states of the
 * dynamic body's composite frame and the planet's planet-fixed frame.
 */
class PropagatedPlanet: public EphemerisInterface {
JEOD_MAKE_SIM_INTERFACES(PropagatedPlanet)

public:

   // Types

   /**
    * Defines the modes in which an active PropagatedPlanet object operates.
    * A PropagatedPlanet contains a BasePlanet pointer PropagatedPlanet::planet
    * and a DynBody PropagatedPlanet::body. The translational states of the
    * planet-centered inertial frame and the body's composite_body frame are
    * tied to one another, as are the rotational states of the planet-fixed
    * frame and the the body's composite_body frame. This enum identifies
    * which of the planet or the body is the source of translational and the
    * the rotational parts of the state.
    */
   enum Mode {
      TransFromPlanet_RotFromPlanet = 0, /*
         The planet-centered inertial frame and planet-fixed frame are the
         sources of the translational and rotational states. The dynamic body's
         composite frame is slaved to these planetary sources. */
      TransFromPlanet_RotFromBody   = 1, /*
         The planet-centered inertial frame is the source of the translational
         state and the dynamic body's composite frame is the source of the
         rotational state. The composite frame's translational state and the
         planet-fixed frame's rotational state are slaved to these sources. */
      TransFromBody_RotFromPlanet   = 2, /*
         The dynamic body's composite frame is the source of the translational
         state and the planet-fixed frame is the source of the rotational state.
         The planet-centered inertial translational state and the composite
         frame's rotational state are slaved to these sources. */
      TransFromBody_RotFromBody     = 3  /*
         The dynamic body's composite frame is the source of the translational
         and rotational states. The planet-centered inertial frame and the
         planet-fixed frame are slaved to these dynamic body sources. */
   };


   // Member functions
   // Note: The copy constructor and assignment operator are deleted.

   // Constructor and destructor
   PropagatedPlanet (void);
   ~PropagatedPlanet (void) override;


   // S_define-level interfaces

   // Initialize the model
   void initialize_model (
      const TimeManager & time_manager, DynManager & dyn_manager);

   // Close the model (free resources)
   void shutdown (void);

   // EphemInterface methods
   void activate (void) override;
   void deactivate (void) override;

   // EphemInterface accessors
   double timestamp (void) const override;

   const char * get_name (void) const override;

   // EphemeridesManager interface methods
   void ephem_initialize (EphemeridesManager & ephem_manager) override;

   void ephem_activate (EphemeridesManager & ephem_manager) override;

   void ephem_build_tree (EphemeridesManager & ephem_manager) override;

   void ephem_update (void) override;

   // Model-specific interfaces
   // Set the commanded mode.
   void set_commanded_mode (Mode new_mode);


   // Member data

   /**
    * The name of the planet.
    * This is used at initialization time only.
    */
   char * planet_name; //!< trick_units(--)

   /**
    * The name of the parent frame.
    * This is used at initialization time only.
    */
   char * parent_name; //!< trick_units(--)

   /**
    * The dynamic body whose state is tied to that of the planet.
    */
   DynBody body; //!< trick_units(--)

   /**
    * The mode in which the model should operate.
    */
   Mode commanded_mode; //!< trick_units(--)


protected:

   // Member functions

   // Set the mode.
   void set_mode (void);


   // Member data

   /**
    * Has the model been initialized?
    */
   bool initialized; //!< trick_units(--)

   /**
    * The mode in which the model is operating.
    */
   Mode mode; //!< trick_units(--)

   /**
    * Model name; used for reporting errors.
    */
   char * ident; //!< trick_units(--)

   /**
    * Is the planet present and marked as active?
    */
   bool active; //!< trick_units(--)

   /**
    * Time of last update, dynamic time seconds
    */
   double update_time; //!< trick_units(s)

   /**
    * The planet tied to the body.
    */
   BasePlanet * planet; //!< trick_units(--)

   /**
    * The parent of the planet.
    */
   EphemerisRefFrame * parent_frame; //!< trick_units(--)

   /**
    * The dynamics manager.
    */
   DynManager * dyn_manager; //!< trick_units(--)

   /**
    * The source of dynamic time information.
    */
   const TimeDyn * time_dyn; //!< trick_units(--)

   /**
    * The ephemeris item that couples the translational states of the
    * body's composite body frame  and the planet's inertial frame.
    */
   PropagatedEphemerisPlanet ephem_planet; //!< trick_units(--)

   /**
    * The ephemeris item that couples the rotational states of the
    * body's composite body frame  and the planet's planet-fixed frame.
    */
   PropagatedEphemerisOrientation ephem_orient; //!< trick_units(--)


private:

   // Make the copy constructor and assignment operator private
   // (and unimplemented) to avoid erroneous copies

   ///
   /// Not implemented.
   PropagatedPlanet (const PropagatedPlanet &);
   ///
   /// Not implemented.
   PropagatedPlanet & operator= (const PropagatedPlanet &);
};


} // End JEOD namespace

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
