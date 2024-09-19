/**
 * @addtogroup Models
 * @{
 * @addtogroup Dynamics
 * @{
 * @addtogroup DynBody
 * @{
 *
 * @file models/dynamics/dyn_body/src/dyn_body_find_body_frame.cc
 * Define DynBody::find_body_frame.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((dyn_body_find_body_frame.cc)
   (dyn_body.cc)
   (dyn_body_messages.cc)
   (dynamics/mass/src/mass.cc)
   (utils/named_item/src/named_item.cc)
   (utils/ref_frames/src/ref_frame.cc))



*******************************************************************************/

// System includes
#include <cstddef>

// JEOD includes
#include "dynamics/dyn_manager/include/base_dyn_manager.hh"
#include "utils/message/include/message_handler.hh"
#include "utils/named_item/include/named_item.hh"

// Model includes
#include "../include/dyn_body.hh"
#include "../include/dyn_body_messages.hh"

//! Namespace jeod
namespace jeod
{

// Find the BodyRefFrame named by the provided identifier.
BodyRefFrame * DynBody::find_body_frame(const std::string & frame_id) const
{
    RefFrame * found_frame;          // -- Reference frame that matches name
    BodyRefFrame * found_body_frame; // -- found_frame cast to BodyRefFrame *

    // Sanity check: Ensure the input name is meaningful.
    if(frame_id.empty())
    {
        MessageHandler::fail(__FILE__,
                             __LINE__,
                             DynBodyMessages::invalid_name,
                             "Invalid frame_id passed to DynBody::find_body_frame.");

        // Not reached
        return nullptr;
    }

    // Find the reference frame.
    found_frame = dyn_manager->find_ref_frame(name.get_name(), name.suffix(frame_id));

    // Cast to a BodyRefFrame if found.
    if(found_frame != nullptr)
    {
        found_body_frame = dynamic_cast<BodyRefFrame *>(found_frame);

        // Sanity check: cast failure is a fatal error.
        if(found_body_frame == nullptr)
        {
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 DynBodyMessages::invalid_name,
                                 "Reference frame '%s' is not a BodyRefFrame.",
                                 found_frame->get_name().c_str());

            // Not reached
            return nullptr;
        }
    }

    // No RefFrame found => no BodyRefFrame.
    else
    {
        found_body_frame = nullptr;
    }

    return found_body_frame;
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
