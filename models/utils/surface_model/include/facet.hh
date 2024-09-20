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
 * @addtogroup SurfaceModel
 * @{
 *
 * @file models/utils/surface_model/include/facet.hh
 * Individual facets for use in the surface model
 */

/************************** TRICK HEADER***************************************
PURPOSE:
    ()

REFERENCE:
    (((None)))

ASSUMPTIONS AND LIMITATIONS:
      ((None))

Library dependencies:
    ((../src/facet.cc))


*******************************************************************************/

#ifndef JEOD_FACET_HH
#define JEOD_FACET_HH

// System includes
#include <string>
#include <utility>

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// Model includes
#include "dynamics/mass/include/mass_point_state.hh"

//! Namespace jeod
namespace jeod
{

class MassBody;
class BaseDynManager;

/**
 * A general base class for all surface model facets.
 */
class Facet
{
    JEOD_MAKE_SIM_INTERFACES(jeod, Facet)

    friend class SurfaceModel;

public:
    Facet() = default;
    virtual ~Facet() = default;
    Facet & operator=(const Facet &) = delete;
    Facet(const Facet &) = delete;

    virtual void initialize_mass_connection(BaseDynManager & manager);

    virtual void update_articulation();

    MassBody * get_mass_body_ptr();

    /**
     * Getter for the mass_rel_struct element,
     */
    MassPointState * get_mass_rel_struct()
    {
        return mass_rel_struct;
    }

    /**
     * Setter for the name.
     */
    void set_name(std::string name_in)
    {
        name = std::move(name_in);
    }

    /**
     * Position of the facet in the vehicle structural
     * frame. The reference point of the facet is
     * positioned on a class by class basis (example: for
     * flat plate it is the position of the center
     * of pressure)
     */
    double position[3]{}; //!< trick_units(m)

    /**
     * Position of the facet in the structure frame of the MassBody this
     * facet is associated with. Used in articulation, contact, etc.
     */
    double local_position[3]{}; //!< trick_units(m)

    /**
     * Name of the facet parameters, usually a material
     * type. Used to match the facet to FacetParams
     */
    std::string param_name; //!< trick_units(--)

    /**
     * Name of the facet.
     */
    std::string name; //!< trick_units(--)

    /**
     * The name of the MassBody this facet is associated with. This is used,
     * only for specific applications, such as contact and articulation.
     * Otherwise it is optional.
     * This is used to find and cache a pointer to the mass_body,
     * stored below in 'mass_body_ptr'.
     */
    std::string mass_body_name; //!< trick_units(--)

    /**
     * Kinetic Temperature of the surface
     */
    double temperature{}; //!< trick_units(K)

    /**
     * Area of the plate
     */
    double area{}; //!< trick_units(m2)

protected:
    virtual void update_articulation_internal();

    /**
     * A pointer to the MassBody this facet is associated with. This is used, only
     * for specific applications, such as contact and articulation.
     * Otherwise it is optional. This pointer is cached from the
     * tree of MassBodies for the mass_body_name set above
     */
    MassBody * mass_body_ptr{}; //!< trick_units(--)

    /**
     * The relative state between the mass body this facet is associated
     * with, and the user set structure point in the mass tree. Only
     * used in certain applications, such as articulation.
     */
    MassPointState * mass_rel_struct{}; //!< trick_units(--)

    /**
     * An intermediate position, used as
     * a working variable when doing calculations
     */
    double int_pos[3]{}; //!< trick_units(m)

    /**
     * Indicates if the mass connections
     * for this Facet have been initialized
     */
    bool connections_initialized{}; //!< trick_units(--)
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
