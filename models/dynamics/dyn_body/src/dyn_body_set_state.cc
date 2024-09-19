/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynBody
 * @{
 *
 * @file models/dynamics/dyn_body/src/dyn_body_set_state.cc
 * Define methods related to setting aspects of a vehicle's state.
 */

/*******************************************************************************

Purpose:
  ()

Reference:
  (((TBS)))

Assumptions and limitations:
  ((TBS))

Library dependencies:
  ((dyn_body_set_state.cc)
   (dyn_body.cc)
   (dyn_body_messages.cc)
   (dynamics/mass/src/mass_point_state.cc)
   (utils/ref_frames/src/ref_frame.cc))



*******************************************************************************/

// System includes
#include <cstddef>

// Jeod includes
#include "utils/math/include/vector3.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/ref_frames/include/ref_frame_items.hh"

// Model includes
#include "../include/dyn_body.hh"
#include "../include/dyn_body_messages.hh"

// Internal functions

//! Namespace jeod
namespace jeod
{

/**
 * Check that the dyn_body 'owns' the subject frame.
 *
 * \param[in] frame Frame to test
 * \param[in] dyn_body Typically this
 * \param[in] file Typically __FILE__
 * \param[in] line Typically __LINE__
 */
static inline void check_frame_ownership(const BodyRefFrame & frame,
                                         const DynBody * dyn_body,
                                         const char * file,
                                         unsigned int line)
{
    if(frame.get_owner() != dyn_body)
    {
        MessageHandler::fail(file,
                             line,
                             DynBodyMessages::invalid_frame,
                             "Body '%s' is not the owner of reference frame '%s'.",
                             dyn_body->name.c_str(),
                             frame.get_name().c_str());
    }
}

// Set the parts of the specified reference frame as indicated by set_items.
void DynBody::set_state(RefFrameItems::Items set_items, const RefFrameState & state, BodyRefFrame & subject_frame)
{
    DynBody * root_body = get_root_body_internal();

    // The subject frame must be owned by this dynamic body.
    check_frame_ownership(subject_frame, this, __FILE__, __LINE__);

    // Copy the state items specified by set_items to the reference frame
    // specified by frame_id.

    // Position:
    // Copy the position vector.
    if((set_items & RefFrameItems::Pos) != 0)
    {
        root_body->set_state_source_internal(RefFrameItems::Pos, subject_frame);
        Vector3::copy(state.trans.position, subject_frame.state.trans.position);
    }

    // Velocity:
    // Copy the velocity vector.
    if((set_items & RefFrameItems::Vel) != 0)
    {
        root_body->set_state_source_internal(RefFrameItems::Vel, subject_frame);
        Vector3::copy(state.trans.velocity, subject_frame.state.trans.velocity);
    }

    // Attitude:
    // Copy the attitude quaternion and compute the transformation matrix.
    if((set_items & RefFrameItems::Att) != 0)
    {
        root_body->set_state_source_internal(RefFrameItems::Att, subject_frame);
        subject_frame.state.rot.Q_parent_this = state.rot.Q_parent_this;
        state.rot.Q_parent_this.left_quat_to_transformation(subject_frame.state.rot.T_parent_this);
    }

    // Attitude rate:
    // Copy the angular velocity vector and compute derived items.
    if((set_items & RefFrameItems::Rate) != 0)
    {
        root_body->set_state_source_internal(RefFrameItems::Rate, subject_frame);
        Vector3::copy(state.rot.ang_vel_this, subject_frame.state.rot.ang_vel_this);
        subject_frame.state.rot.compute_ang_vel_products();
    }
}

// Set the source of aspects of the state.
void DynBody::set_state_source(RefFrameItems::Items items, BodyRefFrame & frame)
{
    DynBody * root_body;         // This body's root body
    const DynBody * source_body; // The body that owns the source frame

    // Handle errors.

    // 1. The supplied frame must be owned by a DynBody.
    source_body = dynamic_cast<const DynBody *>(frame.get_owner());
    if(source_body == nullptr)
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             DynBodyMessages::invalid_frame,
                             "Reference frame '%s' is not a valid body reference frame.",
                             frame.get_name().c_str());

        // Not reached
        return;
    }

    // 2. Minimally, the source body must be in the same tree as is this body.
    //    Because this is a public method, a more restrictive check is made:
    //    The source body or its root body must be this body.
    //    In other words, the method can be invoked in one of two ways:
    //    - Sending the message to the owner of the specified frame, or
    //    - Sending the message to the root body.
    if((source_body != this) && (source_body->get_root_body() != this))
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             DynBodyMessages::invalid_frame,
                             "Reference frame '%s', is owned by body '%s', "
                             "which is not a child body of '%s'.",
                             frame.get_name().c_str(),
                             source_body->name.c_str(),
                             name.c_str());

        // Not reached
        return;
    }

    // Use the internal equivalent to do the bulk of the work.
    if(dyn_parent == nullptr)
    {
        root_body = this;
    }
    else
    {
        root_body = get_root_body_internal();
    }
    root_body->set_state_source_internal(items, frame);
}

