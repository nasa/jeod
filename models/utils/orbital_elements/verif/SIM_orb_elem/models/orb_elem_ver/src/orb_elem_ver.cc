/********************************* TRICK HEADER ***************************

PURPOSE:
  (dummy function that calls the orbital element source code)

REFERENCE:
  (((none)))

ASSUMPTIONS AND LIMITATIONS:
  (none)

CLASS:
  (scheduled)

LIBRARY DEPENDENCY:
  ((orb_elem_ver.cc)
   (utils/orbital_elements/src/orbital_elements.cc))

 
************************************************************************/

// JEOD includes.
#include "../include/orb_elem_ver.hh"

//! Namespace jeod
namespace jeod {

void OrbElemVer::update(        // RETURN: --  None.
   OrbitalElements * orb_elem)  // INOUT:  --  Orbital elements object.
{
   if(to_cartesian) {
      orb_elem->mean_anom_to_nu();
      orb_elem->to_cartesian(mu, position, velocity);
   }
   else {
      orb_elem->from_cartesian(mu, position, velocity);
   }

   return;

}
} // End JEOD namespace
