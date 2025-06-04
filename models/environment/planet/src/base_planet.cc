/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup Planet
 * @{
 *
 * @file models/environment/planet/src/base_planet.cc
 * Planet modeling class methods
 */

/********************************* TRICK HEADER ********************************
Purpose:
   ()

Library Dependency:
   ((base_planet.cc)
    (planet_messages.cc)
    (environment/ephemerides/ephem_interface/src/ephem_ref_frame.cc)
    (utils/message/src/message_handler.cc))



*******************************************************************************/

// System includes
#include <cstddef>
#include <cstring>

// JEOD includes
#include "environment/ephemerides/ephem_manager/include/base_ephem_manager.hh"
#include "utils/math/include/vector3.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/base_planet.hh"
#include "../include/planet_messages.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Set the fixed transformation from J2000 to alt_inertial
 *
 * \par Assumptions and Limitations
 *  - Method only works once
 * \param[in] trans trans J2000->alt_inertial
 */
void BasePlanet::set_alt_inertial(const double trans[3][3])
{
    if(alt_inertial_set)
    {
        return;
    }
    alt_inertial_set = true;

    for(unsigned ii = 0; ii < 3; ++ii)
    {
        for(unsigned jj = 0; jj < 3; ++jj)
        {
            alt_inertial.state.rot.T_parent_this[ii][jj] = trans[ii][jj];
        }
    }

    alt_inertial.state.rot.compute_quaternion();
}

/**
 * Use the celestial and ecliptic poles to set the conventional fixed
 * transformation from J2000 to alt_inertial
 *
 * \par Assumptions and Limitations
 *  - Method only works once
 *  - Celestial and ecliptic poles are not the same
 * \param[in] cp celestial pole unit vector
 * \param[in] ep Ecliptic pole unit vector
 */
void BasePlanet::set_alt_inertial(const double cp[3], const double ep[3])
{
    double m[3][3];

    Vector3::normalize(cp, m[2]);
    Vector3::cross(cp, ep, m[0]);
    Vector3::normalize(m[0]);
    Vector3::cross(m[2], m[0], m[1]);
    Vector3::normalize(m[1]);
    set_alt_inertial(m);
}

/**
 * Set the fixed transformation from pfix to alt_pfix
 *
 * \par Assumptions and Limitations
 *  - Method only works once
 * \param[in] trans trans pfix->alt_pfix
 */
void BasePlanet::set_alt_pfix(const double trans[3][3])
{
    if(alt_pfix_set)
    {
        return;
    }
    alt_pfix_set = true;

    Matrix3x3::copy(trans, alt_pfix_transform);
}

/**
 * Get the fixed transformation from pfix to alt_pfix
 *
 * \param[inout] alt_pfix_trans trans pfix->alt_pfix
 */
void BasePlanet::get_alt_pfix(double alt_pfix_trans[3][3])
{
    Matrix3x3::copy(alt_pfix_transform, alt_pfix_trans);
}

/**
 * Calculate the current transformation from J2000 to alt_pfix
 * using the fixed transformation between pfix and alt_pfix
 *
 * \par Assumptions and Limitations
 *      calculates J2000 to alt_pfix using a fixed
 *      transformation from pfix to alt_pfix
 */
void BasePlanet::calculate_alt_pfix()
{
    Matrix3x3::product(alt_pfix_transform, pfix.state.rot.T_parent_this, alt_pfix.state.rot.T_parent_this);

    alt_pfix.state.rot.compute_quaternion();
}

/**
 * Register a BasePlanet object with the Ephemerides Manager.
 * \param[in,out] ephem_manager Ephemerides Manager
 */
void BasePlanet::register_planet(BaseEphemeridesManager & ephem_manager)
{
    // Sanity check: The name must be valid.
    if(name.size() == 0)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             PlanetMessages::name_error,
                             "BasePlanet object has a null or empty name.");
        return;
    }

    // Name the planet-centered inertial and planet-fixed frames.
    inertial.set_name(name, "inertial");
    alt_inertial.set_name(name, "alt_inertial");
    pfix.set_name(name, "pfix");
    alt_pfix.set_name(name, "alt_pfix");

    // Link the planet-fixed frame to the planet-centered inertial frame.
    inertial.add_child(pfix);
    inertial.add_child(alt_inertial);
    inertial.add_child(alt_pfix);

    // Register the planet and the planet reference frames with the manager.
    ephem_manager.add_planet(*this);
    ephem_manager.add_integ_frame(inertial);
    ephem_manager.add_ref_frame(alt_inertial);
    ephem_manager.add_ref_frame(pfix);
    ephem_manager.add_ref_frame(alt_pfix);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
