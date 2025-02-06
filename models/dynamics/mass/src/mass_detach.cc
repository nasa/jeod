/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup Mass
 * @{
 *
 * @file models/dynamics/mass/src/mass_detach.cc
 * Define MassBody detachment methods.
 */

/******************************************************************************

Purpose:
   ()

Library Dependencies:
   ((mass_detach.o)
    (mass.o)
    (mass_messages.o)
    (mass_point.o)
    (mass_point_state.o)
    (dynamics/dyn_manager/dyn_manager.o)
    (utils/math/dm_invert_symm.o))


******************************************************************************/


// Model includes
#include "../include/mass.hh"
#include "../include/mass_messages.hh"

// JEOD includes
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/ref_frames/include/tree_links_iterator.hh"

// System includes
#include <cstddef>



//! Namespace jeod
namespace jeod {

/**
 * Detach the two bodies, 'this' and the argument body, from each other such
 * such that the detachment occurs between the superior body and the immediate
 * child along the path from the inferior body to the superior body.
 * Returns true to indicate success, false to indicate failure.
 *
 * \par Assumptions and Limitations
 *  - Instances of more derived classes, with presumably more involved dynamics,
 *     are situated higher in the mass tree than are more basic instances.
 *     For example, a simple MassBody can be a child of a DynBody, but not the
 *     other way around.
 *  - The attachment in the mass tree between the immediate child and the superior
 *     body is assumed to reflect a real physical attachment.
 * @return Success flag
 * \param[in,out] mass_body The other body
 */
bool
MassBody::detach (
   MassBody & mass_body)
{
   MassBody * detaching_body = nullptr;
   MassBody * parent_body = nullptr;


   // Find the body that is to be detached from its immediate parent.
   // First search from 'this' up for the input mass_body, and if that fails
   // search from the input mass_body up for 'this'. Both searches failing is
   // an error.


   for (auto* link : TreeLinksAscendRange<MassBodyLinks>(links)) {
      MassBody* iter_body = &(link->container());
      if (iter_body == &mass_body) {
         parent_body = &mass_body;
         break;
      }
      detaching_body = iter_body;
   }

   // parent_body is null => First search failed. Do the second search.
   if (parent_body == nullptr) {
      for (auto* link : TreeLinksAscendRange<MassBodyLinks>(mass_body.links)) {
         MassBody* iter_body = &(link->container());
         if (iter_body == this) {
            parent_body = this;
            break;
         }
         detaching_body = iter_body;
      }
   }

   // parent_body is still null => both searches failed.
   // There is no way to perform the requested detachment.
   // This is a non-fatal error.
   if (parent_body == nullptr) {
      MessageHandler::inform (
         __FILE__, __LINE__, MassBodyMessages::invalid_detach,
         "\nWarning: Unable to find detach point between\n"
            "bodies '%s' and '%s'. No detachment was performed.",
         name.c_str(), mass_body.name.c_str());

      return false;
   }


   // Detach the detaching body from its immediate parent, returning the
   // status from that method as the status from this method.
   return (detaching_body->detach ());
}


/**
 * Detach a mass body from its immediate parent.
 * Returns true to indicate success, false to indicate failure.
 *
 * \par Assumptions and Limitations
 *  - Instances of more derived classes, with presumably more involved dynamics,
 *     are situated higher in the mass tree than are more basic instances.
 *     For example, a simple MassBody can be a child of a DynBody, but not the
 *     other way around.
 *  - The attachment in the mass tree between the immediate child and the superior
 *     body is assumed to reflect a real physical attachment.
 * @return Success flag
 */
bool
MassBody::detach (
   void)
{
   MassBody * parent = links.parent();
   bool success;

   // If parent is a DynBody, call appropriate detach call
   DynBody * parent_dyn_body = parent->dyn_owner;
   if(parent_dyn_body != nullptr)
   {
       success = parent_dyn_body->remove_mass_body(*this);
   } else {

       // Perform the detachment, but only if it is valid to do so.
       if (detach_validate (parent, true)) {

          // Tell the child (this) to break the logical links between the two.
          detach_sever_links (*parent);

          // Tell the parent to update properties/state/...
          parent->detach_update_properties (*this);

          // Note that the detachment was performed.
          success = true;
       }

       // Validation failed. Note that the detachment was not performed.
       else {
          success = false;
       }
   }

   return success;
}


/**
 * Validate whether the pending detachment is legal.
 *
 * Extensibility comments --
 * - Overriding this method doesn't make a whole lot of sense.
 * - Any class that overrides this method must either invoke this
 *   method
 *   or perform the actions performed herein.
 *
 * \par Assumptions and Limitations
 *  - The subject body, this, must be a root body. This is not checked.
 * @return Validity indicator
 * \param[in] parent The parent body; the body from which this body is to be detached.
 * \param[in] generate_message Generate message if invalid?
 */
bool
MassBody::detach_validate (
   const MassBody * parent,
   bool generate_message)
const
{
   bool is_valid;

   // Validate that the child can detach from the parent in terms of
   // - Connectivity (detach_validate_parent, sent to child)
   // - Whatever (detach_validate_child, sent to parent).
   is_valid = (detach_validate_parent (parent, generate_message) &&
               parent->detach_validate_child (*this, generate_message));

   return is_valid;
}


/**
 * Validate whether the pending detachment is legal from a connectivity
 * point of view.
 *
 * Extensibility comments --
 * - This method determines whether invoking detach_sever_links
 *   makes sense.
 * - Any class that overrides this method must either invoke this method
 *   or perform the actions performed herein.
 * @return Validity indicator
 * \param[in] parent The parent body; the body from which this body is to be detached.
 * \param[in] generate_message Generate message if invalid?
 */
bool
MassBody::detach_validate_parent (
   const MassBody * parent,
   bool generate_message)
const
{
   bool is_valid = true;

   // From a MassBody perspective, the only problem with detachment is
   // when a root body attempts to detach.
   is_valid = (parent != nullptr);
   if( parent == nullptr )
   {
       is_valid = false;
       if ( generate_message ) {
           MessageHandler::inform (
         __FILE__, __LINE__, MassBodyMessages::invalid_detach,
         "\nWarning: Body '%s' is a root body and cannot be detached.",
         name.c_str());
       }
   }
   else if( parent->dyn_manager != nullptr
     && parent->dyn_manager->find_dyn_body( parent->name.c_str() ) != nullptr )
   {
       is_valid = false;
       if ( generate_message ) {
           MessageHandler::warn (
         __FILE__, __LINE__, MassBodyMessages::invalid_detach,
         "\nWarning: Body '%s' refers to a DynBody and cannot be detached "
         "through MassBody detach methods.",
         parent->name.c_str());
       }
   }

   return is_valid;
}


/**
 * Validate whether the pending detachment is legal from a mass tree
 * point of view.
 *
 * Extensibility comments --
 * - This method determines whether invoking detach_update_properties
 *   makes sense.
 * @return Validity indicator
 * \param[in] child The child body; the body to be detached from this body.
 * \param[in] generate_message Generate message if invalid?
 */
bool
MassBody::detach_validate_child (
   const MassBody & child,
   bool generate_message )
const
{
    bool is_valid = true;

    if( dyn_manager != nullptr
     && dyn_manager->find_dyn_body( child.name.c_str() ) != nullptr )
    {
        is_valid = false;
        if ( generate_message ) {
            MessageHandler::inform (
                __FILE__, __LINE__, MassBodyMessages::invalid_detach,
                "\nWarning: Body '%s' refers to a DynBody and cannot be detached "
                "through MassBody detach methods.",
                child.name.c_str());
        }
    }

    return is_valid;
}


/**
 * Break the logical connectivity between parent and child.
 *
 * Extensibility comments --
 * - This method is invoked before the updating the parent/child states.
 * - The generic purpose of this method is to sever all connectivity
 *   links between parent and child, in terms of the child class.
 * - Any class that overrides this method must either invoke this method
 *   or perform the actions performed herein.
 *
 * \par Assumptions and Limitations
 *  - The detachment is valid; not checked.
 * \param[in,out] parent The parent body; the body from which this body is to be detached.
 */
void
MassBody::detach_sever_links (
   MassBody & parent JEOD_UNUSED)
{

   // Sever the mass links between child and parent, and siblings.
   links.detach();

   return;
}


/**
 * Update parent and child properties to reflect that they are detached.
 *
 * Extensibility comments --
 * - This method is sent to the parent body of the detachment after
 *   the child body has severed the logical connectivity between
 *   the parent body and child body.
 * - The generic purpose of this method is to update any physical
 *   properties that change as a result of the detachment.
 * - Any class that overrides this method must either invoke this method
 *   or perform the actions performed herein.
 *
 * \par Assumptions and Limitations
 *  - The detachment is valid and logical connectivity has been severed.
 *     Neither assumption is checked.
 * \param[in,out] child The child body; the body newly detached from this body.
 */
void
MassBody::detach_update_properties (
   MassBody & child)
{

   // Update the mass properties from the root down to this body.
   set_update_flag ();
   get_root_body_internal()->update_mass_properties ();

   // Re-initialize the child's auxiliarly attachment info.
   child.structure_point.initialize_mass_point ();
   child.composite_wrt_pstr.initialize_mass_point ();
   child.composite_wrt_pbdy.initialize_mass_point ();

   // Root bodies need a valid inverse inertia tensor.
   // Child bodies don't, so calculate the child's inverse inertia tensor.
   if (child.compute_inverse_inertia &&
       (child.composite_properties.mass > 0.0)) {
      Matrix3x3::invert_symmetric(child.composite_properties.inertia,
                                  child.composite_properties.inverse_inertia);
   } else {
      Matrix3x3::initialize (child.composite_properties.inverse_inertia);
   }

   return;
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
