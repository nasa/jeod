/*
 * relative_derived_state_ut.cc
 *
 *  Created on: Nov 8, 2023
 *      Author: tbrain
 */

#include "body_ref_frame_mock.hh"
#include "dyn_body_mock.hh"
#include "dyn_manager_mock.hh"
#include "dynamics/derived_state/include/relative_derived_state.hh"
#include "mass_mock.hh"
#include "message_handler_mock.hh"
#include "ref_frame_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::An;
using testing::AnyNumber;
using testing::ByRef;
using testing::Mock;
using testing::Ref;
using testing::Return;
using testing::ReturnRef;

using namespace jeod;

class RelativeDerivedStateTest : public RelativeDerivedState
{
public:
    BodyRefFrame * get_subject_frame()
    {
        return subject_frame;
    }

    RefFrame * get_target_frame()
    {
        return target_frame;
    }
};

TEST(RelativeDerivedState, create)
{
    {
        // Execute generic static and dynamic allocation, testing for what we can.
        RelativeDerivedState staticInst;
        BodyRefFrame bodyRefFrame;
        RefFrame refFrame;
        bodyRefFrame.subscribe();
        refFrame.subscribe();
        RelativeDerivedStateTest * dynInst = new RelativeDerivedStateTest;
        RelativeDerivedState * dynInst2 = new RelativeDerivedState;

        EXPECT_EQ("", dynInst->name);
        EXPECT_EQ("", dynInst->subject_frame_name);
        EXPECT_EQ("", dynInst->target_frame_name);
        EXPECT_EQ(RelativeDerivedState::undefined, dynInst->direction_sense);
        EXPECT_TRUE(dynInst->active);

        EXPECT_EQ(nullptr, dynInst->get_subject_frame());
        EXPECT_EQ(nullptr, dynInst->get_target_frame());

        delete dynInst2;
        delete dynInst;
        EXPECT_EQ(1, bodyRefFrame.subscriptions());
        EXPECT_EQ(1, refFrame.subscriptions());
    }

    {
        // Test destructor when frames are assigned. Neither frame has subscriptions, so nothing happens.
        BodyRefFrame bodyRefFrame;
        RefFrame refFrame;
        RelativeDerivedStateTest * dynInst = new RelativeDerivedStateTest;
        dynInst->set_target_frame(refFrame);
        dynInst->set_subject_frame(bodyRefFrame);
        delete dynInst;
        EXPECT_EQ(0, bodyRefFrame.subscriptions());
        EXPECT_EQ(0, refFrame.subscriptions());
    }

    {
        // Test destructor when frames are assigned. Both frames has 1 sub, both decrement to zero.
        BodyRefFrame bodyRefFrame;
        RefFrame refFrame;
        bodyRefFrame.subscribe();
        refFrame.subscribe();
        EXPECT_EQ(1, bodyRefFrame.subscriptions());
        EXPECT_EQ(1, refFrame.subscriptions());
        RelativeDerivedStateTest * dynInst = new RelativeDerivedStateTest;
        dynInst->set_target_frame(refFrame);
        dynInst->set_subject_frame(bodyRefFrame);
        delete dynInst;
        EXPECT_EQ(0, bodyRefFrame.subscriptions());
        EXPECT_EQ(0, refFrame.subscriptions());
    }
}

