/*
 * dyn_body_find_body_frame_ut.cc
 */

#include "dyn_manager_mock.hh"
#include "dynamics/dyn_body/include/dyn_body.hh"
#include "dynamics/dyn_body/include/dyn_body_messages.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;
using testing::_;
using testing::AnyNumber;
using testing::Mock;
using testing::Return;

class DynBodyTest : public DynBody
{
public:
    void set_dyn_manager(DynManager * dyn_mgr)
    {
        dyn_manager = dyn_mgr;
    }
};

TEST(DynBody, find_body_frame)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    Mock::VerifyAndClear(&mockMessageHandler);

    BodyRefFrame * body_ref_out;
    RefFrame ref_frame;
    BodyRefFrame body_ref_frame;
    {
        SCOPED_TRACE("Empty frame ID");
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynBody staticInst;
        Mock::VerifyAndClear(&mockMessageHandler);

        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);

        body_ref_out = staticInst.find_body_frame("");
        EXPECT_EQ(nullptr, body_ref_out);
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }
    {
        SCOPED_TRACE("No dynamics manager");
        DynBody body;

        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Failure, _, _, _, DynBodyMessages::invalid_body, _, _))
            .Times(1);

        EXPECT_EQ(nullptr, body.find_body_frame("structure"));

        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    }
    {
        SCOPED_TRACE("Reference frame not found");
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynBodyTest staticInst;
        Mock::VerifyAndClear(&mockMessageHandler);

        staticInst.set_name("dyn_body");
        staticInst.set_dyn_manager(&mockDynManager);
        EXPECT_CALL(mockDynManager, find_ref_frame(_, _)).Times(1).WillOnce(Return(nullptr));
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(0);

        body_ref_out = staticInst.find_body_frame("frame_id");
        EXPECT_EQ(nullptr, body_ref_out);
        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        EXPECT_CALL(mockDynManager, remove_ref_frame(_)).Times(3);
    }
    {
        SCOPED_TRACE("Reference frame is not a BodyRefFrame");
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynBodyTest staticInst;
        Mock::VerifyAndClear(&mockMessageHandler);

        staticInst.set_name("dyn_body");
        staticInst.set_dyn_manager(&mockDynManager);
        EXPECT_CALL(mockDynManager, find_ref_frame(_, _)).Times(1).WillOnce(Return(&ref_frame));
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);

        body_ref_out = staticInst.find_body_frame("frame_id");
        EXPECT_EQ(nullptr, body_ref_out);
        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        EXPECT_CALL(mockDynManager, remove_ref_frame(_)).Times(3);
    }
    {
        SCOPED_TRACE("Body reference frame found");
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        DynBodyTest staticInst;
        Mock::VerifyAndClear(&mockMessageHandler);

        staticInst.set_name("dyn_body");
        staticInst.set_dyn_manager(&mockDynManager);
        EXPECT_CALL(mockDynManager, find_ref_frame(_, _)).Times(1).WillOnce(Return(&body_ref_frame));
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(0);

        body_ref_out = staticInst.find_body_frame("frame_id");
        EXPECT_EQ(&body_ref_frame, body_ref_out);
        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);

        // For non-unit destructor process_message calls.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
        EXPECT_CALL(mockDynManager, remove_ref_frame(_)).Times(3);
    }
}
