/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup Mass
 * @{
 *
 * @file models/dynamics/mass/src/mass_calc_composite_cm.cc
 * Calculate the aggregate cm for a composite MassBody.
 */

/********************************* TRICK HEADER *******************************
Purpose:
  ()

Library Dependency:
  ((mass_calc_composite_cm.o)
   (mass.o))



*******************************************************************************/


// System includes

// JEOD includes
#include "utils/math/include/vector3.hh"
#include "utils/ref_frames/include/tree_links_iterator.hh"

// Model includes
#include "../include/mass.hh"



//! Namespace jeod
namespace jeod {

/**
 * Calculate the aggregate cm for a composite MassBody.
 *
 * \par Assumptions and Limitations
 *  - Rigid bodies
 *  - Must calculate from bottom to top of tree for meaningful results
 */
void
MassBody::calc_composite_cm (
   void)
{
   double mass;               // kg   Total mass
   double mass_times_com[3];  // M*kg Total mass * CoM


   // For composite bodies, the mass is the sum of the component masses and
   // the center of mass is given by M*CM = sum (m*cm), summed over the
   // component masses. The component masses comprise the core mass of this body
   // alone plus the composite masses of each attached child body.

   // Initialize the mass and mass*center of mass to the core values.
   mass = core_properties.mass;
   Vector3::scale (core_properties.position, core_properties.mass,
                   mass_times_com);

   // Add each child body's contribution to the accumulators.
   for (auto* child_link : TreeLinksChildrenRange<MassBodyLinks>(links)) {
      const MassBody* child = &(child_link->container());

      // Accumulate the mass and mass * center of mass for this child body.
      mass += child->composite_properties.mass;
      Vector3::scale_incr (
         child->composite_wrt_pstr.position, child->composite_properties.mass,
         mass_times_com);
   }

   // Compute the composite center of mass.
   if (mass > 0.0) {
      composite_properties.inverse_mass = 1.0 / mass;
      Vector3::scale (mass_times_com,
                      composite_properties.inverse_mass,
                      composite_properties.position);
   }
   else {
      composite_properties.inverse_mass = 0.0;
      Vector3::initialize (composite_properties.position);
   }

   // Store the total mass back into composite properties
   composite_properties.mass = mass;
   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
