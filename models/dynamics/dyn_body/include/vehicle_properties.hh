//=============================================================================
// Notices:
//
// Copyright © 2022 United States Government as represented by the Administrator
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
 * @addtogroup DynBody
 * @{
 *
 * @file
 * Define the class VehicleProperties.
 */

/*
Purpose: ()
SWIG: (No)
*/


#ifndef JEOD_VEHICLE_PROPERTIES_HH
#define JEOD_VEHICLE_PROPERTIES_HH


#include "experimental/math/include/solver_types.hh"
#include "utils/sim_interface/include/jeod_class.hh"


//! Namespace jeod 
namespace jeod {

/**
 * Captures pointers to various vehicle properties that are commonly used
 * in the constraint concept. As this is potentially quite dangerous, access
 * to the captured members is limited to const getters.
 *
 * This class is not designed for extensibility.
 */
class VehicleProperties
#ifndef SWIG            // Grrr.
                        final
#endif
{

    JEOD_MAKE_SIM_INTERFACES(VehicleProperties)

public:


    // Member functions

    /**
     * Default constructor, for use by Trick only.
     */
    VehicleProperties ()
    :
        parent_to_structure_offset(nullptr),
        parent_to_structure_transform(nullptr),
        mass(nullptr),
        structure_to_body_offset(nullptr),
        inertia(nullptr),
        structure_to_body_transform(nullptr),
        inverse_mass(nullptr),
        inverse_inertia(nullptr)
    {
    }

    /**
     * Non-default constructor that sets all elements.
     * @param parent_to_structure_offset_in  Reference to the vehicle's
     *   structure_point.position vector.
     * @param parent_to_structure_transform_in  Reference to the vehicle's
     *   structure_point.T_parent_this matrix.
     * @param mass_in  Reference to the vehicle's
     *   composite_properties.mass member.
     * @param structure_to_body_offset_in  Reference to the vehicle's
     *   composite_properties.position vector.
     * @param inertia_in  Reference to the vehicle's
     *   composite_properties.inertia tensor.
     * @param structure_to_body_transform_in  Reference to the vehicle's
     *   composite_properties.T_parent_this matrix.
     * @param inverse_mass_in  Reference to the vehicle's
     *   inverse_mass member.
     * @param inverse_inertia_in  Reference to the vehicle's
     *   inverse_inertia member.
     */
    VehicleProperties (
        SolverTypes::Vector3RefT parent_to_structure_offset_in,
        SolverTypes::Matrix3x3RefT parent_to_structure_transform_in,
        double& mass_in,
        SolverTypes::Vector3RefT structure_to_body_offset_in,
        SolverTypes::Matrix3x3RefT inertia_in,
        SolverTypes::Matrix3x3RefT structure_to_body_transform_in,
        double& inverse_mass_in,
        SolverTypes::Matrix3x3RefT inverse_inertia_in)
    :
        parent_to_structure_offset(&parent_to_structure_offset_in),
        parent_to_structure_transform(&parent_to_structure_transform_in),
        mass(&mass_in),
        structure_to_body_offset(&structure_to_body_offset_in),
        inertia(&inertia_in),
        structure_to_body_transform(&structure_to_body_transform_in),
        inverse_mass(&inverse_mass_in),
        inverse_inertia(&inverse_inertia_in)
    {
    }

    // The implicitly defined non-virtual, does-nothing (in this case)
    // destructor works quite fine in this context. With this, there's no need
    // to define a destructor, which means we get implicitly-defined copy and
    // move  constructors and assignment operators "for free".
    // Those don't make much sense, but meh.


    /**
     * @return Const reference to the offset from the parent vehicle's
     * structural frame origin to this vehicle's structural origin,
     * in parent structural coordinates.
     */
    SolverTypes::ConstDecayedVector3T get_parent_to_structure_offset () const
    {
        return *parent_to_structure_offset;
    }

    /**
     * @return Const reference to the transformation matrix from the parent
     * vehicle's structural frame to this vehicle's structural frame.
     */
    SolverTypes::ConstMatrix3x3RefT get_parent_to_structure_transform () const
    {
        return *parent_to_structure_transform;
    }

    /**
     * @return The vehicle mass.
     */
    double get_mass () const
    {
        return *mass;
    }

    /**
     * @return Const reference to the offset from the origin of the vehicle's
     * structural frame to the vehicle's center of mass,
     * in vehicle structural coordinates.
     */
    SolverTypes::ConstDecayedVector3T get_structure_to_body_offset () const
    {
        return *structure_to_body_offset;
    }

    /**
     * @return Const reference to the vehicle's inertia tensor,
     * in vehicle body frame coordinates.
     */
    SolverTypes::ConstMatrix3x3RefT get_inertia () const
    {
        return *inertia;
    }

    /**
     * @return Const reference to the transformation matrix from the vehicle's
     * structural frame to its body frame.
     */
    SolverTypes::Matrix3x3RefT get_structure_to_body_transform () const
    {
        return *structure_to_body_transform;
    }

    /**
     * @return The multiplicative inverse of the vehicle's mass.
     */
    double get_inverse_mass () const
    {
        return *inverse_mass;
    }

    /**
     * @return Const reference to the inverse of the vehicle's inertia tensor,
     * in vehicle body frame coordinates.
     */
    SolverTypes::Matrix3x3RefT get_inverse_inertia () const
    {
        return *inverse_inertia;
    }


private:


    /**
     *  Pointer to the vehicle's structure_point.position vector.
     */
    SolverTypes::Vector3PointerT parent_to_structure_offset; //!< trick_units(m)

    /**
     *  Pointer to the vehicle's structure_point.T_parent_this matrix.
     */
    SolverTypes::Matrix3x3PointerT parent_to_structure_transform; //!< trick_units(--)

    /**
     *  Pointer to the vehicle's composite_properties.mass member.
     */
    double* mass; //!< trick_units(kg)

    /**
     *  Pointer to the vehicle's composite_properties.position vector.
     */
    SolverTypes::Vector3PointerT structure_to_body_offset; //!< trick_units(m)

    /**
     *  Pointer to the vehicle's composite_properties.inertia tensor.
     */
    SolverTypes::Matrix3x3PointerT inertia; //!< trick_units(m^2*kg)

    /**
     *  Pointer to the vehicle's composite_properties.T_parent_this matrix.
     */
    SolverTypes::Matrix3x3PointerT structure_to_body_transform; //!< trick_units(--)

    /**
     *  Pointer to the vehicle's inverse_mass member.
     */
    double* inverse_mass; //!< trick_units(1/kg)

    /**
     *  Pointer to the vehicle's inverse_inertia member.
     */
    SolverTypes::Matrix3x3PointerT inverse_inertia; //!< trick_units(1/kg/m^2)
};

} // End JEOD namespace


#endif


/**
 * @}
 * @}
 * @}
 */
