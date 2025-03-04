/*
 * ref_frame_manager_ut.cc
 */

#include "message_handler_mock.hh"
#include "utils/ref_frames/include/ref_frame_manager.hh"

#include "ref_frame_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

class RefFrameManagerTest : public RefFrameManager
{
public:
    void set_root_node(RefFrame * newFrame)
    {
        root_node = newFrame;
    }

    RefFrame * get_root_node()
    {
        return root_node;
    }
};

TEST(RefFrameManager, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    RefFrameManager staticInst;
    RefFrameManager * dynInst = new RefFrameManager;
    delete dynInst;
}

TEST(RefFrameManager, add_ref_frame) {}

TEST(RefFrameManager, remove_ref_frame) {}

TEST(RefFrameManager, find_ref_frame) {}

TEST(RefFrameManager, check_ref_frame_ownership) {}

TEST(RefFrameManager, reset_tree_root_node)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());

    RefFrameManagerTest staticInst;
    MockRefFrame mockRefFrame;
    staticInst.set_root_node(&mockRefFrame);
    EXPECT_EQ(staticInst.get_root_node(), &mockRefFrame);

    staticInst.reset_tree_root_node();

    EXPECT_EQ(staticInst.get_root_node(), nullptr);
}

TEST(RefFrameManager, add_frame_to_tree) {}

TEST(RefFrameManager, subscribe_to_frame) {}

TEST(RefFrameManager, unsubscribe_to_frame) {}

TEST(RefFrameManager, frame_is_subscribed) {}

TEST(RefFrameManager, validate_name) {}
