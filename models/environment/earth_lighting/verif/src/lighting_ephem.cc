/******************************************************************************
PURPOSE:
    (Code used to accomplish the verif for earth_lighting)
REFERENCES:
    ((None))
ASSUMPTIONS AND LIMITATIONS:
    ((None))
LIBRARY DEPENDENCY:
    ((lighting_ephem.o))
******************************************************************************/

#include "../include/lighting_ephem.hh"
#include "environment/ephemerides/ephem_manager/include/ephem_manager.hh"

//! Namespace jeod
namespace jeod {

LightingEphem::LightingEphem() {

   ident[0] = 'e';
   ident[1] = 'p';
   ident[2] = 'h';
   ident[3] = 'e';
   ident[4] = 'm';
   ident[5] = '\0';

   sun.set_name("Sun");
   moon.set_name("Moon");
   earth.set_name("Earth");


   return;

}

LightingEphem::~LightingEphem() {


   return;

}

double LightingEphem::timestamp(void) const {

   return 0.0;

}

const char * LightingEphem::get_name (void) const {

   return ident;

}

void LightingEphem::ephem_initialize (
      EphemeridesManager& manager) {

   manager.add_ephem_item(sun);
   manager.add_ephem_item(moon);
   manager.add_ephem_item(earth);

   return;

}

void LightingEphem::ephem_activate (
      EphemeridesManager& manager __attribute__ ((unused)) ) {

   return;

}

void LightingEphem::ephem_build_tree(
      EphemeridesManager& manager __attribute__ ((unused)) ) {

   return;

}

void LightingEphem::ephem_update(void) {

   return;

}

} // End JEOD namespace
