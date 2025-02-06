/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Gravity
 * @{
 *
 * @file models/environment/gravity/src/gravity_interaction.cc
 * Define methods for the GravityInteraction class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((gravity_interaction.o)
   (gravity_source.o)
   (gravity_controls.o)
   (gravity_manager.o)
   (gravity_messages.o)
   (dynamics/dyn_body/dyn_body.o)
   (environment/ephemerides/ephem_interface/ephem_ref_frame.o)
   (environment/planet/planet.o))


*******************************************************************************/


// System includes
#include <cstddef>
#include <algorithm>

// JEOD includes
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "dynamics/dyn_manager/include/base_dyn_manager.hh"
#include "environment/ephemerides/ephem_interface/include/ephem_ref_frame.hh"
#include "environment/planet/include/planet.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"
#include "utils/memory/include/jeod_alloc.hh"


// Model includes
#include "../include/gravity_interaction.hh"
#include "../include/gravity_source.hh"
#include "../include/gravity_controls.hh"
#include "../include/gravity_manager.hh"
#include "../include/gravity_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a GravityInteraction instance.
 */
GravityInteraction::GravityInteraction (
   void)
:
   integ_frame_index(9999),
   grav_pot(0.0)
{
   JEOD_REGISTER_CLASS (GravityInteraction);
   JEOD_REGISTER_INCOMPLETE_CLASS (GravityControls);
   JEOD_REGISTER_CHECKPOINTABLE (this, grav_controls);

   Vector3::initialize (grav_accel);
   Matrix3x3::initialize (grav_grad);
}


/**
 * Destruct a GravityInteraction instance.
 */
GravityInteraction::~GravityInteraction (
   void)
{
   JEOD_DEREGISTER_CHECKPOINTABLE (this, grav_controls);
   grav_controls.clear();
}


/**
 * Set the integration frame and associated integration frame index.
 *
 * \par Assumptions and Limitations
 *  - Provided frame is a valid integration frame.
 * \param[in] ref_frame Integration frame
 * \param[in] dyn_manager Dynamics manager
 */
void
GravityInteraction::set_integ_frame (
   const EphemerisRefFrame & ref_frame,
   const BaseDynManager & dyn_manager)
{
   integ_frame_index = dyn_manager.find_integ_frame_index (ref_frame);
}


/**
 * Add a new GravityControls to the grav_controls list.
 * \param[in] control Control to be added
 */
void
GravityInteraction::add_control (
   GravityControls * control)
{
   // Check for a duplicate.
   if (std::find (grav_controls.begin(),
                  grav_controls.end(),
                  control) !=
       grav_controls.end()) {
      MessageHandler::warn (
         __FILE__, __LINE__, GravityMessages::duplicate_entry,
         "Gravity control requested to be added to the list of \n"
         "gravity controls is already in the list.\n"
         "Addition rejected.\n");
      return;
   }

   // Add the gravity control to end of list
   grav_controls.push_back (control);
}


/**
 * Remove a GravityControls from the grav_controls list.
 * \param[in] control GravityControls to be removed.
 */
void
GravityInteraction::remove_control (
   GravityControls* control)
{
   std::vector<GravityControls*>::iterator iter =
      std::find (grav_controls.begin(), grav_controls.end(), control);
   if (iter == grav_controls.end()) {
      MessageHandler::warn (
         __FILE__, __LINE__, GravityMessages::missing_entry,
         "Gravity control requested to be removed from the list of \n"
         "gravity controls is not in the list.\n"
         "Removal failed.\n");
      return;
   }

   grav_controls.erase (iter);
}


/**
 * Initialize all GravityControls in the grav_controls list.
 * \param[in] dyn_manager Ref to Dyn Manager
 * \param[in] grav_manager Ref to Gravity Manager
 */
void
GravityInteraction::initialize_controls (
   BaseDynManager & dyn_manager,
   GravityManager & grav_manager)
{

   // Perform initialization for each gravity control in list
   for (unsigned int ii = 0; ii < grav_controls.size(); ++ii) {

      grav_controls[ii]->initialize_control (grav_manager);
   }

   // Subscribe to the reference frames associated with the controls.
   reset_controls(dyn_manager);
}


/**
 * Reset all GravityControls in the grav_controls list.
 */
void
GravityInteraction::reset_controls ( BaseDynManager& manager )
{
   // Check subscriptions for each gravity control in list
   for (unsigned int ii = 0; ii < grav_controls.size(); ++ii) {

      grav_controls[ii]->reset_control (manager);
   }

   return;
}


/**
 * Sort the GravityControls in the grav_controls list
 * in increasing acceleration magnitude order.
 */
void
GravityInteraction::sort_controls (
   void)
{

   // Compute the magnitude of each gravity acceleration vector.
   for (unsigned int ii = 0; ii < grav_controls.size(); ++ii) {
      GravityControls & control_ii = *grav_controls[ii];
      if (control_ii.active) {
         control_ii.grav_accel_magsq = Vector3::vmagsq(control_ii.grav_accel);
      }
      else {
         control_ii.grav_accel_magsq = 0.0;
      }
   }

   // Sort the controls in increasing acceleration order.
   std::sort (grav_controls.begin(), grav_controls.end(),
              GravityControls::accel_mag_less_ptr);

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
