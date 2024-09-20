/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynManager
 * @{
 *
 * @file models/dynamics/dyn_manager/src/mass_bodies_primitives.cc
 * Define the DynManager member functions that search through and add elements
 * to the collection of MassBody pointers.
 */

/*******************************************************************************
Purpose:
  ()

Library dependencies:
  ((mass_bodies_primitives.cc)
   (dyn_manager.cc)
   (dyn_manager_messages.cc)
   (dynamics/mass/src/mass.cc)
   (utils/message/src/message_handler.cc))


*******************************************************************************/

// System includes
#include <algorithm>
#include <cstddef>

// JEOD includes
#include "dynamics/mass/include/mass.hh"
#include "utils/message/include/message_handler.hh"

/* Model includes */
#include "../include/dyn_manager.hh"
#include "../include/dyn_manager_messages.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Find the mass body with the given name.
 * @param body_name  Mass body name
 * @return Pointer to found MassBody; NULL if not found.
 */
MassBody * DynManager::find_mass_body(const std::string & body_name) const
{
    MassBody * found_body = nullptr;

    // Ensure the passed name has a minimally valid value.
    if(!validate_name(__FILE__, __LINE__, body_name, "Argument", "name"))
    {
        return nullptr;
    }

    // Find the body by name.
    auto name_compare = [&body_name](const MassBody * body)
    {
        return body->name.ends_with(0, body_name);
    };

    const auto iter = std::find_if(mass_bodies.begin(), mass_bodies.end(), name_compare);

    if(iter != mass_bodies.end())
    {
        found_body = *iter;
    }

    return found_body;
}

/**
 * Determine if the specified body has been registered with the DynManager.
 * @param mass_body  Mass body to be found.
 * @return True if body has been registered, false otherwise.
 */
bool DynManager::is_mass_body_registered(const MassBody * mass_body) const
{
    return std::find(mass_bodies.begin(), mass_bodies.end(), mass_body) != mass_bodies.end();
}

/**
 * Add a mass body to the mass body registry.
 * @param mass_body  Mass body to be added to the registry.
 */
void DynManager::add_mass_body(MassBody & mass_body)
{
    // Handle errors.
    // 1. The body must have a minimally valid name.
    if(!validate_name(__FILE__, __LINE__, mass_body.name.get_name(), "Argument", "mass_body.name"))
    {
        return;
    }

    // 2. The body must not have been previously registered.
    if(is_mass_body_registered(&mass_body))
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              DynManagerMessages::duplicate_entry,
                              "Mass body '%s' is already registered.",
                              mass_body.name.c_str());
        return;
    }

    // 3. The body must have a unique name.
    if(find_mass_body(mass_body.name.get_name()) != nullptr)
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              DynManagerMessages::duplicate_entry,
                              "Mass body with name '%s' was previously registered.",
                              mass_body.name.c_str());
        return;
    }

    // All tests passed: Add the body to the mass body registry.
    mass_bodies.push_back(&mass_body);
}

/**
 * Add a mass body to the mass body registry.
 * @param mass_body  Mass body to be added to the registry.
 */
void DynManager::add_mass_body(MassBody * mass_body)
{
    // Sanity check: Pointer must be valid.
    if(mass_body == nullptr)
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              DynManagerMessages::null_pointer,
                              "Attempt to add a null pointer to the MassBody list.");
        return;
    }

    // Use the reference-based method to do the bulk of the work.
    add_mass_body(*mass_body);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
