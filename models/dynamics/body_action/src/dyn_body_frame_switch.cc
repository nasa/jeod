/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/src/dyn_body_frame_switch.cc
 * Define methods for the class DynBodyFrameSwitch.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((dyn_body_frame_switch.o)
   (body_action.o)
   (body_action_messages.o)
   (dynamics/dyn_body/dyn_body.o)
   (dynamics/mass/mass.o)
   (dynamics/mass/mass_point_state.o)
   (utils/message/message_handler.o)
   (utils/ref_frames/ref_frame.o))



*******************************************************************************/


// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "environment/ephemerides/ephem_interface/include/ephem_ref_frame.hh"
#include "environment/gravity/include/gravity_interaction.hh"
#include "environment/gravity/include/gravity_controls.hh"
#include "utils/math/include/vector3.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/body_action_messages.hh"
#include "../include/dyn_body_frame_switch.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a DynBodyFrameSwitch instance.
 */
DynBodyFrameSwitch::DynBodyFrameSwitch (
   void)
:
   integ_frame_name(nullptr),
   switch_sense(SwitchOnApproach),
   sort_grav_controls(false),
   switch_distance(9e99),
   integ_frame(nullptr)
{
   return; // Empty
}


/**
 * Destruct a DynBodyFrameSwitch instance.
 */
DynBodyFrameSwitch::~DynBodyFrameSwitch (
   void)
{
   return; // Empty
}


/**
 * Initialization a DynBodyFrameSwitch instance.
 * \param[in,out] dyn_manager Dynamics manager
 */
void
DynBodyFrameSwitch::initialize (
   DynManager & dyn_manager)
{

   // Forward the request up the chain.
   BodyAction::initialize (dyn_manager);

   // Interpret user spec
   if( subject==nullptr )
   {
       subject = &(dyn_subject->mass);
   }
   else if( dyn_subject==nullptr)
   {
       dyn_subject = dyn_manager.find_dyn_body( subject->name.c_str() );
   }

   // Sanity check: The subject MassBody must be a DynBody.
   if (dyn_subject == nullptr) {
      MessageHandler::fail (
         __FILE__, __LINE__, BodyActionMessages::invalid_object,
         "%s failed:\n"
         "Subject body '%s' is not of a class based on DynBody.\n"
         "A subject body of a class derived from DynBody is required.",
         action_identifier, subject->name.c_str());

      // Not reached
      return;
   }

   // Sanity check: Protect against an empty integration frame name.
   if ((integ_frame_name == nullptr) || (integ_frame_name[0] == '\0')) {
      MessageHandler::fail (
         __FILE__, __LINE__, BodyActionMessages::invalid_name,
         "%s failed:\n"
         "The integration frame name was not specified.",
         action_identifier);

      // Not reached
      return;
   }

   // Find the integration frame corresponding to the specified name.
   integ_frame = dyn_manager.find_integ_frame (integ_frame_name);

   // Sanity check: The provide name must specify a valid integration frame.
   if (integ_frame == nullptr) {
      MessageHandler::fail (
         __FILE__, __LINE__, BodyActionMessages::invalid_name,
         "%s failed:\n"
         "The integration frame name '%s' does not specify a "
         "valid integration frame.",
         action_identifier, integ_frame_name);
   }

   // (Temporarily) subscribe to the new integration frame to force it to be
   // placed in the reference frame tree.
   // The subscription will be withdrawn by apply().
   dyn_manager.subscribe_to_frame (*integ_frame);

   return;
}


/**
 * Switch reference frames.
 * \param[in,out] dyn_manager Jeod manager
 */
void
DynBodyFrameSwitch::apply (
   DynManager & dyn_manager)
{

   // Debug.
   MessageHandler::debug (
         __FILE__, __LINE__, BodyActionMessages::trace,
         "%s: %s integration frame switched to %s.",
         action_identifier,
         dyn_subject->name.c_str(), integ_frame_name);

   // Switch frames.
   dyn_subject->switch_integration_frames (*integ_frame);

   // Compute gravitation and sort if requested.
   if (sort_grav_controls) {

      // Compute the gravitational acceleration from each gravity control.
      const double* integ_pos = dyn_subject->composite_body.state.trans.position;
      GravityInteraction& grav = dyn_subject->grav_interaction;
      unsigned int n_controls = grav.grav_controls.size();
      for (unsigned int ii = 0; ii < n_controls; ++ii) {
         GravityControls & control_ii = *(grav.grav_controls[ii]);
         if (control_ii.active) {
            control_ii.gravitation (integ_pos,
                                    grav.integ_frame_index,
                                    control_ii.grav_accel,
                                    control_ii.grav_grad,
                                    &control_ii.grav_pot);
         }
      }

      // Sort the controls based on the newly computed grav accels.
      grav.sort_controls();
   }

   // Forward the action up the chain.
   BodyAction::apply (dyn_manager);

   return;
}


/**
 * Determine whether it is time to switch frames.
 * A frame-switch action is ready if it is activated and if the
 * vehicle has entered/left the appropriate sphere of influence.
 * @return Can action be applied?
 */
bool
DynBodyFrameSwitch::is_ready (
   void)
{
   bool ready;

   if (BodyAction::is_ready()) {
      if (switch_sense == SwitchOnApproach) {
         double rel_pos[3];
         dyn_subject->composite_body.compute_position_from (*integ_frame, rel_pos);
         ready = Vector3::vmagsq(rel_pos) < switch_distance * switch_distance;
      }
      else {
         ready =
            Vector3::vmagsq(dyn_subject->composite_body.state.trans.position) >
            switch_distance * switch_distance;
      }
   }
   else {
      ready = false;
   }

   return ready;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
