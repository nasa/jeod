/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup RefFrames
 * @{
 *
 * @file models/utils/ref_frames/src/ref_frame_manager.cc
 * Define RefFrameManager methods.
 */

/*****************************************************************************
Purpose:
  ()

Library dependencies:
  ((ref_frame_manager.cc)
   (ref_frame.cc))


******************************************************************************/

// System includes
#include <algorithm> // std::find
#include <cstddef>

// JEOD includes
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/ref_frame.hh"
#include "../include/ref_frame_manager.hh"
#include "../include/ref_frame_messages.hh"

//! Namespace jeod
namespace jeod
{

/**
 * RefFrameManager default constructor.
 */
RefFrameManager::RefFrameManager()
    : BaseRefFrameManager()
{
    JEOD_REGISTER_CLASS(RefFrameManager);
    JEOD_REGISTER_CLASS(RefFrame);
    JEOD_REGISTER_CHECKPOINTABLE(this, ref_frames);
}

/**
 * RefFrameManager destructor.
 */
RefFrameManager::~RefFrameManager()
{
    JEOD_DEREGISTER_CHECKPOINTABLE(this, ref_frames);
}

/*******************************************************************************
Reference frame methods
*******************************************************************************/

/**
 * Add a reference frame to the reference frame registry.
 * @param ref_frame  Reference frame to be added.
 */
void RefFrameManager::add_ref_frame(RefFrame & ref_frame)
{
    // Handle errors.
    // 1. The frame must have a valid name.
    if(!validate_name(__FILE__, __LINE__, ref_frame.get_name(), "Argument", "ref_frame.name"))
    {
        return;
    }

    // 2. The frame must not have been previously registered.
    for(std::vector<RefFrame *>::const_iterator it = ref_frames.begin(); it != ref_frames.end(); ++it)
    {
        if(*it == &ref_frame)
        {
            MessageHandler::error(__FILE__,
                                  __LINE__,
                                  RefFrameMessages::duplicate_entry,
                                  "Reference frame '%s' was previously registered.",
                                  ref_frame.get_name().c_str());
            return;
        }
    }

    // 3. The frame must have a unique name.
    if(find_ref_frame(ref_frame.get_name()) != nullptr)
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              RefFrameMessages::duplicate_entry,
                              "Reference frame with name '%s' was previously registered.",
                              ref_frame.get_name().c_str());
        return;
    }

    // Add the reference frame to the reference frame table.
    ref_frames.push_back(&ref_frame);
}

/**
 * Remove a reference frame from the reference frame registry.
 * @param ref_frame  Reference frame to be removed.
 */
void RefFrameManager::remove_ref_frame(RefFrame & ref_frame)
{
    auto it = std::find(ref_frames.begin(), ref_frames.end(), &ref_frame);

    if(it == ref_frames.end())
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              RefFrameMessages::removal_failed,
                              "Reference frame %s was not found in the registry",
                              ref_frame.get_name().c_str());
        return;
    }

    ref_frames.erase(it);
}

/**
 * Find the reference frame with the given name.
 * @param name  Reference frame name
 * @return Found reference frame, or NULL if not found
 */
RefFrame * RefFrameManager::find_ref_frame(const std::string & name) const
{
    RefFrame * found_frame = nullptr;

    // Search the reference frame list until a match is found.
    for(auto ref_frame : ref_frames)
    {
        if(name == ref_frame->get_name())
        {
            found_frame = ref_frame;
            break;
        }
    }

    // Return the found frame (if any).
    return found_frame;
}

/**
 * Find the reference frame with the dot-conjoined name "${prefix}.${suffix}".
 * @param prefix  Reference frame name prefix
 * @param suffix  Reference frame name suffix
 * @return Found reference frame, or NULL if not found
 */
RefFrame * RefFrameManager::find_ref_frame(const std::string & prefix, const std::string & suffix) const
{
    RefFrame * found_frame = nullptr;

    // Search the reference frame list until a match is found.
    for(auto ref_frame : ref_frames)
    {
        std::string ref_frame_name = ref_frame->get_name();
        if((ref_frame_name.compare(0, prefix.length(), prefix) == 0) && (ref_frame_name[prefix.length()] == '.') &&
           (ref_frame_name.compare(prefix.length() + 1, std::string::npos, suffix) == 0))
        {
            found_frame = ref_frame;
            break;
        }
    }

    // Return the found frame (if any).
    return found_frame;
}

/**
 * Check that each active reference frame has an owner.
 */
void RefFrameManager::check_ref_frame_ownership() const
{
    // Walk over the reference frame list, checking that each active reference
    // frame has an owner.
    for(auto ref_frame : ref_frames)
    {
        if(ref_frame->is_active() && (ref_frame->get_owner() == nullptr))
        {
            MessageHandler::warn(__FILE__,
                                 __LINE__,
                                 RefFrameMessages::inconsistent_setup,
                                 "Reference frame '%s' has no owner",
                                 ref_frame->get_name().c_str());
        }
    }
}

/**
 * Reset the root node in anticipation of rebuilding the entire tree.
 */
void RefFrameManager::reset_tree_root_node()
{
    root_node = nullptr;
}

