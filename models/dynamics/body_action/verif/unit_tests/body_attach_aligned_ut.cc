/*
 * body_attach_aligned_ut.cc
 *
 *  Created on: Nov 7, 2023
 *      Author: tbrain
 */

#include "dyn_body_mock.hh"
#include "dyn_manager_mock.hh"
#include "dynamics/body_action/include/body_attach_aligned.hh"
#include "mass_mock.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::An;
using testing::AnyNumber;
using testing::Mock;
using testing::Ref;
using testing::Return;

using namespace jeod;

TEST(BodyAttachAligned, create)
{
    BodyAttachAligned staticInst;
    BodyAttachAligned * dynInst;
    dynInst = new BodyAttachAligned();

    EXPECT_EQ("", dynInst->subject_point_name);
    EXPECT_EQ("", dynInst->parent_point_name);

    delete dynInst;
}

TEST(BodyAttachAligned, initialize)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    MockMassBody mockMassBody;
    Mock::VerifyAndClear(&mockMessageHandler);

    {
        // Test valid parent and subjects, full parent subject point names. No Error.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(0);
        BodyAttachAligned staticInst;
        staticInst.set_parent_body(mockDynBody);
        staticInst.set_subject_body(mockMassBody);
        staticInst.parent_point_name = "TestParent";
        staticInst.subject_point_name = "TestSubject";
        staticInst.initialize(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    {
        // Test valid parent and subjects, full parent point name but no subject point name. Error.
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        BodyAttachAligned staticInst;
        staticInst.set_parent_body(mockDynBody);
        staticInst.set_subject_body(mockMassBody);
        staticInst.parent_point_name = "TestParent";
        staticInst.initialize(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    {
        // Test valid parent and subjects, full subject point name but no parent point name. Error.
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        BodyAttachAligned staticInst;
        staticInst.set_parent_body(mockDynBody);
        staticInst.set_subject_body(mockMassBody);
        staticInst.subject_point_name = "TestSubject";
        staticInst.initialize(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(BodyAttachAligned, apply)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    MockDynBody mockDynBody2;
    MockMassBody mockMassBody;
    MockMassBody mockMassBody2;
    RefFrame refFrame;
    Mock::VerifyAndClear(&mockMessageHandler);

    {
        // Test attach DynBody subject to DynBody parent. Succeeds
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockDynBody, attach_child(An<const std::string &>(), An<const std::string &>(), Ref(mockDynBody2)))
            .Times(1)
            .WillOnce(Return(true));
        BodyAttachAligned staticInst;
        staticInst.set_parent_body(mockDynBody);
        staticInst.set_subject_body(mockDynBody2);
        staticInst.apply(mockDynManager);
        EXPECT_TRUE(staticInst.succeeded);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
    }

    {
        // Test attach MassBody subject to DynBody parent. Succeeds
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockDynBody, add_mass_body(An<const std::string &>(), An<const std::string &>(), Ref(mockMassBody)))
            .Times(1)
            .WillOnce(Return(true));
        BodyAttachAligned staticInst;
        staticInst.set_parent_body(mockDynBody);
        staticInst.set_subject_body(mockMassBody);
        staticInst.apply(mockDynManager);
        EXPECT_TRUE(staticInst.succeeded);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
    }

    {
        // Test attach DynBody subject to MassBody parent. Fails
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        BodyAttachAligned staticInst;
        staticInst.set_parent_body(mockMassBody);
        staticInst.set_subject_body(mockDynBody);
        staticInst.apply(mockDynManager);
        EXPECT_FALSE(staticInst.succeeded);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    {
        // Test attach MassBody subject to MassBody parent. Succeeds
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockMassBody2, attach_to(An<const std::string &>(), An<const std::string &>(), Ref(mockMassBody)))
            .Times(1)
            .WillOnce(Return(true));
        BodyAttachAligned staticInst;
        staticInst.set_parent_body(mockMassBody);
        staticInst.set_subject_body(mockMassBody2);
        staticInst.apply(mockDynManager);
        EXPECT_TRUE(staticInst.succeeded);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockMassBody2);
    }

    {
        // Test attach DynBody subject to RefFrame parent. Succeeds
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockDynBody, attach_to_frame(An<const std::string &>(), An<const std::string &>(), _, _))
            .Times(1)
            .WillOnce(Return(true));
        BodyAttachAligned staticInst;
        staticInst.set_parent_frame(refFrame);
        staticInst.set_subject_body(mockDynBody);
        staticInst.apply(mockDynManager);
        EXPECT_TRUE(staticInst.succeeded);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
    }

    {
        // Test attach MassBody subject to RefFrame parent. Fails
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        BodyAttachAligned staticInst;
        staticInst.set_parent_frame(refFrame);
        staticInst.set_subject_body(mockMassBody);
        staticInst.apply(mockDynManager);
        EXPECT_FALSE(staticInst.succeeded);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    {
        // Test unset parent and subjects. Fails
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        BodyAttachAligned staticInst;
        staticInst.apply(mockDynManager);
        EXPECT_FALSE(staticInst.succeeded);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}
