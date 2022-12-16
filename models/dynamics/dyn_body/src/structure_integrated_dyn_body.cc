/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynBody
 * @{
 *
 * @file
 * Define base member functions for StructureIntegratedDynBody.
 */

/*

Purpose: ()

Library dependencies:
  ((structure_integrated_dyn_body.o)
   (structure_integrated_dyn_body_collect.o)
   (structure_integrated_dyn_body_integration.o)
   (structure_integrated_dyn_body_pt_accel.o)
   (structure_integrated_dyn_body_solve.o))

*/

#include "../include/structure_integrated_dyn_body.hh"

#include "utils/memory/include/jeod_alloc.hh"

#include <cstddef>


//! Namespace jeod 
namespace jeod {

StructureIntegratedDynBody::StructureIntegratedDynBody ()
:
    effector_wrench_collection(),
    constraints_solver(nullptr),
    effector_wrench(),
    vehicle_properties (
        mass.structure_point.position,
        mass.structure_point.T_parent_this,
        mass.composite_properties.mass,
        mass.composite_properties.position,
        mass.composite_properties.inertia,
        mass.composite_properties.T_parent_this,
        mass.composite_properties.inverse_mass,
        mass.composite_properties.inverse_inertia)
{
    // This class integrates the structural frame.
    integrated_frame = &structure;

    // Register the checkpointable items.
    JEOD_REGISTER_CLASS (StructureIntegratedDynBody);
}


StructureIntegratedDynBody::~StructureIntegratedDynBody ()
{
}


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
