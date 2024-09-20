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
  ((structure_integrated_dyn_body.cc)
   (structure_integrated_dyn_body_collect.cc)
   (structure_integrated_dyn_body_integration.cc)
   (structure_integrated_dyn_body_pt_accel.cc)
   (structure_integrated_dyn_body_solve.cc))

*/

#include "../include/structure_integrated_dyn_body.hh"

#include "utils/memory/include/jeod_alloc.hh"

#include <cstddef>

//! Namespace jeod
namespace jeod
{

StructureIntegratedDynBody::StructureIntegratedDynBody()
    : vehicle_properties(mass.structure_point.position,
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
    JEOD_REGISTER_CLASS(StructureIntegratedDynBody);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
