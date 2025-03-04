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
 * @addtogroup Experimental
 * @{
 * @addtogroup Constraints
 * @{
 *
 * @file
 * Defines the class ConstraintFrame.
 */

/*
Purpose: ()
Library dependencies: ((../src/constraint_frame.cc))
*/

#ifndef JEOD_CONSTRAINT_FRAME_HH
#define JEOD_CONSTRAINT_FRAME_HH

#include "dynamics/dyn_body/include/vehicle_properties.hh"
#include "dynamics/dyn_body/include/wrench.hh"
#include "utils/sim_interface/include/jeod_class.hh"

#include <utility>

// Define macros
#include "utils/math/include/macro_def.hh"

//! Namespace jeod
namespace jeod
{

class DynBodyConstraintsSolver;
class VehicleProperties;
class VehicleNonGravState;

/**
 * A constraint frame is directly connected to a vehicle structural frame, which
 * in turn is connected to a root vehicle structural frame.
 * A ConstraintFrame object contains both the connection to the immediate
 * structural frame and to the root vehicle structural frame.
 */
class ConstraintFrame
{
    JEOD_MAKE_SIM_INTERFACES(jeod, ConstraintFrame)

public:
    // Instances of this class are assumed to be protected members of
    // a DynBodyConstraint object, so all member data are public.

    /**
     * The solver associated with the root DynBody object.
     */
    DynBodyConstraintsSolver * solver{}; //!< trick_units(--)

    /**
     * The transformation from the immediate structural frame to the
     * preferred frame for this constraint.
     */
    double T_struct_constraint[3][3]{IDENTITY_3X3}; //!< trick_units(--)

    /**
     * The offset from the origin of the immediate structural frame to the
     * origin of the preferred frame for this constraint, in immediate
     * structure frame coordinates.
     */
    double origin_position_struct[3]{}; //!< trick_units(m)

    /**
     * The transformation from the root body's structural frame to the
     * preferred frame for this constraint.
     */
    double T_root_constraint[3][3]{IDENTITY_3X3}; //!< trick_units(--)

    /**
     * The offset from the origin of the root body's structural frame to the
     * origin of the preferred frame for this constraint, in root body
     * structure frame coordinates.
     */
    double origin_position_root[3]{}; //!< trick_units(m)

    // Member functions
    ConstraintFrame() = default;
    virtual ~ConstraintFrame() = default;
    // Copying a constraint would be handy, but is fraught with issues.
    // For now, copying and assigning is verboten.
    ConstraintFrame(const ConstraintFrame &) = default;
    ConstraintFrame(ConstraintFrame &&) = default;
    ConstraintFrame & operator=(const ConstraintFrame &) = default;
    ConstraintFrame & operator=(ConstraintFrame &&) = default;

#ifndef SWIG
    /**
     * Set the relationship between the constraint frame and the immediate
     * structural frame.
     * @param transform_in The structure-to-constraint frame transform.
     * @param offset_in  The position of the constraint frame origin, in
     *   structural frame coordinates.
     */
    virtual void set_struct_to_constraint_frame(SolverTypes::ConstDecayedMatrix3x3T transform_in,
                                                SolverTypes::ConstDecayedVector3T offset_in);

    /**
     * Get the structure to constraint frame transform.
     */
    virtual SolverTypes::ConstMatrix3x3RefT get_struct_to_constraint_transform() const
    {
        return T_struct_constraint;
    }

    /**
     * Get the structure to constrame frame offset.
     */
    virtual SolverTypes::ConstDecayedVector3T get_struct_to_constraint_offset() const
    {
        return origin_position_struct;
    }

    /**
     * Get the root structure to constraint frame transform.
     */
    virtual SolverTypes::ConstMatrix3x3RefT get_root_to_constraint_transform() const
    {
        return T_root_constraint;
    }

    /**
     * Get the root structure to constraint frame offset.
     */
    virtual SolverTypes::ConstDecayedVector3T get_root_to_constraint_offset() const
    {
        return origin_position_root;
    }
#endif

    /**
     * Update information about the relation between this constraint
     * and the root DynBody.
     * @param vehicle_properties  Various vehicle properties (not used).
     */
    virtual void update_attachment(const VehicleProperties & vehicle_properties JEOD_UNUSED)
    {
        update_root_to_constraint_frame();
    }

    /**
     * Update the root to constraint frame transformation matrix and
     * offset.
     */
    void update_root_to_constraint_frame();
};

} // namespace jeod

#include "utils/math/include/macro_undef.hh"

#endif

/**
 * @}
 * @}
 * @}
 */
