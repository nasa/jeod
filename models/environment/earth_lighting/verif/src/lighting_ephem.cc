/******************************************************************************
PURPOSE:
    (Code used to accomplish the verif for earth_lighting)
REFERENCES:
    ((None))
ASSUMPTIONS AND LIMITATIONS:
    ((None))
LIBRARY DEPENDENCY:
    ((lighting_ephem.cc))
******************************************************************************/

#include "../include/lighting_ephem.hh"
#include "environment/ephemerides/ephem_manager/include/ephem_manager.hh"

//! Namespace jeod
namespace jeod
{

LightingEphem::LightingEphem()
{
    sun.set_name("Sun");
    moon.set_name("Moon");
    earth.set_name("Earth");
}

double LightingEphem::timestamp() const
{
    return 0.0;
}

std::string LightingEphem::get_name() const
{
    return ident.c_str();
}

void LightingEphem::ephem_initialize(EphemeridesManager & manager)
{
    manager.add_ephem_item(sun);
    manager.add_ephem_item(moon);
    manager.add_ephem_item(earth);
}

void LightingEphem::ephem_activate(EphemeridesManager & manager __attribute__((unused))) {}

void LightingEphem::ephem_build_tree(EphemeridesManager & manager __attribute__((unused))) {}

void LightingEphem::ephem_update() {}

} // namespace jeod
