/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup Mass
 * @{
 *
 * @file models/dynamics/mass/src/mass_calc_composite_inertia.cc
 * Calculate the aggregate inertia matrix for a composite MassBody.
 */

/********************************* TRICK HEADER *******************************
PURPOSE:
  ()

LIBRARY DEPENDENCY:
  ((mass_calc_composite_inertia.cc)
   (mass.cc))



******************************************************************************/


// System includes

// JEOD includes
#include "utils/ref_frames/include/tree_links_iterator.hh"

// Model includes
#include "../include/mass.hh"


//! Namespace jeod
namespace jeod {

/**
 * Calculate the aggregate inertia matrix for a composite MassBody.
 *
 * \par Assumptions and Limitations
 *  - Rigid Bodies
 *  - Tree attachment structure
 *  - Must calculate from bottom to top of tree for meaningful results
 */
void
MassBody::calc_composite_inertia (
   void)
{
   double offset_inertia[3][3];       // kg*M2 Inertia due to child body mass
   double transform_inertia[3][3];    // kg*M2 Inertia due to child body inertia


   // For composite bodies, the composite inertia tensor is the sum of all
   // the inertia tensors for all the component masses shifted per the parallel
   // axis theorem.  The component masses comprise the core mass of this body
   // alone plus the composite masses of each attached child body.


   // The core and composite masses share a common body frame.
   // Initialize the composite inertia as that of the core mass plus the
   // inertia due to the core mass acting as a point mass at the core CoM.

   // Compute the inertia due to the core center of mass being offset from
   // the composite center of mass.
   compute_point_mass_inertia (core_properties.mass,
                               core_wrt_composite.position,
                               offset_inertia);

   // Compute the contribution to the composite inertia by the core mass.
   Matrix3x3::add (offset_inertia, core_properties.inertia,
                   composite_properties.inertia);

   // Add each child body's contribution to the composite inertia.
   for (auto* child_link : TreeLinksChildrenRange<MassBodyLinks>(links)) {
      const MassBody* child = &(child_link->container());

      // Compute the inertia due to the core center of mass being offset from
      // the composite center of mass.
      compute_point_mass_inertia (child->composite_properties.mass,
                                  child->composite_wrt_pbdy.position,
                                  offset_inertia);

      // Transform the child composite inertia to this body's body frame.
      Matrix3x3::transpose_transform_matrix (
         child->composite_wrt_pbdy.T_parent_this,
         child->composite_properties.inertia,
         transform_inertia);

      // Add the child contributions to the composite inertia.
      Matrix3x3::incr (offset_inertia, composite_properties.inertia);
      Matrix3x3::incr (transform_inertia, composite_properties.inertia);
   }
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
