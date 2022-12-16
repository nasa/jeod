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
 * @file models/environment/RNP/RNPMars/src/rnp_mars.cc
 * Implementation of RNPMars
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
((rnp_mars.o)
(nutation_mars.o)
(precession_mars.o)
(rotation_mars.o)
(environment/RNP/GenericRNP/planet_rnp.o)
(environment/RNP/GenericRNP/RNP_messages.o)
(environment/time/time_tt.o)
(utils/message/message_handler.o))

 
*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "environment/time/include/time_manager.hh"
#include "environment/time/include/time_tt.hh"
#include "environment/time/include/time_dyn.hh"
#include "environment/planet/include/planet.hh"
#include "environment/RNP/GenericRNP/include/RNP_messages.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/rnp_mars.hh"


//! Namespace jeod
namespace jeod {

/**
 * Default constructor
 */
RNPMars::RNPMars (
   void)
:
   internal_name("RNPMars"),
   tt_ptr(NULL),
   time_dyn_ptr(NULL),
   last_updated_time_full(0.0),
   never_updated_full(true),
   last_updated_time_rotational(0.0),
   never_updated_rotational(true)
{

   // Disable polar motion to reverse default inherited setting; "Pathfinder"
   // Mars RNP model does not provide polar motion as of 2010
   enable_polar = false;

   // Assign pointers for polymorphic functionality; note that the Pathfinder
   // Mars RNP model does not include polar motion as of 2010 due to
   // difficulties in determining the coefficient values.
   nutation     = &this->NMars;
   precession   = &this->PMars;
   rotation     = &this->RMars;
   polar_motion = NULL;

}


/**
 * Destructor
 */
RNPMars::~RNPMars ()
{ }


/**
 * Perform setup of the object, then call the base class initializer.
 * \param[in,out] dyn_manager Ref to dynamics manager
 */
void
RNPMars::initialize (
   DynManager& dyn_manager)
{

   // Copy value from higher up the polymorphic inheritance chain
   RMars.planet_rotational_velocity = planet_omega;

   // Set the rotational option consistent with user selection
   if ((rnp_type == RotationOnly) || (rnp_type == ConstantNP)) {
      RMars.use_full_rnp = false;
   }
   else {
      RMars.use_full_rnp = true;
   }

   // Set up nutation, set pointers to it and initialize fixed matrices,
   // only if needed
   if (rnp_type == FullRNP) {
      RMars.nutation = &NMars;
      PMars.nutation = &NMars;
      PMars.compute_fixed_matrices();
   }

   // Call parent-class initializer
   PlanetRNP::initialize (dyn_manager);

   return;
}


/**
 * Update the complete RNP and supply it to the Dynamics Manager.
 * \param[in] time_tt Current Terrestrial Time
 */
void
RNPMars::update_rnp (
   TimeTT& time_tt)
{

   // Do nothing if model inactive
   if(!active) {
      return;
   }

   // If the DynTime pointer is empty, connect it
   if(time_dyn_ptr == NULL) {
      get_dyn_time_ptr (time_tt);
   }

   // If the TimeTT pointer hasn't been cached off yet, do so
   if(tt_ptr == NULL) {
      tt_ptr = &time_tt;
   }

   // Check if RNP needs update; if it has been previously updated and the
   // timestamp still matches, then it doesn't
   if(!never_updated_full &&
         (last_updated_time_full - time_dyn_ptr->seconds) == 0.0) {
      return;
   }

   // Proceed with full RNP update, including rotational portion
   never_updated_full = false;
   never_updated_rotational = false;
   last_updated_time_full = last_updated_time_rotational = time_dyn_ptr->seconds;

   double time = 0.0;
   time = time_tt.seconds;

   // Pass in TT to update all if user requested full RNP, or call rotational
   // update only and pass in simulation time to it if only rotation requested
   if (rotation != NULL) {
      if (rnp_type == FullRNP) {
         rotation->update_time (time);
         nutation->update_time (time);
         precession->update_time (time);
      }
      else {
         time = time_tt.time_manager->dyn_time.seconds;
         rotation->update_time (time);
      }
   }

   // Update the timestamp of the controlled reference frame
   planet->pfix.set_timestamp(time_dyn_ptr->seconds);

   // Pass the function call to the parent object
   PlanetRNP::update_rnp();

   return;
}


/**
 * Update rotation portion of RNP and supply RNP to dynamics manager.
 * \param[in] time_tt Current TT time
 */
void
RNPMars::update_axial_rotation (
   TimeTT& time_tt)
{

   // Do nothing if model inactive
   if(!active) {
      return;
   }

   // If the DynTime pointer is empty, connect it
   if(time_dyn_ptr == NULL) {
      get_dyn_time_ptr (time_tt);
   }

   // Check if RNP needs update; if it has been previously updated and the
   // timestamp still matches, then it doesn't
   if(!never_updated_rotational &&
         (last_updated_time_rotational - time_dyn_ptr->seconds) == 0.0) {
      return;
   }

   // Proceed with rotational RNP update only
   never_updated_rotational = false;
   last_updated_time_rotational = time_dyn_ptr->seconds;

   double time = 0.0;
   time = time_tt.seconds;

   // Pass in TT if user requested full RNP (meaning this is an intermediate
   // update), or pass in simulation time if only rotation being used
   if (rotation != NULL) {
      if (rnp_type == FullRNP) {
         rotation->update_time (time);
      }
      else {
         time = time_tt.time_manager->dyn_time.seconds;
         rotation->update_time (time);
      }
   }

   // Update the timestamp of the controlled reference frame
   planet->pfix.set_timestamp(time_dyn_ptr->seconds);

   // Pass the function call to the parent object
   PlanetRNP::update_axial_rotation();

   return;
}


/**
 * Return the last time at which the RNP was updated.
 */
double
RNPMars::timestamp() const {
   return last_updated_time_rotational;
}


/**
 * Return the internal name of the object.
 */
const char*
RNPMars::get_name() const {
   return internal_name.c_str();
}


/**
 * Define 'alias' to call axial update function, for polymorphism.
 */
void
RNPMars::ephem_update() {

   if(active && orient_interface.is_active()) {
      if(tt_ptr == NULL) {
         MessageHandler::inform (
               __FILE__, __LINE__, RNPMessages::setup_error,
               "RNPMars::ephem_update was called without a valid "
               "tt_ptr being set. It is normally set in the "
               "RNPMars::update_rnp function. RNP was not updated.\n");
         return;
      }

      // If everything is ok, then pass call through to axial update routine
      update_axial_rotation(*tt_ptr);
   }

   return;
}


/**
 * Get simulation time via a two-step pointer trail.
 * \param[in] time_tt Current TT time
 */
void
RNPMars::get_dyn_time_ptr(
   TimeTT& time_tt)
{

   // Check if pointer needs to be set; exit if not
   if(time_dyn_ptr != NULL) {
      return;
   }

   // Set the pointer by following the trail
   time_dyn_ptr =
      dynamic_cast<TimeDyn*>(time_tt.time_manager->get_time_ptr("Dyn"));

   if(time_dyn_ptr == NULL) {
      MessageHandler::fail (
            __FILE__, __LINE__, RNPMessages::setup_error,
            "The TimeManager associated with the given TimeTT "
            "did not contain a TimeDyn object.\n");
      return;
   }

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 * @}
 */
