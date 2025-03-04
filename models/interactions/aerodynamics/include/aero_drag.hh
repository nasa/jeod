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
 * @addtogroup Interactions
 * @{
 * @addtogroup Aerodynamics
 * @{
 *
 * @file models/interactions/aerodynamics/include/aero_drag.hh
 * Orbital aerodynamics paramter definitions and the main class for
 * calculating aerodynamic drag.
 */

/*
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
    ((Orbital body modelled as a series of flat plates, or as a simple
     coefficient of drag or ballistic coefficient)
     (All plate information referenced to the orbital vehicle's structural
      frame.))
Library dependencies:
    ((../src/aero_drag.cc))


*/

#ifndef JEOD_AERO_DRAG_HH
#define JEOD_AERO_DRAG_HH

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "default_aero.hh"

//! Namespace jeod
namespace jeod
{

class AtmosphereState;
class AeroSurface;

/**
 * Contains enumerations associated with aerodynamic drag
 */
class AeroDragEnum
{
public:
    /**
     * Dictates how the coefficients of drag will be
     * calculated when using a flat plate model.
     */
    enum CoefCalcMethod
    {
        Specular = 0, /*
          Molecules "bounce" producing forces in the normal-direction only */
        Diffuse,      /*
          Molecules "stick" to the plate, producing forces in the direction of
          the relative velocity */
        Mixed,        /*
          A method that combines the specular and diffuse methods */
        Calc_coef     /*
          Drag coefficients are calculated for direct pressure (normal to the
          surface) and slip-friction (tangential to the surface) based on
          angle of attack, material diffuse/specular properties, wall temperature,
          altitude and gas constant */
    };
};

/**
 * Contains parameters associated with aerodynamic drag.
 */
class AeroDragParameters
{
public:
    /**
     * dynamic pressure, 0.5 * density * velocity^2
     */
    double dynamic_pressure{}; //!< trick_units(N/m2)

    /**
     * gas constant, ala PV = mRT; R = 287 for air.
     */
    double gas_const{}; //!< trick_units(N*m/kg/K)

    /**
     * temperature of the incident stream of free molecular flow.
     */
    double temp_free_stream{}; //!< trick_units(K)
};

/**
 * The main class for calculating aerodynamic drag.
 */
class AerodynamicDrag
{
public:
    /**
     * On = aerodynamics enabled
     */
    bool active{true}; //!< trick_units(--)

    /**
     * Use constant density for aero drag?
     */
    bool constant_density{}; //!< trick_units(--)

    /**
     * Density of the last time AerodynamicDrag was
     * used. If constant_density is set true,
     * then this is the density that will be used
     */
    double density{}; //!< trick_units(kg/m3)

    /**
     * Total Force due to aero drag, resulting from all plates combined
     */
    double aero_force[3]{}; //!< trick_units(N)

    /**
     * Total torque due to aero drag, resulting from all plates combined
     */
    double aero_torque[3]{}; //!< trick_units(N*m)

    /**
     * parameters shared with plate model
     */
    AeroDragParameters param; //!< trick_units(--)

    /**
     * Use the default behavior?
     */
    bool use_default_behavior{true}; //!< trick_units(--)

    /**
     * Pointer to the current aero surface
     */
    AeroSurface * aero_surface_ptr{}; //!< trick_units(--)

    /**
     * Pointer to an object that defines the default aero behavior.
     * This is used if the AeroSurface pointer in aero_drag is set to NULL.
     * Defaults to ballistic drag, but can be overridden
     */
    DefaultAero * default_behavior; //!< trick_units(--)

    /**
     * Spherical, ballistic drag.
     * The default, default behavior. Can be overridden by resetting
     * the "default_behavior" pointer
     */
    DefaultAero ballistic_drag; //!< trick_units(--)

    AerodynamicDrag();
    virtual ~AerodynamicDrag() = default;
    AerodynamicDrag(const AerodynamicDrag &) = delete;
    AerodynamicDrag & operator=(const AerodynamicDrag &) = delete;

    void aero_drag(double inertial_velocity[3],
                   AtmosphereState * atmos_ptr,
                   double T_inertial_struct[3][3],
                   double mass,
                   double center_grav[3]);

    // Set the variable aero_surface_ptr.
    void set_aero_surface(AeroSurface & to_set);

    // clears the AeroSurface used for calculation.
    void clear_aero_surface();
};

} // namespace jeod

#ifdef TRICK_VER
#include "aero_surface.hh"
#endif
#endif

/**
 * @}
 * @}
 * @}
 */
