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
 * @file models/interactions/radiation_pressure/include/radiation_surface.hh
 * Vehicle surface model for general environment interaction models
 */

/************************** TRICK HEADER***************************************
PURPOSE:
()

REFERENCE:
(((None)))

ASSUMPTIONS AND LIMITATIONS:
((None))

Library dependencies:
((../src/radiation_surface.cc))


*******************************************************************************/

#ifndef JEOD_RADIATION_SURFACE_HH
#define JEOD_RADIATION_SURFACE_HH

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"
#include "utils/surface_model/include/interaction_surface.hh"

// Model includes

//! Namespace jeod
namespace jeod
{

class RadiationThirdBody;
class RadiationFacet;
class Facet;
class InteractionFacetFactory;
class FacetParams;
class DynBody;

/**
 * The surface of the vehicle that interacts with the incident flux.
 */
class RadiationSurface : public InteractionSurface
{
    JEOD_MAKE_SIM_INTERFACES(jeod, RadiationSurface)

public:
    /**
     * Array of pointers to the Radiation Facets.
     */
    RadiationFacet ** facets{}; //!< trick_units(--)

    /**
     * number of facets in this surface.
     */
    unsigned int num_facets{}; //!< trick_units(--)

    /**
     * Flag to instruct model to include conduction between plates in the
     * thermal calculation.  Requires a thermal_conduction matrix.
     */
    bool include_conduction{}; //!< trick_units(--)

    /**
     * Conductivity values (Watts per Kelvin) between facets.  Assumed
     * symmetric, only the values [small][large] will be considered
     * (e.g. thermal_conduction[2][3] gives the conduction between facets 2
     * and 3, whereas thermal_conduction[3][2] will never be seen.
     */
    double ** thermal_conduction{}; //!< trick_units(--)

    /**
     * Force resulting from all radiative interactions
     */
    double force[3]{}; //!< trick_units(--)

    /**
     * Torque resulting from all radiative interactions
     */
    double torque[3]{}; //!< trick_units(--)

    /**
     * Simple counter, used repeatedly.
     */
    unsigned int ii_facet{}; //!< trick_units(--)

    // Member functions
public:
    RadiationSurface();
    ~RadiationSurface() override;
    RadiationSurface & operator=(const RadiationSurface &) = delete;
    RadiationSurface(const RadiationSurface &) = delete;

    void initialize(double center_grav[3]);

    void allocate_array(unsigned int size) override;

    void allocate_interaction_facet(Facet * facet,
                                    InteractionFacetFactory * factory,
                                    FacetParams * params,
                                    unsigned int index) override;

    void initialize_runtime_values();

    void incident_radiation(double flux_mag, const double flux_struc_hat[3], bool calculate_forces);

    void interact_with_third_body(RadiationThirdBody * third_body_ptr, const bool calculate_forces);

    void accumulate_thermal_sources() override;

    void thermal_integrator() override;

    void equalize_absorption_emission();

    void radiation_pressure();

    void add_thermal_integrators_to(DynBody * dyn_body);
};

} // namespace jeod

#ifdef TRICK_VER
#include "radiation_facet.hh"
#endif

#endif

/**
 * @}
 * @}
 * @}
 */
