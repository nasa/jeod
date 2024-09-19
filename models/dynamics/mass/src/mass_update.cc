/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup Mass
 * @{
 *
 * @file models/dynamics/mass/src/mass_update.cc
 * Define MassBody::update_mass_properties.
 */

/******************************************************************************

Purpose:
  ()

Library dependencies:
  ((mass_update.cc)
   (mass.cc)
   (mass_point_state.cc)
   (utils/math/src/dm_invert_symm.cc))



*******************************************************************************/

// System includes

// JEOD includes
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"
#include "utils/ref_frames/include/tree_links_iterator.hh"

// Model includes
#include "../include/mass.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Update composite mass properties for a mass tree marked for update.
 * The properties are updated from the bottom up to generate correct results.
 *
 * \par Assumptions and Limitations
 *  - Rigid bodies
 *  - Tree attachment structure
 */
void MassBody::update_mass_properties()
{
    // If this body is not marked for update then just return
    if(!needs_update)
    {
        return;
    }

    // The core and composite properties are the same for an atomic body.
    if(links.is_atomic())
    {
        composite_properties.mass = core_properties.mass;
        if(composite_properties.mass > 0.0)
        {
            composite_properties.inverse_mass = 1.0 / composite_properties.mass;
        }
        else
        {
            composite_properties.inverse_mass = 0.0;
        }

        Vector3::copy(core_properties.position, composite_properties.position);
        Vector3::initialize(core_wrt_composite.position);

        Matrix3x3::copy(core_properties.inertia, composite_properties.inertia);
    }

    // Calculate the composite properties for a composite body.
    else
    {
        // First update all of the attached bodies' mass properties, if needed.
        for(auto * link : TreeLinksChildrenRange<MassBodyLinks>(links))
        {
            MassBody * child = &(link->container());
            if(child->needs_update)
            {
                child->update_mass_properties();
            }
        }

        // Calculate the composite mass and center of mass.
        calc_composite_cm();

        // Calculate the location of the child's composite CoM wrt this body's
        // composite CoM in this body's body frame.
        double r_cm_cm_str[3]; // M     CoM to CoM in structural coords
        for(auto * link : TreeLinksChildrenRange<MassBodyLinks>(links))
        {
            MassBody & child = link->container();

            Vector3::diff(child.composite_wrt_pstr.position, composite_properties.position, r_cm_cm_str);
            Vector3::transform(composite_properties.T_parent_this, r_cm_cm_str, child.composite_wrt_pbdy.position);
        }

        // Calculate the location of the parent's (i.e. this body) core CoM
        // wrt its new composite CoM in this body's body frame.
        Vector3::diff(core_properties.position, composite_properties.position, r_cm_cm_str);
        Vector3::transform(composite_properties.T_parent_this, r_cm_cm_str, core_wrt_composite.position);

        // Calculate the composite mass, CoM, and inertia for this body.
        calc_composite_inertia();
    }

    // For a root body, calculate the inverse inertia tensor (this is only
    // done for root bodies) and set the rather meaningless locations of
    // of various points with respect to a parent point to zero.
    if(links.is_root())
    {
        if(compute_inverse_inertia && (composite_properties.mass > 0.0))
        {
            Matrix3x3::invert_symmetric(composite_properties.inertia, composite_properties.inverse_inertia);
        }
        else
        {
            Matrix3x3::initialize(composite_properties.inverse_inertia);
        }

        Vector3::initialize(structure_point.position);
        Vector3::initialize(composite_wrt_pstr.position);
        Vector3::initialize(composite_wrt_pbdy.position);
    }

    // For non-root bodies compute the location of the composite CoM wrt the
    // the parent structural origin in the parent structural frame.
    // Note that the location of the composite CoM wrt the parent composite CoM
    // is calculated above for child bodies using this element of the child
    // structure.
    else
    {
        Vector3::transform_transpose(structure_point.T_parent_this,
                                     composite_properties.position,
                                     composite_wrt_pstr.position);
        Vector3::incr(structure_point.position, composite_wrt_pstr.position);
    }

    // Clear the update flag
    needs_update = false;
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
