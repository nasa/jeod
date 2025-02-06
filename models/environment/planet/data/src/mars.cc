/*******************************************************************************
Purpose:
   (Default data to model the Mars planetary parameters.)

References:
   (((GSFC Mars Fact Sheet)
     (nssdc.gsfc.nasa.gov/planetary/factsheet/marsfact.html)
     (Retrieved February 10, 2012)))

Assumptions and Limitations:
   ((None))



*******************************************************************************/

class Planet;
#define JEOD_FRIEND_CLASS Planet_mars_default_data

// JEOD includes
#include "environment/planet/include/base_planet.hh"
#include "environment/planet/include/planet.hh"
#include "utils/named_item/include/named_item.hh"

// Unsupported includes
#include "../include/mars.hh"

//! Namespace jeod
namespace jeod
{

void Planet_mars_default_data::initialize(Planet * Planet_ptr)
{
    /* Transformation from J2000 to IAUMars at the J2000 epoch */
    //    double m[3][3]={
    //                   {-0.704244, -0.709203, 0.0327291},
    //                   {0.552253, -0.576196, -0.602508},
    //                   {0.446159, -0.406238, 0.797442}};
    double celestial_pole[3] = {0.446159, -0.406238, 0.797442};
    double ecliptic_pole[3] = {0.0, 0.41749959299426825, 0.90867710978632033};

    Planet_ptr->name = "Mars";

    /* GSFC Mars fact sheet */
    Planet_ptr->flat_coeff = 0.005186;
    Planet_ptr->r_eq = 1000 * (3396.0);

    //       Planet_ptr->set_alt_inertial(m);
    Planet_ptr->set_alt_inertial(celestial_pole, ecliptic_pole);
}
} // namespace jeod
