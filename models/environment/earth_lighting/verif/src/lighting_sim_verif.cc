#include "../include/lighting_sim_verif.hh"

//! Namespace jeod 
namespace jeod {

void connect_planets(
   Planet& earth,
   Planet& moon,
   Planet& sun ){

   earth.inertial.add_child(moon.inertial);
   earth.inertial.add_child(sun.inertial);

   return;

}

} // End JEOD namespace
