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
 * Defines class DynBodyPendulumConstraint.
 */

/*
Purpose: ()
Library dependencies: ((../src/dyn_body_pendulum_constraint.cc))
*/


#ifndef JEOD_DYN_BODY_PENDULUM_CONSTRAINT_HH
#define JEOD_DYN_BODY_PENDULUM_CONSTRAINT_HH


#include "dyn_body_constraint.hh"

#include "constrained_point_mass.hh"
#include "pendulum_constraint_component.hh"

#include "utils/sim_interface/include/jeod_class.hh"

#include <cassert>


//! Namespace jeod 
namespace jeod {

class BasePendulumModel;


class DynBodyPendulumConstraint : public DynBodyConstraint
{

    JEOD_MAKE_SIM_INTERFACES(DynBodyPendulumConstraint)

public:

    /**
     * Default constructor.
     * Constructs a DynBodyPendulumConstraint modeled by the supplied model.
     * @param pendulum_model  Pointer to the object that models pendulum
     * parameters. The default null pointer should only be used for
     * for simulation checkpoint/restart.
     */
    DynBodyPendulumConstraint (
        BasePendulumModel* pendulum_model = nullptr)
    :
        DynBodyConstraint(),
        constrained_mass(&constraint_frame),
        pendulum_component(&constraint_frame, &constrained_mass, pendulum_model)
    {
        n_dimensions = 1;
    }

    /**
     * Destructor.
     */
    ~DynBodyPendulumConstraint() override = default;


    /**
     * Activate the constraint.
     */
    void activate () override;

    /**
     * Deactivate the constraint.
     */
    void deactivate () override;

    /**
     * Get the known wrench for this constrained object.
     * @return A const reference to the effector wrench for this object.
     * @note This function is called prior to any calls to setup_constraint(),
     *   with the effects of all of the pre-constraint wrenches incorporated
     *   into the non_grav_state argument to that function.
     */
    const Wrench& get_effector_wrench () const override
    {
        return pendulum_component.get_effector_wrench();
    }

    /**
     * Get the wrench this constrained object exerts on the vehicle.
     * @return A const reference to the constraint wrench for this object.
     * @note This function is called after the constraint values have been set.
     */
    const Wrench& get_constraint_wrench () const override
    {
        return pendulum_component.get_constraint_wrench();
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
        return pendulum_component.get_nonlinear_response_wrench();
    }

    /**
     * Get the indexed component constraint for this composite constraint.
     * @param index Index of the component constraint to be retrieved.
     *   This is not bounds-checked.
     * @return The indexed component constraint.
     */
    ConstraintComponent* get_component(unsigned index) override
    {
        assert (index == 0);
        return &pendulum_component;
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
        pendulum_component.update_attachment (vehicle_properties);
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
        const VehicleNonGravState& non_grav_state) override
    {
        constrained_mass.setup_constraint (vehicle_properties, non_grav_state);
        pendulum_component.setup_constraint (vehicle_properties, non_grav_state);
    }

    /**
     * Determine whether the integrator needs to be reset, which occurs
     * on transition between modes.
     * This should be called as a scheduled job at the dynamics rate.
     */
    bool check_for_reset ()
    {
        return pendulum_component.check_for_reset();
    }

    /**
     * Trigger that the pendulum should be set to its quiescent vertical state,
     */
    void reset_quiescent ()
    {
        pendulum_component.reset_quiescent ();
    }

    /**
     * Set the points at which the transitions from Cartesian to angular
     * and angular to Cartesian representations occur.
     */
    void set_transition_points (
        double new_cos_sq_theta_angular,
        double new_cos_sq_theta_cartesian)
    {
        pendulum_component.set_transition_points (
            new_cos_sq_theta_angular, new_cos_sq_theta_cartesian);
    }

    /**
     * Getter for the bob position.
     */
    const double* get_bob_position () const
    {
        return pendulum_component.get_bob_position();
    }

protected:

    /**
     * The pendulum as a constrained mass.
     */
    ConstrainedPointMass constrained_mass;

    /**
     * The pendulum as a component of this constraint.
     */
    PendulumConstraintComponent pendulum_component;
};


} // End JEOD namespace

#endif


/**
 * @}
 * @}
 * @}
 */
