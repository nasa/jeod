/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup Mass
 * @{
 *
 * @file models/dynamics/mass/src/mass_reattach.cc
 * Define MassBody::reattach.
 */

/********************************* TRICK HEADER *******************************
Purpose:
  ()

Library Dependency:
  ((mass_reattach.cc)
   (mass.cc)
   (mass_messages.cc)
   (utils/quaternion/src/quat.cc))



******************************************************************************/

// System includes

// JEOD includes
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/mass.hh"
#include "../include/mass_messages.hh"


//! Namespace jeod
namespace jeod {

/**
 * Re-attach a child MassBody to a parent MassBody.
 *
 * \par Assumptions and Limitations
 *  - Rigid Bodies
 *  - Tree attachment structure
 *  - Child is known to be a root or atomic body
 *  - Re-establishing an attachment that previously existed
 * \param[in] offset Desired offset from parent struct frame to attached child's struct frame, expressed in parent struct frame\n Units: M
 * \param[in] T_pstr_cstr Desired transformation matrix from parent struct frame to attached child's struct frame
 */
void
MassBody::reattach (
   double offset[3],
   double T_pstr_cstr[3][3])

{
   MassBody * parent = links.parent ();

   /* Check to see if the body is attached to anything. */
   if (links.is_root()) {
      MessageHandler::fail (
         __FILE__, __LINE__, MassBodyMessages::invalid_node,
         "Body '%s' is a root body and cannot be reattached.",
         name.c_str());

      // Not reached
      return;
   }


   // Update the attachment attributes.
   structure_point.update_orientation (T_pstr_cstr);
   structure_point.update_point (offset);

   // Construct the transformation from the parent body's structural frame
   // to child body's composite body frame:
   //   T_pstr_cbdy = T_cstr_cbdy * T_pstr_cstr
   composite_properties.Q_parent_this.multiply (
      structure_point.Q_parent_this,
      composite_wrt_pstr.Q_parent_this);
   composite_wrt_pstr.compute_transformation ();

   // Compute the displace from the parent body's structural origin to the
   // child body's composite CoM in the parent body's structural frame:
   //   x_pstr_cbdy = x_pstr_cstr + (T_cstr_pstr^T)*x_cstr_cbdy
   Vector3::transform_transpose (
      structure_point.T_parent_this,
      composite_properties.position,
      composite_wrt_pstr.position);
   Vector3::incr (
      structure_point.position,
      composite_wrt_pstr.position);

   // Construct the transformation from the parent body's body frame
   // to child body's composite body frame.
   composite_wrt_pstr.Q_parent_this.multiply_conjugate (
      parent->composite_properties.Q_parent_this,
      composite_wrt_pbdy.Q_parent_this);
   composite_wrt_pbdy.compute_transformation ();

   // Update the parent's mass properties.
   // Note that this sets composite_wrt_pbdy.position.
   parent->set_update_flag ();
   parent->get_root_body_internal()->update_mass_properties ();

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
