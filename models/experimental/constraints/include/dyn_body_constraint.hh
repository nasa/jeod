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
 * Defines the class DynBodyConstraint.
 */

/*
Purpose: ()
Library dependencies: ((../src/dyn_body_constraint.cc))
*/


#ifndef JEOD_DYNBODY_CONSTRAINT_HH
#define JEOD_DYNBODY_CONSTRAINT_HH


#include "constraint_frame.hh"

#include "dynamics/dyn_body/include/vehicle_properties.hh"
#include "dynamics/dyn_body/include/wrench.hh"

#include "experimental/math/include/solver_types.hh"
#include "experimental/math/include/forward_view.hh"
#include "utils/sim_interface/include/jeod_class.hh"

#include <utility>


//! Namespace jeod
namespace jeod {

class ConstraintComponent;
class DynBody;
class DynBodyConstraintsSolver;
class VehicleProperties;
class VehicleNonGravState;


/**
 * Represents a constrained object, which comprises one or more scalar
 * constraints on a vehicle.
 */
class DynBodyConstraint
{
    JEOD_MAKE_SIM_INTERFACES(DynBodyConstraint)
    friend class DynBodyConstraintsSolver;

public:


    // Member functions

    /**
     * Default constructor.
     */
    DynBodyConstraint ();

    /**
     * Destructor.
     * Destruction removes a constraint from the solver.
     */
    virtual ~DynBodyConstraint();

    /**
     * Activate the constraint.
     */
    virtual void activate ();

    /**
     * Deactivate the constraint.
     */
    virtual void deactivate ();

    /**
     * Is the constraint active?
     * @return True if the constraint is active.
     */
    bool is_active () const
    {
        return !inactive;
    }

    /**
     * Get the solver for this constrained object.
     * @return The solver for this constraint.
     */
    virtual const DynBodyConstraintsSolver* get_solver () const
    {
        return solver;
    }

#ifndef SWIG
    /**
     * Set the relationship between the constraint frame and the immediate
     * structural frame.
     * @param transform_in The structure-to-constraint frame transform.
     * @param offset_in  The position of the constraint frame origin, in
     *   structural frame coordinates.
     */
    virtual void set_struct_to_constraint_frame (
        SolverTypes::ConstDecayedMatrix3x3T transform_in,
        SolverTypes::ConstDecayedVector3T offset_in)
    {
        constraint_frame.set_struct_to_constraint_frame (transform_in, offset_in);
    }
#endif


    /**
     * Update information about the relation between this constraint
     * and the root DynBody.
     * @param vehicle_properties  Various vehicle properties.
     */
    virtual void update_attachment (
        const VehicleProperties& vehicle_properties)
    {
        constraint_frame.update_attachment (vehicle_properties);
    }

    /**
     * Send update_attachment to each component.
     * @note Instantiable derived classes should override update_attachment.
     * In that override, the derived class should call parent class(es)
     * update_attachment methods (in the proper order), and then call
     * this function.
     */
    void update_component_attachment
        (const VehicleProperties& vehicle_properties);

    /**
     * Prepare this object for solving a constraint problem.
     * @param vehicle_properties  Various vehicle properties.
     * @param non_grav_state  The non-gravitational response of the
     *   root body to non-constraint forces and torques, including
     *   the pre-constraint wrenches.
     * @note This function is called prior to the calls to set_self_coeff(),
     *   set_cross_coeff(), and set_r_h_s().
     */
    virtual void setup_constraint(
        const VehicleProperties& vehicle_properties,
        const VehicleNonGravState& non_grav_state) = 0;

    /**
     * Send setup_constraint to each component.
     * @note Instantiable derived classes should override setup_constraint.
     * In that override, the derived class should call parent class(es)
     * setup_constraint methods (in the proper order), and then call
     * this function.
     */
    void setup_component_constraint (
        const VehicleProperties& vehicle_properties,
        const VehicleNonGravState& non_grav_state);


    /**
     * Get the known wrench for this constrained object.
     * @return A const reference to the effector wrench for this object.
     * @note This function is called prior to any calls to setup_constraint(),
     *   with the effects of all of the pre-constraint wrenches incorporated
     *   into the non_grav_state argument to that function.
     */
    virtual const Wrench& get_effector_wrench () const = 0;

    /**
     * Get the indexed component constraint for this composite constraint.
     * @param index Index of the component constraint to be retrieved.
     *   This is not bounds-checked.
     * @return The indexed component constraint.
     */
    virtual ConstraintComponent* get_component(unsigned index) = 0;

    /**
     * Const version of get_component().
     */
    virtual const ConstraintComponent* get_component(
        unsigned index) const
    {
        return const_cast<DynBodyConstraint&>(*this).get_component(index);
    }

    /**
     * Set the sum of the terms in the constraint equation that don't involve
     * other constrained objects, expressed as the right hand side of the
     * constraint equation.
     * @param rhs_slice  A view into the overall right hand side vector
     *   of the constraint equation, such that rhs_slice[0] corresponds to this
     *   composite constraint's first scalar constraint, etc.
     */
    virtual void set_r_h_s(jeod::VectorView<double>& rhs_slice) const;

    /**
     * So one can use set_r_h_s with a temporary.
     * @param rhs_slice  A temporary view into the overall right hand side
     *   vector of the constraint equation,
     */
    virtual void set_r_h_s(jeod::VectorView<double>&& rhs_slice) const
    {
        set_r_h_s (rhs_slice);
    }

