/**
 * @addtogroup Models
 * @{
 * @addtogroup Experimental
 * @{
 * @addtogroup Constraints
 * @{
 *
 * @file
 * Defines DynBodyConstraintsSolver member functions.
 */

/*
Purpose: ()
Library dependencies: (
  (dyn_body_constraint.cc))
*/


#include "../include/dyn_body_constraints_solver.hh"

#include "dynamics/dyn_body/include/dyn_body.hh"
#include "dynamics/dyn_body/include/structure_integrated_dyn_body.hh"
#include "dynamics/dyn_body/include/wrench.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "experimental/message/include/fail_simulation.hh"

#include <algorithm>
#include <type_traits>
#include <cmath>


//! Namespace jeod 
namespace jeod {

static inline bool
is_zero_vector (const double vec[3])
{
    return (std::fpclassify(vec[0]) == FP_ZERO) && (std::fpclassify(vec[1]) == FP_ZERO) && (std::fpclassify(vec[2]) == FP_ZERO);
}


/**
 * Find the provided element in the container.
 *
 * @return Iterator the points to the provided element, or container.end(),
 * as a ContainerT::const_iterator if the container is const, otherwise
 * as a ContainerT::iterator.
 *
 * @tparam ContainerT  The type of the container, typically inferred.
 * @tparam ElemT       The type of the element, typically inferred.
 * @param container    The container to be searched.
 * @param elem         The element to be found.
 */
template <typename ContainerT, typename ElemT>
typename std::conditional<
    std::is_const<ContainerT>::value,
    typename ContainerT::const_iterator,
    typename ContainerT::iterator>::type
find_iterator (
    ContainerT& container,
    const ElemT* elem)
{
    return std::find(container.begin(), container.end(), elem);
}


/**
 * Remove the provided element from the container.
 *
 * @tparam ContainerT  The type of the container, typically inferred.
 * @tparam ElemT       The type of the element, typically inferred.
 * @param container    The container to be searched.
 * @param elem         The element to be removed.
 * @param err_msg       Message to be sent if elem is not in container.
 */
template <typename ContainerT, typename ElemT>
void
remove_element (
    ContainerT& container,
    const ElemT* elem,
    const char* err_msg)
{
    auto iter = find_iterator(container, elem);
    if (iter == container.end())
    {
        FAIL_SIMULATION (err_msg);
    }

    container.erase (iter);
}


/**
 * Check whether the provided element is in the container.
 *
 * @return True if @a elem is in @a container, false otherwise.
 *
 * @tparam ContainerT  The type of the container, typically inferred.
 * @tparam ElemT       The type of the element, typically inferred.
 * @param container    The container to be searched.
 * @param elem         The element to be found.
 */
template <typename ContainerT, typename ElemT>
bool
is_in_container (
    const ContainerT& container,
    const ElemT* elem)
{
    return find_iterator(container, elem) != container.end();
}


/**
 * Accumulate the total of the get_wrench calls to the active constraints.
 * @return True if the sum is non-zero.
 * @tparam T  The class that defines the get_wrench call.
 & @param  get_wrench  Pointer to member function that returns a Wrench.
 * @param  active_constraints  Vector of active constraints.
 * @param  point The vector offset for computing the wrenching torque.
 * @param  sum  The sum to be accumulated.
 */
template <typename T>
bool
accumulate_wrenches (
    const Wrench& (T::*get_wrench)() const,
    const DynBodyConstraintsSolver::ConstraintsVectorT& active_constraints,
    const double point[3],
    Wrench& sum)
{
    bool have_non_trivial_sum = false;

    sum.reset_force_and_torque ();
    sum.set_point (point);

    // Accumulate the wrench from the calls to get_wrench.
    for (auto constraint : active_constraints)
    {
        const Wrench& wrench = (constraint->*get_wrench)();
        if (wrench.is_active())
        {
            have_non_trivial_sum = true;
            sum += wrench;
        }
    }

    return
        have_non_trivial_sum &&
        ! (is_zero_vector(sum.get_force()) && is_zero_vector(sum.get_torque()));
}


// Public non-default constructor.
DynBodyConstraintsSolver::DynBodyConstraintsSolver(
    LinearSystemSolver& solver_in,
    StructureIntegratedDynBody& dyn_body_in)
:
    DynBodyConstraintsSolver (&solver_in, &dyn_body_in)
{
    dyn_body_in.set_solver (*this);
}


// Private non-default constructor.
DynBodyConstraintsSolver::DynBodyConstraintsSolver(
    LinearSystemSolver* solver_in,
    DynBody* dyn_body_in)
:
    dyn_body(dyn_body_in),
    solver(solver_in),
    x_vector(),
    parent(nullptr),
    root_solver(this),
    children(),
    own_constraints(),
    child_constraints(),
    all_constraints(),
    active_constraints()
{
    JEOD_REGISTER_CLASS(DynBodyConstraintsSolver);
    JEOD_REGISTER_CHECKPOINTABLE(this, own_constraints);
    JEOD_REGISTER_CHECKPOINTABLE(this, child_constraints);
    JEOD_REGISTER_CHECKPOINTABLE(this, all_constraints);
    JEOD_REGISTER_CHECKPOINTABLE(this, active_constraints);

    Matrix3x3::identity (root_to_this_transform);
    Vector3::initialize (root_to_this_offset);
}


// Destructor.
DynBodyConstraintsSolver::~DynBodyConstraintsSolver()
{
    JEOD_DEREGISTER_CHECKPOINTABLE(this, own_constraints);
    JEOD_DEREGISTER_CHECKPOINTABLE(this, child_constraints);
    JEOD_DEREGISTER_CHECKPOINTABLE(this, all_constraints);
    JEOD_DEREGISTER_CHECKPOINTABLE(this, active_constraints);
    // JEOD_DEREGISTER_CHECKPOINTABLE(this, active_constraint_indices);

    // Constraints have a pointer to a solver which will become invalid upon
    // return from this destructor.
    for (auto constraint : own_constraints)
    {
        constraint->reset_solver();
    }
}


// Check whether the constraint is registered.
bool
DynBodyConstraintsSolver::is_registered (
    const DynBodyConstraint* constraint) const
{
    return is_in_container(all_constraints, constraint);
}


// Check whether the constraint is ours.
bool
DynBodyConstraintsSolver::is_ours (
    const DynBodyConstraint* constraint) const
{
    return is_in_container(own_constraints, constraint);
}


// Check whether the constraint is active.
bool
DynBodyConstraintsSolver::is_active (
    const DynBodyConstraint* constraint) const
{
    return is_in_container(active_constraints, constraint);
}


// Add the specified constraint to the vector of registered constraints.
void
DynBodyConstraintsSolver::add_constraint (
    DynBodyConstraint* constraint)
{
    // Ensure that the new constraint is "new".
    if (is_ours(constraint))
    {
        FAIL_SIMULATION (
            "add_constraint called with an already registered constraint.");
    }

    // Make this constraint one of our own.
    own_constraints.push_back (constraint);
    constraint->set_solver (this);

    // Bubble this addition up the kinematic chain.
    add_child_constraint (constraint);
}


// Remove the specified constraint from the vector of registered constraints.
void
DynBodyConstraintsSolver::add_child_constraint (
    DynBodyConstraint* constraint)
{
    // Ensure the constraint is not already registered (owned+inherited).
    if (is_registered(constraint))
    {
        FAIL_SIMULATION (
            "Internal error: Child constraint is already registered.");
    }

    // Add the constraint to the vector of owned+inherited constraints,
    // and to the active constraints if it is active.
    all_constraints.push_back (constraint);

    if (constraint->is_active())
    {
        activate_constraint (constraint);
    }

    // Add this constraint to the parent (and up the chain).
    if (parent != nullptr)
    {
        parent->add_child_constraint (constraint);
    }
}


// Remove the specified constraint from all constraints vectors.
void
DynBodyConstraintsSolver::remove_constraint (
    DynBodyConstraint* constraint)
{
    remove_child_constraint (constraint);

    remove_element (
        own_constraints,
        constraint,
        "Constraint is not registered with this solver.");
}


// Remove the specified constraint from the vector of registered constraints.
void
DynBodyConstraintsSolver::remove_child_constraint (
    DynBodyConstraint* constraint)
{
    if (constraint->is_active())
    {
        deactivate_constraint (constraint);
    }

    remove_element (
        all_constraints,
        constraint,
        "Internal error: Child constraint is not registered.");

    if (parent != nullptr)
    {
        parent->remove_child_constraint (constraint);
    }
}


// Add the specified constraint to the vector of active constraints.
void
DynBodyConstraintsSolver::activate_constraint (
    DynBodyConstraint* constraint)
{
    // Ensure the constraint is registered with this and is active.
    if (! is_registered(constraint))
    {
        FAIL_SIMULATION (
            "activate_constraint called with an unregistered constraint.");
    }
    if (is_active(constraint))
    {
        FAIL_SIMULATION (
            "activate_constraint called with an already active constraint.");
    }

    active_constraints.push_back (constraint);

    if (parent != nullptr)
    {
        parent->activate_constraint (constraint);
    }
}


// Remove the specified constraint from the vector of active constraints.
void
DynBodyConstraintsSolver::deactivate_constraint (
    DynBodyConstraint* constraint)
{
    if (! is_registered(constraint))
    {
        FAIL_SIMULATION (
            "deactivate_constraint called with an unregistered constraint.");
    }

    remove_element (
        active_constraints,
        constraint,
        "deactivate_constraint called with an already inactive constraint.");

    // rebuild_active_constraint_indices ();

    if (parent != nullptr)
    {
        parent->deactivate_constraint (constraint);
    }
}


// Add the collect_constraints to the object's vector of constraints.
void
DynBodyConstraintsSolver::collect_collected_constraints ()
{
    for (auto constraint : collect_constraints)
    {
        add_constraint (constraint);
    }
    // collect_constraints.clear();
}


// Compute root-to-this transform and offset.
void
DynBodyConstraintsSolver::compute_root_to_this (
    const VehicleProperties& vehicle_properties)
{
    if (parent == nullptr)
    {
        Matrix3x3::identity (root_to_this_transform);
        Vector3::initialize (root_to_this_offset);
    }
    else if (parent->parent == nullptr)
    {
        Matrix3x3::copy (
            vehicle_properties.get_parent_to_structure_transform(),
            root_to_this_transform);
        Vector3::sum (
            vehicle_properties.get_parent_to_structure_offset(),
            parent->root_to_this_offset,
            root_to_this_offset);
    }
    else
    {
        Matrix3x3::product (
            vehicle_properties.get_parent_to_structure_transform(),
            parent->root_to_this_transform,
            root_to_this_transform);
        Vector3::incr (
            parent->root_to_this_offset,
            Vector3::transform_transpose (
                parent->root_to_this_transform,
                vehicle_properties.get_parent_to_structure_offset(),
                root_to_this_offset));
    }

    for (auto constraint : own_constraints)
    {
        constraint->update_attachment (vehicle_properties);
    }

    for (auto child : children)
    {
        child->compute_root_to_this (vehicle_properties);
    }
}


SolverTypes::DecayedVector3T
DynBodyConstraintsSolver::transform_direction_to_root (
    SolverTypes::ConstVector3T direction,
    SolverTypes::Vector3T root_direction) const
{
    return Vector3::transform_transpose (
        root_to_this_transform, direction, root_direction);
}


SolverTypes::DecayedVector3T
DynBodyConstraintsSolver::transform_point_to_root (
    SolverTypes::ConstVector3T point,
    SolverTypes::Vector3T root_point) const
{
    return Vector3::incr (
        root_to_this_offset,
        transform_direction_to_root(point, root_point));
}


SolverTypes::ConstMatrix3x3RefT
DynBodyConstraintsSolver::transform_matrix_from_root (
    SolverTypes::ConstMatrix3x3RefT this_to_frame_transform,
    SolverTypes::Matrix3x3RefT root_to_frame_transform) const
{
    Matrix3x3::product (
        this_to_frame_transform,
        root_to_this_transform,
        root_to_frame_transform);
    return root_to_frame_transform;
}


void
DynBodyConstraintsSolver::attach (
    DynBodyConstraintsSolver& new_parent,
    const VehicleProperties& vehicle_properties)
{
    // Ensure object isn't already attached;
    // this would be an internal (as opposed to user) logic error.
    if (parent != nullptr)
    {
        FAIL_SIMULATION ("DynBodyConstraintsSolver::attach called "
                         "when already attached.");
    }

    // Set the parent and the root solver.
    parent = &new_parent;
    root_solver = parent->root_solver;

    // Add our constraints (own and inherited) up the chain.
    for (DynBodyConstraintsSolver* p_solver = parent;
         p_solver != nullptr;
         p_solver = p_solver->parent)
    {
        p_solver->add_attached_constraints (all_constraints, active_constraints);
    }

    // Compute root-to-this transform and offset.
    compute_root_to_this (vehicle_properties);
}


void
DynBodyConstraintsSolver::add_attached_constraints (
    const ConstraintsVectorT& new_inherited_constraints,
    const ConstraintsVectorT& new_active_constraints)
{

    // FIXME: Use a loop here to check for duplicates?
    // (No need for a loop on active_constraints.)
    all_constraints.insert (
        all_constraints.end(),
        new_inherited_constraints.begin(), new_inherited_constraints.end());

    active_constraints.insert (
        active_constraints.end(),
        new_active_constraints.begin(), new_active_constraints.end());

    // rebuild_active_constraint_indices();
}


void
DynBodyConstraintsSolver::reattach (
    const VehicleProperties& vehicle_properties)
{
    // Compute root-to-this transform and offset.
    compute_root_to_this (vehicle_properties);
}


void
DynBodyConstraintsSolver::detach (
    const VehicleProperties& vehicle_properties)
{
    // Ensure object is currently attached to some other object;
    // this would be an internal (as opposed to user) logic error.
    if (parent == nullptr)
    {
        FAIL_SIMULATION ("DynBodyConstraintsSolver::detach called "
                         "when not attached.");
    }

    // Remove constraints from parent (and up the chain).
    for (auto constraint : all_constraints)
    {
        parent->remove_child_constraint (constraint);
    }

    // Remove this from the parent.
    remove_element (
        parent->children,
        this,
        "Internal error: Attached child is not in parent's children.");

    // This object is now the root of its tree.
    parent = nullptr;
    root_solver = this;

    // Compute root-to-this transform and offset.
    compute_root_to_this (vehicle_properties);
}


void
DynBodyConstraintsSolver::solve (
    const VehicleProperties& vehicle_properties,
    VehicleNonGravState& non_grav_state)
{
    unsigned n_constraints = active_constraints.size();
    if (n_constraints == 0)
    {
        return;
    }

    // Accumulate the pre-constraint wrenches and update the
    // non_grav_state to reflect the vehicle's response to these wrenches.
    if (accumulate_wrenches<DynBodyConstraint>(
            &DynBodyConstraint::get_effector_wrench,
            active_constraints,
            vehicle_properties.get_structure_to_body_offset(),
            effector_wrench))
    {
        update_vehicle_response (
            effector_wrench, vehicle_properties, non_grav_state);
    }

    // Build the constraints system of equations.
    constraint_indices.clear();
    constraint_indices.reserve(n_constraints);
    build_system_of_equations (
        vehicle_properties, non_grav_state, n_constraints, constraint_indices);

    // Solve the constraints system of equations and broadcast the solution.
    solver->solve (x_vector);
    send_solution (n_constraints, constraint_indices);

    // Accumulate the constraint wrenches and update the
    // non_grav_state to reflect the vehicle's response to these wrenches.
    if (accumulate_wrenches<DynBodyConstraint>(
            &DynBodyConstraint::get_constraint_wrench,
            active_constraints,
            vehicle_properties.get_structure_to_body_offset(),
            constraints_wrench))
    {
        update_vehicle_response (
            constraints_wrench, vehicle_properties, non_grav_state);
    }

    // Tell the constraints to set up the differential equations
    // due to the vehicle response that the constraints need to integrate.
    compute_constraint_responses (vehicle_properties, non_grav_state);

    // Accumulate the nonlinear response wrenches and update the
    // non_grav_state to reflect the vehicle's response to these wrenches.
    if (accumulate_wrenches<DynBodyConstraint>(
            &DynBodyConstraint::get_nonlinear_response_wrench,
            active_constraints,
            vehicle_properties.get_structure_to_body_offset(),
            nonlinear_responses_wrench))
    {
        update_vehicle_response (
            nonlinear_responses_wrench, vehicle_properties, non_grav_state);
    }
}


// Construct A and b in the system of equations A*x=b, where
//  - A is the matrix of partial derivatives, dx_i/dx_j,
//  - x is the vector of constraint values (not referenced here), and
//  - b is the vector of right hand side values.
void
DynBodyConstraintsSolver::build_system_of_equations (
    const VehicleProperties& vehicle_properties,
    const VehicleNonGravState& non_grav_state,
    unsigned n_constraints,
    IndexPairVectorT& constraint_indices)
{
    // Tell each constraint to set its internal state in preparation
    // for constructing the system of equations.
    for (auto constraint : active_constraints)
    {
        constraint->setup_constraint (vehicle_properties, non_grav_state);
    }

    // Build the mapping from constraints to indices.
    // Note: This could be combined with the above loop, but that would
    // require writing "and".
    unsigned prev_end = 0u;

    constraint_indices.clear();
    for (auto constraint : active_constraints)
    {
        unsigned new_end  = prev_end + constraint->n_dimensions;
        constraint_indices.push_back (IndexPairT(prev_end, new_end));
        prev_end = new_end;
    }

    // Set the overall dimensionality of the problem.
    unsigned n_dims = constraint_indices.back().second;
    solver->set_n_dimensions(n_dims);
    x_vector.resize(n_dims);

    // Populate the A matrix and b vector by querying each constraint.
    for (unsigned ii = 0; ii < n_constraints; ++ii)
    {
        auto& constraint_ii = *active_constraints[ii];
        auto& range_ii = constraint_indices[ii];

        // This constraint is not truly active if its dimensionality is zero.
        if (range_ii.second == range_ii.first)
        {
            continue;
        }

        // Set the right hand side element for this constraint.
        constraint_ii.set_r_h_s (solver->make_b_vector_view(range_ii));

        // Set the elements of the A matrix for this constraint vs itself.
        constraint_ii.set_self_coeff (
            vehicle_properties,
            solver->make_a_matrix_view(range_ii, range_ii));

        // Set the elements of the A matrix for this constraint vs others.
        for (unsigned jj = 0; jj < n_dims; ++jj)
        {
            if (jj == ii)
            {
                continue;
            }

            auto& constraint_jj = *active_constraints[jj];
            auto& range_jj = constraint_indices[jj];
            if (range_jj.second == range_jj.first)
            {
                continue;
            }

            constraint_ii.set_cross_coeff (
                vehicle_properties,
                constraint_jj,
                solver->make_a_matrix_view(range_ii, range_jj));
        }
    }
}


// Send the solution to the constraint equation to the constraints.
void
DynBodyConstraintsSolver::send_solution (
    unsigned n_constraints,
    const IndexPairVectorT& constraint_indices)
{
    for (unsigned ii = 0; ii < n_constraints; ++ii)
    {
        auto& range_ii = constraint_indices[ii];
        if (range_ii.second > range_ii.first)
        {
            auto& constraint_ii = *active_constraints[ii];
            constraint_ii.set_constraint_values (
                SubVectorView<std::vector<double>, double>(x_vector, range_ii));
        }
    }
}


// Compute the total of the get_wrench calls and the vehicle's response to it.
void
DynBodyConstraintsSolver::update_vehicle_response (
    const Wrench& sum,
    const VehicleProperties& vehicle_properties,
    VehicleNonGravState& non_grav_state)
{
     // Transform the constraints wrench to the vehicle center of mass.
     const Wrench& wrench_co_m = sum.transform_to_point(
         vehicle_properties.get_structure_to_body_offset());

     // Increment the vehicle's center of mass non-grav accel (struct coords)
     // by the translational response to the constraint wrench at the CoM:
     // accel += force / mass.
     Vector3::scale_incr (
         wrench_co_m.get_force(),
         vehicle_properties.get_inverse_mass(),
         non_grav_state.accel_struct);

     // Increment the vehicle's rotational accel (body coordinates) by the
     // rotational response to the constraint wrench:
     //    \dot{omega_{body}} += I^{-1} * (R * torque)
     // where R is the structure-to-body transformation and I is the
     // body-referenced inertia tensor.
     double torque_body[3];
     Vector3::transform_incr (
         vehicle_properties.get_inverse_inertia(),
         Vector3::transform (
             vehicle_properties.get_structure_to_body_transform(),
             wrench_co_m.get_torque(),
             torque_body),
         non_grav_state.omega_dot_body);

     // Transform the angular acceleration to structural.
     Vector3::transform_transpose (
         vehicle_properties.get_structure_to_body_transform(),
         non_grav_state.omega_dot_body,
         non_grav_state.omega_dot_struct);
}


// Compute each constrained object's response to the total constraint wrench.
void
DynBodyConstraintsSolver::compute_constraint_responses (
    const VehicleProperties& vehicle_properties,
    const VehicleNonGravState& non_grav_state)
{
    for (auto constraint : active_constraints)
    {
        constraint->compute_constraint_response (
            vehicle_properties, non_grav_state);
    }
}


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
