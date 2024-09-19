/*
 * lvlh_relative_derived_state_ut.cc
 *
 *  Created on: Aug 5, 2024
 *      Author: lmccartney
 */

#include "body_ref_frame_mock.hh"
#include "dyn_body_mock.hh"
#include "dyn_manager_mock.hh"
#include "dynamics/derived_state/include/lvlh_relative_derived_state.hh"
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

class LvlhRelativeDerivedStateTest : public LvlhRelativeDerivedState
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

TEST(LvlhRelativeDerivedState, create)
{
    {
        // Execute generic static and dynamic allocation, testing for what we can.
        LvlhRelativeDerivedState staticInst;
        BodyRefFrame bodyRefFrame;
        RefFrame refFrame;
        bodyRefFrame.subscribe();
        refFrame.subscribe();
        LvlhRelativeDerivedStateTest * dynInst = new LvlhRelativeDerivedStateTest;
        LvlhRelativeDerivedState * dynInst2 = new LvlhRelativeDerivedState;

        EXPECT_EQ("", dynInst->name);
        EXPECT_EQ("", dynInst->subject_frame_name);
        EXPECT_EQ("", dynInst->target_frame_name);
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
        LvlhRelativeDerivedStateTest * dynInst = new LvlhRelativeDerivedStateTest;
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
        LvlhRelativeDerivedStateTest * dynInst = new LvlhRelativeDerivedStateTest;
        dynInst->set_target_frame(refFrame);
        dynInst->set_subject_frame(bodyRefFrame);
        delete dynInst;
        EXPECT_EQ(0, bodyRefFrame.subscriptions());
        EXPECT_EQ(0, refFrame.subscriptions());
    }
}

