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
 * @file models/interactions/radiation_pressure/include/radiation_source.hh
 * Radiation pressure parameter and variable definitions
 */

/***************************TRICK * HEADER**************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
    ((vehicle modeled as a series of plates)
     (All plate information referenced wrt structural frame))

Library dependencies:
    ((../src/radiation_source.cc))



*****************************************************************************/

#ifndef JEOD_RADIATION_SOURCE_HH
#define JEOD_RADIATION_SOURCE_HH

// System includes
#include <string>
#include <utility>

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod
{

class RefFrame;
class DynManager;

// Model includes
class RadiationThirdBody;

/**
 * Provides information on the source of the incident radiation
 */
class RadiationSource
{
    JEOD_MAKE_SIM_INTERFACES(jeod, RadiationSource)

public:
    /**
     * Solar Luminosity.
     */
    const double solar_luminosity{3.827E+26}; //!< trick_units(--)

    /**
     * Mean solar radius.
     */
    const double solar_radius{6.98E+08}; //!< trick_units(m)

    /**
     * The name of the source of the illumination (usually Sun)
     */
    std::string name{"Sun"}; //!< trick_units(--)

    /**
     * (inertial) unit vector associated with primary radiative flux.
     */
    double flux_hat[3]{}; //!< trick_units(--)

    /**
     * magnitude of principle radiative flux vector
     */
    double flux_mag{}; //!< trick_units(N/m2)

    /**
     * distance from source to vehicle cg.
     */
    double d_source_to_cg{}; //!< trick_units(m)

    /**
     * Luminosity of primary source
     */
    double luminosity{solar_luminosity}; //!< trick_units(--)

    /**
     * Radius of primary source
     */
    double radius{solar_radius}; //!< trick_units(m)

    /**
     * (N/M/s)  Power per unit area in the inertial reference frame
     */
    double flux_inertial[3]{}; //!< trick_units(--)

    /**
     * (N/M*s)  Power per unit area in the vehicle structural reference frame
     */
    double flux_struc[3]{}; //!< trick_units(--)

    /**
     * Unit vector representing flux vector in the vehicle structural
     * reference frame
     */
    double flux_struc_hat[3]{}; //!< trick_units(--)

    /**
     * vehicle cg position w.r.t vehicle strucural origin, expressed in
     * inertial RF.
     */
    double inertial_cg[3]{}; //!< trick_units(m)

    /**
     * vehicle cg position relative to the source
     */
    double source_to_cg[3]{}; //!< trick_units(m)

    /**
     * vehicle struc frame origin position relative to source.
     */
    double source_to_struc_origin[3]{}; //!< trick_units(m)

    /**
     * flags that more than one body are casting shadows on the vehicle.
     */
    bool multiple_shadow_bodies{}; //!< trick_units(--)

    /**
     * the inertial reference frame associated with this source
     */
    RefFrame * inertial_frame_ptr{}; //!< trick_units(--)

    /*  The following 5 objects are a hold-over from JEOD 2.1 running under Trick 7.
        With Trick10, there is an alternate preferred method for declaring
        RadiationThirdBody objects, and these elements should not be used.  However,
        they are retained for backwards-compatibility.*/

    /**
     * can there be 3rd-body interference, either shadowing or reflection.
     */
    bool bodies_active{}; //!< trick_units(--)

    /**
     * number of ThirdBodies available.
     */
    unsigned int num_bodies{}; //!< trick_units(count)

    /**
     * Planetary bodies that provide shadowing or indirect, reflected,
     * illumination.
     */
    RadiationThirdBody ** third_body{}; //!< trick_units(--)

    /**
     * distinguishes between conical and cylindrical shadowing.  NOTE this is
     * provided for backward-compatibility and should not be used.
     */
    enum OldShadowGeometry
    {
        Cylindrical = 0, /**< planet casts a cylindrical shadow */
        Cyl = 1,         /**< planet casts a cylindrical shadow */
        Conical = 2,     /**< planet casts a conical shadow */
        Con = 3          /**< planet casts a conical shadow */
    };

    /**
     * Flag indicating cylindrical / conical shadow geometry
     */
    OldShadowGeometry shadow_geometry{}; //!< trick_units(--)

    //  END OF COMPATIBILITY-DRIVEN BLOCK

    RadiationSource() = default;
    virtual ~RadiationSource() = default;
    RadiationSource(const RadiationSource &) = delete;
    RadiationSource & operator=(const RadiationSource &) = delete;

    virtual void initialize(DynManager * dyn_manager_ptr);

    virtual void calculate_flux(RefFrame & veh_struc_frame, const double center_grav[3]);

    /**
     * Setter for the name.
     */
    void set_name(std::string name_in)
    {
        name = std::move(name_in);
    }
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
