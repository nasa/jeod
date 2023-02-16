/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/src/dyn_body_init.cc
 * Define methods for the base body initialization class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((dyn_body_init.o)
   (body_action.o)
   (body_action_messages.o)
   (dynamics/dyn_body/dyn_body.o)
   (dynamics/dyn_body/dyn_body_set_state.o)
   (dynamics/mass/mass.o)
   (dynamics/mass/mass_point_state.o)
   (utils/message/message_handler.o)
   (utils/orientation/orientation.o)
   (utils/quaternion/quat_from_mat.o)
   (utils/ref_frames/ref_frame.o)
   (utils/named_item/named_item.o))



*******************************************************************************/


// System includes
#include <cstddef>
#include <typeinfo>

// JEOD includes
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "environment/ephemerides/ephem_interface/include/ephem_ref_frame.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/ref_frames/include/ref_frame_items.hh"
#include "utils/named_item/include/named_item.hh"

// Model includes
#include "../include/body_action_messages.hh"
#include "../include/dyn_body_init.hh"


//! Namespace jeod
namespace jeod {

/**
 * Construct a DynBodyInit.
 */
DynBodyInit::DynBodyInit (
   void)
:
   BodyAction(),
   body_frame_id(),
   reference_ref_frame_name(),
   state(),
   orientation(),
   reverse_sense(false),
   rate_in_parent(false),
   body_ref_frame(nullptr),
   reference_ref_frame(nullptr),
   subscribed_frame(nullptr)
{
   Vector3::initialize (position);
   Vector3::initialize (velocity);
   Vector3::initialize (ang_velocity);

   return;
}


/**
 * Destruct a DynBodyInit.
 */
DynBodyInit::~DynBodyInit (
   void)
{
   return;
}


/**
 * Complete initialization of a DynBodyInit.
 * The initialize method for all subclasses of DynBodyInit *nust* pass
 * the initialize call to their immediate parent class, which in turn
 * must do the same, eventually invoking this method.
 * \param[in,out] dyn_manager Dynamics manager
 */
void
DynBodyInit::initialize (
   DynManager & dyn_manager)
{

   // Forward the request up the chain.
   BodyAction::initialize (dyn_manager);

   // Sanity check:
   // The subject MassBody must belong a DynBody.
   if (dyn_subject == nullptr) {
      MessageHandler::fail (
         __FILE__, __LINE__, BodyActionMessages::invalid_object,
         "%s failed:\n"
         "Subject body '%s' is a %s.\n"
         "A subject body of a class derived from DynBody is required.",
         action_identifier.c_str(), mass_subject->name.c_str(), typeid(*mass_subject).name());
      return;
   }

   // Sanity check: The body must be registered with the dynamics manager.
   if (! dyn_manager.is_dyn_body_registered (dyn_subject)) {
      MessageHandler::fail (
         __FILE__, __LINE__, BodyActionMessages::invalid_object,
         "The subject '%s' of action '%s' has not been initialized.\n",
         dyn_subject->name.c_str(),
         action_identifier.c_str());
   }


   // Find the body frame that will be populated by this initialization.
   // Note: A child class may have already set this, in which case the
   // search is not needed.
   if (body_ref_frame == nullptr) {
      if (body_frame_id.empty()) {
        MessageHandler::warn(
          __FILE__,__LINE__,"Incomplete assignment to dyn-body-init\n",
          "The body_ref_frame and body_frame_id have not been set.\n"
          "Typically, the body_frame_id would be used to identify \n"
          "the body_ref_frame.\n"
          "Setting body_frame_id to the default `composite_body`.\n");
         body_frame_id = "composite_body";
      }
      body_ref_frame =
         find_body_frame (*dyn_subject, body_frame_id.c_str(), "body_frame_id");
   }


   // Find the reference frame to which the initialization data refers.
   // Note: A child class may have already set this, in which case the
   // search is not needed.
   if (reference_ref_frame == nullptr) {
      reference_ref_frame =
         find_ref_frame (dyn_manager, reference_ref_frame_name.c_str(),
                         "reference_ref_frame_name");
   }


   // (Temporarily) subscribe to the frame to force it to be placed in the
   // reference frame tree. The subscription will be withdrawn by apply().
   subscribed_frame = reference_ref_frame;
   dyn_manager.subscribe_to_frame (*subscribed_frame);

   return;
}


/**
 * In general, specify what state elements are to be initialized.
 * This method indicates that no such elements are initialized.
 * A subclass that does something *must* override this default method.
 * @return Initialized states
 */
RefFrameItems::Items
DynBodyInit::initializes_what (
   void)
{
   return RefFrameItems::No_Items;
}


/**
 * In general, determine if the initializer is ready to be applied.
 * This method determines whether the self-dependencies are satisfied.
 * Dependencies on the reference reference frame are the responsibility
 * of derived classes.
 * @return Can initializer run?
 */
bool
DynBodyInit::is_ready (
   void)
{
   RefFrameItems set_items (initializes_what ());
   RefFrameItems required_items;


   // All BodyAction derived classes must query the parent class' readiness.
   if (! BodyAction::is_ready ()) {
      return false;
   }

   // A backwards (reverse_sense) specification requires some self knowledge.
   if (reverse_sense) {
      // Attitude is always needed in the case of a backards specification.
      required_items.add (RefFrameItems::Att);

      // Position and rate are also needed for a reverse-sense velocity spec.
      if (set_items.contains (RefFrameItems::Vel)) {
         required_items.add (RefFrameItems::Pos_Rate);
      }

   }

   // In a normal sense specification, computation of velocity nominally
   // requires knowledge of relative position and computation of rate requires
   // knowledge of relative orientation.
   // This information is *not* needed if the reference reference frame is
   // known to be non-rotating. However, this information is not available
   // without extensive kludginess.
   else {
      if (set_items.contains (RefFrameItems::Vel)) {
         required_items.add (RefFrameItems::Pos);
      }
      if (set_items.contains (RefFrameItems::Rate)) {
         required_items.add (RefFrameItems::Att);
      }
   }


   // Self-knowledge of the attitude is needed when rate is specified in parent.
   if (set_items.contains (RefFrameItems::Rate) && rate_in_parent) {
      required_items.add (RefFrameItems::Att);
   }


   // The items provided by this initializer do not need to be previously set.
   required_items.remove (set_items.get ());


   // The initializer is ready-to-run if the requisite elements of the
   // to-be-initialized state have been set.
   return body_ref_frame->initialized_items.contains (required_items.get ());
}


/**
 * Complete initialization of the subject DynBody.
 * The apply method for all subclasses of DynBodyInit *nust* pass
 * the apply call to their immediate parent class, which in turn
 * must do the same, eventually invoking this method.
 * \param[in,out] dyn_manager Jeod manager
 */
void
DynBodyInit::apply (
   DynManager & dyn_manager)
{
   RefFrameItems::Items set_items =   // The newly set state elements
      initializes_what ();

   // Debug.
   MessageHandler::debug (
         __FILE__, __LINE__, BodyActionMessages::trace,
         "%s:\n"
         "Initialized %s %s with respect to %s.",
         action_identifier.c_str(),
         body_ref_frame->get_name(),
         RefFrameItems::to_string (set_items),
         reference_ref_frame->get_name());

   // Initialize selected aspects of the subject vehicle's state,
   // as indicated by the set_items member datum and then propagate
   // state throughout the vehicle, including vehicle points.
   // Note that the body's vehicle points are forced to be updated here.
   bool autoupdate_vehicle_points = dyn_subject->autoupdate_vehicle_points;
   dyn_subject->autoupdate_vehicle_points = true;
   dyn_subject->set_state (set_items, state, *body_ref_frame);
   dyn_subject->propagate_state ();
   dyn_subject->autoupdate_vehicle_points = autoupdate_vehicle_points;

   // Withdraw the subscription made in the initialize() method.
   dyn_manager.unsubscribe_to_frame (*subscribed_frame);

   // Forward the action up the chain.
   BodyAction::apply (dyn_manager);

   return;
}


/**
 * Report on an initializer that could not be processed.
 */
void
DynBodyInit::report_failure (
   void)
{
   RefFrameItems::Items set_items =   // The newly set state elements
      initializes_what ();

   MessageHandler::error (
         __FILE__, __LINE__, BodyActionMessages::not_performed,
         "%s:\n"
         "Unable to initialize %s %s with respect to %s.",
         action_identifier.c_str(),
         ((body_ref_frame != nullptr) ?
             body_ref_frame->get_name() : "unspecified frame"),
         RefFrameItems::to_string (set_items),
         reference_ref_frame->get_name());

   return;
}


/**
 * Compute the state wrt the reference reference frame,
 * incorporate the user-input items to this relative state, and
 * compute the state relative to the target frame's parent.
 */
void
DynBodyInit::apply_user_inputs (
   void)
{
   RefFrame user_frame;   /* --
      The user-specified orientation and angular velocity are specified with
      respect to some user-specified reference ref frame. This user frame
      is attached to this reference ref frame and populated with the user-
      specified rotational state. */
   RefFrameItems set_items (initializes_what ()); /* --
      The items to be set from the user inputs. */


   // (Temporarily) connect the above user_frame under the reference ref frame.
   reference_ref_frame->add_child (user_frame);

   // Initialize the user_frame state with the existing state information.
   body_ref_frame->compute_relative_state (
      *reference_ref_frame, reverse_sense, user_frame.state);

   // Overlay any user inputs on top of the state.
   if (set_items.contains (RefFrameItems::Pos)) {
      Vector3::copy (position, user_frame.state.trans.position);
   }
   if (set_items.contains (RefFrameItems::Vel)) {
      Vector3::copy (velocity, user_frame.state.trans.velocity);
   }
   if (set_items.contains (RefFrameItems::Att)) {
      orientation.compute_transform();
      Matrix3x3::copy (orientation.trans, user_frame.state.rot.T_parent_this);
      user_frame.state.rot.compute_quaternion ();
   }
   if (set_items.contains (RefFrameItems::Rate)) {
      if (rate_in_parent) {
         Vector3::transform (user_frame.state.rot.T_parent_this,
                             ang_velocity,
                             user_frame.state.rot.ang_vel_this);
      }
      else {
         Vector3::copy (ang_velocity, user_frame.state.rot.ang_vel_this);
      }
      user_frame.state.rot.compute_ang_vel_products ();
   }

   if (reverse_sense) {
      RefFrameState user_state_copy (user_frame.state);
      user_frame.state.negate (user_state_copy);
   }

   // Compute the state relative to the vehicle's integration frame.
   user_frame.compute_relative_state (*(dyn_subject->integ_frame), state);

   // Remove the temporary connection established above.
   user_frame.remove_from_parent ();
}


/**
 * This method is obsolete. Use apply_user_inputs instead.
 */
void
DynBodyInit::compute_rotational_state (
   void)
{
   static bool reported = false;

   if (! reported) {
      reported = true;
      MessageHandler::error (
         __FILE__, __LINE__, BodyActionMessages::invalid_name,
         "compute_rotational_state "
         "is obsolute and is scheduled for deletion.\n"
         "Use apply_user_inputs() instead.");
   }
   apply_user_inputs ();

   return;
}


/**
 * This method is obsolete. Use apply_user_inputs instead.
 */
void
DynBodyInit::compute_translational_state (
   void)
{
   static bool reported = false;

   if (! reported) {
      reported = true;
      MessageHandler::error (
         __FILE__, __LINE__, BodyActionMessages::invalid_name,
         "compute_translational_state "
         "is obsolete and is scheduled for deletion.\n"
         "Use apply_user_inputs() instead.");
   }
   apply_user_inputs ();

   return;
}



/**
 * Find the Planet with the given name, failing if not found.
 * @return Found Planet
 * \param[in] dyn_manager Dynamics manager
 * \param[in] planet_name Planet name
 * \param[in] variable_name For error reporting
 */
Planet *
DynBodyInit::find_planet (
   DynManager & dyn_manager,
   const std::string & planet_name,
   const std::string & variable_name)
{
   Planet * found_planet = nullptr;

   // Sanity check: The planet name must be provided.
   validate_name (planet_name, variable_name, "Planet");

   // Find the Planet with the given name.
   found_planet = dyn_manager.find_planet (planet_name.c_str());

   // Sanity check: The planet_name must name a Planet.
   if (found_planet == nullptr) {
      MessageHandler::fail (
         __FILE__, __LINE__, BodyActionMessages::invalid_name,
         "%s failed:\n"
         "Could not find planet named '%s' (variable %s)",
         action_identifier.c_str(), planet_name.c_str(), variable_name.c_str());

      // Not reached
      return nullptr;
   }

   return found_planet;
}


/**
 * Find the DynBody with the given name, failing if not found.
 * @return Found DynBody
 * \param[in] dyn_manager Dynamics manager
 * \param[in] dyn_body_name DynBody name
 * \param[in] variable_name For error reporting
 */
DynBody *
DynBodyInit::find_dyn_body (
   DynManager & dyn_manager,
   const std::string & dyn_body_name,
   const std::string & variable_name)
{
   DynBody * found_dyn_body = nullptr;

   // Sanity check: The dyn_body name must be provided.
   validate_name (dyn_body_name, variable_name, "DynBody");

   // Find the DynBody with the given name.
   found_dyn_body = dyn_manager.find_dyn_body (dyn_body_name.c_str());

   // Sanity check: The dyn_body_name must name a DynBody.
   if (found_dyn_body == nullptr) {
      MessageHandler::fail (
         __FILE__, __LINE__, BodyActionMessages::invalid_name,
         "%s failed:\n"
         "Could not find vehicle named '%s' (variable %s)",
         action_identifier.c_str(), dyn_body_name.c_str(), variable_name.c_str());

      // Not reached
      return nullptr;
   }

   return found_dyn_body;
}


/**
 * Find the RefFrame with the given name, failing if not found.
 * @return Found ref_frame
 * \param[in] dyn_manager Dynamics manager
 * \param[in] ref_frame_name RefFrame name
 * \param[in] variable_name For error reporting
 */
RefFrame *
DynBodyInit::find_ref_frame (
   DynManager & dyn_manager,
   const std::string & ref_frame_name,
   const std::string & variable_name)
{
   RefFrame * found_ref_frame = nullptr;

   // Sanity check: The ref_frame name must be provided.
   validate_name (ref_frame_name, variable_name, "RefFrame");

   // Find the RefFrame with the given name.
   found_ref_frame = dyn_manager.find_ref_frame (ref_frame_name.c_str());

   // Sanity check: The ref_frame_name must name a RefFrame.
   if (found_ref_frame == nullptr) {
      MessageHandler::fail (
         __FILE__, __LINE__, BodyActionMessages::invalid_name,
         "%s failed:\n"
         "Could not find reference frame named '%s' (variable %s)",
         action_identifier.c_str(), ref_frame_name.c_str(), variable_name.c_str());

      // Not reached
      return nullptr;
   }

   return found_ref_frame;
}


/**
 * Find the RefFrame with the given name, failing if not found.
 * @return Found BodyRefFrame
 * \param[in] frame_container Body containing frame
 * \param[in] body_frame_identifier BodyRefFrame identifier
 * \param[in] variable_name For error reporting
 */
BodyRefFrame *
DynBodyInit::find_body_frame (
   DynBody & frame_container,
   const std::string & body_frame_identifier,
   const std::string & variable_name)
{
   BodyRefFrame * found_body_frame = nullptr;

   // Sanity check: The body frame identifier must be provided.
   validate_name (body_frame_identifier, variable_name, "BodyRefFrame");

   // Find the BodyRefFrame with the given ID.
   found_body_frame = frame_container.find_body_frame (body_frame_identifier.c_str());

   // Sanity check: The body_frame_identifier name must name a BodyRefFrame.
   if (found_body_frame == nullptr) {
      MessageHandler::fail (
         __FILE__, __LINE__, BodyActionMessages::invalid_name,
         "%s failed:\n"
         "Could not to find body frame '%s' in vehicle '%s' (variable %s)",
         action_identifier.c_str(),
         body_frame_identifier.c_str(), frame_container.name.c_str(), variable_name.c_str());

      // Not reached
   }

   return found_body_frame;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
