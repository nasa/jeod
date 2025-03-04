//=============================================================================
// Notices:
//
// Copyright © 2025 United States Government as represented by the Administrator
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
 * @addtogroup Environment
 * @{
 * @addtogroup Planet
 * @{
 *
 * @file models/environment/planet/include/planet.hh
 * Planetary modeling constant parameter definitions.
 */

/********************************* TRICK HEADER *******************************
Purpose:
   ()

References:
   (((Szebeheley, Victor G.) (Adventures In Celestial Mechanics) (University of
      Texas Press, Austin) (Page 159) (ISBN:0-292-75105-2))
    ((Vallado, David. A) (Fundamentals of Astrodynamics and Applications,
      2nd Ed.) (Microcosm Press: El Segundo, CA) (2004) (Page 141)
      (ISBN:1-881883-12-4)))

Assumptions and Limitations:
   ((Central body assumed to be a perfect ellipsoid))

Library Dependency:
   ((../src/planet.cc))



*******************************************************************************/

#ifndef JEOD_PLANET_HH
#define JEOD_PLANET_HH

// System includes

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "base_planet.hh"

//! Namespace jeod
namespace jeod
{

class BaseDynManager;

/**
 * Describes a planet with mass and shape.
 */
class Planet : public BasePlanet
{
    JEOD_MAKE_SIM_INTERFACES(jeod, Planet)

    // Member Data
public:
    /**
     * Mean planet equatorial radius
     */
    double r_eq{}; //!< trick_units(m)

    /**
     * Mean planet polar radius
     */
    double r_pol{}; //!< trick_units(m)

    /**
     * Planet ellipsoid eccentricity, a value between 0 and 1.
     * NOTE: This parameter relates to the planet's shape, not its orbit.
     */
    double e_ellipsoid{}; //!< trick_units(--)

    /**
     * The square of the planet ellipsoid eccentricity.
     */
    double e_ellip_sq{}; //!< trick_units(--)

    /**
     * Planet ellipsoid flattening coefficient, a value between 0 and 1.
     * The Earth's flattening, for example, is about 1/298.3.
     */
    double flat_coeff{}; //!< trick_units(--)

    /**
     * Inverse of the planet ellipsoid flattening constant above.
     */
    double flat_inv{}; //!< trick_units(--)

    // Member functions
public:
    Planet() = default;
    ~Planet() override = default;
    Planet(const Planet & frame) = delete;
    Planet & operator=(const Planet & frame) = delete;

    // Initialization
    void register_model(GravitySource & grav_source, BaseDynManager & dyn_manager);

    void initialize();
};

} // namespace jeod

#ifdef TRICK_VER
#include "environment/gravity/include/gravity_source.hh"
#endif

#endif

/**
 * @}
 * @}
 * @}
 */
