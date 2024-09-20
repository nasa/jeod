//=============================================================================
// Notices:
//
// Copyright © 2023 United States Government as represented by the Administrator
// of the National Aeronautics and Space Administration.  All Rights Reserved.
//
//
// Disclaimers:
//
// No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF
// ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED
// TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO SPECIFICATIONS, ANY
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
// FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL BE ERROR
// FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO THE
// SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN
// ENDORSEMENT BY GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS,
// RESULTING DESIGNS, HARDWARE, SOFTWARE PRODUCTS OR ANY OTHER APPLICATIONS
// RESULTING FROM USE OF THE SUBJECT SOFTWARE.  FURTHER, GOVERNMENT AGENCY
// DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING THIRD-PARTY SOFTWARE,
// IF PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT "AS IS."
//
// Waiver and Indemnity:  RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS AGAINST THE
// UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
// PRIOR RECIPIENT.  IF RECIPIENT'S USE OF THE SUBJECT SOFTWARE RESULTS IN ANY
// LIABILITIES, DEMANDS, DAMAGES, EXPENSES OR LOSSES ARISING FROM SUCH USE,
// INCLUDING ANY DAMAGES FROM PRODUCTS BASED ON, OR RESULTING FROM, RECIPIENT'S
// USE OF THE SUBJECT SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE
// UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
// PRIOR RECIPIENT, TO THE EXTENT PERMITTED BY LAW.  RECIPIENT'S SOLE REMEDY FOR
// ANY SUCH MATTER SHALL BE THE IMMEDIATE, UNILATERAL TERMINATION OF THIS
// AGREEMENT.
//
//=============================================================================
//
//
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
 * @file models/utils/planet_fixed/planet_fixed_posn/include/planet_fixed_posn.hh
 * Planet centered fixed position model: alternate coordinate system
 * definitions and transformations to those coordinate systems.
 */

/********************************* TRICK HEADER *******************************
Purpose:
   ()

References:
   (((Vallado, David. A) (Fundamentals of Astrodynamics and Applications,
      2nd Ed.) (Microcosm Press: El Segundo, CA) (2004) (Page 139-140)
      (ISBN:1-881883-12-4)))

Assumptions and Limitations:
   ((Given Cartesian coordinates are assumed to be in planet-centered, planet-
     fixed coordinates.))

Library Dependency:
   ((../src/planet_fixed_posn.cc))



*******************************************************************************/

#ifndef JEOD_PLANET_FIXED_POSITION_HH
#define JEOD_PLANET_FIXED_POSITION_HH

// System includes

// JEOD includes
#include "environment/planet/include/class_declarations.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "alt_lat_long_state.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Contains various representations of position with respect to a planet.
 */
class PlanetFixedPosition
{
    JEOD_MAKE_SIM_INTERFACES(jeod, PlanetFixedPosition)

    // Static data
public:
    /**
     * Limit of ratio of radial distance to planet equatorial radius below
     * which planetary coordinates are deemed to be invalid.
     */
    static constexpr double Small_radius_limit{1e-60}; //!< trick_io(*o) trick_units(--)

    /**
     * Limit of number of iterations used to solve elliptic parameters.
     */
    static constexpr int Max_iteration_limit{10}; //!< trick_io(*o) trick_units(--)

    // Member Data
public:
    /**
     * An object's current position in elliptical coordinates. Per Vallado
     * p. 140, elliptical latitude is the angle between the equatorial plane
     * and the surface normal on the ellipsoid at the point of interest.
     * Similarly, elliptical longitude is assumed to be the angle between the
     * reference meridian and the surface normal on the ellipsoid at the
     * point of interest.
     */
    AltLatLongState ellip_coords; //!< trick_units(--)

    /**
     * The same object's current position in spherical coordinates. Per Vallado
     * p. 140, spherical latitude is the angle measured at the planet's center
     * from the equatorial plane to the point of interest. Similarly, spherical
     * longitude is also assumed to be the angle measured at the planet's center
     * from the reference meridian to the point of interest.
     */
    AltLatLongState sphere_coords; //!< trick_units(--)

    /**
     * The planet-centered, planet-fixed position of the object
     */
    double cart_coords[3]{}; //!< trick_units(m)

    /**
     * The planet currently associated with this
     */
    Planet * planet{}; //!< trick_units(--)

    // Member functions
public:
    PlanetFixedPosition() = default;
    virtual ~PlanetFixedPosition() = default;

    // Initialize model
    virtual void initialize(Planet * planet_in);

    // Update from Cartesian position input
    virtual void update_from_cart(const double cart[3]);

    // Update from spherical position input
    virtual void update_from_spher(const AltLatLongState & spher);

    // Update from elliptical position input
    virtual void update_from_ellip(const AltLatLongState & ellip);

protected:
    // Calculate the spherical representation for the current cartesian coords
    void cart_to_spher();

    // Calculate the elliptical representation for the current cartesian coords
    void cart_to_ellip();

    // Calculate the cartesian representation for the current spherical coords
    void spher_to_cart();

    // Calculate the cartesian representation for the current elliptical coords
    void ellip_to_cart();

    // Calculate elliptic latitude and altitude
    int get_elliptic_parameters(double r, double z, double & f, double & h, int maxIters = Max_iteration_limit);
};

} // namespace jeod

#ifdef TRICK_VER
#include "environment/planet/include/planet.hh"
#endif

#endif

/**
 * @}
 * @}
 * @}
 * @}
 */
