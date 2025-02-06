/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup BodyAction
 * @{
 *
 * @file models/dynamics/body_action/src/body_attach_aligned.cc
 * Define methods for the mass body initialization class.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((body_attach_aligned.cc)
   (body_action.cc)
   (body_action_messages.cc)
   (body_attach.cc)
   (dynamics/mass/src/mass_point_state.cc)
   (dynamics/dyn_manager/src/dyn_manager.cc)
   (dynamics/dyn_body/src/dyn_body_attach.cc)
   (dynamics/mass/src/mass_attach.cc)
   (utils/message/src/message_handler.cc))



*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "dynamics/dyn_manager/include/dyn_manager.hh"
#include "dynamics/mass/include/mass.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/body_action_messages.hh"
#include "../include/body_attach_aligned.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Initialize a MassBodyAttach.
 * \param[in,out] dyn_manager Dynamics manager
 */
void BodyAttachAligned::initialize(DynManager & dyn_manager)
{
    // Forward the request up the chain.
    BodyAttach::initialize(dyn_manager);

    // Sanity check: Names must be supplied.
    if(subject_point_name.empty())
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             BodyActionMessages::invalid_name,
                             "%s failed:\n"
                             "Empty subject_point_name string",
                             action_identifier.c_str());
        return;
    }

    if(parent_point_name.empty())
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             BodyActionMessages::invalid_name,
                             "%s failed:\n"
                             "Empty parent_point_name string",
                             action_identifier.c_str());
        return;
    }
}

/**
 * Initialize the core mass properties of the subject MassBody.
 * \param[in,out] dyn_manager Jeod manager
 */
void BodyAttachAligned::apply(DynManager & dyn_manager)
{
    if(dyn_parent != nullptr)
    {
        if(dyn_subject != nullptr) // DynBody to DynBody
        {
            succeeded = dyn_parent->attach_child(parent_point_name, subject_point_name, *dyn_subject);
        }
        else // MassBody subbody/subassembly to DynBody
        {
            succeeded = dyn_parent->add_mass_body(parent_point_name, subject_point_name, *mass_subject);
        }
    }
    else if(mass_parent != nullptr)
    {
        if(dyn_subject != nullptr) // DynBody to MassBody -- ILLEGAL
        {
            succeeded = false;
        }
        else // MassBody/MassBody assembly to MassBody
        {
            succeeded = mass_subject->attach_to(subject_point_name, parent_point_name, *mass_parent);
        }
    }
    else if(ref_parent != nullptr)
    {
        if(dyn_subject != nullptr) // DynBody to RefFrame
        {
            // Construct the affine transformation from the attach point on the child
            // body to the parent ref frame.
            // Per JEOD convention, this is always defined as a zero offset with a
            // corresponding 180 degree yaw orientation.
            // Thus, the point is initialized with zero offset and identity transform.
            double vec[3], mat[3][3];
            Vector3::initialize(vec);
            Matrix3x3::identity(mat);
            mat[0][0] = -1.0;
            mat[1][1] = -1.0;
            succeeded = dyn_subject->attach_to_frame(subject_point_name, parent_point_name, vec, mat);
        }
        else // MassBody to RefFrame -- ILLEGAL
        {
            succeeded = false;
        }
    }
    else // Should be unreachable
    {
        succeeded = false;
    }

    // Forward the action up the chain. The parent class deals with status.
    BodyAttach::apply(dyn_manager);
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
