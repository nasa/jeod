/**
 * @addtogroup Models
 * @{
 * @addtogroup Environment
 * @{
 * @addtogroup EarthLighting
 * @{
 *
 * @file models/environment/earth_lighting/src/earth_lighting.cc
 * Implementation of the classes necessary for calculating
 * Sun, Moon and Earth lighting effects on a vehicle orbiting
 * the Earth
 */

/***************************** TRICK HEADER *******************************
PURPOSE:
   ()
REFERENCE:
     (((Vallado, D.)
      (Fundamentals of Astrodynamics and Applications, 2nd Ed.)
      (Kluwer Acedemic Publishers) (2001) (Pages 285-297)))

ASSUMPTIONS AND LIMITATIONS:
    ((See sun_ephemeris for assumptions)
     (Only computes earth_lighting for Sun and Moon around Earth))

CLASS:
    (scheduled)

LIBRARY DEPENDENCY:
    ((earth_lighting.cc)
     (earth_lighting_messages.cc)
     (utils/message/src/message_handler.cc))


******************************************************************************/

// System includes
#include <cmath>
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "environment/planet/include/planet.hh"
#include "utils/math/include/vector3.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/earth_lighting.hh"
#include "../include/earth_lighting_messages.hh"

//! Namespace jeod
namespace jeod
{

static constexpr double epsilon = 1.0e-12;

/* Initialize the EarthLighting object with the applicable DynManager */

/**
 * Initializes the EarthLighting object form the DynManager
 * object. Will find the Earth, Sun and Moon objects and do
 * the necessary setup to calculate earth lighting
 * \param[in] manager The Dyn Manager object that includes the ephemeris for Sun, Earth and Moon
 */

/*******************************************************************************
  Function: initialize
  Purpose: (Initializes the EarthLighting object form the DynManager
            object. Will find the Earth, Sun and Moon objects and do
            the necessary setup to calculate earth lighting)
  Class: (initialization)
*******************************************************************************/

void EarthLighting::initialize(DynManager & manager)
{
    earth = manager.find_planet("Earth");

    if(earth == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             EarthLightingMessages::initialization_error,
                             "No planet named Earth was found in the Dynamics Manager "
                             "supplied to EarthLighting::initialize. This is required, "
                             "and initialization has failed.\n");

        return;
    }

    moon = manager.find_planet("Moon");

    if(moon == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             EarthLightingMessages::initialization_error,
                             "No planet named Moon was found in the Dynamics Manager "
                             "supplied to EarthLighting::initialize. This is required, "
                             "and initialization has failed.\n");

        return;
    }

    sun = manager.find_planet("Sun");

    if(sun == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             EarthLightingMessages::initialization_error,
                             "No planet named Sun was found in the Dynamics Manager "
                             "supplied to EarthLighting::initialize. This is required, "
                             "and initialization has failed.\n");

        return;
    }

    // cache pointers to frames and radius information into the
    // earth lighting object, and subscribe to the necessary frames

    earth_frame = &earth->inertial;
    moon_frame = &moon->inertial;
    sun_frame = &sun->inertial;

    manager.subscribe_to_frame(earth->inertial);
    manager.subscribe_to_frame(moon->inertial);
    manager.subscribe_to_frame(sun->inertial);

    earth_body.radius = earth->r_eq;
    moon_body.radius = moon->r_eq;
    sun_body.radius = sun->r_eq;
}

/***************************** TRICK HEADER *******************************

FUNCTION: circle_intersect
PURPOSE:
    (This routine computes the area of intersection of two overlapping
     circles.)


ASSUMPTIONS AND LIMITATIONS:
    ((epsilon comparison in if test help protect against a numerical
      singularity for d_centers = 0.0 ))

******************************************************************************/
int EarthLighting::circle_intersect(double r_bottom, double r_top, double d_centers, double * area)
{
    double d_c2;
    double r_t2;
    double r_b2;
    double diff_r2;
    double top_ang;
    double cos_top_ang;
    double top_area;
    double bottom_ang;
    double cos_bottom_ang;
    double bottom_area;

    /***************************************************/
    /* Quick check to see if circles are intersecting. */
    /***************************************************/
    if(d_centers > r_bottom + r_top)
    {
        /* Compute the area of intersection. */
        *area = 0.0;

        /* Return negative intersection flag. */
        return (0);
    }

    /*************************************************************/
    /* Check to see if one circle completely contains the other. */
    /*************************************************************/
    if(r_bottom > r_top)
    {
        /* Check to see if top circle is completely inside bottom circle. */
        if(d_centers < (r_bottom - r_top) + epsilon)
        {
            /* Area of intersection is entire top circle. */
            *area = M_PI * r_top * r_top;

            /* Return positive intersection flag. */
            return (1);
        }
    }
    else
    {
        /* Check to see if top circle completely covers bottom circle. */
        if(d_centers < (r_top - r_bottom) + epsilon)
        {
            /* Area of intersection is entire bottom circle. */
            *area = M_PI * r_bottom * r_bottom;

            /* Return positive intersection flag. */
            return (1);
        }
    }

    /*********************************************************************/
    /* Compute the area of intersection of the two overlappping circles. */
    /*********************************************************************/

    /* Compute working variables. */
    d_c2 = d_centers * d_centers;
    r_t2 = r_top * r_top;
    r_b2 = r_bottom * r_bottom;
    diff_r2 = r_b2 - r_t2;

    /* Compute the bottom intersection angle (law of cosines). */
    cos_bottom_ang = (d_c2 + diff_r2) / (2.0 * d_centers * r_bottom);
    bottom_ang = acos(cos_bottom_ang);

    /* Compute the top intersection angles (law of cosines). */
    cos_top_ang = (d_c2 - diff_r2) / (2.0 * d_centers * r_top);
    top_ang = acos(cos_top_ang);

    /* Compute the area of intersection. */
    top_area = r_t2 * (top_ang - (sin(top_ang) * cos_top_ang));
    bottom_area = r_b2 * (bottom_ang - (sin(bottom_ang) * cos_bottom_ang));
    *area = top_area + bottom_area;

    /* Return a positive for intersection. */
    return (1);
}

