/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynBody
 * @{
 *
 * @file models/dynamics/dyn_body/src/dyn_body_attach.cc
 * Define DynBody attachment methods.
 */

/*******************************************************************************

 Purpose:
  ()

Library dependencies:
  ((dyn_body_attach.cc)
   (dyn_body.cc)
   (dyn_body_messages.cc)
   (dynamics/mass/src/mass.cc)
   (utils/ref_frames/src/ref_frame.cc)
   (environment/ephemerides/ephem_interface/src/ephem_ref_frame.cc))



*******************************************************************************/

// System includes
#include <cstddef>
#include <list>
#include <string>

// JEOD includes
#include "dynamics/dyn_manager/include/base_dyn_manager.hh"
#include "dynamics/mass/include/mass.hh"
#include "utils/math/include/matrix3x3.hh"
#include "utils/math/include/vector3.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/ref_frames/include/tree_links_iterator.hh"

// Model includes
#include "../../derived_state/include/relative_derived_state.hh"
#include "../../dyn_manager/include/dynamics_integration_group.hh"
#include "../include/body_ref_frame.hh"
#include "../include/dyn_body.hh"
#include "../include/dyn_body_messages.hh"
#include "environment/ephemerides/ephem_interface/include/ephem_ref_frame.hh"

//! Namespace jeod
namespace jeod
{

// Validate whether the pending attachment is legal from a connectivity
// point of view.
bool DynBody::attach_validate_parent(const DynBody & parent, bool generate_message) const
{
    bool is_valid = true;

    // Sanity check: This body itself must be valid.
    if((dyn_manager == nullptr) || !dyn_manager->is_dyn_body_registered(this))
    {
        is_valid = false;
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             DynBodyMessages::invalid_body,
                             "DynBody '%s' has not been initialized.\n",
                             name.c_str());
    }

    // Perform mass compatibility checks.
    if(get_root_body() == parent.get_root_body())
    {
        is_valid = false;
        if(generate_message)
        {
            MessageHandler::error(__FILE__,
                                  __LINE__,
                                  DynBodyMessages::invalid_attachment,
                                  "Error: Bodies '%s' and '%s' are part of the same mass tree.\n"
                                  "Attachment of body '%s' to '%s' was not performed.",
                                  name.c_str(),
                                  parent.name.c_str(),
                                  name.c_str(),
                                  parent.name.c_str());
        }
    }
    // Uninitialized parent. Denote failure and complain.
    else if(!dyn_manager->is_dyn_body_registered(&parent))
    {
        is_valid = false;
        if(generate_message)
        {
            MessageHandler::warn(__FILE__,
                                 __LINE__,
                                 DynBodyMessages::not_dyn_body,
                                 "Dynamic attachments can only be made to valid DynBodies.\n"
                                 "%s has not been initialized.\n"
                                 "Attachment of body '%s' to '%s' was not performed.",
                                 parent.name.c_str(),
                                 name.c_str(),
                                 parent.name.c_str());
        }
    }

    return is_valid;
}

// Validate whether the pending attachment is legal from a physical
// point of view.
bool DynBody::attach_validate_child(const DynBody & child, bool generate_message) const
{
    const DynBody * root_body = get_root_body();
    bool child_has_state;
    bool root_has_state;
    bool is_valid = true;
    std::string reason;

    // Determine if the child has state (pos/vel/att/rate) information.
    // An uninitialized DynBody child  has no state.
    if(child.initialized_states.is_empty())
    {
        child_has_state = false;
    }
    // A fully-initialized DynBody child has state.
    else if(child.initialized_states.is_full())
    {
        child_has_state = true;
    }
    // A partially initialized DynBody child is messed up.
    else
    {
        is_valid = false;
        child_has_state = false; // Stifle "may be used uninitialized" warning.
        reason = "Child body has an incomplete state";
    }

    // Determine if the root body has state information.
    // Completely uninitialized: No state.
    if(root_body->initialized_states.is_empty())
    {
        root_has_state = false;
    }
    // Fully initialized: Has state.
    else if(root_body->initialized_states.is_full())
    {
        root_has_state = true;
    }
    // Partially initialized: Messed up.
    else if(is_valid)
    {
        is_valid = false;
        reason = "Root body has an incomplete state";
    }
    else
    {
        reason = "Parent and child bodies have incomplete states";
    }

    if(is_valid && child_has_state && (!root_has_state))
    {
        is_valid = false;
        reason = "Parent body has an uninitialized state";
    }

    // Generate a message if things are messed up.
    if((!is_valid) && generate_message)
    {
        MessageHandler::warn(__FILE__,
                             __LINE__,
                             DynBodyMessages::invalid_attachment,
                             "%s\n"
                             "Attachment of body '%s' to '%s' was not performed.",
                             reason.c_str(),
                             child.name.c_str(),
                             name.c_str());
    }

    return is_valid;
}

/**
 * Validate whether the pending sub body is legal from a mass tree
 * point of view
 *
 * @note Assumptions and Limitations@n
 *  - The subject mass, child, must not belong to a child body.
 *
 * @return Validity indicator
 * @param[in] child The child body; the body to be attached to this body.
 * @param[in] generate_message Generate message if invalid?
 */
bool DynBody::add_mass_body_validate(const MassBody & child, bool generate_message) const
{
    bool is_valid = true;

    // MassBody connection methods must not be established for a DynBody
    if(dyn_manager != nullptr && dyn_manager->find_dyn_body(child.name.get_name()) != nullptr)
    {
        is_valid = false;
        if(generate_message)
        {
            MessageHandler::error(__FILE__,
                                  __LINE__,
                                  MassBodyMessages::invalid_attach,
                                  "Error: Body  '%s' is a DynBody.\n"
                                  "MassBody attachment of body '%s' to '%s' was not performed.",
                                  child.name.c_str(),
                                  child.name.c_str(),
                                  name.c_str());
        }
    }
    // MassBody must not be attached to another DynBody in order to become a
    // sub mass to the current dynamic body
    else if(child.get_root_body() != nullptr && dyn_manager != nullptr)
    {
        std::string root_name = child.get_root_body()->name.get_name();
        DynBody * parent_check = dyn_manager->find_dyn_body(root_name);
        if(parent_check != nullptr)
        {
            is_valid = false;
        }
        if(!is_valid && generate_message)
        {
            MessageHandler::error(__FILE__,
                                  __LINE__,
                                  MassBodyMessages::invalid_attach,
                                  "Error: Body  '%s' is already attached.\n"
                                  "MassBody attachment of body '%s' to '%s' was not performed.",
                                  child.name.c_str(),
                                  child.name.c_str(),
                                  name.c_str());
        }
    }

    return is_valid;
}

/**
 * Attach this dyn body's root body as a child of the specified dyn
 * body such that the specified mass points on the two bodies are
 * coincident and the frames associated with those mass points are
 * related by a 180 degree yaw.
 * @return Success indicator: true=success, false=attachment not performed.
 * \param[in] this_point_name The name of a mass point contained in this dyn body's list of mass points.
 * \param[in] parent_point_name The name of a mass point contained in the parent body's list of mass points.
 * \param[in,out] parent The parent body; the body to which this body's root body is to be attached.
 */
bool DynBody::attach_to(const std::string & this_point_name, const std::string & parent_point_name, DynBody & parent)
{
    return parent.attach_child(parent_point_name, this_point_name, *this);
}

/**
 * Attach this dyn body's root body as a child of the specified dyn
 * body such that this body's structural origin is offset from the
 * parent body's structural origin and this body's structural axes
 * are oriented with respect to the parent body's structural axes as
 * specified.
 * @return Success indicator: true=success, false=attachment not performed.
 * \param[in] offset_pstr_cstr_pstr Location of this body's structural origin with respect to the new parent body's
 * structural origin, specified in structural coordinates of the parent body.\n Units: M \param[in] T_pstr_cstr
 * Transformation matrix from the parent body's structural frame to this body's structural frame. \param[in,out] parent
 * The parent body; the body to which this body's root body is to be attached.
 */
bool DynBody::attach_to(const double offset_pstr_cstr_pstr[3], const double T_pstr_cstr[3][3], DynBody & parent)
{
    return parent.attach_child(offset_pstr_cstr_pstr, T_pstr_cstr, *this);
}

bool DynBody::attach_to_frame(const std::string & parent_ref_frame_name)
{
    bool success = false;
    RefFrame * parent_ref_frame = dyn_manager->find_ref_frame(parent_ref_frame_name);
    if(parent_ref_frame == nullptr)
    {
        MessageHandler::warn(__FILE__,
                             __LINE__,
                             DynBodyMessages::invalid_attachment,
                             "Unable to find RefFrame to attach to\n"
                             "Attachment of '%s.structure' to '%s' was not performed.\n",
                             mass.name.c_str(),
                             parent_ref_frame_name.c_str());
    }
    else
    {
        attach_to_frame(*parent_ref_frame);
        success = true;
    }
    return success;
}

bool DynBody::attach_to_frame(RefFrame & parent)
{
    DynBody * root_body = get_root_body_internal();
    RefFrameState curr_rel_state;
    root_body->structure.compute_relative_state(parent, curr_rel_state);
    root_body->frame_attach.initialize_attachment(parent, curr_rel_state);
    return true;
}

bool DynBody::attach_to_frame(const std::string & this_point_name,
                              const std::string & parent_ref_frame_name,
                              const double offset_pframe_cpt_pframe[3],
                              const double T_pframe_cpt[3][3])
{
    bool success = false;
    const BodyRefFrame * this_vehicle_pt = find_vehicle_point(this_point_name);
    const MassPoint * subject_pt;
    RefFrame * parent_ref_frame = dyn_manager->find_ref_frame(parent_ref_frame_name);
    if(this_vehicle_pt == nullptr)
    {
        subject_pt = mass.find_mass_point(this_point_name);
    }
    else
    {
        subject_pt = this_vehicle_pt->mass_point;
    }

    if(subject_pt == nullptr)
    {
        MessageHandler::warn(__FILE__,
                             __LINE__,
                             DynBodyMessages::invalid_attachment,
                             "Unable to find DynBody attachment point\n"
                             "Attachment of '%s' to '%s' was not performed.\n",
                             this_point_name.c_str(),
                             parent_ref_frame_name.c_str());
    }
    else if(parent_ref_frame == nullptr)
    {
        MessageHandler::warn(__FILE__,
                             __LINE__,
                             DynBodyMessages::invalid_attachment,
                             "Unable to find RefFrame to attach to\n"
                             "Attachment of '%s' to '%s' was not performed.\n",
                             this_point_name.c_str(),
                             parent_ref_frame_name.c_str());
    }
    else
    {
        // Perform the final attachment to the root_body only.
        DynBody * root_body = get_root_body_internal();
        RefFrameState X_pframe_to_cpt;
        Vector3::copy(offset_pframe_cpt_pframe, X_pframe_to_cpt.trans.position);
        Matrix3x3::copy(T_pframe_cpt, X_pframe_to_cpt.rot.T_parent_this);
        X_pframe_to_cpt.rot.Q_parent_this.left_quat_from_transformation(T_pframe_cpt);

        MassPointState MP_struct_to_cpt;
        subject_pt->compute_state_wrt_pred(*(root_body->structure.mass_point), MP_struct_to_cpt);

        RefFrameState X_struct_to_cpt;
        Vector3::copy(MP_struct_to_cpt.position, X_struct_to_cpt.trans.position);
        Matrix3x3::copy(MP_struct_to_cpt.T_parent_this, X_struct_to_cpt.rot.T_parent_this);
        X_struct_to_cpt.rot.Q_parent_this = MP_struct_to_cpt.Q_parent_this;

        RefFrameState X_pframe_to_struct(X_pframe_to_cpt);
        X_pframe_to_struct.decr_right(X_struct_to_cpt);

        root_body->frame_attach.initialize_attachment(*parent_ref_frame, X_pframe_to_struct);

        success = true;
    }
    return success;
}

bool DynBody::attach_to_frame(const double offset_pframe_cstr_pframe[3],
                              const double T_pframe_cstr[3][3],
                              RefFrame & parent)
{
    DynBody * root_body = get_root_body_internal();

    RefFrameState offset_init;
    Vector3::copy(offset_pframe_cstr_pframe, offset_init.trans.position);
    Matrix3x3::copy(T_pframe_cstr, offset_init.rot.T_parent_this);

    root_body->frame_attach.initialize_attachment(parent, offset_init);
    return true;
}

/**
 * Attach a child DynBody by point specification. See corresponding
 * DynBody::attach_to() method for more information.
 */
bool DynBody::attach_child(const std::string & this_point_name, const std::string & child_point_name, DynBody & child)
{
    // References looked up from input args
    const BodyRefFrame * parent_vehicle_pt;
    const BodyRefFrame * child_vehicle_pt;
    const MassPoint * parent_point;
    const MassPoint * child_point;

    // status variables
    std::string reason;
    bool success = true;

    // Temporary points used for transformation math
    MassPointState child_struct_wrt_child_point;
    MassPointState child_point_wrt_parent_point;
    MassPointState child_struct_wrt_parent_struct;
    double T_tmp[3][3];
    Matrix3x3::initialize(T_tmp);

    // Validate later when we call the attach by pos/orientatation method. For
    // now, find the mass points corresponding to the input names and verify
    // that they exist.
    parent_vehicle_pt = find_vehicle_point(this_point_name);
    if(parent_vehicle_pt == nullptr)
    {
        parent_point = mass.find_mass_point(this_point_name);
    }
    else
    {
        parent_point = parent_vehicle_pt->mass_point;
    }
    child_vehicle_pt = child.find_vehicle_point(child_point_name);
    if(child_vehicle_pt == nullptr)
    {
        child_point = child.mass.find_mass_point(child_point_name);
    }
    else
    {
        child_point = child_vehicle_pt->mass_point;
    }

    // Handle failed searches as non-fatal errors (warning + failure return).
    if(parent_point == nullptr)
    {
        reason = "Failed to find attach point on parent DynBody";
        success = false;
    }
    if(child_point == nullptr)
    {
        if(success == false) // already found issue with this_point
        {
            reason = "Failed to find attach points on both parent and child";
        }
        else
        {
            reason = "Failed to find attach point on child MassBody";
        }
        success = false;
    }
    if(!success)
    {
        MessageHandler::warn(__FILE__,
                             __LINE__,
                             DynBodyMessages::invalid_attachment,
                             "%s\n"
                             "Attachment of '%s' to '%s' was not performed.\n",
                             reason.c_str(),
                             child_point_name.c_str(),
                             this_point_name.c_str());
        return success;
    }

    // Construct the affine transformation from the mass point location and
    // frame of the attach point on child body to child body's structural
    // origin and frame (i.e.: the inverse of the mass point specification
    // within its own body)
    Vector3::transform(child_point->T_parent_this, child_point->position, child_struct_wrt_child_point.position);
    Vector3::negate(child_struct_wrt_child_point.position);
    Matrix3x3::transpose(child_point->T_parent_this, child_struct_wrt_child_point.T_parent_this);
    child_point->Q_parent_this.conjugate(child_struct_wrt_child_point.Q_parent_this);

    // Construct the affine transformation from the attach point on the child
    // body to the attach point on the parent dynamic body.
    // Per JEOD convention, this is always defined as a zero offset with a
    // corresponding 180 degree yaw orientation.
    // Thus, the point is initialized with zero offset and identity transform.
    child_point_wrt_parent_point.T_parent_this[0][0] = -1.0;
    child_point_wrt_parent_point.T_parent_this[1][1] = -1.0;

    // Calculate the compound affine transformation from the attach point on
    // the parent structure to the child structure
    // 1) create position vector
    // 1a) Create transform for childpt_wrt_childstruct -> parent_struct frame
    Matrix3x3::product_transpose_transpose(parent_point->T_parent_this,
                                           child_point_wrt_parent_point.T_parent_this,
                                           T_tmp);
    // 1b) Transform childpt_wrt_childstruct into parent struct frame
    Vector3::transform(T_tmp, child_struct_wrt_child_point.position, child_struct_wrt_parent_struct.position);
    // 1c) sum position vectors in parent frame
    Vector3::incr(parent_point->position, child_struct_wrt_parent_struct.position);
    // 2) Create transformation
    //     T_pstr_cstr = T_pstr_ppt -> T_ppt_cpt -> T_cpt_cstr
    // 2a) T_pstr_ppr -> T_ppt_cpt
    Matrix3x3::product(child_point_wrt_parent_point.T_parent_this, parent_point->T_parent_this, T_tmp);
    // 2b) -> T_cpt_cstr
    Matrix3x3::product(child_struct_wrt_child_point.T_parent_this, T_tmp, child_struct_wrt_parent_struct.T_parent_this);

    // Attach using geometry-based attachment
    success = attach_child(child_struct_wrt_parent_struct.position,
                           child_struct_wrt_parent_struct.T_parent_this,
                           child);

    return success;
}

/**
 * Attach a child DynBody by location specification. See corresponding
 * DynBody::attach_to() method for more information. Note that the offset and
 * transformation are specified w.r.t. the parent in both attach_to() and
 * attach_child()
 */
bool DynBody::attach_child(const double xyz_cstr_wrt_pstr[3], const double T_pstr_to_cstr[3][3], DynBody & child)
{
    // References looked up
    DynBody * child_root;

    // Status variables
    bool success;

    // Validate
    if(!attach_validate_child(child, true) || !child.attach_validate_parent(*this, true))
    {
        success = false;
        return success;
    }

    child_root = child.get_root_body_internal();

    // If child is not the root DynBody, need further transformations
    // to apply attachments.
    if(child_root == &child)
    {
        child.attach_establish_links(*this);
        attach_update_properties(xyz_cstr_wrt_pstr, T_pstr_to_cstr, child);
        success = true;
    }
    else
    {
        // Final transformation variables
        double xyz_rstr_wrt_pstr[3];
        double T_pstr_to_rstr[3][3];
        MassPointState child_struct_wrt_root_struct;

        // Give a low-level warning that the attaching body is not a root body.
        MessageHandler::inform(__FILE__,
                               __LINE__,
                               MassBodyMessages::attach_info,
                               "DynBody '%s' is itself attached to root body '%s'.\n"
                               "Connecting '%s' to '%s' would violated the existing\n"
                               "attachment and create an invalid tree.\n"
                               "Auto-attaching the child's root body, '%s', to DynBody '%s'",
                               child.name.c_str(),
                               child_root->name.c_str(),
                               child.name.c_str(),
                               name.c_str(),
                               child_root->name.c_str(),
                               name.c_str());

        // Compute the affine transformation from the root structural frame
        // to child body's structural frame.
        child.mass.structure_point.compute_state_wrt_pred(child_root->mass.structure_point,
                                                          child_struct_wrt_root_struct);

        // Recast the attachment transformation / offset in terms of the root.
        Matrix3x3::product_left_transpose(child_struct_wrt_root_struct.T_parent_this, T_pstr_to_cstr, T_pstr_to_rstr);

        Vector3::copy(xyz_cstr_wrt_pstr, xyz_rstr_wrt_pstr);
        Vector3::transform_transpose_decr(T_pstr_to_rstr, child_struct_wrt_root_struct.position, xyz_rstr_wrt_pstr);

        child_root->attach_establish_links(*this);
        attach_update_properties(xyz_rstr_wrt_pstr, T_pstr_to_rstr, *child_root);
        success = true;
    }

    return success;
}

// Attach mass body aligned at specified mass point
bool DynBody::add_mass_body(const std::string & this_point_name, const std::string & child_point_name, MassBody & child)
{
    // Refernces looked up from input args
    const BodyRefFrame * parent_vehicle_pt;
    const MassPoint * parent_point;
    const MassPoint * child_point;

    // status variables
    std::string reason;
    bool success = true;

    // Temporary points used for transformation math
    MassPointState child_struct_wrt_child_point;
    MassPointState child_point_wrt_parent_point;
    MassPointState child_struct_wrt_parent_struct;
    double T_tmp[3][3];
    Matrix3x3::identity(T_tmp);

    // Validate later when we call the attach by pos/orientatation method. For
    // now, find the mass points corresponding to the input names and verify
    // that they exist.
    parent_vehicle_pt = find_vehicle_point(this_point_name);
    if(parent_vehicle_pt == nullptr)
    {
        parent_point = mass.find_mass_point(this_point_name);
    }
    else
    {
        parent_point = parent_vehicle_pt->mass_point;
    }
    child_point = child.find_mass_point(child_point_name);

    // Handle failed searches as non-fatal errors (warning + failure return).
    if(parent_point == nullptr)
    {
        reason = "Failed to find attach point on parent DynBody";
        success = false;
    }
    if(child_point == nullptr)
    {
        if(success == false) // already found issue with this_point
        {
            reason = "Failed to find attach points on both parent and child";
        }
        else
        {
            reason = "Failed to find attach point on child MassBody";
        }
        success = false;
    }
    if(!success)
    {
        MessageHandler::warn(__FILE__,
                             __LINE__,
                             DynBodyMessages::invalid_attachment,
                             "%s\n"
                             "Attachment of '%s' to '%s' was not performed.\n",
                             reason.c_str(),
                             child_point_name.c_str(),
                             this_point_name.c_str());
        return success;
    }

    // Construct the affine transformation from the mass point location and
    // frame of the attach point on child body to child body's structural
    // origin and frame (i.e.: the inverse of the mass point specification
    // within its own body)
    Vector3::transform(child_point->T_parent_this, child_point->position, child_struct_wrt_child_point.position);
    Vector3::negate(child_struct_wrt_child_point.position);
    Matrix3x3::transpose(child_point->T_parent_this, child_struct_wrt_child_point.T_parent_this);
    child_point->Q_parent_this.conjugate(child_struct_wrt_child_point.Q_parent_this);

    // Construct the affine transformation from the attach point on the child
    // body to the attach point on the parent dynamic body.
    // Per JEOD convention, this is always defined as a zero offset with a
    // corresponding 180 degree yaw orientation.
    // Thus, the point is initialized with zero offset and identity transform.
    child_point_wrt_parent_point.T_parent_this[0][0] = -1.0;
    child_point_wrt_parent_point.T_parent_this[1][1] = -1.0;

    // Calculate the compound affine transformation from the attach point on
    // the parent structure to the child structure
    // 1) create position vector
    // 1a) Create transform for childpt_wrt_childstruct -> parent_struct frame
    Matrix3x3::product_transpose_transpose(parent_point->T_parent_this,
                                           child_point_wrt_parent_point.T_parent_this,
                                           T_tmp);
    // 1b) Transform childpt_wrt_childstruct into parent struct frame
    Vector3::transform(T_tmp, child_struct_wrt_child_point.position, child_struct_wrt_parent_struct.position);
    // 1c) sum position vectors in parent frame
    Vector3::incr(parent_point->position, child_struct_wrt_parent_struct.position);
    // 2) Create transformation
    //     T_pstr_cstr = T_pstr_ppt -> T_ppt_cpt -> T_cpt_cstr
    // 2a) T_pstr_ppr -> T_ppt_cpt
    Matrix3x3::product(child_point_wrt_parent_point.T_parent_this, parent_point->T_parent_this, T_tmp);
    // 2b) -> T_cpt_cstr
    Matrix3x3::product(child_struct_wrt_child_point.T_parent_this, T_tmp, child_struct_wrt_parent_struct.T_parent_this);

    // Attach using geometry-based attachment
    success = add_mass_body(child_struct_wrt_parent_struct.position,
                            child_struct_wrt_parent_struct.T_parent_this,
                            child);

    return success;
}

// Attach mass body aligned at specified mass point
bool DynBody::add_mass_body(const double xyz_cstr_wrt_pstr[3], const double T_pstr_to_cstr[3][3], MassBody & child)
{
    // References looked up
    MassBody * child_root;

    // Status variables
    bool success;

    // Validate
    if(!add_mass_body_validate(child, true))
    {
        success = false;
        return success;
    }

    child_root = child.get_root_body_internal();

    // If child is not the root MassBody, need further transformations
    // to apply attachments.
    if(child_root == &child)
    {
        child.attach_establish_links(mass);
        mass.attach_update_properties(xyz_cstr_wrt_pstr, T_pstr_to_cstr, child);
    }
    else
    {
        // Final transformation variables
        double xyz_rstr_wrt_pstr[3];
        double T_pstr_to_rstr[3][3];
        MassPointState child_struct_wrt_root_struct;

        // Give a low-level warning that the attaching body is not a root body.
        MessageHandler::inform(__FILE__,
                               __LINE__,
                               MassBodyMessages::attach_info,
                               "MassBody '%s' is itself attached to MassBody '%s'. Adding\n"
                               "'%s' as a subbody to '%s' would violated the existing\n"
                               "attachment and create an invalid tree.\n"
                               "Auto-attaching the root MassBody, '%s', to DynBody '%s'.",
                               child.name.c_str(),
                               child.links.parent()->name.c_str(),
                               child.name.c_str(),
                               name.c_str(),
                               child_root->name.c_str(),
                               name.c_str());

        // Compute the affine transformation from the root structural frame
        // to child body's structural frame.
        child.structure_point.compute_state_wrt_pred(child_root->structure_point, child_struct_wrt_root_struct);

        // Recast the attachment transformation / offset in terms of the root.
        Matrix3x3::product_left_transpose(child_struct_wrt_root_struct.T_parent_this, T_pstr_to_cstr, T_pstr_to_rstr);

        Vector3::copy(xyz_cstr_wrt_pstr, xyz_rstr_wrt_pstr);
        Vector3::transform_transpose_decr(T_pstr_to_rstr, child_struct_wrt_root_struct.position, xyz_rstr_wrt_pstr);

        child_root->attach_establish_links(mass);

        mass.attach_update_properties(xyz_rstr_wrt_pstr, T_pstr_to_rstr, *child_root);
    }

    // Create BodyFrames for each subbody mass point
    add_mass_body_frames(*child_root);

    mass_children.push_back(child_root);

    success = true;
    return success;
}

// Create BodyRefFrame for each MassBody point and add it to vehicle_points
void DynBody::add_mass_body_frames(MassBody & subbody)
{
    // Add points from current subbody
    for(auto & mass_point : subbody.mass_points)
    {
        BodyRefFrame * pt_frame = JEOD_ALLOC_CLASS_OBJECT(BodyRefFrame, ());
        pt_frame->set_name(name.get_name(), mass_point->get_name());
        pt_frame->mass_point = mass_point;
        pt_frame->set_owner(this);
        if(integ_frame != nullptr)
        {
            integ_frame->add_child(*pt_frame);
        }
        vehicle_points.push_back(pt_frame);
        dyn_manager->add_ref_frame(*pt_frame);
    }

    // Call for each of subbody's children
    for(auto * child_link : TreeLinksChildrenRange<MassBodyLinks>(subbody.links))
    {
        add_mass_body_frames(child_link->container());
    }
}

// Establish the logical connectivity between parent and child.
void DynBody::attach_establish_links(DynBody & parent)
{
    // Establish base linkage.
    mass.attach_establish_links(parent.mass);

    // Cast the parent to a DynBody, saving in the dyn_parent instance var.
    // Note well: This cast is assumed to work.
    dyn_parent = &parent;

    // Add "this" to the parent's list of dynamic child bodies.  This list is
    // used when the state is propagated through the tree; the tree identifies
    // MassBodies, not all of whic
    dyn_parent->dyn_children.push_back(this);

    // Make this body's integration frame the same as the parent's.
    if(integ_frame != dyn_parent->get_integ_frame())
    {
        set_integ_frame(*(dyn_parent->get_integ_frame()));
    }
}

// Set the relation between parent and child and update the mass
// properties.
void DynBody::attach_update_properties(const double offset_pstr_cstr_pstr[3],
                                       const double T_pstr_cstr[3][3],
                                       DynBody & child)
{
    DynBody * root_body = get_root_body_internal();
    bool child_has_state;
    bool root_has_state;

    // Determine if the child has state (pos/vel/att/rate) information.
    if(child.initialized_states.is_empty())
    {
        child_has_state = false;
    }
    else
    {
        child_has_state = true;
    }

    // Determine if the root body has state information.
    if(root_body->initialized_states.is_empty())
    {
        root_has_state = false;
    }
    else
    {
        root_has_state = true;
    }

    // There are three cases to handle:
    // - Simple attachment:
    //   -- A DynBody with an uninitialized state attaches
    //      to a parent DynBody with an uninitialized state.
    //   -- Since the state is uninitialized there is no state to update.
    // - Attachment with simple update
    //   -- A DynBody with an uninitialized state attaches
    //      to a parent DynBody with a fully initialized state.
    //   -- The parent state is updated per assumption that the parent structure
    //      state is unchanged by the attachment.
    // - Dynamic attachment
    //   -- A DynBody with a fully initialized state attaches to a parent
    //      DynBody with a fully initialized state.
    //   -- State is updated to conserve momenta.

    // Simple attachment:
    // No state yet, so just update mass properties.
    if((!child_has_state) && (!root_has_state))
    {
        mass.attach_update_properties(offset_pstr_cstr_pstr, T_pstr_cstr, child.mass);
    }

    // Attachment, simple update:
    // Update properties and set source from structure.
    // Even this simple attachment may cause a jolt to state integration,
    // so reset integrators for this body.
    else if((!child_has_state) && root_has_state)
    {
        mass.attach_update_properties(offset_pstr_cstr_pstr, T_pstr_cstr, child.mass);

        root_body->set_state_source_internal(RefFrameItems::Pos_Vel_Att_Rate, root_body->structure);
        root_body->propagate_state();

        get_dynamics_integration_group()->reset_integrators();
    }

    // Dynamic attachment:
    // Perform attachment, conserving momenta.
    // This will definitely jolt state integration, so reset integrators for
    // this body.
    else
    {
        process_dynamic_attachment(offset_pstr_cstr_pstr, T_pstr_cstr, *root_body, child);

        get_dynamics_integration_group()->reset_integrators();
    }
}

// Process the attachment event of one body from another.
void DynBody::process_dynamic_attachment(const double offset_pstr_cstr_pstr[3],
                                         const double T_pstr_cstr[3][3],
                                         DynBody & root_body,
                                         DynBody & child_body)
{
    // Definitions:
    //   child -      The DynBody that is being attached to this body.
    //   parent -     The body to which the child is being attached; this body.
    //   root -       The DynBody at the head of this body's attachment tree.

    // Mass
    double mass_parent; /* kg
       mass of the parent-composite. */

    // Positions
    double orig_cm[3];                   /* M
                         The location of the root body's composite center of mass prior to
                         attachment, expressed in root structural coordinates. */
    double new_cm[3];                    /* M
                         The location of the root body's composite center of mass after
                         attachment, expressed in root structural coordinates. */
    double cm_delta[3];                  /* M
                        The change in the center of mass, new_cm - orig_cm. */
    double cm_delta_inrtl[3];            /* M
                  cm_delta expressed in inertial coordinates. */
    double child_pos_rel_newcm_inrtl[3]; /* M
       The position of the child body's composite center of mass with respect to
       the post-attachment root body center of mass, expressed in inertial. */

    // Velocities
    double parent_vel_inrtl[3];    /* M/s
          The inertial velocity of the root body's composite CoM prior to attach. */
    double child_vel_inrtl[3];     /* M/s
           The inertial velocity of the child body's composite CoM. */
    double child_vel_relative[3];  /* M/s
        The child body's velocity relative to the combined CoM. */
    double parent_vel_relative[3]; /* M/s
       The root body's velocity relative to the combined CoM. */

    // Linear momenta
    double linear_momentum_child_composite[3];  /*  N*s
        The linear momentum of child-composite, expressed in inertial axes,
        measured w.r.t. the integration frame origin. */
    double child_lin_mom_relative[3];           /* N*s
                 The linear momentum of child-composite, expressed in inertial axes,
                 measured w.r.t. the center of mass of the combined body. */
    double linear_momentum_parent_composite[3]; /*  N*s
       The linear momentum of parent-composite, expressed in inertial axes,
       measured w.r.t. the integration frame origin. */
    double parent_lin_mom_relative[3];          /*  N*s
                The linear momentum of parent-composite, expressed in inertial axes,
                measured w.r.t. the center of mass of the combined body. */
    double linear_momentum[3];                  /* N*s
                        The linear momentum of combined-body.  Expressed in the
                        inertial axes w.r.t. the integration frame origin. */

    // Angular momenta
    double parent_ang_mom_pbdy[3]; /* J*s
       Angular momentum of parent-composite.  Expressed in the body axes of
       parent-composite, and measured with respect to the momentary (and
       inertial) location of the center of mass of that same composite body
       (so includes only rotation terms, with no angular momentum due to
       linear motion on a moment arm). */
    double child_ang_mom_cbdy[3];  /* J*s
        Angular momentum of child-composite.  Expressed in the body axes of
        child-composite, and measured with respect to the momentary (and
        inertial) location of the center of mass of that same composite body
        (so includes only rotation terms, with no angular momentum due to
        linear motion on a moment arm). */
    double child_ang_mom_inrtl[3]; /* J*s
       The transform of child_ang_mom_cbdy so that it is expressed in inertial
       axes. */
    double child_ang_mom_vel[3];   /* J*s
         The angular momentum of child-composite due to the linear motion of
         same body past the center of mass of combined-body.  Measured w.r.t the
         momentary (and inertial) location of the center of mass of combined-body,
         expresssed in inertial axes.*/
    double parent_ang_mom_vel[3];  /* J*s
        The angular momentum of parent-composite due to the linear motion of
        same body past the center of mass of combined-body.  Measured w.r.t the
        momentary (and inertial) location of the center of mass of combined-body,
        expresssed in inertial axes.*/
    double angular_momentum[3];    /* J*s
          Multiple angular momentum representations in different frames.  Ultimately
          becomes the angular momentum of combined-body, w.r.t. inertial axes and
          its own center of mass, expressed in the body axes of combined-body.*/

    // Get pointers to the root bodies corresponding to the parent and
    // child bodies.

    // Compute the linear momentum for these bodies (individually & combined).
    Vector3::scale(child_body.composite_body.state.trans.velocity,
                   child_body.mass.composite_properties.mass,
                   linear_momentum_child_composite);
    Vector3::scale(root_body.composite_body.state.trans.velocity,
                   root_body.mass.composite_properties.mass,
                   linear_momentum_parent_composite);
    Vector3::sum(linear_momentum_child_composite, linear_momentum_parent_composite, linear_momentum);

    // Compute the angular momentum of the parent composite body.
    // NOTE: This is about the parent's composite body CM prior to attachment,
    // expressed in parent composite body coordinates.
    Vector3::transform(root_body.mass.composite_properties.inertia,
                       root_body.composite_body.state.rot.ang_vel_this,
                       parent_ang_mom_pbdy);

    // Compute the angular momentum of the child composite body.
    // NOTE: This is about the child's composite body CM prior to attachment,
    // expressed in child composite body coordinates.
    Vector3::transform(child_body.mass.composite_properties.inertia,
                       child_body.composite_body.state.rot.ang_vel_this,
                       child_ang_mom_cbdy);

    // Store the parent's and child's linear velocities and parent mass.
    Vector3::copy(root_body.composite_body.state.trans.velocity, parent_vel_inrtl);
    Vector3::copy(child_body.composite_body.state.trans.velocity, child_vel_inrtl);
    mass_parent = root_body.mass.composite_properties.mass;

    // Store the original CM of the parent's root body (in parent's root
    // structural frame).
    Vector3::copy(root_body.mass.composite_properties.position, orig_cm);

    // Attach the bodies as MassBodies.
    mass.attach_update_properties(offset_pstr_cstr_pstr, T_pstr_cstr, child_body.mass);

    // Store the CM of the new root body
    // (i.e. the combined-body body-frame origin w.r.t., and
    // expressed in, its structural-frame).
    Vector3::copy(root_body.mass.composite_properties.position, new_cm);

    // Compute the change in CM. */
    Vector3::diff(new_cm, orig_cm, cm_delta);

    // Update the translational position of the new root body.
    // NOTE: The parent reference frame of a root body is inertial, so
    // T_parent_this is inertial to structural, we need structural to inertial)
    Vector3::transform_transpose(root_body.structure.state.rot.T_parent_this, cm_delta, cm_delta_inrtl);

    Vector3::incr(cm_delta_inrtl, root_body.composite_body.state.trans.position);

    // Compute linear velocity from conservation of linear momentum.
    // NOTE: These computations must be in the inertial reference frame.
    // Total linear momentum was already calculated before the attachment
    Vector3::scale(linear_momentum,
                   root_body.mass.composite_properties.inverse_mass,
                   root_body.composite_body.state.trans.velocity);

    // Compute angular velocity from conservation of angular momentum.
    // NOTE: subscript c = child
    //                 p = parent
    //                 t = total (combined)
    // L = (r_p x (m_p v_p)) + I_p w_p + (r_c x (m_c v_c)) + I_c w_c
    //   = (r_t x (m_t v_t)) + I_t w_t
    //
    // Shift to a reference frame in which v_t = 0 and r_t = 0 (due to
    // "magical" migration of child, this is a non-physical problem, and
    // angular momentum cannot be conserved in all frames simultaneously, this
    // is the best choice).
    //
    // L = (r_p - r_t) x (m_p (v_p - v_t)) + I_p w_p +
    //     (r_c - r_t) x (m_c (v_c - v_t)) + I_c w_c
    //   = I_t w_t
    //
    // hence,
    // w_t = inv(I_t) *
    //    ((r_p-r_t) x (m_p (v_p - v_t)) +
    //     (r_c-r_t) x (m_c (v_c - v_t)) + I_c w_c + I_p w_p)
    // r_t and v_t already calculated

    // Compute the relative position of the child's (pre-attachment) root
    // composite body with respect to the new center of mass.
    // NOTE:  Wanting the position of child prior to the "snap" w.r.t updated
    // position of parent c.o.m..
    // Child position has not been updated yet, but parent has, and it reflects
    // the new composite CoM.
    Vector3::diff(child_body.composite_body.state.trans.position,
                  root_body.composite_body.state.trans.position,
                  child_pos_rel_newcm_inrtl);

    // Compute the relative momentum of the child and (pre-attachment) parent
    // with respect to the new center of mass.
    Vector3::diff(child_vel_inrtl, root_body.composite_body.state.trans.velocity, child_vel_relative);

    Vector3::scale(child_vel_relative, child_body.mass.composite_properties.mass, child_lin_mom_relative);

    Vector3::diff(parent_vel_inrtl, root_body.composite_body.state.trans.velocity, parent_vel_relative);

    Vector3::scale(parent_vel_relative, mass_parent, parent_lin_mom_relative);

    // Compute the angular momentum components due to relative velocities.

    // Compute the contribution made by the child's relative velocity to the
    // overall angular momentum.  L = r x p
    Vector3::cross(child_pos_rel_newcm_inrtl, child_lin_mom_relative, child_ang_mom_vel);

    // Compute the contribution made by the (unattached) parent's relative
    // velocity to the overall angular momentum.
    //  Use L = p x (-r) since -r is already known.
    Vector3::cross(parent_lin_mom_relative, cm_delta_inrtl, parent_ang_mom_vel);

    // Transform the child's inherent angular momentum (i.e. I*w) to the
    // inertial frame so that it can be added.
    // NOTE: Parent's angular momentum stays in parent body frm for later

    Vector3::transform_transpose(child_body.composite_body.state.rot.T_parent_this,
                                 child_ang_mom_cbdy,
                                 child_ang_mom_inrtl);

    // Add together the 3 components calculated so far:
    //   angular momentum due to child's rotation
    //   angular momentum due to child's relative motion
    //   angular momentum due to parent's relative motion

    Vector3::sum(child_ang_mom_vel, parent_ang_mom_vel, child_ang_mom_inrtl, angular_momentum);

    // Convert back to the body frame of the parent (now the composite)
    // NOTE:  the new parent body frame is colinear with the original body frame
    // (just offset and moving), so T_inertial_body is unchanged.
    // NOTE: angular_momentum is in the body frame after this transformation.
    Vector3::transform(root_body.composite_body.state.rot.T_parent_this, angular_momentum);

    //  now add in the parent's Iw term for total angular momentum
    // NOTE: parent_ang_mom_pbdy is already in the new parent body frame.
    Vector3::incr(parent_ang_mom_pbdy, angular_momentum);

    // Compute the body rate from the angular momentum (inverse inertia already
    // calculated as part of mass_update)
    Vector3::transform(root_body.mass.composite_properties.inverse_inertia,
                       angular_momentum,
                       root_body.composite_body.state.rot.ang_vel_this);

    // Propagate state down through the tree:
    // Tell the function where to obtain the data from.
    // Position and attitude from the core body,
    // Velocity and rate from the composite (values computed above).
    root_body.set_state_source_internal(RefFrameItems::Pos_Att, root_body.core_body);
    root_body.set_state_source_internal(RefFrameItems::Vel_Rate, root_body.composite_body);

    // Finally, propagate the state.
    root_body.propagate_state();
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
