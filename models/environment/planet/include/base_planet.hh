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
 * @file models/environment/planet/include/base_planet.hh
 * Define the class BasePlanet.
 */

/*******************************************************************************
Purpose:
  ()

Library Dependency:
   ((../src/base_planet.cc))


*******************************************************************************/

#ifndef JEOD_BASE_PLANET_HH
#define JEOD_BASE_PLANET_HH

// Model includes

// JEOD includes
#include "environment/ephemerides/ephem_interface/include/ephem_ref_frame.hh"
#include "environment/gravity/include/gravity_source.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// System includes
#include <string>
#include <utility>

// Define macros
#include "utils/math/include/macro_def.hh"

//! Namespace jeod
namespace jeod
{

class BaseEphemeridesManager;

/**
 * A BasePlanet contains the base data needed to model a planet in JEOD.
 * A BasePlanet has a name, a planet-centered inertial reference frame, and
 * a planet-centered planet-fixed reference frame.
 * Details of the planet's shape and mass are in the Planet class,
 * which derives from BasePlanet.
 */
class BasePlanet
{
    JEOD_MAKE_SIM_INTERFACES(jeod, BasePlanet)

    // Member Data
public:
    /**
     * Planet name
     */
    std::string name{""}; //!< trick_units(--)

    /**
     * The GravitySource corresponding to the same planet represented by this
     */
    GravitySource * grav_source{}; //!< trick_units(--)

    /**
     * The planet-centered J2000 pseudo-inertial frame associated with the
     * planet represented by this
     */
    EphemerisRefFrame inertial; //!< trick_units(--)

    /**
     * A secondary pseudo-inertial frame which can be defined by the user
     * to be equatorial for this planet
     */
    EphemerisRefFrame alt_inertial; //!< trick_units(--)

    /**
     * The planet-centered, planet-fixed Cartesian reference frame associated
     * with the planet represented by this
     */
    EphemerisRefFrame pfix; //!< trick_units(--)

    /**
     * A secondary planet-fixed frame which can be defined by the user.
     */
    EphemerisRefFrame alt_pfix; //!< trick_units(--)

    // Member functions
public:
    BasePlanet() = default;
    virtual ~BasePlanet() = default;
    BasePlanet(const BasePlanet &) = delete;
    BasePlanet & operator=(const BasePlanet &) = delete;

    /**
     * Setter for the name.
     */
    void set_name(const std::string & name_in)
    {
        name = name_in;
    }

    // Set the fixed transformation from J2000 to alt_inertial
    virtual void set_alt_inertial(const double trans[3][3]);

    // Set the fixed transformation from J2000 to conventional alt_inertial
    // by using the celestial and ecliptic poles.
    virtual void set_alt_inertial(const double cp[3], const double ep[3]);

    // Set the fixed transformation from pfix to alt_pfix
    virtual void set_alt_pfix(const double trans[3][3]);

    // Get the fixed transformation from pfix to alt_pfix
    virtual void get_alt_pfix(double alt_pfix_trans[3][3]);

    // Calculate the current transformation from J2000 to alt_pfix
    // using the fixed transformation between pfix and alt_pfix
    virtual void calculate_alt_pfix();

    // Register the planet with the ephemerides manager
    virtual void register_planet(BaseEphemeridesManager & ephem_manager);

protected:
    /**
     * Flag to insure the alt_inertial frame is set only once
     */
    bool alt_inertial_set{}; //!< trick_units(--)

    /**
     * The transform from pfix to alt_pfix.
     */
    double alt_pfix_transform[3][3]{IDENTITY_3X3}; //!< trick_units(--)

    /**
     * Flag to insure the alt_pfix transform never changed
     */
    bool alt_pfix_set{}; //!< trick_units(--)
};

} // namespace jeod

#include "utils/math/include/macro_undef.hh"

#endif

/**
 * @}
 * @}
 * @}
 */