/**
 * Calculate earth lighting effects at the given position
 * \param[in] pos_veh The position of the point of interest in the earth inertial frame\n Units: M
 */

void EarthLighting::calc_lighting(const double pos_veh[3])
{
    if(active == false)
    {
        return;
    }

    int iinc;

    double eclipse_area;

    double cross_prod[3];
    double cross_mag;
    double cos_obs_ang;
    double sin_obs_ang;

    // Get the necessary relative positions
    sun_frame->compute_position_from(*earth_frame, pos_sun);
    moon_frame->compute_position_from(*earth_frame, pos_moon);

    /* Compute the relative positions of the celestial bodies. */
    for(iinc = 0; iinc < 3; iinc++)
    {
        moon_body.position[iinc] = pos_moon[iinc] - pos_veh[iinc];
        sun_body.position[iinc] = pos_sun[iinc] - pos_veh[iinc];
        earth_body.position[iinc] = -pos_veh[iinc];
    }

    /* Compute the distance to the celestial bodies. */
    moon_body.distance = Vector3::vmag(moon_body.position);
    sun_body.distance = Vector3::vmag(sun_body.position);
    earth_body.distance = Vector3::vmag(earth_body.position);

    /* Compute the apparent half angles of the celestial bodies. */
    moon_body.half_angle = asin(moon_body.radius / moon_body.distance);
    sun_body.half_angle = asin(sun_body.radius / sun_body.distance);
    if(earth_body.distance >= earth_body.radius)
    {
        earth_body.half_angle = asin(earth_body.radius / earth_body.distance);
    }
    else
    {
        earth_body.half_angle = M_PI_2;
    }

    /**********************************************************************/
    /* Compute the observation angle between the earth and light sources. */
    /**********************************************************************/
    /* Moon */
    //   cos_obs_ang =    V_DOT( moon_body.position, earth_body.position )
    //                  / ( moon_body.distance* earth_body.distance );
    cos_obs_ang = Vector3::dot(moon_body.position, earth_body.position) / (moon_body.distance * earth_body.distance);
    // V_CROSS( cross_prod, moon_body.position, earth_body.position );
    Vector3::cross(moon_body.position, earth_body.position, cross_prod);
    cross_mag = Vector3::vmag(cross_prod);
    sin_obs_ang = cross_mag / (moon_body.distance * earth_body.distance);
    moon_earth.obs_angle = atan2(sin_obs_ang, cos_obs_ang);

    /* Sun */
    //   cos_obs_ang =   V_DOT( sun_body.position, earth_body.position )
    //                 / ( sun_body.distance* earth_body.distance );
    cos_obs_ang = Vector3::dot(sun_body.position, earth_body.position) / (sun_body.distance * earth_body.distance);
    //    V_CROSS( cross_prod, sun_body.position, earth_body.position );
    Vector3::cross(sun_body.position, earth_body.position, cross_prod);
    cross_mag = Vector3::vmag(cross_prod);
    sin_obs_ang = cross_mag / (sun_body.distance * earth_body.distance);
    sun_earth.obs_angle = atan2(sin_obs_ang, cos_obs_ang);

    /* Determine if earth occludes the sun. */
    /* Note: these represent arc-lengths on a unit sphere. */
    circle_intersect(sun_body.half_angle, earth_body.half_angle, sun_earth.obs_angle, &eclipse_area);

    /* Compute the sun lighting from the eclipse area. */
    sun_earth.occlusion = eclipse_area / (sun_body.half_angle * sun_body.half_angle * M_PI);
    sun_earth.visible = 1.0 - sun_earth.occlusion;
    sun_earth.lighting = sun_earth.phase * sun_earth.visible;

    /* Determine if earth occludes the moon. */
    /* Note: these represent arc-lengths on a unit sphere. */
    circle_intersect(moon_body.half_angle, earth_body.half_angle, moon_earth.obs_angle, &eclipse_area);

    /* Compute the moon lighting from the eclipse area. */
    moon_earth.occlusion = eclipse_area / (moon_body.half_angle * moon_body.half_angle * M_PI);
    moon_earth.visible = 1.0 - moon_earth.occlusion;

    /* Apply further scaling by apparent lunar phase. */
    moon_earth.lighting = moon_earth.phase * moon_earth.visible;

    /* Crude approximation for Earth albedo. */
    earth_albedo.lighting = fabs(sun_earth.obs_angle / M_PI);
    earth_albedo.lighting *= sun_earth.lighting;
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
