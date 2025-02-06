/*******************************************************************************
Purpose:
   (Default data to model the Moon's planetary parameters.)

References:
   (((GSFC Moon Fact Sheet)
     (nssdc.gsfc.nasa.gov/planetary/factsheet/moonfact.html)
     (Retrieved February 10, 2012)))

Assumptions and Limitations:
   ((None))


*******************************************************************************/

class Planet;
#define JEOD_FRIEND_CLASS Planet_moon_default_data

// JEOD includes
#include "environment/planet/include/base_planet.hh"
#include "environment/planet/include/planet.hh"
#include "utils/named_item/include/named_item.hh"

// Unsupported includes
#include "../include/moon.hh"

//! Namespace jeod
namespace jeod
{

void Planet_moon_default_data::initialize(Planet * Planet_ptr)
{
    /*
        // Transformation from Moon_PA to Moon_ME extracted from pxform_c at full precision from DE421
        double m[3][3] = {
            {    0.9999998732547140, -0.0003292854223755712,  0.0003808696186713873},
            { 0.0003292860002109470,     0.9999999457843058, -1.454440937836270e-06},
            {-0.0003808691190960780,  1.579855786826908e-06,     0.9999999274681064}
        };
    */

    // Transformation from Moon_PA to Moon_ME extracted from pxform_c at full precision from DE440
    double m[3][3] = {
        {    0.9999998731138765, -0.0003289586579141938,  0.0003815212082114573},
        { 0.0003289591969874853,     0.9999999458920105, -1.350206003622702e-06},
        {-0.0003815207434061568,  1.475710742587233e-06,     0.9999999272198697}
    };

    Planet_ptr->name = "Moon";

    /* GSFC Moon fact sheet */
    Planet_ptr->flat_coeff = 0.00125;
    Planet_ptr->r_eq = 1000 * (1738.14);

    Planet_ptr->set_alt_pfix(m);
}
} // namespace jeod
