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
 * Defines the class DynBodyConstraintsSolver.
 */

/*
Purpose: ()
Library dependencies: ((../src/dyn_body_constraints_solver.cc))
*/

#ifndef JEOD_DYNBODY_CONSTRAINTS_SOLVER_HH
#define JEOD_DYNBODY_CONSTRAINTS_SOLVER_HH

#include "dyn_body_constraint.hh"

#include "utils/container/include/object_vector.hh"
#include "utils/container/include/pointer_vector.hh"
#include "utils/container/include/primitive_vector.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/sim_interface/include/jeod_class.hh"

#include "experimental/math/include/linear_system_solver.hh"

#include <functional>

//! Namespace jeod
namespace jeod
{

class DynBody;
class StructureIntegratedDynBody;
class Wrench;

/**
 * Builds and then solves a system of linear equations that describe the
 * linear constraint forces and torques that act on a vehicle.
 */
class DynBodyConstraintsSolver
{
    JEOD_MAKE_SIM_INTERFACES(jeod, DynBodyConstraintsSolver)

    friend class DynBodyConstraint;
    friend class StructureIntegratedDynBody;

public:
    // Data types

    /**
     * Vector of pointers to DynBodyConstraintsSolver objects.
     */
    using SolverVectorT = JeodPointerVector<DynBodyConstraintsSolver>::type;

    /**
     * Vector of pointers to DynBodyConstraint objects.
     */
    using ConstraintsVectorT = JeodPointerVector<DynBodyConstraint>::type;

    /**
     * Vector of doubles.
     */
    using DoubleVectorT = JeodPrimitiveVector<double>::type;

    /**
     * Pairs of unsigned integers, as indices.
     */
    using IndexPairT = std::pair<unsigned, unsigned>;

    /**
     * Vector of pairs of unsigned integers.
     */
    using IndexPairVectorT = JeodObjectVector<IndexPairT>::type;

    // Member data

    /**
     * Vector of pointers to wrench constraints.
     * This member is public so it can be collected.
     * The wrench constraints are collected into the vector at the
     * S_define level via
     *     vcollect containing.object.constraints {
     *         pointer_to_dyn_body_constraint1,
     *         ...
     *         pointer_to_dyn_body_constraint_n
     *     };
     * The collected constraints become registered constraints at the
     * call to collect_collected_constraints().
     * @warning
     * Being public, elements can be added to or removed from this member,
     * willy nilly.
     * This is fine prior to the call to collect_collected_constraints,
     * not so fine after that call.
     * @warning
     * Changes to this member after the call to collect_collected_constraints()
     * have no effect.
     */
    ConstraintsVectorT collect_constraints; //!< trick_io(**)

    // Member functions

    // Note: The pointer-based constructor is private.
    // The default constructor would be, too, but it can't.

    /**
     * Default constructor, for internal (Trick and JEOD) use only.
     */
    DynBodyConstraintsSolver()
        : DynBodyConstraintsSolver(nullptr, nullptr)
    {
    }

    /**
     * Non-default constructor.
     * @param solver_   The linear system of equations solver to be used
     *                  to solve the constraints equation.
     * @param dyn_body_  The DynBody associated with this constraints solver.
     */
    DynBodyConstraintsSolver(LinearSystemSolver & solver_, StructureIntegratedDynBody & dyn_body_);

    /**
     * Destructor.
     */
    ~DynBodyConstraintsSolver();

    DynBodyConstraintsSolver(const DynBodyConstraintsSolver &) = delete;
    DynBodyConstraintsSolver & operator=(const DynBodyConstraintsSolver &) = delete;

    /**
     * Add the specified constraint to the constraints vectors.
     * @param constraint Constraint to be added.
     */
    void add_constraint(DynBodyConstraint * constraint);

    /**
     * Remove the specified constraint from the constraints vectors.
     * @param constraint Constraint to be removed.
     */
    void remove_constraint(DynBodyConstraint * constraint);

    /**
     * Check whether the constraint has been registered.
     * @param constraint Constraint to be tested.
     */
    bool is_registered(const DynBodyConstraint * constraint) const;

    /**
     * Check whether the constraint is active.
     * @param constraint Constraint to be tested.
     */
    bool is_active(const DynBodyConstraint * constraint) const;

    /**
     * Add the collect_constraints to the constraints owned by this object.
     */
    void collect_collected_constraints();

