/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Ephemerides
 * @{
 * @addtogroup EphemManager
 * @{
 *
 * @file models/environment/ephemerides/ephem_manager/src/find_planet.cc
 * Define EphemeridesManager::find_planet.
 * This method is isolated from the other EphemeridesManager methods
 * because the object file drags in a whole lot of stuff.
 */

/******************************************************************************
Purpose:
  ()

Library dependencies:
  ((find_planet.cc)
   (ephem_manager.cc))


******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "environment/ephemerides/ephem_interface/include/ephem_messages.hh"
#include "environment/planet/include/base_planet.hh"
#include "environment/planet/include/planet.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/ephem_manager.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Find the planet with the given name.
 * @param name  Planet name
 * @return Found planet, as a Planet rather than a BasePlanet
 */
Planet * EphemeridesManager::find_planet(const std::string & name) const
{
    BasePlanet * base_planet = find_base_planet(name);
    Planet * planet;

    // Found a BasePlanet with the given name:
    // Try converting it to a Planet.
    if(base_planet != nullptr)
    {
        planet = dynamic_cast<Planet *>(base_planet);

        // Conversion failed: Warn user about this.
        if(planet == nullptr)
        {
            MessageHandler::warn(__FILE__,
                                 __LINE__,
                                 EphemeridesMessages::invalid_item,
                                 "BasePlanet '%s' is not a Planet. "
                                 "Returning NULL from find_planet().",
                                 name.c_str());
        }
    }

    // BasePlanet not found: planet is NULL.
    else
    {
        planet = nullptr;
    }

    return planet;
}

/**
 * Add a planet to the registry.
 * @param planet  Planet to be added to the registry.
 */
void EphemeridesManager::add_planet(Planet & planet)
{
    add_planet(static_cast<BasePlanet &>(planet));
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 * @}
 */
