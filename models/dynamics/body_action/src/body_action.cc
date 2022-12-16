/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/src/body_action.cc
 * Define methods for the BodyAction class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((body_action.o)
   (body_action_messages.o)
   (dynamics/mass/mass_point_state.o)
   (utils/sim_interface/memory_interface.o)
   (utils/message/message_handler.o)
   (utils/named_item/named_item.o)
   (utils/named_item/named_item_demangle.o))



*******************************************************************************/


// System includes
#include <cstddef>
#include <cstdlib>
#include <string>
#include <typeinfo>

// JEOD includes
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/named_item/include/named_item.hh"

// Model includes
#include "../include/body_action.hh"
#include "../include/body_action_messages.hh"



//! Namespace jeod
namespace jeod {


/**
 * Construct a BodyAction.
 */
BodyAction::BodyAction (
   void)
:
   subject(nullptr),
   dyn_subject(nullptr),
   active(true),
   terminate_on_error(true),
   action_name(nullptr),
   action_identifier(nullptr)
{
   return;
}


/**
 * Destruct a BodyAction.
 */
BodyAction::~BodyAction (
   void)
{
   shutdown();
}


/**
 * Release resources allocated by a BodyAction object.
 */
void
BodyAction::shutdown (
   void)
{

   // Free the constructed action identifier.
   if (action_identifier != nullptr) {
      JEOD_DELETE_ARRAY (action_identifier);
      action_identifier = nullptr;
   }
}


/**
 * Begin initialization of a BodyAction.
 * The initialize method for all subclasses of BodyAction *must* pass
 * the initialize call to their immediate parent class, which in turn
 * must do the same, eventually invoking this method.
 * \param[in,out] dyn_manager Dynamics manager
 */
void
BodyAction::initialize (
   DynManager & dyn_manager JEOD_UNUSED)
{
   std::string type_name(NamedItem::demangle(typeid(*this)));  // Demangled name

   // Construct the action identifier.
   if (action_name != nullptr) {
      action_identifier =
         NamedItem::construct_name (
            type_name.c_str(), action_name);
   }
   else {
      action_identifier =
         NamedItem::construct_name (
            type_name.c_str(), "unnamed instance");
   }

   // Sanity check: Subject must not be null.
   if( subject==nullptr && dyn_subject==nullptr )
   {
      MessageHandler::fail (
         __FILE__, __LINE__, BodyActionMessages::null_pointer,
         "%s failed:\n"
         "The subject body was not assigned",
         action_identifier);
   }
   else if(   subject!=nullptr && dyn_subject!=nullptr
           && ( subject != &(dyn_subject->mass) )      )
   {
      MessageHandler::fail (
         __FILE__, __LINE__, BodyActionMessages::fatal_error,
         "%s failed:\n"
         "Both subject and dyn_subject are redundantly defined,\n"
         "but correspond to different bodies.",
         action_identifier);
   }
}


/**
 * Complete initialization.
 * \param[in,out] dyn_manager Jeod manager
 */
void
BodyAction::apply (
   DynManager & dyn_manager JEOD_UNUSED)
{
   // At this point the action should be finished, kaput, out of commission.
   // Some people reuse body actions. This will leak memory unless we
   // perform shutdown actions.
   shutdown ();
}


/**
 * In general, determine if the initializer is ready to be applied.
 * This base class method simply queries the active flag.
 * Subclasses should override this default method.
 * @return Can initializer run?
 */
bool
BodyAction::is_ready (
   void)
{
   return active;
}



/**
 * Ensure that a string is not trivially invalid.
 * \param[in] variable_value String to be checked
 * \param[in] variable_name For error reporting
 * \param[in] variable_type For error reporting
 */
void
BodyAction::validate_name (
   const char * variable_value,
   const char * variable_name,
   const char * variable_type)
{

   // Check for a null value, which means the user didn't assign anything.
   if (variable_value == NULL) {
      MessageHandler::fail (
         __FILE__, __LINE__, BodyActionMessages::null_pointer,
         "%s failed:\n"
         "Member variable %s value not initialized (this must name a %s)",
         action_identifier, variable_name, variable_type);

      // Not reached
      return;
   }

   // Check for the empty string, which isn't much better than leaving it null.
   if (*variable_value == '\0') {
      MessageHandler::fail (
         __FILE__, __LINE__, BodyActionMessages::invalid_name,
         "%s failed:\n"
         "Member variable %s is the empty string (this must name a %s)",
         action_identifier, variable_name, variable_type);

      // Not reached
      return;
   }
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