    // Various utilities.

#ifndef SWIG
    /**
     * Transform a direction expressed in local structural coordinates
     * to the root structural frame.
     * @param direction  Input direction.
     * @param root_direction  The transformed direction.
     * @return root_direction.
     */
    SolverTypes::DecayedVector3T transform_direction_to_root(SolverTypes::ConstVector3T direction,
                                                             SolverTypes::Vector3T root_direction) const;

    /**
     * Transform a position expressed in local structural coordinates
     * to the root structural frame.
     * @param point  Input position.
     * @param root_point  The transformed position.
     * @return root_point.
     */
    SolverTypes::DecayedVector3T transform_point_to_root(SolverTypes::ConstVector3T point,
                                                         SolverTypes::Vector3T root_point) const;

    /**
     * Transform a transformation matrix that transforms from local structural
     * coordinates to some other frame to a transformation matrix that
     * transforms from the root structural frame to that other frame.
     * @param this_to_frame_transform  Input transformation matrix.
     * @param root_to_frame_transform  The transformed transformation matrix.
     * @return root_to_frame_transform.
     */
    SolverTypes::ConstMatrix3x3RefT transform_matrix_from_root(
        SolverTypes::ConstMatrix3x3RefT this_to_frame_transform,
        SolverTypes::Matrix3x3RefT root_to_frame_transform) const;

    /**
     * Get the root structural to local structural transformation matrix.
     * @return root_to_this_transform.
     */
    SolverTypes::ConstMatrix3x3RefT get_root_to_struct_transform() const
    {
        return root_to_this_transform;
    }

    /**
     * Get the location of the local structural origin in root structural
     * coordinates.
     * @return root_to_this_offset.
     */
    SolverTypes::ConstDecayedVector3T get_root_to_struct_offset() const
    {
        return root_to_this_offset;
    }
#endif

private:
    // Member data.

    /**
     * The DynBody object associated with this solver.
     */
    DynBody * dyn_body{}; //!< trick_units(--)

    /**
     * The object used to solve the constraints.
     */
    LinearSystemSolver * solver{}; //!< trick_units(--)

    /**
     * Vector of constraint values.
     * These are the solution to A*x = b.
     */
    DoubleVectorT x_vector; //!< trick_io(**)

    /**
     * The solver associated with the parent DynBody object.
     */
    DynBodyConstraintsSolver * parent{}; //!< trick_units(--)

    /**
     * The solver associated with the root DynBody object.
     */
    DynBodyConstraintsSolver * root_solver{}; //!< trick_units(--)

    /**
     * The solvers associated with child DynBody objects.
     */
    SolverVectorT children; //!< trick_units(--)

    /**
     * Vector of pointers to the registered wrench constraints
     * for this DynBody object.
     */
    ConstraintsVectorT own_constraints; //!< trick_io(**)

    /**
     * Vector of pointers to the registered wrench constraints
     * for child DynBody objects.
     */
    ConstraintsVectorT child_constraints; //!< trick_io(**)

    /**
     * Vector of pointers to the registered wrench constraints
     * for this DynBody and child DynBody objects.
     */
    ConstraintsVectorT all_constraints; //!< trick_io(**)

    /**
     * Vector of pointers to the active wrench constraints.
     */
    ConstraintsVectorT active_constraints; //!< trick_io(**)

    /**
     * Vector of begin, end pairs of row indices for each active constraint.
     */
    IndexPairVectorT constraint_indices; //!< trick_io(**)

    /**
     * The sum of the individual constraint's effector wrenches.
     */
    Wrench effector_wrench; //!< trick_units(--)

    /**
     * The sum of the individual constraint's constraint wrenches.
     */
    Wrench constraints_wrench; //!< trick_units(--)

    /**
     * The sum of the individual constraint's nonlinear response wrenches.
     */
    Wrench nonlinear_responses_wrench; //!< trick_units(--)

    /**
     * The transformation matrix from the root DynBody object's structural
     * frame to the DynBody object immediately associated with this solver.
     */
    double root_to_this_transform[3][3]{IDENTITY}; //!< trick_units(--)

    /**
     * The origin of the structural frame of the DynBody object immediately
     * associated with this solver, in root DynBody structural coordinates.
     */
    double root_to_this_offset[3]{}; //!< trick_units(m)

    // Member functions.

    /**
     * Non-default constructor, for internal use only.
     * @param solver_   The linear system of equations solver to be used
     *                  to solve the constraints equation.
     * @param dyn_body_  The DynBody associated with this constraints solver.
     */
    DynBodyConstraintsSolver(LinearSystemSolver * solver_, DynBody * dyn_body_);

