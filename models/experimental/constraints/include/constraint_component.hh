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
 * Defines the class ConstraintComponent.
 */

/*
Purpose: ()
Library dependencies: ((../src/constraint_component.cc))
*/

#ifndef JEOD_CONSTRAINT_COMPONENT_HH
#define JEOD_CONSTRAINT_COMPONENT_HH

#include "dynamics/dyn_body/include/wrench.hh"
#include "experimental/math/include/solver_types.hh"
#include "utils/sim_interface/include/jeod_class.hh"

//! Namespace jeod
namespace jeod
{

class ConstraintFrame;
class DynBody;
class DynBodyConstraintsSolver;
class VehicleProperties;
class VehicleNonGravState;

/**
 * Represents a single scalar constraint, either a pure force in a specific
 * direction at a specific point or a pure torque in a specific direction,
 * that acts on some vehicle.

 * The force or torque on the vehicle is some scalar value times the direction
 * in which the force/torque acts. The force/torque the vehicle exerts on
 * the constraint body in this direction is the additive inverse of the
 * force/torque the constraint body exerts the vehicle. The behavior of the
 * constraint body can also be described as some function of the total forces
 * and torques on the vehicle (external and constraints), plus inertial effects.
 * This leads to an equation for the scalar constraint force/torque value for
 * each constraint.
 *
 * The class DynBodyConstraintsSolver builds and then solves a system of
 * equations for all of the constrained bodies. This yields values for each
 * of the constraints, and hence for the total constraint forces and torques
 * on the vehicle.
 */
class ConstraintComponent
{
    JEOD_MAKE_SIM_INTERFACES(jeod, ConstraintComponent)

public:
    // Member functions

    /**
     * Default constructor.
     */
    ConstraintComponent(ConstraintFrame * constraint_frame_in = nullptr);

    /**
     * Destructor.
     */
    virtual ~ConstraintComponent() = default;

    ConstraintComponent(const ConstraintComponent &) = default;
    ConstraintComponent & operator=(const ConstraintComponent &) = default;
    ConstraintComponent(ConstraintComponent &&) = default;
    ConstraintComponent & operator=(ConstraintComponent &&) = default;

    /**
     * Perform actions related to the just-performed addition of this constraint
     * to a constraints solver.
     */
    virtual void attach_to_solver(DynBodyConstraintsSolver * solver, DynBody * dyn_body) = 0;

    /**
     * Perform actions related to the just-performed removal of this constraint
     * from a constraints solver.
     */
    virtual void detach_from_solver(DynBodyConstraintsSolver * solver, DynBody * dyn_body) = 0;

    /**
     * Update information about the relation between this constraint
     * and the root DynBody.
     * @note This method needs to be called after the root structure frame
     * to root constraint frame relationship has been updated.
     */
    virtual void update_attachment(const VehicleProperties &)
    {
        compute_root_direction();
    }

    /**
     * Prepare this object for solving a constraint problem.
     * @param vehicle_properties  Various vehicle properties.
     * @param non_grav_state  The non-gravitational response of the
     *   root body to non-constraint forces and torques, including
     *   the pre-constraint wrenches.
     */
    virtual void setup_constraint(const VehicleProperties & vehicle_properties,
                                  const VehicleNonGravState & non_grav_state) = 0;

#ifndef SWIG
    /**
     * Set the direction in which the constraint acts on the vehicle.
     * @param constraint_direction  Constraint direction, in constraint frame
     * coordinates.
     */
    void set_constraint_direction(SolverTypes::ConstVector3T constraint_direction);

    /**
     * Compute the constraint direction in root structural coordinates.
     */
    void compute_root_direction();

    /**
     * Get the vehicle's unit translational response to this constraint at a
     * specified point in root body structural coordinates.
     * @param vehicle_properties  Various vehicle properties.
     * @param point  The point, relative to the root body center and expressed
     *    in root body structural coordinates, at with the translational
     *    response is to be computed.
     * @param response  The response vector, which is filled with the
     *    translational acceleration at the specified point on the vehicle
     *    due to a unitary value of the specified component of this constraint.
     * @return The response vector.
     */
    virtual SolverTypes::DecayedVector3T get_translational_response(const VehicleProperties & vehicle_properties,
                                                                    SolverTypes::ConstVector3T point,
                                                                    SolverTypes::Vector3T response) const = 0;