/**
 * Insert a reference frame in the reference frame tree.
 * @param ref_frame  Reference frame to be added to the ref frame tree.
 * @param parent     Parent frame
 */
void RefFrameManager::add_frame_to_tree(RefFrame & ref_frame, RefFrame * parent)
{
    /* Insert the node in the tree. */
    if(parent == nullptr)
    {
        root_node = &ref_frame;
        root_node->make_root();
    }
    else
    {
        parent->add_child(ref_frame);
    }
}

/*******************************************************************************
Frame subscription methods
*******************************************************************************/

/**
 * Subscribe to a reference frame, with the frame specified by name.
 *
 * ## Assumptions and limitations:
 *  - A subscriber should not double-subscribe to a frame.
 *
 * @param frame_name  Name of reference frame
 */
void RefFrameManager::subscribe_to_frame(const std::string & frame_name)
{
    RefFrame * ref_frame;

    // Handle errors.
    // 1. The frame_name must be a minimally valid name.
    if(!validate_name(__FILE__, __LINE__, frame_name, "Argument", "frame_name"))
    {
        return;
    }

    // 2. The frame must exist.
    ref_frame = find_ref_frame(frame_name);
    if(ref_frame == nullptr)
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              RefFrameMessages::invalid_name,
                              "Invalid reference frame name '%s'",
                              frame_name.c_str());
        return;
    }

    // Enter subscription.
    subscribe_to_frame(*ref_frame);
}

/**
 * Subscribe to a reference frame, with the frame specified as an argument.
 *
 * ## Assumptions and limitations:
 *  - A subscriber should not double-subscribe to a frame.
 *
 * @param frame  The reference frame to be subscribed to.
 */
void RefFrameManager::subscribe_to_frame(RefFrame & frame)
{
    frame.subscribe();
}

/**
 * Remove subscription to a reference frame, with the frame specified by name.
 *
 * ## Assumptions and limitations:
 *  - The caller is subscribed to the frame.
 *
 * @param frame_name  Name of reference frame
 */
void RefFrameManager::unsubscribe_to_frame(const std::string & frame_name)
{
    RefFrame * ref_frame;

    // Handle errors.
    // 1. The frame_name must be a minimally valid name.
    if(!validate_name(__FILE__, __LINE__, frame_name, "Argument", "frame_name"))
    {
        return;
    }

    // 2. The frame must exist.
    ref_frame = find_ref_frame(frame_name);
    if(ref_frame == nullptr)
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              RefFrameMessages::invalid_name,
                              "Invalid reference frame name '%s'",
                              frame_name.c_str());
        return;
    }

    unsubscribe_to_frame(*ref_frame);
}

/**
 * Remove subscription to a reference frame, with the frame specified as
 * an argument.
 *
 * ## Assumptions and limitations:
 *  - The caller is subscribed to the frame.
 *
 * @param frame  The reference frame
 */
void RefFrameManager::unsubscribe_to_frame(RefFrame & frame)
{
    if(frame.subscriptions() == 0)
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              RefFrameMessages::invalid_item,
                              "Frame '%s' has no subscriptions.",
                              frame.get_name().c_str());
        return;
    }

    frame.unsubscribe();
}

/**
 * Checks whether frame has subscribers; frame specified by name.
 * @param frame_name  Name of reference frame
 * @return True if the frame has subscribers; false otherwise.
 */
bool RefFrameManager::frame_is_subscribed(const std::string & frame_name)
{
    RefFrame * ref_frame;

    // Handle errors.
    // 1. The frame_name must be a minimally valid name.
    if(!validate_name(__FILE__, __LINE__, frame_name, "Argument", "frame_name"))
    {
        return false;
    }

    // 2. The frame must exist.
    ref_frame = find_ref_frame(frame_name);
    if(ref_frame == nullptr)
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              RefFrameMessages::invalid_name,
                              "Invalid reference frame name '%s'",
                              frame_name.c_str());
        return false;
    }

    return frame_is_subscribed(*ref_frame);
}

/**
 * Checks whether frame has subscribers; frame provided as an argument.
 * @param frame  The reference frame
 * @return True if the frame has subscribers; false otherwise.
 */
bool RefFrameManager::frame_is_subscribed(RefFrame & frame)
{
    return (frame.subscriptions() > 0);
}

/*******************************************************************************
Internal methods
*******************************************************************************/

/**
 * Check whether a name is trivially valid/invalid.
 * @param file            Usually __FILE__
 * @param line            Usually __LINE__
 * @param variable_value  Value to check
 * @param variable_type   Variable description
 * @param variable_name   Variable name
 * @return True if the name is valid, false if invalid.
 */
bool RefFrameManager::validate_name(const char * file,
                                    unsigned int line,
                                    const std::string & variable_value,
                                    const std::string & variable_type,
                                    const std::string & variable_name) const
{
    // Check for the empty string.
    if(variable_value.empty())
    {
        MessageHandler::error(file,
                              line,
                              RefFrameMessages::invalid_name,
                              "%s %s is the empty string",
                              variable_type.c_str(),
                              variable_name.c_str());

        return false;
    }

    return true;
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
