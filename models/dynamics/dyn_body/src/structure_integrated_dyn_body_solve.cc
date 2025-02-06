/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynBody
 * @{
 *
 * @file
 * Define StructureIntegratedDynBody methods related to force and torque
 * accumulation and propagation.
 */

/*
Purpose: ()
Library dependencies:
  ((structure_integrated_dyn_body.o)
   (experimental/constraints/dyn_body_constraints_solver.o))
*/



#include "../include/structure_integrated_dyn_body.hh"

#include "../include/dyn_body_messages.hh"
#include "utils/message/include/message_handler.hh"

#include "utils/math/include/vector3.hh"

#include "experimental/constraints/include/dyn_body_constraints_solver.hh"


//! Namespace jeod 
namespace jeod {

void
StructureIntegratedDynBody::attach_update_properties (
   double offset_pstr_cstr_pstr[3],
   double T_pstr_cstr[3][3],
   DynBody & child)
{
    DynBody::attach_update_properties (
        offset_pstr_cstr_pstr, T_pstr_cstr, child);

    StructureIntegratedDynBody* child_dyn_body =
        dynamic_cast<StructureIntegratedDynBody*>(&child);
    if (child_dyn_body != nullptr)
    {
        if (child_dyn_body->constraints_solver != nullptr)
        {
            if (constraints_solver == nullptr)
            {
                MessageHandler::fail (
                    __FILE__, __LINE__, DynBodyMessages::invalid_attachment,
                    "Solver for DynBody '%s' is not set.\n"
                    "Solvers must be set prior to making attachments.",
                    name.c_str());
            }

            child_dyn_body->constraints_solver->attach (
                *constraints_solver,
                child_dyn_body->vehicle_properties);
        }
    }
}


bool
StructureIntegratedDynBody::detach (
    DynBody & other_body)
{
    bool success = true;

    // Flip who calls if called by parent
    if( other_body.get_parent_body() == this )
    {
        StructureIntegratedDynBody* sidyn_body;
        sidyn_body = dynamic_cast<StructureIntegratedDynBody*>(&other_body);
        success = sidyn_body->detach(*this);
    }
    else
    {
        success = DynBody::detach (other_body);
        if( !success )
        {
            MessageHandler::fail (
                    __FILE__, __LINE__, DynBodyMessages::invalid_attachment,
                    "\nDuring detach process:\n"
                    "'%s' failed to detach with '%s'\n "
                    "Cannot detach associated constraints.",
                    name.c_str(),
                    other_body.name.c_str() );
            return success;
        }

        if (constraints_solver != nullptr)
        {
            constraints_solver->detach (vehicle_properties);
        }
    }
    return success;
}


// Set the solver.
void
StructureIntegratedDynBody::set_solver (
    DynBodyConstraintsSolver& solver_in)
{
    if (constraints_solver != nullptr)
    {
        MessageHandler::fail (
            __FILE__, __LINE__, DynBodyMessages::invalid_body,
            "Duplicate call to set_solver for DynBody '%s'",
            name.c_str());
    }

    constraints_solver = &solver_in;
}


// Add a constraint.
void
StructureIntegratedDynBody::add_constraint (
    DynBodyConstraint* constraint)
{
    if (constraints_solver == nullptr)
    {
        MessageHandler::fail (
            __FILE__, __LINE__, DynBodyMessages::invalid_body,
            "add_constraint called prior to setting the solver.");
    }

    constraints_solver->add_constraint (constraint);
}


// Solve for and apply constraints acting on the vehicle.
void
StructureIntegratedDynBody::solve_constraints ()
{

    // Both translational and rotational dynamics need to be on, and
    // this must be a root body. (The constraint solver collects the
    // constraints on attached bodies.)
    if (! (translational_dynamics && rotational_dynamics
           && (dyn_parent == nullptr)))
    {
        return;
    }

    if (constraints_solver == nullptr)
    {
        MessageHandler::fail (
            __FILE__, __LINE__, DynBodyMessages::invalid_body,
            "solve_constraints called prior to setting the solver.");
    }

    // Populate the non-gravitational state structure.
    Vector3::copy (
        composite_body.state.rot.ang_vel_this, non_grav_state.omega_body);
    Vector3::copy (
        structure.state.rot.ang_vel_this, non_grav_state.omega_struct);
    Vector3::copy (
        derivs.rot_accel, non_grav_state.omega_dot_body);
    Vector3::copy (
        struct_derivs.rot_accel, non_grav_state.omega_dot_struct);
    Vector3::scale (
        collect.extern_forc_struct, mass.composite_properties.inverse_mass, non_grav_state.accel_struct);
    Vector3::transform_transpose (
        mass.composite_properties.T_parent_this,
        collect.inertial_torq,
        non_grav_state.inertial_torque_struct);

    // Solve the constraints, yielding an updated non-gravitational structure.
    constraints_solver->solve (vehicle_properties, non_grav_state);

    // Update the rotational state from the solver's solution.
    Vector3::copy (non_grav_state.omega_dot_struct, struct_derivs.rot_accel);
    Vector3::copy (non_grav_state.omega_dot_body, derivs.rot_accel);

    // ... and the translational state.
    Vector3::transform_transpose (
        structure.state.rot.T_parent_this,
        non_grav_state.accel_struct,
        derivs.non_grav_accel);
    complete_translational_acceleration();
}


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