TEST(LvlhRelativeDerivedState, initialize_DynManager)
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
        LvlhRelativeDerivedState staticInst;
        staticInst.reference_name = "refname";
        staticInst.initialize(mockDynBody, mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    {
        // Test failure when subject_frame_name is set, subject is not null, but find_body_frame returns null
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockDynBody, find_body_frame(_)).Times(1).WillOnce(Return(nullptr));
        LvlhRelativeDerivedState staticInst;
        staticInst.reference_name = "refname";
        staticInst.subject_frame_name = "SubjectFrameName";
        staticInst.subject = &mockDynBody;
        staticInst.initialize(mockDynBody, mockDynManager);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    {
        // Test failure when subject_frame_name is set, subject is not null, but find_body_frame returns a
        // non-BodyRefFrame pointer
        BodyRefFrame bodyRefFrame;
        RefFrame refFrame;
        EXPECT_CALL(mockDynBody, find_body_frame(_)).Times(1).WillOnce(Return(&bodyRefFrame));
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        LvlhRelativeDerivedState staticInst;
        staticInst.reference_name = "refname";
        staticInst.subject_frame_name = "SubjectFrameName";
        staticInst.initialize(mockDynBody, mockDynManager);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockMessageHandler);
        EXPECT_EQ(0, bodyRefFrame.subscriptions());
        EXPECT_EQ(0, refFrame.subscriptions());
    }

    {
        // Test failure when subject_frame is set but find_ref_frame returns null
        BodyRefFrame bodyRefFrame;
        EXPECT_CALL(mockDynBody, find_body_frame(_)).Times(1).WillOnce(Return(&bodyRefFrame));
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        LvlhRelativeDerivedState staticInst;
        staticInst.reference_name = "refname";
        staticInst.subject_frame_name = "SubjectFrameName";
        staticInst.initialize(mockDynBody, mockDynManager);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockMessageHandler);
        EXPECT_EQ(0, bodyRefFrame.subscriptions());
    }

    {
        // Test failure when subject_frame passes all checks but no target_frame_name is not set
        BodyRefFrame bodyRefFrame;
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockDynBody, find_body_frame(_)).Times(1).WillOnce(Return(&bodyRefFrame));
        LvlhRelativeDerivedState staticInst;
        staticInst.reference_name = "refname";
        staticInst.subject_frame_name = "SubjectFrameName";
        staticInst.initialize(mockDynBody, mockDynManager);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockMessageHandler);
        EXPECT_EQ(0, bodyRefFrame.subscriptions());
    }

    {
        // Test failure when subject_frame passes all checks, target_frame_name is set but find_ref_frame returns null
        BodyRefFrame bodyRefFrame;
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockDynBody, find_body_frame(_)).Times(1).WillOnce(Return(&bodyRefFrame));
        EXPECT_CALL(mockDynManager, find_ref_frame(_)).Times(1).WillOnce(Return(nullptr));
        LvlhRelativeDerivedState staticInst;
        staticInst.reference_name = "refname";
        staticInst.subject_frame_name = "SubjectFrameName";
        staticInst.target_frame_name = "TargetFrameName";
        staticInst.initialize(mockDynBody, mockDynManager);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockMessageHandler);
        EXPECT_EQ(0, bodyRefFrame.subscriptions());
    }

    {
        // Test success when subject_frame and target_frame passes all checks
        BodyRefFrame bodyRefFrame;
        RefFrame refFrame;
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(0);
        EXPECT_CALL(mockDynBody, find_body_frame(_)).Times(1).WillOnce(Return(&bodyRefFrame));
        EXPECT_CALL(mockDynManager, find_ref_frame(_)).Times(1).WillOnce(Return(&refFrame));
        LvlhRelativeDerivedState staticInst;
        staticInst.reference_name = "refname";
        staticInst.subject_frame_name = "SubjectFrameName";
        staticInst.target_frame_name = "TargetFrameName";
        staticInst.initialize(mockDynBody, mockDynManager);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    {
        // Test wrapper initialize method success when subject_frame and target_frame passes all checks
        BodyRefFrame bodyRefFrame;
        RefFrame refFrame;
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(0);
        EXPECT_CALL(mockDynBody, find_body_frame(_)).Times(1).WillOnce(Return(&bodyRefFrame));
        EXPECT_CALL(mockDynManager, find_ref_frame(_)).Times(1).WillOnce(Return(&refFrame));
        LvlhRelativeDerivedState staticInst;
        staticInst.reference_name = "refname";
        staticInst.subject_frame_name = "SubjectFrameName";
        staticInst.target_frame_name = "TargetFrameName";
        staticInst.initialize(mockDynBody, mockDynManager);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(LvlhRelativeDerivedState, update)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    Mock::VerifyAndClear(&mockMessageHandler);

    {
        // Rectilinear lvlh_type
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        BodyRefFrame bodyRefFrame;
        RefFrame refFrame;
        LvlhRelativeDerivedState staticInst;
        staticInst.set_subject_frame(bodyRefFrame);
        staticInst.set_target_frame(refFrame);
        staticInst.update();
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    {
        // CircularCurvilinear lvlh_type
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        BodyRefFrame bodyRefFrame;
        RefFrame refFrame;
        refFrame.state.trans.position[0] = 1;
        LvlhRelativeDerivedState staticInst;
        staticInst.set_subject_frame(bodyRefFrame);
        staticInst.set_target_frame(refFrame);
        staticInst.lvlh_type = jeod::LvlhType::CircularCurvilinear;
        staticInst.update();
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    {
        // EllipticalCurvilinear lvlh_type
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        BodyRefFrame bodyRefFrame;
        RefFrame refFrame;
        refFrame.state.trans.position[0] = 1;
        LvlhRelativeDerivedState staticInst;
        staticInst.set_subject_frame(bodyRefFrame);
        staticInst.set_target_frame(refFrame);
        staticInst.lvlh_type = jeod::LvlhType::EllipticalCurvilinear;
        staticInst.update();
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    {
        // EllipticalCurvilinear lvlh_type
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        BodyRefFrame bodyRefFrame;
        RefFrame refFrame;
        refFrame.state.trans.position[0] = 1;
        LvlhRelativeDerivedState staticInst;
        staticInst.set_subject_frame(bodyRefFrame);
        staticInst.set_target_frame(refFrame);
        staticInst.lvlh_type = (jeod::LvlhType::Type)-1;
        staticInst.update();
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(LvlhRelativeDerivedState, convert_rect_to_circ)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        // Reference radius > 1e-9
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        BodyRefFrame bodyRefFrame;
        RefFrame refFrame;
        refFrame.state.trans.position[0] = 1;
        LvlhRelativeDerivedState staticInst;
        staticInst.set_subject_frame(bodyRefFrame);
        staticInst.set_target_frame(refFrame);
        staticInst.lvlh_type = jeod::LvlhType::CircularCurvilinear;
        staticInst.update();
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    {
        // Reference radius < 1e-9
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(2);
        BodyRefFrame bodyRefFrame;
        RefFrame refFrame;
        LvlhRelativeDerivedState staticInst;
        staticInst.set_subject_frame(bodyRefFrame);
        staticInst.set_target_frame(refFrame);
        staticInst.lvlh_type = jeod::LvlhType::CircularCurvilinear;
        staticInst.update();
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(LvlhRelativeDerivedState, convert_circ_to_rect)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        // Reference radius > 1e-9
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(0);
        BodyRefFrame bodyRefFrame;
        RefFrame refFrame;
        RefFrameState refFrameState;
        refFrame.state.trans.position[0] = 1;
        LvlhRelativeDerivedStateTest staticInst;
        staticInst.set_subject_frame(bodyRefFrame);
        staticInst.set_target_frame(refFrame);
        staticInst.lvlh_type = jeod::LvlhType::CircularCurvilinear;
        staticInst.convert_circ_to_rect(refFrameState);
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    {
        // Reference radius < 1e-9
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        BodyRefFrame bodyRefFrame;
        RefFrame refFrame;
        RefFrameState refFrameState;
        LvlhRelativeDerivedStateTest staticInst;
        staticInst.set_subject_frame(bodyRefFrame);
        staticInst.set_target_frame(refFrame);
        staticInst.lvlh_type = jeod::LvlhType::CircularCurvilinear;
        staticInst.convert_circ_to_rect(refFrameState);
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(LvlhRelativeDerivedState, do_theta_dot_correction)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        // use_theta_dot_correction true, c2r false
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        BodyRefFrame bodyRefFrame;
        RefFrame refFrame;
        refFrame.state.trans.position[0] = 1;
        LvlhRelativeDerivedState staticInst;
        staticInst.set_subject_frame(bodyRefFrame);
        staticInst.set_target_frame(refFrame);
        staticInst.use_theta_dot_correction = true;
        staticInst.lvlh_type = jeod::LvlhType::CircularCurvilinear;
        staticInst.update();
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    {
        // use_theta_dot_correction true, c2r true
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(0);
        BodyRefFrame bodyRefFrame;
        RefFrame refFrame;
        RefFrameState refFrameState;
        refFrame.state.trans.position[0] = 1;
        LvlhRelativeDerivedStateTest staticInst;
        staticInst.set_subject_frame(bodyRefFrame);
        staticInst.set_target_frame(refFrame);
        staticInst.lvlh_type = jeod::LvlhType::CircularCurvilinear;
        staticInst.use_theta_dot_correction = true;
        staticInst.convert_circ_to_rect(refFrameState);
        Mock::VerifyAndClear(&mockMessageHandler);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}
