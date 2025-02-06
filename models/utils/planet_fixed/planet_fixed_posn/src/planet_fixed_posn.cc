/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup PlanetFixed
 * @{
 * @addtogroup PlanetFixedPosn
 * @{
 *
 * @file models/utils/planet_fixed/planet_fixed_posn/src/planet_fixed_posn.cc
 * Define PlanetFixedPosition class methods.
 */

/*******************************************************************************
Purpose:
   ()

Library Dependency:
   ((planet_fixed_posn.cc)
    (alt_lat_long_state.cc)
    (planet_fixed_messages.cc)
    (utils/message/src/message_handler.cc))



*******************************************************************************/

// System includes
#include <cmath>
#include <cstddef>

// JEOD includes
#include "environment/planet/include/planet.hh"
#include "utils/math/include/vector3.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/planet_fixed_messages.hh"
#include "../include/planet_fixed_posn.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Initialize a PlanetFixedPosition object.
 * \param[in] planet_in Associated planet
 */
void PlanetFixedPosition::initialize(Planet * planet_in)
{
    planet = planet_in;
}

/**
 * Update from Cartesian position input.
 * \param[in] cart Cartesian coords, PCPF\n Units: M
 */
void PlanetFixedPosition::update_from_cart(const double cart[3])
{
    Vector3::copy(cart, cart_coords);

    cart_to_spher();
    cart_to_ellip();
}

/**
 * Update from Spherical position input.
 * \param[in] spher Spherical AltLatLong position
 */
void PlanetFixedPosition::update_from_spher(const AltLatLongState & spher)
{
    sphere_coords.set_data(spher.altitude, spher.latitude, spher.longitude);

    spher_to_cart();
    cart_to_ellip();
}

/**
 * Update from Elliptical position input.
 * \param[in] ellip Elliptical AltLatLong position
 */
void PlanetFixedPosition::update_from_ellip(const AltLatLongState & ellip)
{
    ellip_coords.set_data(ellip.altitude, ellip.latitude, ellip.longitude);

    ellip_to_cart();
    cart_to_spher();
}

/**
 * Convert from cartesian to spherical position
 */
void PlanetFixedPosition::cart_to_spher()
{
    // Compute the radial distance from planet center
    double r_local = Vector3::vmag(cart_coords);

    // Protect against division by zero.
    if(r_local < planet->r_eq * Small_radius_limit)
    {
        // Check for a NaN error having crept in before determining which
        // error to send.
        if(std::isnan(cart_coords[0]))
        {
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 PlanetFixedMessages::domain_error,
                                 "Prior computation of coordinates has introduced a NaN "
                                 "(not-a-number) error.\n"
                                 "Likely a problem with vehicle instability\n. Exiting.\n");
        }
        else
        {
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 PlanetFixedMessages::domain_error,
                                 "Cartesian coordinates are nearly zero.");
        }
        return;
    }

    // Compute the latitude from the cartesian Z position
    sphere_coords.latitude = asin(cart_coords[2] / r_local);

    // Compute the longitude from X and Y positions.
    sphere_coords.longitude = atan2(cart_coords[1], cart_coords[0]);

    // Compute the altitude from the radius and the mean equtorial radius
    sphere_coords.altitude = r_local - planet->r_eq;
}

/**
 * Convert from cartesian to elliptical position
 */
void PlanetFixedPosition::cart_to_ellip()
{
    // Compute the magnitude of the projection onto the equatorial plane.
    double x_ellipse_sq = (cart_coords[0] * cart_coords[0]) + (cart_coords[1] * cart_coords[1]);
    double x_ellipse = sqrt(x_ellipse_sq);

    // Store locally the Z position in plane of ellipse and its square.
    double z_ellipse = cart_coords[2];
    double z_ellipse_sq = z_ellipse * z_ellipse;

    // Compute the magnitude of the position.
    double r_ellipse_sq = x_ellipse_sq + z_ellipse_sq;
    double r_ellipse = sqrt(r_ellipse_sq);

    // Protect against division by zero.
    if(r_ellipse < planet->r_eq * Small_radius_limit)
    {
        // Check for a NaN error having crept in before determining which
        // error to send.
        if(std::isnan(cart_coords[0]))
        {
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 PlanetFixedMessages::domain_error,
                                 "Prior computation of coordinates has introduced a NaN "
                                 "(not-a-number) error.\n"
                                 "Likely a problem with vehicle instability\n. Exiting.\n");
        }
        else
        {
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 PlanetFixedMessages::domain_error,
                                 "Cartesian coordinates are nearly zero.");
        }
        return;
    }

    // Solve for elliptic parameters
    get_elliptic_parameters(x_ellipse, z_ellipse, ellip_coords.latitude, ellip_coords.altitude);

    // Check for being directly over the pole
    if(std::fpclassify(x_ellipse) != FP_ZERO)
    {
        // Compute the longitude from the X and Y components
        ellip_coords.longitude = atan2(cart_coords[1], cart_coords[0]);
    } // end if
}