TEST(RelativeDerivedState, initialize_DynManager)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    mockDynBody.set_name("TestDynBody");
    Mock::VerifyAndClear(&mockMessageHandler);

    {
        // Test failure when no subject frame name set
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        RelativeDerivedState staticInst;
        staticInst.reference_name = "refname";
        staticInst.initialize(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    {
        // Test failure when subject_frame_name is set, subject is not null, but find_body_frame returns null
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockDynBody, find_body_frame(_)).Times(1).WillOnce(Return(nullptr));
        RelativeDerivedState staticInst;
        staticInst.reference_name = "refname";
        staticInst.subject_frame_name = "SubjectFrameName";
        staticInst.subject = &mockDynBody;
        staticInst.initialize(mockDynManager);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    {
        // Test failure when subject_frame_name is set, subject is not null, but find_body_frame returns a
        // non-BodyRefFrame pointer
        BodyRefFrame bodyRefFrame;
        RefFrame refFrame;
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockDynManager, find_ref_frame(_)).Times(1).WillOnce(Return(&refFrame));
        RelativeDerivedState staticInst;
        staticInst.reference_name = "refname";
        staticInst.subject_frame_name = "SubjectFrameName";
        staticInst.initialize(mockDynManager);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockMessageHandler);
        EXPECT_EQ(0, bodyRefFrame.subscriptions());
        EXPECT_EQ(0, refFrame.subscriptions());
    }

    {
        // Test failure when subject_frame is set but find_ref_frame returns null
        BodyRefFrame bodyRefFrame;
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockDynManager, find_ref_frame(_)).Times(1).WillOnce(Return(nullptr));
        RelativeDerivedState staticInst;
        staticInst.reference_name = "refname";
        staticInst.subject_frame_name = "SubjectFrameName";
        staticInst.initialize(mockDynManager);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockMessageHandler);
        EXPECT_EQ(0, bodyRefFrame.subscriptions());
    }

    {
        // Test failure when subject_frame passes all checks but no target_frame_name is not set
        BodyRefFrame bodyRefFrame;
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockDynManager, find_ref_frame(_)).Times(1).WillOnce(Return(&bodyRefFrame));
        RelativeDerivedState staticInst;
        staticInst.reference_name = "refname";
        staticInst.subject_frame_name = "SubjectFrameName";
        staticInst.initialize(mockDynManager);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockMessageHandler);
        EXPECT_EQ(0, bodyRefFrame.subscriptions());
    }

    {
        // Test failure when subject_frame passes all checks, target_frame_name is set but find_ref_frame returns null
        BodyRefFrame bodyRefFrame;
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockDynManager, find_ref_frame(_))
            .Times(2)
            .WillOnce(Return(&bodyRefFrame))
            .WillOnce(Return(nullptr));
        RelativeDerivedState staticInst;
        staticInst.reference_name = "refname";
        staticInst.subject_frame_name = "SubjectFrameName";
        staticInst.target_frame_name = "TargetFrameName";
        staticInst.initialize(mockDynManager);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockMessageHandler);
        EXPECT_EQ(0, bodyRefFrame.subscriptions());
    }

    {
        // Test success when subject_frame and target_frame passes all checks
        BodyRefFrame bodyRefFrame;
        RefFrame refFrame;
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(0);
        EXPECT_CALL(mockDynManager, find_ref_frame(_))
            .Times(2)
            .WillOnce(Return(&bodyRefFrame))
            .WillOnce(Return(&refFrame));
        RelativeDerivedState staticInst;
        staticInst.reference_name = "refname";
        staticInst.subject_frame_name = "SubjectFrameName";
        staticInst.target_frame_name = "TargetFrameName";
        staticInst.initialize(mockDynManager);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockMessageHandler);
        EXPECT_EQ(1, bodyRefFrame.subscriptions());
        EXPECT_EQ(1, refFrame.subscriptions());
    }

    {
        // Test wrapper initialize method success when subject_frame and target_frame passes all checks
        BodyRefFrame bodyRefFrame;
        RefFrame refFrame;
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(0);
        EXPECT_CALL(mockDynBody, find_body_frame(_)).Times(1).WillOnce(Return(&bodyRefFrame));
        EXPECT_CALL(mockDynManager, find_ref_frame(_)).Times(1).WillOnce(Return(&refFrame));
        RelativeDerivedState staticInst;
        staticInst.reference_name = "refname";
        staticInst.subject_frame_name = "SubjectFrameName";
        staticInst.target_frame_name = "TargetFrameName";
        staticInst.initialize(mockDynBody, mockDynManager);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockMessageHandler);
        EXPECT_EQ(1, bodyRefFrame.subscriptions());
        EXPECT_EQ(1, refFrame.subscriptions());
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(RelativeDerivedState, update)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    Mock::VerifyAndClear(&mockMessageHandler);

    {
        // Test with no direction_sense set. Fail
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        BodyRefFrame bodyRefFrame;
        RefFrame refFrame;
        RelativeDerivedState staticInst;
        staticInst.set_subject_frame(bodyRefFrame);
        staticInst.set_target_frame(refFrame);
        staticInst.update();
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    {
        // Test with subject state in target direction_sense. Success
        MockRefFrame refFrame;
        MockBodyRefFrame bodyRefFrame;

        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(0);
        EXPECT_CALL(bodyRefFrame, compute_relative_state(Ref(refFrame), _)).Times(1);

        RelativeDerivedState staticInst;
        staticInst.direction_sense = RelativeDerivedState::ComputeSubjectStateinTarget;
        staticInst.set_subject_frame(bodyRefFrame);
        staticInst.set_target_frame(refFrame);
        staticInst.update();
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    {
        // Test with subject state in target direction_sense. Success
        MockRefFrame refFrame;
        MockBodyRefFrame bodyRefFrame;

        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(0);
        EXPECT_CALL(refFrame, compute_relative_state(Ref(bodyRefFrame), _)).Times(1);

        RelativeDerivedState staticInst;
        staticInst.direction_sense = RelativeDerivedState::ComputeTargetStateinSubject;
        staticInst.set_subject_frame(bodyRefFrame);
        staticInst.set_target_frame(refFrame);
        staticInst.update();
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(RelativeDerivedState, set_activation_flag)
{
    {
        // De-activate the derived state, sets active to false and unsubscribes.
        BodyRefFrame bodyRefFrame;
        RefFrame refFrame;
        bodyRefFrame.subscribe();
        refFrame.subscribe();
        RelativeDerivedState staticInst;
        staticInst.set_subject_frame(bodyRefFrame);
        staticInst.set_target_frame(refFrame);
        staticInst.active = true;
        staticInst.set_activation_flag(false);
        EXPECT_FALSE(staticInst.active);
        EXPECT_EQ(0, bodyRefFrame.subscriptions());
        EXPECT_EQ(0, refFrame.subscriptions());
    }

    {
        // Activate the derived state, sets active to true and subscribes.
        BodyRefFrame bodyRefFrame;
        RefFrame refFrame;
        RelativeDerivedState staticInst;
        staticInst.set_subject_frame(bodyRefFrame);
        staticInst.set_target_frame(refFrame);
        staticInst.active = false;
        staticInst.set_activation_flag(true);
        EXPECT_TRUE(staticInst.active);
        EXPECT_EQ(1, bodyRefFrame.subscriptions());
        EXPECT_EQ(1, refFrame.subscriptions());
    }
}
