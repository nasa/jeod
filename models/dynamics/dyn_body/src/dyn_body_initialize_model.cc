/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynBody
 * @{
 *
 * @file models/dynamics/dyn_body/src/dyn_body_initialize_model.cc
 * Define DynBody::initialize_model.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((dyn_body_initialize_model.o)
   (dyn_body.o)
   (dyn_body_messages.o))



*******************************************************************************/


// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_manager/include/base_dyn_manager.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/dyn_body.hh"
#include "../include/dyn_body_messages.hh"


//! Namespace jeod
namespace jeod {

// Prepare the body for initialization.
void
DynBody::initialize_model (
   BaseDynManager & dyn_manager_in)
{

   // Sanity check: protect against an empty name for this body
   if (name.size() == 0) {
      MessageHandler::fail (
         __FILE__, __LINE__, DynBodyMessages::invalid_name,
         "All DynBody instances must have names");

      // Not reached
      return;
   }

   // Setting the integration frame from the input file is not proper.
   if (integ_frame != NULL) {
      MessageHandler::error (
         __FILE__, __LINE__, DynBodyMessages::invalid_frame,
         "DynBody '%s' integ_frame member was set in the input file.\n"
         "This is not the recognized interface. "
         "Set the integration frame name (integ_frame_name) instead.",
         name.c_str());
      integ_frame = NULL;
   }

   // Sanity check: protect against an empty integration frame name
   if ((integ_frame_name == NULL) || (integ_frame_name[0] == '\0')) {
      MessageHandler::fail (
         __FILE__, __LINE__, DynBodyMessages::invalid_name,
         "DynBody '%s' does not have an integration frame.",
         name.c_str());

      // Not reached
      return;
   }


   // Cache the dynamics manager for later use.
   dyn_manager = &dyn_manager_in;

   // Tell the dynamics manager to manage this DynBody.
   dyn_manager->add_dyn_body (*this);


   // For each reference frame associated with this body,
   // - Denote that the frame is owned by this body.
   // - Set the name of the frame based on the body name.
   // - Tell the dynamics manager that the frame exists.
   // - Subscribe to the frame (which will mark the frame as active).

   // Core body frame
   core_body.set_owner (this);
   core_body.set_name (name.c_str(), "core_body");
   dyn_manager->add_ref_frame (core_body);
   core_body.subscribe ();

   // Composite body frame
   composite_body.set_owner (this);
   composite_body.set_name (name.c_str(), "composite_body");
   dyn_manager->add_ref_frame (composite_body);
   composite_body.subscribe ();

   // Structural frame
   structure.set_owner (this);
   structure.set_name (name.c_str(), "structure");
   dyn_manager->add_ref_frame (structure);
   structure.subscribe ();


   // Mark the body as unitialized.
   mass.needs_update = true;
   initialized_states.set (RefFrameItems::No_Items);


   // Set the integration frame.
   // Note that this links each of the three primary frames as children of
   // the integration frame.
   set_integ_frame (integ_frame_name);
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