    /**
     * Set the constraint matrix elements that pertain to this constrained
     * object with respect to some other constrained object.
     * @param vehicle_properties  Various vehicle properties.
     * @param other   The other constrained object.
     * @param a_slice  A view into the overall A matrix of the constraint
     *   equation, such that a_slice(0:n,0:m) represents the NxM submatrix that
     *   pertains to this constraint with respect to the other.
     */
    virtual void set_cross_coeff (
        const VehicleProperties& vehicle_properties,
        const DynBodyConstraint& other,
        jeod::MatrixView<double>& a_slice) const;

    /**
     * So one can call set_cross_coeff with a temporary.
     * @param vehicle_properties  Various vehicle properties.
     * @param other   The other constrained object.
     * @param a_slice  See the lvalue reference version of this function.
     */
    virtual void set_cross_coeff (
        const VehicleProperties& vehicle_properties,
        const DynBodyConstraint& other,
        jeod::MatrixView<double>&& a_slice) const
    {
        set_cross_coeff (vehicle_properties, other, a_slice);
    }

    /**
     * Set the constraint matrix elements for this constraint object
     * with respect to itself.
     * This oftentimes is set_cross_coeff(*this) plus the identity matrix, which
     * is the default implementation.
     * @param vehicle_properties  Various vehicle properties.
     * @param a_slice  A view into the overall A matrix of the constraint
     *   equation, such that a_slice(0:n,0:n) represents the NxN submatrix that
     *   pertains to this constraint with respect to itself.
     */
    virtual void set_self_coeff(
        const VehicleProperties& vehicle_properties,
        jeod::MatrixView<double>& a_slice) const;

    /**
     * So one can call set_self_coeff with a temporary.
     * @param vehicle_properties  Various vehicle properties.
     * @param a_slice  See the lvalue reference version of this function.
     */
    virtual void set_self_coeff(
        const VehicleProperties& vehicle_properties,
        jeod::MatrixView<double>&& a_slice) const
    {
        set_self_coeff (vehicle_properties, a_slice);
    }

    /**
     * Set this constrained object's constraint values.
     * @param solution_slice  The solution to the matrix constraint equation
     *   for this constrained object, such that solution_slice[0] contains
     *   the constraint value for the zeroth component, etc.
     * @note This function is called after the matrix constraint equation has
     *   been formulated and solved.
     */
    virtual void set_constraint_values (
        const jeod::VectorView<double, double>& solution_slice);

    /**
     * Get the wrench this constrained object exerts on the vehicle.
     * @return A const reference to the constraint wrench for this object.
     * @note This function is called after the constraint values have been set.
     */
    virtual const Wrench& get_constraint_wrench () const = 0;

   /**
     * Compute the response (if any) of the constrained object to the
     * overall behavior of the vehicle.
     * @param vehicle_properties  Various vehicle properties.
     * @param non_grav_state  The non-gravitational response of the
     *   root body to external forces and torques, including the constraints.
     */
    virtual void compute_constraint_response (
        const VehicleProperties& vehicle_properties,
        const VehicleNonGravState& non_grav_state);

    /**
     * Get the residual nonlinear wrench for this constrained object.
     * @return A const reference to the nonlinear wrench for this object.
     * @note This function is called after calls to compute_constraint_response.
     *   The nonlinear aspects are assumed to be small; they are not
     *   taken into account while solving the constraints equation.
     */
    virtual const Wrench& get_nonlinear_response_wrench () const = 0;


protected:

    // Member data.

    /**
     * The reference frame associated with this constraint.
     */
    ConstraintFrame constraint_frame; //!< trick_units(--)

    /**
     * The solver to which this constraint is assigned.
     */
    DynBodyConstraintsSolver* solver; //!< trick_units(--)

    /**
     * The number of active components associated with this constraint.
     */
    unsigned n_dimensions; //!< trick_units(--)

    /**
     * Is the constraint inactive?
     * Note: This is expressed in the negative sense to support reduced
     * Trick checkpointing.
     */
    bool inactive; //!< trick_units(--)


    // Member functions.

    /**
     * Perform actions related to the just-performed addition of this constraint
     * to a constraints solver.
     * The default implementation tells each component to perform such actions.
     */
    virtual void attach_to_solver ();

    /**
     * Perform actions related to the just-performed removal of this constraint
     * from a constraints solver.
     * The default implementation tells each component to perform such actions.
     */
    virtual void detach_from_solver ();

    /**
     * Get the dyn_body object associated with this constrained object.
     * @return The dyn_body object for this constraint.
     */
    virtual DynBody* get_dyn_body ();


private:

    // set_solver and reset_solver are private to keep the relation between
    // solvers and constraints correct.
    // These functions are callable by a solver due to friendship.

    /**
     * Set the solver for this constraint.
     * @param solver_in  The solver.
     */
    void set_solver (DynBodyConstraintsSolver* solver_in)
    {
        solver = solver_in;
        constraint_frame.solver = solver_in;
        attach_to_solver ();
    }

    /**
     * Clear the solver for this constraint.
     */
    void reset_solver ()
    {
        detach_from_solver ();
        constraint_frame.solver = nullptr;
        solver = nullptr;
    }

    // Copying a constraint would be handy, but is fraught with issues.
    // For now, copying and assigning are not implemented.
    DynBodyConstraint (const DynBodyConstraint&);
    DynBodyConstraint (DynBodyConstraint&&);
    DynBodyConstraint& operator= (const DynBodyConstraint&);
    DynBodyConstraint& operator= (DynBodyConstraint&&);
};


} // End JEOD namespace

#endif


/**
 * @}
 * @}
 * @}
 */
