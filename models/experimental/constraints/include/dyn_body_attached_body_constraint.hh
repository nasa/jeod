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
 * @addtogroup Experimental
 * @{
 * @addtogroup Constraints
 * @{
 *
 * @file
 * Defines class DynBodyAttachedBodyConstraint.
 */

/*
Purpose: ()
Library dependencies: ((../src/dyn_body_attached_body_constraint.cc))
*/


#ifndef JEOD_DYN_BODY_ATTACHED_BODY_CONSTRAINT_HH
#define JEOD_DYN_BODY_ATTACHED_BODY_CONSTRAINT_HH


#include "dyn_body_constraint.hh"

#include "constrained_rigid_mass.hh"
#include "force_constraint_component.hh"
#include "torque_constraint_component.hh"

#include "utils/sim_interface/include/jeod_class.hh"

#include <cassert>



//! Namespace jeod 
namespace jeod {

class DynBodyAttachedBodyConstraint : public DynBodyConstraint
{

    JEOD_MAKE_SIM_INTERFACES(DynBodyAttachedBodyConstraint)

public:

    /**
     * Default constructor.
     */
    DynBodyAttachedBodyConstraint ();

    /**
     * Destructor.
     */
    ~DynBodyAttachedBodyConstraint() override = default;

    /**
     * Get the known wrench for this constrained object.
     * @return A const reference to the effector wrench for this object.
     * @note This function is called prior to any calls to setup_constraint(),
     *   with the effects of all of the pre-constraint wrenches incorporated
     *   into the non_grav_state argument to that function.
     */
    const Wrench& get_effector_wrench () const override
    {
        return null_wrench;
    }

    /**
     * Get the wrench this constrained object exerts on the vehicle.
     * @return A const reference to the constraint wrench for this object.
     * @note This function is called after the constraint values have been set.
     */
    const Wrench& get_constraint_wrench () const override
    {
        return constraint_wrench;
    }

    /**
     * Get the residual nonlinear wrench for this constrained object.
     * @return A const reference to the nonlinear wrench for this object.
     * @note This function is called after calls to compute_constraint_response.
     *   The nonlinear aspects are assumed to be small; they are not
     *   taken into account while solving the constraints equation.
     */
    const Wrench& get_nonlinear_response_wrench () const override
    {
        return null_wrench;
    }

    /**
     * Get the indexed component constraint for this composite constraint.
     * @param index Index of the component constraint to be retrieved.
     *   This is not bounds-checked.
     * @return The indexed component constraint.
     */
    ConstraintComponent* get_component(unsigned index) override
    {
        assert (index < 6);
        if (index < 3)
        {
            return &force_constraints[index];
        }
        else
        {
            return &torque_constraints[index-3];
        }
    }

    /**
     * Update information about the relation between this constraint
     * and the root DynBody.
     * @param vehicle_properties  Various vehicle properties.
     */
    void update_attachment (
        const VehicleProperties& vehicle_properties) override
    {
        DynBodyConstraint::update_attachment (vehicle_properties);
        constrained_mass.update_attachment (vehicle_properties);
        update_component_attachment (vehicle_properties);
    }

    /**
     * Prepare this object for solving a constraint problem.
     * @param vehicle_properties  Various vehicle properties.
     * @param non_grav_state  The non-gravitational response of the
     *   root body to non-constraint forces and torques, including
     *   the pre-constraint wrenches.
     * @note This function is called prior to the calls to set_self_coeff(),
     *   set_cross_coeff(), and set_r_h_s().
     */
    void setup_constraint(
        const VehicleProperties& vehicle_properties,
        const VehicleNonGravState& non_grav_state) override;

    /**
     * Set this constrained object's constraint values.
     * @param solution_slice  The solution to the matrix constraint equation
     *   for this constrained object, such that solution_slice[0] contains
     *   the constraint value for the zeroth component, etc.
     * @note This function is called after the matrix constraint equation has
     *   been formulated and solved.
     */
    void set_constraint_values (
        const VectorView<double, double>& solution_slice) override;

    /**
     * Determine whether the integrator needs to be reset, which occurs
     * on transition between modes.
     * This should be called as a scheduled job at the dynamics rate.
     */
    bool check_for_reset ()
    {
        return false;
    }


protected:

    /**
     * The body as a constrained mass.
     */
    ConstrainedRigidMass constrained_mass;

    /**
     * The force constraints that arise from this attached body.
     */
    ForceConstraintComponent force_constraints[3];

    /**
     * The torque constraints that arise from this attached body.
     */
    TorqueConstraintComponent torque_constraints[3];

    /**
     * A wrench that is always zero and inactive.
     */
    Wrench null_wrench; //!< trick_units(--)

    /**
     * The total wrench on the vehicle from this constrained object.
     */
    Wrench constraint_wrench; //!< trick_units(--)
};


} // End JEOD namespace

#endif


/**
 * @}
 * @}
 * @}
 */