    /**
     * Callback for a DynBody that has attached to another.
     * @param new_parent The constraints solver for the parent DynBody
     *   of the DynBody associated with this solver.
     * @param vehicle_properties The child body's vehicle_properties object.
     */
    void attach(DynBodyConstraintsSolver & new_parent, const VehicleProperties & vehicle_properties);

    /**
     * Callback for a DynBody that has detached from a parent DynBody.
     */
    void detach(const VehicleProperties & vehicle_properties);

    /**
     * Callback for a DynBody that has adjusted its attach point/orientation
     * with respect to a parent DynBody.
     * @note This is a future hook; The MassBody reattach function is not
     *   virtual and hence can't be overridden by DynBody.
     */
    void reattach(const VehicleProperties & vehicle_properties);

    /**
     * Solve for the constraints.
     * @param vehicle_properties  Properites of the vehicle
     * @param non_grav_state  Non-gravitational state in structural coordinates.
     *     On input, the translational and rotational acceleration reflect
     *     the response to external forces and torques. These are updated
     *     to also reflect the constraints on return.
     */
    void solve(const VehicleProperties & vehicle_properties, VehicleNonGravState & non_grav_state);

    // Callbacks from DynBodyConstraint, which is a friend class.

    /**
     * DynBodyConstraint activate callback to mark the constraint as active.
     * @param constraint Constraint to be marked as active.
     */
    void activate_constraint(DynBodyConstraint * constraint);

    /**
     * DynBodyConstraint deactivate callback to mark the constraint as inactive.
     * @param constraint Constraint to be marked as inactive.
     */
    void deactivate_constraint(DynBodyConstraint * constraint);

    /**
     * Get the dyn_body object associated with this solver.
     * @return The dyn_body object for this solver.
     */
    DynBody * get_dyn_body()
    {
        return dyn_body;
    }

    // Various utilities.

    /**
     * Check whether the constraint belongs directly to this solver.
     * @param constraint Constraint to be tested.
     */
    bool is_ours(const DynBodyConstraint * constraint) const;

    /**
     * Add the specified constraint to inherited constraints vectors.
     * @param constraint Constraint to be added.
     */
    void add_child_constraint(DynBodyConstraint * constraint);

    /**
     * Remove the specified constraint from inherited constraints vectors.
     * @param constraint Constraint to be removed.
     */
    void remove_child_constraint(DynBodyConstraint * constraint);

    /**
     * Add constraints inherited from a newly attached solver
     * to this object's constraints.
     * @param new_inherited_constraints  Items to be added to all_constraints.
     * @param new_active_constraints  Items to be added to active_constraints.
     */
    void add_attached_constraints(const ConstraintsVectorT & new_inherited_constraints,
                                  const ConstraintsVectorT & new_active_constraints);

    /**
     * Compute the transformation and offset from the root to the this,
     * and also for all children.
     */
    void compute_root_to_this(const VehicleProperties & vehicle_properties);

    // Functions that implement solve().

    /**
     * Updates the vehicle's response to sn accumulated wrench.
     * @param sum  The accumulated value of the calls to get_wrench.
     * @param vehicle_properties  Properites of the vehicle
     * @param non_grav_state  Non-gravitational state in structural coordinates,
     *   which is updated to reflect the accumulated wrench on return.
     */
    void update_vehicle_response(const Wrench & sum,
                                 const VehicleProperties & vehicle_properties,
                                 VehicleNonGravState & non_grav_state);

    /**
     * Build the system of linear equations A*x = b, where each row
     * corresponds to a specific constraint.
     * @param vehicle_properties  Properites of the vehicle
     * @param non_grav_state  Non-gravitational state in structural coordinates.
     * @param n_constraints  The number of active constraints.
     * @param constraint_indices  Active constraints start and end indices.
     */
    void build_system_of_equations(const VehicleProperties & vehicle_properties,
                                   const VehicleNonGravState & non_grav_state,
                                   unsigned n_constraints,
                                   IndexPairVectorT & constraint_indices);

    /**
     * Tell each active constrained object the solution for that constraint.
     * @param n_constraints  The number of active constraints.
     * @param constraint_indices  Active constraints start and end indices.
     */
    void send_solution(unsigned n_constraints, const IndexPairVectorT & constraint_indices);

    /**
     * Compute the constrained objects' responses to the constraints.
     * @param vehicle_properties  Properites of the vehicle
     * @param non_grav_state  Non-gravitational state in structural coordinates,
     *   updated to include the constraints.
     */
    void compute_constraint_responses(const VehicleProperties & vehicle_properties,
                                      const VehicleNonGravState & non_grav_state);
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
