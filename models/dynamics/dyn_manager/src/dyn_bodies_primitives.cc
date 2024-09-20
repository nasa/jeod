/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynManager
 * @{
 *
 * @file models/dynamics/dyn_manager/src/dyn_bodies_primitives.cc
 * Define the DynManager member functions that search through and add elements
 * to the collection of DynBody pointers.
 */

/*******************************************************************************
Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((N/A))

Library dependencies:
  ((dyn_bodies_primitives.cc)
   (dyn_manager.cc)
   (dyn_manager_messages.cc)
   (dynamics/dyn_body/src/dyn_body.cc)
   (utils/message/src/message_handler.cc))


*******************************************************************************/

// System includes
#include <algorithm>
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/dyn_manager.hh"
#include "../include/dyn_manager_messages.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Find the dynamic body with the given name.
 * @param body_name  Dynamic body name
 * @return Pointer to found DynBody; NULL if not found.
 */
DynBody * DynManager::find_dyn_body(const std::string & body_name) const
{
    DynBody * found_body = nullptr;

    // Ensure the passed name has a minimally valid value.
    if(!validate_name(__FILE__, __LINE__, body_name, "Argument", "name"))
    {
        return nullptr;
    }

    // Find the body by name.
    auto name_compare = [&body_name](const DynBody * body)
    {
        return body->name.get_name().compare(body_name) == 0;
    };

    const auto iter = std::find_if(dyn_bodies.begin(), dyn_bodies.end(), name_compare);

    if(iter != dyn_bodies.end())
    {
        found_body = *iter;
    }

    return found_body;
}

/**
 * Determine if the specified body has been registered with the DynManager.
 * @param dyn_body  Dynamic body to be found.
 * @return True if body has been registered, false otherwise.
 */
bool DynManager::is_dyn_body_registered(const DynBody * dyn_body) const
{
    return std::find(dyn_bodies.begin(), dyn_bodies.end(), dyn_body) != dyn_bodies.end();
}

/**
 * Add a dynamic body to the dynamic body registry.
 * @param dyn_body  Dynamic body to be added to the registry.
 */
void DynManager::add_dyn_body(DynBody & dyn_body)
{
    MassBody * mass_body;

    // Handle errors.
    // 1. The body must have a minimally valid name.
    if(!validate_name(__FILE__, __LINE__, dyn_body.name.get_name(), "Argument", "dyn_body.name"))
    {
        return;
    }

    // 2. The body must not have been previously registered.
    if(is_dyn_body_registered(&dyn_body))
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              DynManagerMessages::duplicate_entry,
                              "Dynamic body '%s' is already registered.",
                              dyn_body.name.c_str());
        return;
    }

    // 3. The body must have a unique name.
    if(find_dyn_body(dyn_body.name.get_name()) != nullptr)
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              DynManagerMessages::duplicate_entry,
                              "Dynamic body with name '%s' was previously registered.",
                              dyn_body.name.c_str());
        return;
    }

    // 4. The name uniqueness also applies to mass bodies.
    mass_body = find_mass_body(dyn_body.name.get_name());
    if(mass_body != nullptr)
    {
        if(mass_body != &dyn_body.mass)
        {
            MessageHandler::error(__FILE__,
                                  __LINE__,
                                  DynManagerMessages::invalid_name,
                                  "Attempt to add a DynBody with the same name '%s' "
                                  "as a previously registered MassBody.",
                                  dyn_body.name.c_str());
            return;
        }
        else
        {
            MessageHandler::warn(__FILE__,
                                 __LINE__,
                                 DynManagerMessages::duplicate_entry,
                                 "Adding a DynBody whose MassBody member already "
                                 "previously registered. Implies that add_mass_body was called when "
                                 "add_dyn_body was more appropriate.");
        }
    }
    else
    {
        // All tests passed: Add the body to the registries.

        // Add the mass to the list of mass body registry.
        add_mass_body(dyn_body.mass);
    }

    // Add the body to the list of dynamic body registry.
    dyn_bodies.push_back(&dyn_body);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
