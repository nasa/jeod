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
 * @addtogroup RadiationPressure
 * @{
 *
 * @file models/interactions/radiation_pressure/include/radiation_facet.hh
 * Individual facets for use with radiation environment interaction models
 */

/************************** TRICK HEADER***************************************
PURPOSE:
()

REFERENCE:
(((None)))

ASSUMPTIONS AND LIMITATIONS:
((None))

Library dependencies:
((../src/radiation_facet.cc))


*******************************************************************************/

#ifndef JEOD_RADIATION_FACET_HH
#define JEOD_RADIATION_FACET_HH

// JEOD includes
#include "interactions/thermal_rider/include/thermal_facet_rider.hh"
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/surface_model/include/interaction_facet.hh"

// Model includes
#include "radiation_base_facet.hh"

//! Namespace jeod
namespace jeod
{

class RadiationParams;
class Facet;
class RadiationThirdBody;

/**
 * Generic type of facet for radiation pressure
 */
class RadiationFacet : public InteractionFacet,
                       public RadiationBaseFacet
{
    JEOD_MAKE_SIM_INTERFACES(jeod, RadiationFacet)

public:
    // run-time values, typically not defined prior to simulation.

    /**
     * position of center of pressure w.r.t. center of rotation (usually
     * center of mass, or center of gravity), expressed in
     * structural reference frame.  Potentially variable, depending on the
     * topology of the facet and the configuration of the vehicle.
     */
    double crot_to_cp[3]{}; //!< trick_units(m)

    /**
     * Center of pressure (in structural frame). Potentially
     * variable, depending on the topology of the facet.  For a flat plate
     * facet, this just points to the position found in FlatPlate
     */
    double * center_pressure{}; //!< trick_units(m)

protected:
    // shared between plates

    /**
     * quite literally, 2/3.  Occurs frequently with diffuse reflection and
     * emission, so it is calculated once.
     */
    const static double two_thirds; //!< trick_io(*o) trick_units(--)

    // Member methods

public:
    RadiationFacet() = default;
    ~RadiationFacet() override = default;
    RadiationFacet & operator=(const RadiationFacet &) = delete;
    RadiationFacet(const RadiationFacet &) = delete;

    /**
     * Calculates the effect of radiation pressure on the facet.
     */
    virtual void radiation_pressure() = 0;

    /**
     * Initialize the facet geometry.
     * @param cg Center of mass.
     */
    virtual void initialize_geom(double cg[3]) = 0;

    void initialize() override;

    void define_facet_core(Facet * facet, const ThermalFacetRider & facet_thermal, RadiationParams * params);

    /**
     * Get the integrator for thermal characteristics.
     * @return Integrable object that integrates temperature.
     */
    ThermalIntegrableObject & get_thermal_integrator()
    {
        return thermal.integrable_object;
    }
};

} // namespace jeod

#ifdef TRICK_VER
#include "utils/surface_model/include/facet.hh"
#endif

#endif

/**
 * @}
 * @}
 * @}
 */
