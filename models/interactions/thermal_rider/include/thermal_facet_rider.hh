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
 * @addtogroup ThermalRider
 * @{
 *
 * @file models/interactions/thermal_rider/include/thermal_facet_rider.hh
 * Defining the thermal characteristics of surface facets
 */

/************************** TRICK HEADER***************************************
PURPOSE:
()

REFERENCE:
(((None)))

ASSUMPTIONS AND LIMITATIONS:
((None))

Library dependencies:
((../src/thermal_facet_rider.cc))


*******************************************************************************/

#ifndef JEOD_THERMAL_FACET_RIDER_HH
#define JEOD_THERMAL_FACET_RIDER_HH

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "thermal_integrable_object.hh"

//! Namespace jeod
namespace jeod
{

class InteractionFacet;

/**
 * Defining the thermal characteristics of surface facets
 */
class ThermalFacetRider
{
    JEOD_MAKE_SIM_INTERFACES(jeod, ThermalFacetRider)

public:
    // calculated at runtime
    /**
     * Time since the last temperature calculation for the particular
     * model under consideration.
     */
    static double cycle_time; //!< trick_units(s)

    /**
     * Change in temperature.
     */
    double d_temperature{}; //!< trick_units(--)

    /**
     * Rate at which energy is absorbed from the environment, vehicle, and
     * surface.
     */
    double power_absorb{}; //!< trick_units(--)

    /**
     * Rate at which energy is thermally radiated from the facet.
     */
    double power_emit{}; //!< trick_units(--)

    // Calculated at initialization if using material parameters.  Or can be defined
    // straight out if not.

    /**
     * Heat capacity of the facet.
     */
    double heat_capacity{}; //!< trick_units(--)

    // calculated at initialization

    /**
     * Combination of variables that are held constant for any given facet.
     */
    double rad_constant{}; //!< trick_units(--)

    /**
     * Pointer back to facet that contains this thermal rider.
     */
    InteractionFacet * facet{}; //!< trick_units(--)

    // defined for each facet
    /**
     * Flag set to indicate whether the facet to which this rider is attached
     * has a dynamic temperature variability.
     */
    bool active{true}; //!< trick_units(--)

    /**
     * Rate at which thermal energy is transfered to the facet from within the
     * vehicle.
     */
    double thermal_power_dump{}; //!< trick_units(--)

    // defined for each material that defines the facet (i.e. parameter list)

    /**
     * Fraction of sigma-T^4 (potential for emissive radiation) that is
     * actually emitted.
     */
    double emissivity{}; //!< trick_units(--)

    /**
     * The encapsulation of a first order integrator which can optionally be used
     * to integrate the temperature
     */
    ThermalIntegrableObject integrable_object; //!< trick_units(--)

protected:
    // set at creation
    /**
     * Stefan-Boltzmann constant
     */
    const static double stefan_boltzmann; //!< trick_io(*o) trick_units(--)

private:
    /**
     * The predicted value of temperature at the next time-step.
     * THIS VALUE IS USED
     * ONLY BY THE  THERMAL INTEGRATOR, AND SHOULD NOT BE SET EXTERNALLY.
     */
    double next_temperature{}; //!< trick_units(K)

    /**
     * The dynamic value of the facet kinetic temperature. THIS VALUE IS USED
     * ONLY BY THE  THERMAL INTEGRATOR, AND SHOULD NOT BE SET EXTERNALLY.
     */
    double dynamic_temperature{}; //!< trick_units(K)

    //  static bool conduction_enabled;
    //  static bool conduction_active;
    //  int num_contact_facets;
    //  double * conduction;
    //  facet ** facet_ptr;
    //  double cond_calc;/* --
    //         Power conducted into the facet from a neighboring facet. */

    // Member methods
public:
    ThermalFacetRider() = default;
    virtual ~ThermalFacetRider() = default;
    ThermalFacetRider & operator=(const ThermalFacetRider &) = delete;
    ThermalFacetRider(const ThermalFacetRider &) = delete;

    void initialize(double temperature, double surface_area);

    void accumulate_thermal_sources();

    double integrate();
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
