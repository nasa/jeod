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
 * @addtogroup Experimental
 * @{
 * @addtogroup Constraints
 * @{
 *
 * @file
 * Defines the class PendulumConstraintComponent.
 */

/*
Purpose: ()
Library dependencies: ((pendulum_constraint_component.o))
*/


#ifndef JEOD_PENDULUM_CONSTRAINT_COMPONENT_HH
#define JEOD_PENDULUM_CONSTRAINT_COMPONENT_HH


#include "force_constraint_component.hh"

#include "constrained_point_mass.hh"
#include "pendulum_constraint_mode.hh"

#include "utils/integration/include/restartable_state_integrator.hh"
#include "utils/integration/include/restartable_2d_second_order_integrator.hh"
#include "utils/sim_interface/include/jeod_class.hh"

// ER7 utilities includes
#include "er7_utils/integration/core/include/integrable_object.hh"
#include "er7_utils/integration/core/include/integrator_result.hh"
#include "er7_utils/integration/core/include/integrator_result_merger_container.hh"


//! Namespace jeod 
namespace jeod {

class BasePendulumModel;
class DynBody;
class DynBodyConstraintsSolver;


/**
 * Models a spherical pendulum as a constrained object, with the constraint
 * force acting at the hinge point.
 */
class PendulumConstraintComponent :
    public ForceConstraintComponent,
    public er7_utils::IntegrableObject
{

    JEOD_MAKE_SIM_INTERFACES(PendulumConstraintComponent)

public:


    /**
     * Default constructor.
     */
    PendulumConstraintComponent(
        ConstraintFrame* constraint_frame_in = nullptr,
        ConstrainedPointMass* constrained_mass_in = nullptr,
        BasePendulumModel* pendulum_model_in = nullptr);

    /**
     * Destructor.
     */
    ~PendulumConstraintComponent() = default;


    // Interfaces needed by IntegrableObject

    /**
     * Create the integrator that integrates the pendulum bob state.
     * @param generator  Integrator constructor to be used to create
     *                   state integrators.
     * @param controls   The integration controls created the integrator
     *                   constructor's create_integration_controls method.
     */
    virtual void create_integrators(
       const er7_utils::IntegratorConstructor & generator,
       er7_utils::IntegrationControls & controls,
       const er7_utils::TimeInterface &);

    /**
     * Advance bob state by the specified dynamic time interval.
     * @param dyn_dt        Dynamic time step, in dynamic time seconds.
     * @param target_stage  The stage of the integration process
     *                     that the integrator should try to attain.
     *
     * @return The status of the integration step.
     */
    virtual er7_utils::IntegratorResult integrate(
         double dyn_dt,
         unsigned int target_stage);

    /**
     * Reset the integrator that integrates the pendulum bob state.
     */
    virtual void reset_integrators();

    /**
     * Destroy the integrator that integrates the pendulum bob state.
     */
    virtual void destroy_integrators()
    {
        bob_integrator.destroy_integrator ();
    }


    // Interfaces needed for ForceConstraintComponent / ConstraintComponent.

    /**
     * Prepare this object for solving a constraint problem.
     * @param vehicle_properties  Various vehicle properties.
     * @param non_grav_state  The non-gravitational response of the
     *   root body to non-constraint forces and torques, including
     *   the pre-constraint wrenches.
     */
    virtual void setup_constraint (
        const VehicleProperties& vehicle_properties,
        const VehicleNonGravState& non_grav_state);

    /**
     * Compute the response of the constrained object to the overall behavior
     * of the vehicle.
     * @param vehicle_properties  Various vehicle properties.
     * @param non_grav_state  The non-gravitational response of the
     *   root body to external forces and torques, including the constraints.
     */
    virtual void compute_constraint_response (
        const VehicleProperties& vehicle_properties,
        const VehicleNonGravState& non_grav_state);

    /**
     * Add this object as an integrator for the DynBody associated with the
     * constraints solver associated with the parent of this component.
     */
    virtual void attach_to_solver (
        DynBodyConstraintsSolver* solver,
        DynBody* dyn_body_in);

    /**
     * Remove this object as an integrator for the DynBody associated with the
     * constraints solver associated with the parent of this component.
     */
    virtual void detach_from_solver (
        DynBodyConstraintsSolver* solver,
        DynBody* dyn_body_in);

    /**
     * Activate the constraint.
     */
    virtual void activate();

    /**
     * Deactivate the constraint.
     */
    virtual void deactivate();

    /**
     * Get the known wrench for this constrained object.
     * In this case, there is none.
     * @return A null wrench.
     */
    virtual const Wrench& get_effector_wrench () const
    {
        return null_wrench;
    }

    /**
     * Get the residual nonlinear wrench for this constrained object.
     * @return A const reference to the nonlinear wrench for this object.
     */
    virtual const Wrench& get_nonlinear_response_wrench () const
    {
        return damping_wrench;
    }


    // New functionality.

    /**
     * Determine whether the integrator needs to be reset, which occurs
     * on transition between modes.
     * This should be called as a scheduled job at the dynamics rate.
     */
    bool check_for_reset ();

    /**
     * Trigger that the pendulum should be set to its quiescent vertical state,
     */
    void reset_quiescent ()
    {
        if (mode != PendulumConstraintMode::Inactive)
        {
            new_mode = PendulumConstraintMode::Reset;
        }
    }

    /**
     * Set the points at which the transitions from Cartesian to angular
     * and angular to Cartesian representations occur.
     */
    void set_transition_points (
        double new_cos_sq_theta_angular,
        double new_cos_sq_theta_cartesian)
    {
        cos_sq_theta_angular = new_cos_sq_theta_angular;
        cos_sq_theta_cartesian = new_cos_sq_theta_cartesian;
    }

    /**
     * Getter for the bob position.
     */
    const double* get_bob_position () const
    {
        return bob_position;
    }


protected:

    // Member data; should be private but needs to be protected for Trick.

    /**
     * The object that provides pendulum parameters.
     */
    BasePendulumModel* pendulum_model; //!< trick_units(--)

    /**
     * The DynBody object on which this pendulum acts.
     */
    DynBody* dyn_body; //!< trick_units(--)

    /**
     * Bob state checkpointable/restartable integrator generator.
     */
    Restartable2DSecondOrderIntegrator bob_integrator; //!< trick_units(--)

    /**
     * The object that merges integration results.
     */
    er7_utils::IntegratorResultMergerContainer
       integ_results_merger; //!< trick_units(--)

    /**
     * Value of cos(theta)^2 below which the model switchs from cartesian to
     * angular integration.
     */
    double cos_sq_theta_angular; //!< trick_units(--)

    /**
     * Value of cos(theta)^2 above which the model switchs from angular to
     * cartesian integration.
     */
    double cos_sq_theta_cartesian; //!< trick_units(--)

    /**
     * The pendulum length; vmag(bob_position).
     */
    double pendulum_length; //!< trick_units(m)

    /**
     * The pendulum bob position, in tank coordinates.
     */
    double bob_position[3]; //!< trick_units(m)

    /**
     * Time derivative of the pendulum bob position, in tank coordinates.
     */
    double bob_velocity[3]; //!< trick_units(m/s)

    /**
     * Second time derivative of the pendulum bob position, in tank coordinates.
     * Note: Size is three even though only x and y are integrated.
     * The z accel is not computed.
     */
    double bob_accel[3]; //!< trick_units(m/s^2)

    /**
     * Polar bob angle.
     */
    double theta; //!< trick_units(rad)

    /**
     * Azimuthal bob angle.
     */
    double phi; //!< trick_units(rad)

    /**
     * Time derivative of theta.
     */
    double theta_dot; //!< trick_units(rad/s)

    /**
     * Time derivative of phi.
     */
    double phi_dot; //!< trick_units(rad/s)

    /**
     * Second time derivative of theta.
     */
    double theta_dot_dot; //!< trick_units(rad/s^2)

    /**
     * Second time derivative of phi.
     */
    double phi_dot_dot; //!< trick_units(rad/s^2)

    /**
     * sin(theta).
     */
    double sin_theta; //!< trick_units(--)

    /**
     * cos(theta).
     */
    double cos_theta; //!< trick_units(--)

    /**
     * sin(phi).
     */
    double sin_phi; //!< trick_units(--)

    /**
     * cos(phi).
     */
    double cos_phi; //!< trick_units(--)

    /**
     * cos^2(theta).
     */
    double cos_sq_theta; //!< trick_units(--)

    /**
     * Local spherical transformation matrix.
     */
    double constraint_to_spherical_transform[3][3]; //!< trick_units(--)

    /**
     * Vehicle angular velocity expressed in the constraint frame.
     */
    double omega_constraint_frame[3]; //!< trick_units(rad/s)

    /**
     * Vehicle angular acceleration expressed in the constraint frame.
     */
    double omega_dot_constraint_frame[3]; //!< trick_units(rad/s^2)

    /**
     * Cross product of angular velocity and bob position.
     */
    double omega_cross_pos[3]; //!< trick_units(rad*m/s)

    /**
     * Cross product of angular velocity and bob velocity.
     */
    double omega_cross_vel[3]; //!< trick_units(rad*m/s^2)

    /**
     * Time derivative of the bob direction unit vector.
     */
    double rho_hat_dot[3]; //!< trick_units(1/s)

    /**
     * A wrench that is always zero and inactive.
     */
    Wrench null_wrench; //!< trick_units(--)

    /**
     * The damping torque as a wrench, which is hopefully small.
     */
    Wrench damping_wrench; //!< trick_units(--)

    /**
     * The mode in which the pendulum's state is integrated.
     */
    PendulumConstraintMode mode; //!< trick_units(--)

    /**
     * The mode to which the model is to transition.
     */
    PendulumConstraintMode new_mode; //!< trick_units(--)


    // Member functions.

    /**
     * Set the length of the pendulum.
     * @param new_length  New pendulum length.
     */
    void set_pendulum_length (double new_length);


private:

    /**
     * Compute bob angles in preparation for integrating in horizontal mode.
     */
    void transition_to_horizontal();

    /**
     * Construct the transformation matrix from the constraint frame
     * to the local spherical frame.
     */
    void construct_constraint_to_spherical_transform();

    /**
     * Compute the time derivative of the unit vector in the direction
     * of the pendulum bob.
     */
    void compute_rho_hat_dot ();

    /**
     * Compute the bob position and velocity given its spherical coordinates.
     */
    void compute_bob_state_spherical ();

    /**
     * Compute the bob z position and velocity given its x and y position
     * and velocity.
     */
    void compute_bob_state_cartesian ();

    /**
     * Integrate spherical state by the specified dynamic time interval.
     * @param dyn_dt        Dynamic time step, in dynamic time seconds.
     * @param target_stage  The stage of the integration process
     *                     that the integrator should try to attain.
     *
     * @return The status of the integration step.
     */
    virtual er7_utils::IntegratorResult integrate_spherical(
         double dyn_dt,
         unsigned int target_stage);

    /**
     * Integrate Cartesian state by the specified dynamic time interval.
     * @param dyn_dt        Dynamic time step, in dynamic time seconds.
     * @param target_stage  The stage of the integration process
     *                     that the integrator should try to attain.
     *
     * @return The status of the integration step.
     */
    virtual er7_utils::IntegratorResult integrate_cartesian(
         double dyn_dt,
         unsigned int target_stage);
};


} // End JEOD namespace

#endif


/**
 * @}
 * @}
 * @}
 */
