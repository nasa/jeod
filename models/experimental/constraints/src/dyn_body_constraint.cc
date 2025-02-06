/**
 * @addtogroup Models
 * @{
 * @addtogroup Experimental
 * @{
 * @addtogroup Constraints
 * @{
 *
 * @file
 * Defines DynBodyConstraint member functions.
 */

/*
Purpose: ()
Library dependencies: (
  (constraint_frame.cc))
*/

#include "../include/dyn_body_constraint.hh"

#include "../include/constraint_component.hh"
#include "../include/dyn_body_constraints_solver.hh"

#include "dynamics/dyn_body/include/vehicle_properties.hh"

#include "experimental/math/include/matrix_view.hh"
#include "experimental/math/include/vector_view.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"

#include <utility>

//! Namespace jeod
namespace jeod
{

// Destructor.
DynBodyConstraint::~DynBodyConstraint()
{
    if(solver != nullptr)
    {
        solver->remove_constraint(this);
    }
}

// Mark the constraint as active.
void DynBodyConstraint::activate()
{
    if(inactive)
    {
        inactive = false;

        if(solver != nullptr)
        {
            solver->activate_constraint(this);
        }
    }
}

// Mark the constraint as inactive.
void DynBodyConstraint::deactivate()
{
    if(!inactive)
    {
        inactive = true;

        if(solver != nullptr)
        {
            solver->deactivate_constraint(this);
        }
    }
}

void DynBodyConstraint::attach_to_solver()
{
    for(unsigned ii = 0; ii < n_dimensions; ++ii)
    {
        get_component(ii)->attach_to_solver(solver, solver->get_dyn_body());
    }
}

void DynBodyConstraint::detach_from_solver()
{
    for(unsigned ii = 0; ii < n_dimensions; ++ii)
    {
        get_component(ii)->detach_from_solver(solver, solver->get_dyn_body());
    }
}

void DynBodyConstraint::update_component_attachment(const VehicleProperties & vehicle_properties)
{
    for(unsigned ii = 0; ii < n_dimensions; ++ii)
    {
        get_component(ii)->update_attachment(vehicle_properties);
    }
}

void DynBodyConstraint::setup_component_constraint(const VehicleProperties & vehicle_properties,
                                                   const VehicleNonGravState & non_grav_state)
{
    for(unsigned ii = 0; ii < n_dimensions; ++ii)
    {
        get_component(ii)->setup_constraint(vehicle_properties, non_grav_state);
    }
}

void DynBodyConstraint::set_r_h_s(VectorView<double> & rhs_slice) const
{
    for(unsigned ii = 0; ii < n_dimensions; ++ii)
    {
        rhs_slice[ii] = get_component(ii)->get_right_hand_side();
    }
}

void DynBodyConstraint::set_cross_coeff(const VehicleProperties & vehicle_properties,
                                        const DynBodyConstraint & other,
                                        MatrixView<double> & a_slice) const
{
    for(unsigned ii = 0; ii < n_dimensions; ++ii)
    {
        auto & this_component = *get_component(ii);
        for(unsigned jj = 0; jj < other.n_dimensions; ++jj)
        {
            a_slice(ii, jj) = this_component.get_cross_coeff(vehicle_properties, *other.get_component(jj));
        }
    }
}

void DynBodyConstraint::set_self_coeff(const VehicleProperties & vehicle_properties, MatrixView<double> & a_slice) const
{
    set_cross_coeff(vehicle_properties, *this, a_slice);
    for(unsigned ii = 0; ii < n_dimensions; ++ii)
    {
        a_slice(ii, ii) += 1.0;
    }
}

void DynBodyConstraint::set_constraint_values(const VectorView<double, double> & solution_slice)
{
    for(unsigned ii = 0; ii < n_dimensions; ++ii)
    {
        get_component(ii)->set_constraint_value(solution_slice[ii]);
    }
}

void DynBodyConstraint::compute_constraint_response(const VehicleProperties & vehicle_properties,
                                                    const VehicleNonGravState & non_grav_state)
{
    for(unsigned ii = 0; ii < n_dimensions; ++ii)
    {
        get_component(ii)->compute_constraint_response(vehicle_properties, non_grav_state);
    }
}

DynBody * DynBodyConstraint::get_dyn_body()
{
    return solver->get_dyn_body();
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
