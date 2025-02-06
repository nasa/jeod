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
 * @addtogroup Interactions
 * @{
 * @addtogroup RadiationPressure
 * @{
 *
 * @file models/interactions/radiation_pressure/include/radiation_pressure.hh
 * Radiation pressure top-level definition
 */

/***************************TRICK * HEADER**************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
    ((Vehicle surface represented in a surface model, or default sphere)
     (All facet information referenced wrt structural frame))

Library dependencies:
    ((../src/radiation_pressure.cc))



*****************************************************************************/

#ifndef JEOD_RADIATION_PRESSURE_HH
#define JEOD_RADIATION_PRESSURE_HH

// JEOD includes
#include "interactions/thermal_rider/include/thermal_model_rider.hh"
#include "utils/container/include/pointer_vector.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "radiation_source.hh"

//! Namespace jeod
namespace jeod
{

class RadiationThirdBody;
class RadiationSurface;
class RadiationDefaultSurface;
class DynManager;

/**
 * Radiation pressure top-level definition
 */
class RadiationPressure
{
    JEOD_MAKE_SIM_INTERFACES(jeod, RadiationPressure)

    // Member data
public:
    /**
     * Is radiation pressure desired?
     */
    bool active{true}; //!< trick_units(--)

    /**
     * Net force due to radiation
     */
    double force[3]{}; //!< trick_units(N)

    /**
     * Net torque due to radiation
     */
    double torque[3]{}; //!< trick_units(N*m)

    /**
     * fraction of primary flux that is not eclipsed
     */
    double illum_factor{}; //!< trick_units(--)

    /**
     * Collection of data for radiation source
     */
    RadiationSource source; //!< trick_units(--)

    /**
     * Rider to allow dynamic thermal variation on facets in this model.
     */
    ThermalModelRider thermal; //!< trick_units(--)

protected:
    /**
     * Has model been initialized?
     */
    bool initialized{}; //!< trick_units(--)

    /**
     * Flag to indicate whether forces and torques should be calculated.
     */
    bool calculate_forces{true}; //!< trick_units(--)

    /**
     * Flag indicates whether there are any of the third bodies currently
     * active.
     */
    bool third_bodies_active{}; //!< trick_units(--)

    /**
     * Number of ThirdBodies available.
     */
    unsigned int num_third_bodies{}; //!< trick_units(count)

    /**
     * Planetary bodies that provide shadowing or indirect, reflected,
     * illumination.
     */
    JeodPointerVector<RadiationThirdBody>::type third_bodies; //!< trick_io(**)

    /**
     * The surface over which the radiation pressure will be collected.
     * If this is NULL, use the default radiation surface (which is a whole
     * separate entity from a RadiationSurface.
     */
    RadiationSurface * surface_ptr{}; //!< trick_units(--)

    /**
     * If no surface is defined, use a DefaultSurface; this is the pointer to
     * that default surface,
     */
    RadiationDefaultSurface * default_surface_ptr{}; //!< trick_units(--)

    /**
     * Pointer to the Dynamics Manager.
     */
    DynManager * dyn_manager_ptr{}; //!< trick_units(--)

    // Member functions

public:
    RadiationPressure();
    virtual ~RadiationPressure();
    RadiationPressure(const RadiationPressure &) = delete;
    RadiationPressure & operator=(const RadiationPressure &) = delete;

    void initialize(DynManager & dyn_manager, RadiationSurface * surf_ptr, double center_grav[3]);

    void initialize(DynManager & dyn_manager, RadiationDefaultSurface * surf_ptr);

    void update(RefFrame & vehicle_structural_frame, double center_grav[3], double scale_factor, double time);

    void add_third_body(RadiationThirdBody * third_body_ptr);
    void set_third_body_active(const std::string & third_body_name);
    void set_third_body_inactive(const std::string & third_body_name);
    void set_calculate_forces(bool calc_forces);

protected:
    void update_default_surface();

    void update_facet_surface();
    void initialize_environment(DynManager * dyn_mgr_ptr);
    void third_body_adjustments(double time, RefFrame & vehicle_structural_frame);
    int find_third_body(const std::string & third_body_name);
};

} // namespace jeod

#ifdef TRICK_VER
#include "radiation_default_surface.hh"
#include "radiation_surface.hh"
#endif

#endif

/**
 * @}
 * @}
 * @}
 */
