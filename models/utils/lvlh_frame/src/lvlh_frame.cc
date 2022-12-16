/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup LvlhFrame
 * @{
 *
 * @file models/utils/lvlh_frame/src/lvlh_frame.cc
 * Define methods for the LVLH reference frame class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((lvlh_frame.o)
   (lvlh_frame_messages.o)
   (utils/message/message_handler.o)
   (utils/ref_frames/ref_frame.o))

 

*******************************************************************************/


// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "environment/planet/include/base_planet.hh"
#include "utils/math/include/vector3.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/named_item/include/named_item.hh"

// Model includes
#include "../include/lvlh_frame.hh"
#include "../include/lvlh_frame_messages.hh"



//! Namespace jeod
namespace jeod {

/**
 * Construct an LvlhFrame object.
 */
LvlhFrame::LvlhFrame (
   void)
:
   frame(),
   subject_name(""),
   planet_name(""),
   subject_frame(NULL),
   planet_centered_inertial(NULL),
   local_dm(NULL)
{
   ;
}



/**
 * Destruct an LvlhFrame object.
 */
LvlhFrame::~LvlhFrame (
   void)
{
   // Disconnect the LVLH frame from the planet-centered inertial frame.
   frame.remove_from_parent();

   if (local_dm != NULL) {
      // Remove from dyn manager
      local_dm->remove_ref_frame (frame);
   }

   // Remove the initialization-time frame subscriptions.
   if (subject_frame != NULL) {
      subject_frame->unsubscribe();
   }
   if (planet_centered_inertial != NULL) {
      planet_centered_inertial->unsubscribe();
   }
}



/**
 * Begin initialization of an LvlhFrame.
 * \param[in,out] dyn_manager Dynamics manager
 */
void
LvlhFrame::initialize (
   DynManager & dyn_manager)
{

   //Cache a pointer to the dyn manager. We will need it for the destructor.
   local_dm = &dyn_manager;

   if (subject_frame == NULL) {
      // Check that subject_name has something to it.
      if (subject_name.empty()) {
         MessageHandler::fail (
            __FILE__, __LINE__, LvlhFrameMessages::invalid_configuration,
            "Either subject_name or subject_frame must be specified.");

         //Not reached
         return;
      }

      // Find the subject frame in the dynamics manager's list.
      subject_frame = dyn_manager.find_ref_frame (subject_name.c_str());

      // Ensure the above worked.
      if (subject_frame == NULL) {
         MessageHandler::fail (
            __FILE__, __LINE__, LvlhFrameMessages::invalid_name,
            "Invalid subject frame '%s' for LvlhFrame",
            subject_name.c_str());

         // Not reached
         return;
      }
   } else {
      subject_name = subject_frame->get_name();
   }

   // (Try to) ensure the subject frame is computed.
   subject_frame->subscribe();


   if (planet_centered_inertial == NULL) {
      // Check that some kind of planet name was provided.
      if (planet_name.empty()) {
         MessageHandler::fail (
            __FILE__, __LINE__, LvlhFrameMessages::invalid_configuration,
            "Either planet_centered_inertial frame or planet_name must be specified.");;

         //Not reached
         return;
      }

      // Find the planet in the dynamics manager's list.
      BasePlanet * planet;
      planet = dyn_manager.find_base_planet (planet_name.c_str());

      // Ensure the above worked.
      if (planet == NULL) {
         MessageHandler::fail (
            __FILE__, __LINE__, LvlhFrameMessages::invalid_name,
            "Invalid planet name '%s' for LvlhFrame",
            planet_name.c_str());

         // Not reached
         return;
      }
   planet_centered_inertial = & planet->inertial;
   } else {
      planet_name = planet_centered_inertial->get_name();
      planet_name = planet_name.substr(0, planet_name.rfind(".inertial"));
   }

   // Name the LVLH frame as <subject_frame>.<planet>.lvlh
   frame.set_name (subject_name.c_str(), planet_name.c_str(), "lvlh");

   // Add the LVLH frame to the dynamics manager's frame list,
   // connect it to the planet's inertial frame, and subscribe
   // to that frame.
   dyn_manager.add_ref_frame (frame);
   planet_centered_inertial->add_child (frame);
   planet_centered_inertial->subscribe ();


   // Initialize the LVLH frame's angular velocity.
   // Note: Only the y-component of the angular velocity will be changed
   // via update(). The x and z components are identically zero.
   Vector3::initialize (frame.state.rot.ang_vel_this);
   Vector3::initialize (frame.state.rot.ang_vel_unit);
   frame.state.rot.ang_vel_unit[1] = -1;
}


/**
 * Update the state.
 */
void
LvlhFrame::update (
   void)
{

   // Compute the LVLH frame directly from the subject frame's position and
   // velocity if the planet centered inertial frame is the direct parent
   // of the subject frame.
   if (subject_frame->get_parent() == planet_centered_inertial) {
      compute_lvlh_frame (subject_frame->state.trans);
   }

   // Otherwise, compute the subject frame's translational state wrt the
   // planet and use that to determine the LVLH frame.
   else {
      RefFrameState subject_rel_state;
      subject_frame->compute_relative_state (
         *planet_centered_inertial, subject_rel_state);
      compute_lvlh_frame (subject_rel_state.trans);
   }

   // Timestamp the frame per the vehicle timestamp.
   frame.set_timestamp (subject_frame->timestamp());
}


/**
 * Set the subject_name to the supplied value.
 * \param[in] new_name new name.
 */
void
LvlhFrame::set_subject_name (
   const std::string new_name)
{
   subject_name = new_name;
}

/**
 * Set the subject_frame to the supplied value.
 * \param[in] new_frame new frame.
 */
void
LvlhFrame::set_subject_frame (
   RefFrame & new_frame)
{
   subject_frame = & new_frame;
}


/**
 * Set the planet_name to the supplied value.
 * \param[in] new_name new name.
 */
void
LvlhFrame::set_planet_name (
   const std::string new_name)
{
   planet_name = new_name;
}


/**
 * Set the planet whose PCI frame will be the reference for LVLH
 * \param[in] new_planet new planet.
 */
void
LvlhFrame::set_planet (
   BasePlanet &new_planet)
{
   planet_centered_inertial = &new_planet.inertial;
}


/**
 * Update the state of the LVLH frame wrt its parent.
 * \param[in] rel_trans Planet relative state
 */
void
LvlhFrame::compute_lvlh_frame (
   const RefFrameTrans & rel_trans)
{
   double angmom[3];
   double hmag;
   double rmagsq;
   double rmag;
   double wmag;
   RefFrameTrans & lvlh_trans = frame.state.trans;
   RefFrameRot & lvlh_rot     = frame.state.rot;

   // The origin of the LVLH frame is co-located and co-moving with the vehicle.
   Vector3::copy (rel_trans.position, lvlh_trans.position);
   Vector3::copy (rel_trans.velocity, lvlh_trans.velocity);

   // Compute the orbital parameters related to angular momentum.
   Vector3::cross (rel_trans.position, rel_trans.velocity, angmom);
   hmag   = Vector3::vmag (angmom);
   rmagsq = Vector3::vmagsq (rel_trans.position);
   rmag   = std::sqrt (rmagsq);
   wmag   = hmag / rmagsq;

   // Compute the orientation of the LVLH frame wrt inertial:
   //   zhat = -r/rmag
   //   yhat = -h/hmag
   //   xhat = yhat cross zhat
   Vector3::scale (rel_trans.position, -1.0 / rmag,
                   lvlh_rot.T_parent_this[2]);
   Vector3::scale (angmom, -1.0 / hmag,
                   lvlh_rot.T_parent_this[1]);
   Vector3::cross (lvlh_rot.T_parent_this[1], lvlh_rot.T_parent_this[2],
                   lvlh_rot.T_parent_this[0]);
   Vector3::normalize (lvlh_rot.T_parent_this[0]);

   // Compute the quaternion from the transformation matrix.
   frame.state.rot.compute_quaternion();

   // Update the angular velocity.
   lvlh_rot.ang_vel_mag     =  wmag;
   lvlh_rot.ang_vel_this[1] = -wmag;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
