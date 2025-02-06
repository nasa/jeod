/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup Mass
 * @{
 *
 * @file models/dynamics/mass/src/mass_attach.cc
 * Define MassBody attach methods.
 */

/*******************************************************************************

Purpose:
    ()

LIBRARY DEPENDENCY:
    ((mass_attach.o)
     (mass.o)
     (mass_point.o)
     (mass_messages.o)
     (mass_point_state.o)
     (utils/quaternion/quat.o))



*******************************************************************************/


// Model includes
#include "../include/mass.hh"
#include "../include/mass_messages.hh"

// JEOD includes
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"
#include "utils/message/include/message_handler.hh"

// System includes
#include <cstddef>


//! Namespace jeod
namespace jeod {

// Internal function prototypes
static void generate_bad_point_message (
   const char * file,
   unsigned int line,
   const char * child_body_name,
   const char * child_point_name,
   const MassPoint * child_point,
   const char * parent_body_name,
   const char * parent_point_name,
   const MassPoint * parent_point);


/**
 * Attach this mass body's root body as a child of the specified mass
 * body such that the specified mass points on the two bodies are
 * coincident and the frames associated with those mass points are
 * related by a 180 degree yaw.
 * @return Success indicator: true=success, false=attachment not performed.
 * \param[in] this_point_name The name of a mass point contained in this mass body's list of mass points.
 * \param[in] parent_point_name The name of a mass point contained in the new parent body's list of mass points.
 * \param[in,out] parent The new parent body; the body to which this body's root body is to be attached.
 */
bool
MassBody::attach_to (
   const char * this_point_name,
   const char * parent_point_name,
   MassBody & parent)
{
   const MassPoint * this_point;
   const MassPoint * parent_point;
   MassPoint this_struct_wrt_this_point;
   MassPoint this_point_wrt_parent_point;
   MassPointState this_struct_wrt_parent_struct;
   bool success;

   if( !attach_validate ( parent, true ) )
   {
       success = false;
       return success;
   }

   // Find the mass points corresponding to the input names.
   this_point   = find_mass_point (this_point_name);
   parent_point = parent.find_mass_point (parent_point_name);


   // Handle failed searches as non-fatal errors (warning + failure return).
   if ((this_point == nullptr) || (parent_point == nullptr)) {
      generate_bad_point_message (
         __FILE__, __LINE__,
         name.c_str(), this_point_name, this_point,
         parent.name.c_str(), parent_point_name, parent_point);

      return false;
   }


   // Construct the affine transformation from the mass point location and
   // frame of the attach point on this body to this body's structural origin
   // and frame; the inverse of the mass point specification.
   Vector3::transform (this_point->T_parent_this, this_point->position,
                       this_struct_wrt_this_point.position);
   Vector3::negate (this_struct_wrt_this_point.position);
   Matrix3x3::transpose (this_point->T_parent_this,
                         this_struct_wrt_this_point.T_parent_this);
   this_point->Q_parent_this.conjugate (
      this_struct_wrt_this_point.Q_parent_this);


   // Construct the affine transformation from the attach point on this body
   // to the attach point on the new parent body.
   // This is always a zero offset and a 180 degree yaw.
   // The point is initialized with a zero offset and an identity transform.
   this_point_wrt_parent_point.T_parent_this[0][0] = -1.0;
   this_point_wrt_parent_point.T_parent_this[1][1] = -1.0;
   this_point_wrt_parent_point.Q_parent_this.scalar    = 0.0;
   this_point_wrt_parent_point.Q_parent_this.vector[2] = 1.0;


   // Link the constructed points via the parent elements.
   // The result will be a chain from this body's structural origin/frame
   // to the new parent body's structural origin/frame.
   this_point_wrt_parent_point.attach (
      const_cast <MassPoint &> (*parent_point));
   this_struct_wrt_this_point.attach (this_point_wrt_parent_point);

   // Compute the affine transformation from the parent body's structural
   // origin/frame to this body's structural origin/frame.
   this_struct_wrt_this_point.compute_state_wrt_pred (
      parent.structure_point, this_struct_wrt_parent_struct);

   // Complete attachment using the geometry-based attach method,
   // returning the success indicator from that method as this method's status.
   success = attach_to (this_struct_wrt_parent_struct.position,
                     this_struct_wrt_parent_struct.T_parent_this,
                     parent);

   // Detach the temporary points.
   this_struct_wrt_this_point.detach ();
   this_point_wrt_parent_point.detach ();

   return success;
}


/**
 * Attach this mass body's root body as a child of the specified mass
 * body such that this body's structural origin is offset from the
 * parent body's structural origin and this body's structural axes
 * are oriented with respect to the parent body's structural axes as
 * specified.
 * @return Success indicator: true=success, false=attachment not performed.
 * \param[in] offset_pstr_cstr_pstr Location of this body's structural origin with respect to the new parent body's structural origin, specified in structural coordinates of the new parent body.\n Units: M
 * \param[in] T_pstr_cstr Transformation matrix from the new parent body's structural frame to this body's structural frame.
 * \param[in,out] parent The new parent body; the body to which this body's root body is to be attached.
 */
bool
MassBody::attach_to (
   double offset_pstr_cstr_pstr[3],
   double T_pstr_cstr[3][3],
   MassBody & parent)
{
   MassBody * root_body;            // This body's root body (pre-attachment).
   bool success;

   if( !attach_validate ( parent, true ) )
   {
       success = false;
       return success;
   }

   // Find this body's root body. It is the root body that is attached.
   root_body = MassBody::get_root_body_internal();

   // Sanity check: Don't make an invalid tree.
   if (root_body == parent.get_root_body()) {
      MessageHandler::error (
         __FILE__, __LINE__, MassBodyMessages::invalid_attach,
         "Bodies '%s' and '%s' are part of the same mass tree.\n"
         "Attachment of body '%s' to '%s' was not performed.",
         name.c_str(), parent.name.c_str(), name.c_str(), parent.name.c_str());

      return false;
   }

   // This is a root body: Use the attach_root_body primitive directly.
   if (root_body == this) {
      success = attach_root_body (
         offset_pstr_cstr_pstr, T_pstr_cstr, parent);
   }

   // This is not a root body: Offset the input transformation by the
   // transformation to the root, then attach.
   else {
      double T_pstr_rstr[3][3];        // Parent struct to root struct xform
      double offset_pstr_rstr_pstr[3]; // Parent to child-root offset in parent
                                       // structure frame
      MassPointState this_struct_wrt_root_struct;

      // Give a low-level warning that the attaching body is not a root body.
      MessageHandler::inform (
         __FILE__, __LINE__, MassBodyMessages::attach_info,
         "Body '%s' is already attached to '%s'. Reattaching to '%s' would\n"
         "either discard this existing attachment or create an invalid tree.\n"
         "Attaching the root body for '%s', '%s', to '%s'.",
         name.c_str(), links.parent()->name.c_str(), parent.name.c_str(),
         name.c_str(), root_body->name.c_str(), parent.name.c_str());

      // Compute the affine transformation from the root structural frame
      // to this body's structural frame.
      structure_point.compute_state_wrt_pred (
         root_body->structure_point, this_struct_wrt_root_struct);

      // Recast the attachment transformation / offset in terms of the root.
      Matrix3x3::product_left_transpose (
         this_struct_wrt_root_struct.T_parent_this, T_pstr_cstr, T_pstr_rstr);

      Vector3::copy (offset_pstr_cstr_pstr, offset_pstr_rstr_pstr);
      Vector3::transform_transpose_decr (
         T_pstr_rstr, this_struct_wrt_root_struct.position,
         offset_pstr_rstr_pstr);

      // Attach.
      success = root_body->attach_root_body (
         offset_pstr_rstr_pstr, T_pstr_rstr, parent);
   }

   return success;
}

/**
 * Attach a child MassBody by point specification. See corresponding
 * MassBody::attach_to() method for more information.
 */
bool
MassBody::attach_child(
    const char * this_point_name,
    const char * child_point_name,
    MassBody & child )
{
    bool status;
    status = child.attach_to( child_point_name,
                              this_point_name,
                              *this );
    return status;
}

/**
 * Attach a child MassBody by location specification. See corresponding
 * MassBody::attach_to() method for more information. Note that the offset and
 * transformation are specified w.r.t. the parent in both attach_to() and
 * attach_child()
 */
bool
MassBody::attach_child (
   double offset_pstr_cstr_pstr[3],
   double T_pstr_cstr[3][3],
   MassBody & child )
{
    bool status;
    status = child.attach_to( offset_pstr_cstr_pstr,
                              T_pstr_cstr,
                              *this);

    return status;
}

/**
 * Attach this mass body, which must be a root body, as a child of the
 * specified mass body such that this body's structural origin is offset
 * from the parent body's structural origin and this body's structural
 * axes are oriented with repect to the parent body's structural axes as
 * specified.
 *
 * \par Assumptions and Limitations
 *  - The subject body, this, must be a root body.
 * @return Success indicator: true=success, false=attachment not performed.
 * \param[in] offset_pstr_cstr_pstr Location of this body's structural origin with respect to the new parent body's structural origin, specified in structural coordinates of the new parent body.\n Units: M
 * \param[in] T_pstr_cstr Transformation matrix from the new parent body's structural frame to this body's structural frame.
 * \param[in,out] parent The new parent body; the body to which this body is to be attached.
 */
bool
MassBody::attach_root_body (
   double offset_pstr_cstr_pstr[3],
   double T_pstr_cstr[3][3],
   MassBody & parent)
{
   bool success;                     // -- Was attachment performed?


   // Enforce that this must be a root body.
   // Note that this is a fatal error. This condition will only occur if
   // some non-JEOD method erroneous invokes
   if (! links.is_root()) {
      MessageHandler::error (
         __FILE__, __LINE__, MassBodyMessages::invalid_attach,
         "MassBody::attach_root_body erroneously invoked on non-root body.\n"
         "Body name: '%s'",
         name.c_str());
      return false;
   }

   // Perform the attachment, but only if it is valid to do so.
   if (attach_validate (parent, true)) {

      // Tell the child (this) to establish the connectivity.
      attach_establish_links (parent);

      // Tell the parent to update properties/state/...
      parent.attach_update_properties (
         offset_pstr_cstr_pstr, T_pstr_cstr, *this);

      // Note that the attachment was performed.
      success = true;
   }

   // Validation failed. Note that the attachment was not performed.
   else {
      success = false;
   }

   return success;
}


/**
 * Validate whether the pending attachment is legal.
 *
 * Extensibility comments --
 * - Overriding this method doesn't make a whole lot of sense.
 * - Any class that overrides this method must either invoke this method
 *   or perform the actions performed herein.
 *
 * \par Assumptions and Limitations
 *  - The subject body, this, must be a root body. This is not checked.
 * @return Validity indicator
 * \param[in] parent The new parent body; the body to which this body is to be attached.
 * \param[in] generate_message Generate message if invalid?
 */
bool
MassBody::attach_validate (
   const MassBody & parent,
   bool generate_message)
const
{
   bool is_valid;

   // Validate that the child can attach to the parent in terms of
   // - Connectivity (attach_validate_parent, sent to child)
   // - Properties (attach_validate_child, sent to parent).
   is_valid = (attach_validate_parent (parent, generate_message) &&
               parent.attach_validate_child (*this, generate_message));

   return is_valid;
}


/**
 * Validate whether the pending attachment is legal from a connectivity
 * point of view.
 *
 * Extensibility comments --
 * - This method determines whether invoking attach_establish_links
 *   makes sense.
 * - Any class that overrides this method must either invoke this method
 *   or perform the actions performed herein.
 *
 * \par Assumptions and Limitations
 *  - The subject body, this, must be a root body. This is not checked.
 * @return Validity indicator
 * \param[in] parent The new parent body; the body to which this body is to be attached.
 * \param[in] generate_message Generate message if invalid?
 */
bool
MassBody::attach_validate_parent (
   const MassBody & parent,
   bool generate_message)
const
{
    bool is_valid = true;

    // MassBody connection methods must not be established for DynBody
    if(   dyn_manager != NULL
       && dyn_manager->find_dyn_body( parent.name.c_str() ) != NULL )
    {
        if( generate_message )
        {
            MessageHandler::error (
                    __FILE__, __LINE__, MassBodyMessages::invalid_attach,
                    "Error: Body  '%s' is a DynBody.\n"
                    "MassBody attachment of body '%s' to '%s' was not performed.",
                    parent.name.c_str(), name.c_str(), parent.name.c_str() );
        }
        is_valid = false;
    }
    // From a MassBody perspective, the only invalid attachment is one that
    // would make a cyclic graph. This happens if and only if the parent and
    // this body are already members of the same mass tree.
    else if (   get_root_body() != nullptr
             && parent.get_root_body() == get_root_body() )
    {
        if ( generate_message ) {
            MessageHandler::error (
                    __FILE__, __LINE__, MassBodyMessages::invalid_attach,
                    "Error: Bodies '%s' and '%s' are part of the same mass tree.\n"
                    "Attachment of body '%s' to '%s' was not performed.",
                    name.c_str(), parent.name.c_str(), name.c_str(), parent.name.c_str());
        }
        is_valid = false;
    }
    return is_valid;
}


/**
 * Validate whether the pending attachment is legal from a physical
 * point of view.
 *
 * Extensibility comments --
 * - This method determines whether invoking attach_update_properties
 *   makes sense.
 * - This is a free pass for a MassBody.
 *
 * @return Validity indicator
 * \param[in] child The child body; the body to be attached to this body.
 * \param[in] generate_message Generate message if invalid?
 */
bool
MassBody::attach_validate_child (
   const MassBody & child,
   bool generate_message )
const
{
    bool is_valid = true;

    // MassBody connection methods must not be established for a DynBody
    if(    dyn_manager != nullptr
       && !dyn_manager->find_dyn_body( child.name.c_str() ) )
    {
        is_valid = false;
        if( generate_message )
        {
            MessageHandler::error (
                    __FILE__, __LINE__, MassBodyMessages::invalid_attach,
                    "Error: Body  '%s' is a DynBody.\n"
                    "MassBody attachment of body '%s' to '%s' was not performed.",
                    child.name.c_str(), child.name.c_str(), name.c_str() );
        }
    }
    // MassBody connection methods must not be established if already connected
    // to another dynamic body
    else if( dyn_manager != nullptr
         && !dyn_manager->find_dyn_body( child.get_root_body()->name.c_str()))
    {
        is_valid = false;
        if( generate_message )
        {
            MessageHandler::error (
                    __FILE__, __LINE__, MassBodyMessages::invalid_attach,
                    "Error: Body  '%s' is already attached.\n"
                    "MassBody attachment of body '%s' to '%s' was not performed.",
                    child.name.c_str(), child.name.c_str(), name.c_str() );
        }
    }

    return is_valid;
}


/**
 * Establish the logical connectivity between parent and child.
 *
 * Extensibility comments --
 * - This method is invoked before the computing the physical relation
 *   between parent and child.
 * - The generic purpose of this method is to establish the logical
 *   connectivity between parent and child in terms of the child class.
 * - Any class that overrides this method must either invoke this method
 *   or perform the actions performed herein.
 *
 * \par Assumptions and Limitations
 *  - The attachment is valid; not checked.
 * \param[in,out] parent The new parent body; the body to which this body is to be attached.
 */
void
MassBody::attach_establish_links (
   MassBody & parent)
{

   // Establish links between mass points in the child (this) and the parent.
   structure_point.attach (parent.structure_point);
   composite_wrt_pstr.attach (parent.structure_point);
   composite_wrt_pbdy.attach (parent.composite_wrt_pbdy);

   // Link this body as a child of the parent.
   links.attach (parent.links);

   return;
}


/**
 * Set the relation between parent and child and update the mass
 * properties.
 *
 * Extensibility comments --
 * - This method is sent to the parent body of the attachment after
 *   the child body has established the logical connectivity between
 *   the parent body and child body.
 * - The generic purpose of this method is to establish the physical
 *   relation between parent and child and to update any physical
 *   properties that change as a result of the attachment.
 * - Any class that overrides this method must either invoke this method
 *   or perform the actions performed herein.
 *
 * \par Assumptions and Limitations
 *  - The attachment is valid and logical connectivity has been established.
 *     Neither assumption is checked.
 * \param[in] offset_pstr_cstr_pstr Location of this body's structural origin with respect to the new parent body's structural origin, specified in structural coordinates of the new parent body.\n Units: M
 * \param[in] T_pstr_cstr Transformation matrix from the new parent body's structural frame to this body's structural frame.
 * \param[in,out] child The child body; the body newly attached to this body.
 */
void
MassBody::attach_update_properties (
   double offset_pstr_cstr_pstr[3],
   double T_pstr_cstr[3][3],
   MassBody & child)
{

   // Set the child's attachment transformation and offset w.r.t. this body.
   child.structure_point.update_orientation (T_pstr_cstr);
   child.structure_point.update_point (offset_pstr_cstr_pstr);

   // Construct the transformation from the parent body's structural frame
   // to child body's composite body frame:
   //   T_pstr_cbdy = T_cstr_cbdy * T_pstr_cstr
   child.composite_properties.Q_parent_this.multiply (
      child.structure_point.Q_parent_this,
      child.composite_wrt_pstr.Q_parent_this);
   child.composite_wrt_pstr.compute_transformation ();

   // Compute the displace from the parent body's structural origin to the
   // child body's composite CoM in the parent body's structural frame:
   //   x_pstr_cbdy = x_pstr_cstr + (T_cstr_pstr^T)*x_cstr_cbdy
   Vector3::transform_transpose (
      child.structure_point.T_parent_this,
      child.composite_properties.position,
      child.composite_wrt_pstr.position);
   Vector3::incr (
      child.structure_point.position,
      child.composite_wrt_pstr.position);

   // Construct the transformation from the parent body's body frame
   // to child body's composite body frame.
   child.composite_wrt_pstr.Q_parent_this.multiply_conjugate (
      composite_properties.Q_parent_this,
      child.composite_wrt_pbdy.Q_parent_this);
   child.composite_wrt_pbdy.compute_transformation ();

   // Update this body's mass properties.
   // Note that this sets child.composite_wrt_pbdy.position.
   set_update_flag ();
   get_root_body_internal()->update_mass_properties ();

   return;
}


/**
 * Generate a message regarding failure to find mass points on a vehicle.
 *
 * \par Assumptions and Limitations
 *  - One or both of the input MassPoint pointers is null.
 * \param[in] file File name
 * \param[in] line Line number
 * \param[in] child_body_name Name of child body
 * \param[in] child_point_name Name of child mass point
 * \param[in] child_point Child mass point
 * \param[in] parent_body_name Name of parent body
 * \param[in] parent_point_name Name of parent mass point
 * \param[in] parent_point Parent mass point
 */
static void
generate_bad_point_message (
   const char * file,
   unsigned int line,
   const char * child_body_name,
   const char * child_point_name,
   const MassPoint * child_point,
   const char * parent_body_name,
   const char * parent_point_name,
   const MassPoint * parent_point)
{

   // Neither point found.
   if ((child_point == nullptr) && (parent_point == nullptr)) {
      if ((child_point_name == nullptr) || (child_point_name[0] == '\0') ||
          (parent_point_name == nullptr) || (parent_point_name[0] == '\0')) {
         MessageHandler::error (
            file, line, MassBodyMessages::invalid_attach,
            "Error: Null/empty value(s) supplied as attachment point names.\n"
            "Attachment of body '%s' to '%s' was not performed.",
            child_body_name, parent_body_name);
      }
      else {
         MessageHandler::error (
            file, line, MassBodyMessages::invalid_attach,
            "Error: Unable to find either specified attachment points.\n"
            "Child point name: '%s'\n"
            "Parent point name: '%s'\n"
            "Attachment of body '%s' to '%s' was not performed.",
            child_point_name, parent_point_name,
            child_body_name, parent_body_name);
      }
   }

   // Child point not found.
   else if (child_point == nullptr) {
      if ((child_point_name == nullptr) || (child_point_name[0] == '\0')) {
         MessageHandler::error (
            file, line, MassBodyMessages::invalid_attach,
            "Error: Null/empty value supplied as the child point name.\n"
            "Attachment of body '%s' to '%s' was not performed.",
            child_body_name, parent_body_name);
      }
      else {
         MessageHandler::error (
            file, line, MassBodyMessages::invalid_attach,
            "Error: Unable to find the child attachment point.\n"
            "Child point name: '%s'\n"
            "Attachment of body '%s' to '%s' was not performed.",
            child_point_name, child_body_name, parent_body_name);
      }
   }

   // Parent point not found.
   else {
      if ((parent_point_name == nullptr) || (parent_point_name[0] == '\0')) {
         MessageHandler::error (
            file, line, MassBodyMessages::invalid_attach,
            "Error: Null/empty value supplied as the parent point name.\n"
            "Attachment of body '%s' to '%s' was not performed.",
            child_body_name, parent_body_name);
      }
      else {
         MessageHandler::error (
            file, line, MassBodyMessages::invalid_attach,
            "Error: Unable to find the parent attachment point.\n"
            "Parent point name: '%s'\n"
            "Attachment of body '%s' to '%s' was not performed.",
            parent_point_name, child_body_name, parent_body_name);
      }
   }
}

} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
