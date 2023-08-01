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
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynBody
 * @{
 *
 * @file
 * Define the class StructureIntegratedDynBody, which integrates a DynBody
 * object's structural state.
 */

/*
Purpose: ()
Library dependencies: ((../src/structure_integrated_dyn_body.cc))
*/


#ifndef JEOD_STRUCTURE_INTEGRATED_DYN_BODY_HH
#define JEOD_STRUCTURE_INTEGRATED_DYN_BODY_HH


#include "body_wrench_collect.hh"
#include "vehicle_properties.hh"
#include "vehicle_non_grav_state.hh"

#include "dynamics/dyn_body/include/dyn_body.hh"
#include "utils/sim_interface/include/jeod_class.hh"


//! Namespace jeod 
namespace jeod {

class DynBodyConstraint;
class DynBodyConstraintsSolver;


/**
 * Extends DynBody to integrate an object's structural reference frame
 * as opposed to its center of mass.
 *
 * In addition to structure integration, this class introduces two new concepts,
 * wrenches and constrained objects. A wrench encapsulates a force applied at a
 * point and a torque, with the torque induced by the force due to an
 * off-centerline force direction automatically calculated by JEOD.
 * A constrained object is an object that lies outside the DynBody system
 * boundary that exchanges translational and/or rotational momentum with
 * the DynBody and that is somehow constrained by the translation and/or
 * rotational behavior of the DynBody.
 *
 * These new concepts might be migrated up the DynBody inheritance chain in
 * subsequent releases of JEOD.
 */
class StructureIntegratedDynBody : public DynBody
{
    JEOD_MAKE_SIM_INTERFACES(StructureIntegratedDynBody)

    friend class DynBodyConstraintsSolver;

public:

    // Member data.

    /**
     * Collection of effector wrenches.
     * The effector wrenches are assembled into the collection at the
     * S_define level via @code
     *
     *     vcollect containing_body.effector_wrench_collection.collect_wrench {
     *         pointer_to_wrench1,
     *         ...
     *         pointer_to_wrench_n
     *     };
     * @endcode
     * The collected effector wrenches are processed by the
     * collect_forces_and_torques member function.
     *
     * Note: For completion, there probably should be collected environmental
     * and non-transmitted wrenches as well as effector wrenches.
     */
    BodyWrenchCollect effector_wrench_collection; //!< trick_units(--)


    // Member functions.


    /**
     * Constructor.
     */
    StructureIntegratedDynBody();


    /**
     * Destructor.
     */
    ~StructureIntegratedDynBody () override;


    /**
     * Compute the rotational and translational accelerations that result
     * from the collected forces and torques acting on the vehicle.
     *
     * This function should be called as a derivative class job, with a
     * moderately high phase number. Functions that calculate the
     * gravitational acceleration and the effector, environmental, and
     * non-transmitted forces and torques should be called as scheduled jobs
     * or as lower phase derivative class jobs.
     */
   void collect_forces_and_torques () override;


    /**
     * Set the solver to be used to solve contraints.
     */
    void set_solver (DynBodyConstraintsSolver& solver_in);

    /**
     * Add a constraint to the constraints solver.
     * @note Both the constraint and the solver must be non-null.
     * @param constraint The constraint to be added to the solver.
     */
    void add_constraint (
        DynBodyConstraint* constraint);

    /**
     * Solve for constraint forces and torques acting on the vehicle
     * and apply them to the vehicle.
     *
     * This function should be called as a derivative class job, with a
     * very high phase number. Functions that calculate the constraints
     * should be called as derivative class jobs with a phase intermediate
     * between that of collect_forces_and_torques and of this function.
     */
   virtual void solve_constraints ();

    /**
     * Compute the state derivatives at a vehicle point.
     * @param frame   The vehicle point, as a BodyRefFrame, at which derivatives
     *                are to be calculated.
     * @param derivs  The calculated derivatives.
     */
   void compute_vehicle_point_derivatives (
      const BodyRefFrame & frame, FrameDerivs &derivs) override;

   /**
    * Break the logical connectivity between parent and child.
    * @param[in,out] other_body The other body to detach from
    */
   bool detach (DynBody & other_body) override;


protected:

    // Member data.

    /**
     * The solver for constraint forces and torques, if there are any.
     * This needs to be assigned prior to initialization time in simulations
     * that invoke member function solve_constraints() during runtime.
     * This can be left unassigned (null) in simulations that do not have
     * vehicular constraints.
     */
    DynBodyConstraintsSolver* constraints_solver; //!< trick_units(--)

    /**
     * Wrench into which the effector wrenches are accumulated.
     */
    Wrench effector_wrench; //!< trick_units(--)

    /**
     * Translational/rotational accelerations of the structural frame.
     */
    FrameDerivs struct_derivs; //!< trick_units(--)

    /**
     * Various properties of the vehicle, for the constraints solver.
     */
    VehicleProperties vehicle_properties; //!< trick_units(--)

    /**
     * Rotational and translational behaviors, for the constraints solver.
     */
    VehicleNonGravState non_grav_state; //!< trick_units(--)

    /**
     * Structure-referenced inertial acceleration at the structure frame origin
     * due to vehicle angular velocity.
     */
    double inertial_accel_struct_omega[3]; //!< trick_units(m/s2)

    /**
     * Structure-referenced inertial acceleration at the structure frame origin
     * due to vehicle angular acceleration.
     */
    double inertial_accel_struct_omega_dot[3]; //!< trick_units(m/s2)

    /**
     * Structure-referenced inertial acceleration at the structure frame origin.
     */
    double inertial_accel_struct[3]; //!< trick_units(m/s2)

    /**
     * Inertial-referenced inertial acceleration at the structure frame origin.
     */
    double inertial_accel_inrtl[3]; //!< trick_units(m/s2)


    // Member functions.

    /**
     * Set the relation between parent and child and update the mass
     * properties.
     *
     * @param[in] offset_pstr_cstr_pstr Location of the child body's structural
     *            origin with respect to the parent body's structural origin,
     *            specified in structural coordinates of the parent body.
     * @param[in] T_pstr_cstr Transformation matrix from the parent body's
     *            structural frame to the child body's structural frame.
     * @param[in,out] child The child body being attached to this body.
     */
    void attach_update_properties (
        const double offset_pstr_cstr_pstr[3],
        const double T_pstr_cstr[3][3],
        DynBody & child) override;




    /**
     * Get the vehicle properties as a const reference.
     */
    const VehicleProperties& get_vehicle_properties () const
    {
        return vehicle_properties;
    }

    /**
     * Integrate the translational state of a StructureIntegratedDynBody.
     * @param[in]     dyn_dt        Dynamic time step, in dynamic time seconds.
     * @param[in]     target_stage  The stage of the integration process
     *                             that the integrator should try to attain.
     * @return The status (time advance, pass/fail status) of the integration.
     */
    er7_utils::IntegratorResult trans_integ (
        double dyn_dt,
        unsigned int target_stage) override;

    /**
     * Integrate the rotational state of a StructureIntegratedDynBody.
     * @param[in]     dyn_dt        Dynamic time step, in dynamic time seconds.
     * @param[in]     target_stage  The stage of the integration process
     *                             that the integrator should try to attain.
     * @return The status (time advance, pass/fail status) of the integration.
     */
    er7_utils::IntegratorResult rot_integ (
        double dyn_dt,
        unsigned int target_stage) override;

    /**
     * Collect the local forces and torques that directly act on the vehicle.
     */
    void collect_local_forces_and_torques ();

    /**
     * Propagate forces and torques up the kinematic chain.
     */
    void PropagateForcesAndTorques ();

    /**
     * Compute the inertial torque 
     */
    void compute_inertial_torque ();

    /**
     * Compute the body- and structure-referenced rotational acceleration.
     */
    void compute_rotational_acceleration ();

    /**
     * Compute the inertial-referenced translational acceleration vector.
     */
    void compute_translational_acceleration ();

    /**
     * Finalize computation of the inertial-referenced translational
     * acceleration vector.
     */
    void complete_translational_acceleration ();

private:

   // Make the copy constructor and assignment operator private
   // (and unimplemented) to avoid erroneous copies

   /**
    * Not implemented.
    */
   StructureIntegratedDynBody (const StructureIntegratedDynBody &);

   /**
    * Not implemented.
    */
   StructureIntegratedDynBody & operator= (const StructureIntegratedDynBody &);

};

} // End JEOD namespace


#endif


/**
 * @}
 * @}
 * @}
 */