/**
 * Convert from spherical to cartesian position
 */
void PlanetFixedPosition::spher_to_cart()
{
    // Compute the Sine & Cosine functions of spherical latitude
    double sin_lat = sin(sphere_coords.latitude);
    double cos_lat = cos(sphere_coords.latitude);

    // Compute the Sine & Cosine functions of spherical longitude
    double sin_lon = sin(sphere_coords.longitude);
    double cos_lon = cos(sphere_coords.longitude);

    // Compute the planet fixed cartesian coordinates
    double radius = planet->r_eq + sphere_coords.altitude;
    cart_coords[0] = radius * cos_lat * cos_lon;
    cart_coords[1] = radius * cos_lat * sin_lon;
    cart_coords[2] = radius * sin_lat;
}

/**
 * Convert from elliptical to cartesian position
 */
void PlanetFixedPosition::ellip_to_cart()
{
    // Compute the Sine function of elliptical latitude
    double sin_lat = sin(ellip_coords.latitude);
    double cos_lat = cos(ellip_coords.latitude);

    // Compute the radius of curvature for the local vertical
    double e = planet->e_ellipsoid;  // Planet's ellipsoid eccentricity
    double radius_eq = planet->r_eq; // Equatorial radius
    double e_sq = e * e;             // Square of elliptic eccentricity
    double rc_ellipse = radius_eq / (sqrt(1.0 - (e_sq * sin_lat * sin_lat)));

    // Compute the X position in the plane of the ellipse
    double x_ellipse = (rc_ellipse + ellip_coords.altitude) * cos_lat;

    // Compute the planet fixed cartesian coordinates
    cart_coords[0] = x_ellipse * cos(ellip_coords.longitude);
    cart_coords[1] = x_ellipse * sin(ellip_coords.longitude);
    cart_coords[2] = ((rc_ellipse * (1.0 - e_sq)) + ellip_coords.altitude) * sin_lat;
}

/*******************************************************************************
Function: PlanetFixedPosition::get_elliptic_parameters
Purpose: Calculate latitude and altitude of a Cartesian point relative to
an oblate ellipsoid.
References:
(((Borkowski, K.m)
(Accurate Algorithms To Transform Geocentric To Geodetic Coordinates)
(Bull. God., 63 (1989), pp. 5056.)))
*******************************************************************************/
int PlanetFixedPosition::get_elliptic_parameters(              /* Return: --
                       Number of iterations */
                                                 double r,     // In:  M Equatorial position, sqrt(x^2+y^2)
                                                 double z,     // In:  M Polar position
                                                 double & lat, // Out: r Latitude
                                                 double & alt, // Out: M Altitude
                                                 int maxIters) // In: -- Maximum number of iterations
{
    int numIters = 0;
    double a = planet->r_eq;
    double b = planet->r_pol;
    double y = 0.0;

    if(r > 0.0)
    {
        double y0 = atan(a * z / (b * r));
        double ar = a * r;
        double bz = b * z;
        double w = atan(bz / ar);
        double c = (a * a - b * b) / sqrt(ar * ar + bz * bz);

        for(numIters = 0; numIters < maxIters; numIters++)
        {
            double d = 2.0 * (cos(y0 - w) - c * cos(2.0 * y0));
            y = y0 - (2.0 * sin(y0 - w) - c * sin(2.0 * y0)) / d;
            if(fabs(y - y0) < 1.0e-12)
            {
                break;
            } // end if
            y0 = y;
        } // end for numIters
        lat = atan(a * sin(y) / (b * cos(y)));
    } // end if
    else
    {
        y = lat = 0.5 * z * M_PI / fabs(z);
    } // end else
    alt = (r - a * cos(y)) * cos(lat) + (z - b * sin(y)) * sin(lat);
    return numIters;
} // end get_elliptic_parameters

} // namespace jeod

/**
 * @}
 * @}
 * @}
 * @}
 */
