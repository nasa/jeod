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
 * @addtogroup Dynamics
 * @{
 * @addtogroup Mass
 * @{
 *
 * @file models/dynamics/mass/include/mass_properties.hh
 * Define the class MassProperties.
 */

/********************************* TRICK HEADER *******************************

Purpose:
  ()

*******************************************************************************/

#ifndef JEOD_MASS_PROPERTIES_HH
#define JEOD_MASS_PROPERTIES_HH

// Model includes
#include "class_declarations.hh"
#include "mass_point.hh"

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Defines mass properties -- mass and inertia tensor.
 * A MassBody contains two MassProperties objects that represent the body's
 * core and composite mass properties.
 * A MassProperties object is-a MassPoint and hence a MassPointState
 * by inheritance. The parent point of a MassProperties object is always
 * the structural frame of the MassBody that 'owns' the MassProperties
 * object.
 *
 * Note that a MassProperties does not explicitly define the center of mass
 * location and body frame orientation. These properties are defined by
 * elements inherited from MassPointState:
 * - position defines the center of mass in the structural frame.
 * - Q_parent_this/T_parent_this define the orientation of the body frame
 *   with respect to the structural frame.
 */
class MassProperties : public MassPoint
{
    JEOD_MAKE_SIM_INTERFACES(jeod, MassProperties)

    // Member data
public:
    /**
     * Mass of the subject mass element (always >= 0). The core mass element's
     * mass is set externally while the composite mass element's mass is computed
     * by the mass model.
     */
    double mass{}; //!< trick_units(kg)

    /**
     * Inertia tensor of the subject mass element about the subject mass element's
     * center of mass, expressed in subject mass element body coordinates.
     * Note: The diagonal elements of the inertia tensor are positive moments
     * of inertial while the off-diagonal elements are megative
     * products of inertia.
     */
    double inertia[3][3]{}; //!< trick_units(kg*m2)

    /**
     * The inverse of mass.
     * Used only for root dynamic bodies composite props.
     */
    double inverse_mass{}; //!< trick_units(1/kg)

    /**
     * The inverse of the composite inertia.
     * Used only for root dynamic bodies composite props.
     */
    double inverse_inertia[3][3]{}; //!< trick_units(1/kg/m2)

    // Member functions
public:
    MassProperties() = default;
    MassProperties(const MassProperties &) = delete;
    MassProperties & operator=(const MassProperties &) = delete;
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
