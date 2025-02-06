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
  ((body_action.cc)
   (body_action_messages.cc)
   (dynamics/mass/src/mass_point_state.cc)
   (utils/message/src/message_handler.cc)
   (utils/named_item/src/named_item.cc)
   (utils/named_item/src/named_item_demangle.cc))



*******************************************************************************/

// System includes
#include <cstddef>
#include <cstdlib>
#include <string>
#include <typeinfo>

// JEOD includes
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/named_item/include/named_item.hh"

// Model includes
#include "../include/body_action.hh"
#include "../include/body_action_messages.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Release resources allocated by a BodyAction object.
 */
void BodyAction::shutdown()
{
    // No default shutdown actions.
}

/**
 * Begin initialization of a BodyAction.
 * The initialize method for all subclasses of BodyAction *must* pass
 * the initialize call to their immediate parent class, which in turn
 * must do the same, eventually invoking this method.
 * \param[in,out] dyn_manager Dynamics manager
 */
void BodyAction::initialize(DynManager & dyn_manager JEOD_UNUSED)
{
    std::string type_name(NamedItem::demangle(typeid(*this))); // Demangled name

    // Construct the action identifier.
    if(!action_name.empty())
    {
        action_identifier = type_name + "." + action_name;
    }
    else
    {
        action_identifier = type_name + ".unnamed instance";
    }

    validate_body_inputs(dyn_subject, mass_subject, "subject");
}

/**
 * Complete initialization.
 * \param[in,out] dyn_manager Jeod manager
 */
void BodyAction::apply(DynManager & dyn_manager JEOD_UNUSED)
{
    // At this point the action should be finished, kaput, out of commission.
    // Some people reuse body actions. This will leak memory unless we
    // perform shutdown actions.
    shutdown();
}

/**
 * In general, determine if the initializer is ready to be applied.
 * This base class method simply queries the active flag.
 * Subclasses should override this default method.
 * @return Can initializer run?
 */
bool BodyAction::is_ready()
{
    return active;
}

void BodyAction::set_subject_body(MassBody & mass_body_in)
{
    mass_subject = &mass_body_in;
    dyn_subject = nullptr;
}

void BodyAction::set_subject_body(DynBody & dyn_body_in)
{
    dyn_subject = &dyn_body_in;
    mass_subject = nullptr;
}

bool BodyAction::validate_body_inputs(DynBody *& dyn_body_in,
                                      MassBody *& mass_body_in,
                                      const std::string & body_base_name,
                                      bool allow_failure)
{
    bool is_good_body = false;

    if(dyn_body_in == nullptr)
    {
        if(mass_body_in == nullptr)
        {
            // Sanity check: Subject must not be null. Check for bad scoobies
            if(!allow_failure)
            {
                MessageHandler::fail(__FILE__,
                                     __LINE__,
                                     BodyActionMessages::null_pointer,
                                     "%s failed:\n"
                                     "The %s body was not assigned",
                                     action_identifier.c_str(),
                                     body_base_name.c_str());
            }
        }
        else
        {
            dyn_body_in = mass_body_in->dyn_owner;
            is_good_body = true;
        }
    }
    else
    {
        if(mass_body_in == nullptr)
        {
            mass_body_in = &dyn_body_in->mass;
            is_good_body = true;
        }
        else
        {
            if(mass_body_in != &dyn_body_in->mass)
            {
                if(!allow_failure)
                {
                    MessageHandler::fail(__FILE__,
                                         __LINE__,
                                         BodyActionMessages::fatal_error,
                                         "%s failed:\n"
                                         "Both mass_%s and dyn_%s are defined,\n"
                                         "only one may be set. This should not be able to happen.",
                                         action_identifier.c_str(),
                                         body_base_name.c_str(),
                                         body_base_name.c_str());
                }
            }
            else
            {
                is_good_body = true;
            }
        }
    }

    return is_good_body;
}

bool BodyAction::is_same_subject_body(MassBody & mass_body_in)
{
    if(dyn_subject != nullptr)
    {
        return &dyn_subject->mass == &mass_body_in;
    }
    else
    {
        return mass_subject == &mass_body_in;
    }
}

bool BodyAction::is_subject_dyn_body()
{
    if(dyn_subject != nullptr)
    {
        return true;
    }
    else if(mass_subject != nullptr)
    {
        return (mass_subject->dyn_owner != nullptr);
    }
    else
    {
        return false;
    }
}

DynBody * BodyAction::get_subject_dyn_body()
{
    if(dyn_subject != nullptr)
    {
        return dyn_subject;
    }
    else if(mass_subject != nullptr)
    {
        return mass_subject->dyn_owner;
    }
    else
    {
        return nullptr;
    }
}

/**
 * Ensure that a string is not trivially invalid.
 * \param[in] variable_value String to be checked
 * \param[in] variable_name For error reporting
 * \param[in] variable_type For error reporting
 */
void BodyAction::validate_name(const std::string & variable_value,
                               const std::string & variable_name,
                               const std::string & variable_type)
{
    // Check for the empty string, which isn't much better than leaving it null.
    if(variable_value.empty())
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             BodyActionMessages::invalid_name,
                             "%s failed:\n"
                             "Member variable %s is the empty string (this must name a %s)",
                             action_identifier.c_str(),
                             variable_name.c_str(),
                             variable_type.c_str());

        // Not reached
        return;
    }
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