    /**
     * Get the vehicle's unit rotational response to this constraint.
     * @param vehicle_properties  Various vehicle properties.
     * @return  The rotational accleration by the vehicle due to a unit
     *    value of this constraint.
     */
    virtual SolverTypes::ConstDecayedVector3T get_rotational_response(
        const VehicleProperties & vehicle_properties) const;

    /**
     * Get the vehicle's unit rotational response to this constraint.
     * @param vehicle_properties  Various vehicle properties.
     * @param response  The response vector, which is filled with the
     *    rotational acceleration by the vehicle due to a unit value of
     *    this constraint.
     * @return The response vector.
     */
    virtual SolverTypes::DecayedVector3T get_rotational_response(const VehicleProperties & vehicle_properties,
                                                                 SolverTypes::Vector3T response) const;
#endif

    /**
     * Set the right hand side of the constraint equation for this constraint.
     */
    virtual void set_right_hand_side(double rhs_value)
    {
        right_hand_side = rhs_value;
    }

    /**
     * Get the right hand side of the constraint equation for this constraint.
     */
    virtual double get_right_hand_side() const
    {
        return right_hand_side;
    }

    /**
     * Computes the coefficient of the solver A matrix that represents the
     * interaction of some other constrained object and this constrained object.
     * @param vehicle_properties  Various vehicle properties.
     * @param other   The other constrained object.
     * @return  Interaction coefficient.
     */
    virtual double get_cross_coeff(const VehicleProperties & vehicle_properties,
                                   const ConstraintComponent & other) const = 0;

    /**
     * Computes the coefficient of the solver A matrix that represents the
     * interaction of this constrained object with itself.
     * @param vehicle_properties  Various vehicle properties.
     * @return  Interaction coefficient.
     */
    virtual double get_self_coeff(const VehicleProperties & vehicle_properties) const
    {
        return 1.0 + get_cross_coeff(vehicle_properties, *this);
    }

    /**
     * Set this constrained object's scalar constraint value.
     * @param value  The solution to the matrix constraint equation
     *   for this constrained object.
     */
    virtual void set_constraint_value(double value) = 0;

    /**
     * Get the wrench this constrained object exerts on the vehicle.
     * @return A const reference to the constraint wrench for this object.
     * @note This function is called after the constraint values have been set.
     */
    virtual const Wrench & get_constraint_wrench() const
    {
        return constraint_wrench;
    }

    /**
     * Compute the response (if any) of the constrained object to the
     * overall behavior of the vehicle.
     * @param vehicle_properties  Various vehicle properties.
     * @param non_grav_state  The non-gravitational response of the
     *   root body to external forces and torques, including the constraints.
     */
    virtual void compute_constraint_response(const VehicleProperties & vehicle_properties,
                                             const VehicleNonGravState & non_grav_state) = 0;

protected:
    /**
     * The reference frame associated with this constraint.
     */
    ConstraintFrame * constraint_frame; //!< trick_units(--)

    /**
     * The indeterminate part of the wrench due to this constraint.
     */
    Wrench constraint_wrench; //!< trick_units(--)

    /**
     * The direction in which the constraint acts on the vehicle,
     * in constraint frame coordinates.
     */
    double constraint_direction_constraint_frame[3]{}; //!< trick_units(--)

    /**
     * The direction in which the constraint acts on the vehicle,
     * in root body structural coordinates.
     */
    double constraint_direction_root[3]{}; //!< trick_units(--)

    /**
     * The contribution of this constraint to the rotational acceleration
     * of the root body, in root structural components.
     */
    double constraint_omega_dot_root[3]{}; //!< trick_units(1/s^2)

    /**
     * The right hand side of the constraint equation pertaining to this
     * constraint. This has units of force for a force constraint, torque
     * for a torque constraint, etc.
     */
    double right_hand_side{}; //!< trick_units(--)

    /**
     * Compute the vehicle's rotational response to a torque.
     * @param vehicle_properties  Various vehicle properties.
     * @param torque  The torque for which the response is to be calculated.
     */
    void compute_rotational_response(const VehicleProperties & vehicle_properties, const double torque[3]);
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
