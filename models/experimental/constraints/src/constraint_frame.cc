/**
 * @addtogroup Models
 * @{
 * @addtogroup Experimental
 * @{
 * @addtogroup Constraints
 * @{
 *
 * @file
 * Defines ConstraintFrame member functions.
 */

/*
Purpose: ()
*/


#include "../include/constraint_frame.hh"

#include "../include/dyn_body_constraints_solver.hh"

#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"


//! Namespace jeod 
namespace jeod {

ConstraintFrame::ConstraintFrame()
{
    Matrix3x3::identity (T_struct_constraint);
    Vector3::initialize (origin_position_struct);
    Matrix3x3::identity (T_root_constraint);
    Vector3::initialize (origin_position_root);
}


void
ConstraintFrame::set_struct_to_constraint_frame (
    SolverTypes::ConstDecayedMatrix3x3T transform_in,
    SolverTypes::ConstDecayedVector3T offset_in)
{
    Matrix3x3::copy (transform_in, T_struct_constraint);
    Vector3::copy (offset_in, origin_position_struct);

    if (solver != nullptr)
    {
        update_root_to_constraint_frame ();
    }
}


// Compute the root-to-this transformation and offset given
// that the associate dyn body has attached / reattached / detached
// to / from some parent dyn body.
void
ConstraintFrame::update_root_to_constraint_frame ()
{
    Matrix3x3::product (
        T_struct_constraint,
        solver->get_root_to_struct_transform(),
        T_root_constraint);

    Vector3::incr (
        solver->get_root_to_struct_offset(),
        Vector3::transform_transpose (
            solver->get_root_to_struct_transform(),
            origin_position_struct,
            origin_position_root));
}


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