// Set the position of the vehicle.
void DynBody::set_position(const double position[3], BodyRefFrame & subject_frame)
{
    DynBody * root_body = get_root_body_internal();

    // The subject frame must be owned by this dynamic body.
    check_frame_ownership(subject_frame, this, __FILE__, __LINE__);

    // Set the position source frame and set the position of that frame.
    root_body->set_state_source_internal(RefFrameItems::Pos, subject_frame);
    Vector3::copy(position, subject_frame.state.trans.position);
}

// Set the velocity of the vehicle.
void DynBody::set_velocity(const double velocity[3], BodyRefFrame & subject_frame)
{
    DynBody * root_body = get_root_body_internal();

    // The subject frame must be owned by this dynamic body.
    check_frame_ownership(subject_frame, this, __FILE__, __LINE__);

    // Set the velocity source frame and set the velocity of that frame.
    root_body->set_state_source_internal(RefFrameItems::Vel, subject_frame);
    Vector3::copy(velocity, subject_frame.state.trans.velocity);
}

// Set the attitude of the vehicle.
void DynBody::set_attitude_left_quaternion(const Quaternion & left_quat, BodyRefFrame & subject_frame)
{
    DynBody * root_body = get_root_body_internal();

    // The subject frame must be owned by this dynamic body.
    check_frame_ownership(subject_frame, this, __FILE__, __LINE__);

    // Set the attitude source frame and set the attitude of that frame.
    root_body->set_state_source_internal(RefFrameItems::Att, subject_frame);
    subject_frame.state.rot.Q_parent_this = left_quat;
    subject_frame.state.rot.Q_parent_this.left_quat_to_transformation(subject_frame.state.rot.T_parent_this);
}

// Set the attitude of the vehicle.
void DynBody::set_attitude_right_quaternion(const Quaternion & right_quat, BodyRefFrame & subject_frame)
{
    DynBody * root_body = get_root_body_internal();

    // The subject frame must be owned by this dynamic body.
    check_frame_ownership(subject_frame, this, __FILE__, __LINE__);

    // Set the attitude source frame and set the attitude of that frame.
    root_body->set_state_source_internal(RefFrameItems::Att, subject_frame);
    right_quat.conjugate(subject_frame.state.rot.Q_parent_this);
    subject_frame.state.rot.Q_parent_this.left_quat_to_transformation(subject_frame.state.rot.T_parent_this);
}

// Set the attitude of the vehicle.
void DynBody::set_attitude_matrix(const double matrix[3][3], BodyRefFrame & subject_frame)
{
    DynBody * root_body = get_root_body_internal();

    // The subject frame must be owned by this dynamic body.
    check_frame_ownership(subject_frame, this, __FILE__, __LINE__);

    // Set the attitude source frame and set the attitude of that frame.
    root_body->set_state_source_internal(RefFrameItems::Att, subject_frame);
    Matrix3x3::copy(matrix, subject_frame.state.rot.T_parent_this);
    subject_frame.state.rot.Q_parent_this.left_quat_from_transformation(subject_frame.state.rot.T_parent_this);
}

// Set the attitude rate of the vehicle.
void DynBody::set_attitude_rate(const double attitude_rate[3], BodyRefFrame & subject_frame)
{
    DynBody * root_body = get_root_body_internal();

    // The subject frame must be owned by this dynamic body.
    check_frame_ownership(subject_frame, this, __FILE__, __LINE__);

    // Set the rate source frame and set the rate of that frame.
    root_body->set_state_source_internal(RefFrameItems::Rate, subject_frame);
    Vector3::copy(attitude_rate, subject_frame.state.rot.ang_vel_this);
    subject_frame.state.rot.compute_ang_vel_products();
}

// Set the source of aspects of the state.
void DynBody::set_state_source_internal(RefFrameItems::Items items, BodyRefFrame & frame)
{
    // For each of the state elements specified by the input 'items',
    // Denote that the state element in question is not initialized and
    // set the source of that state element to the supplied frame.

    // Position.
    if((items & RefFrameItems::Pos) == RefFrameItems::Pos)
    {
        initialized_states.remove(RefFrameItems::Pos);
        position_source = &frame;
    }

    // Velocity.
    if((items & RefFrameItems::Vel) == RefFrameItems::Vel)
    {
        initialized_states.remove(RefFrameItems::Vel);
        velocity_source = &frame;
    }

    // Attitude.
    if((items & RefFrameItems::Att) == RefFrameItems::Att)
    {
        initialized_states.remove(RefFrameItems::Att);
        attitude_source = &frame;
    }

    // Attitude rate.
    if((items & RefFrameItems::Rate) == RefFrameItems::Rate)
    {
        initialized_states.remove(RefFrameItems::Rate);
        rate_source = &frame;
    }

    // Denote that the specified items are set in the specified frame.
    frame.initialized_items.add(items);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
