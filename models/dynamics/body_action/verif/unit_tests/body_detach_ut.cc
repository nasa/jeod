/*
 * body_detach_ut.cc
 *
 *  Created on: June 6, 2024
 *      Author: lmccartney
 */

#include "dyn_body_mock.hh"
#include "dyn_manager_mock.hh"
#include "dynamics/body_action/include/body_detach.hh"
#include "mass_mock.hh"
#include "message_handler_mock.hh"
#include <iostream>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::An;
using testing::AnyNumber;
using testing::Mock;
using testing::Ref;
using testing::Return;
using namespace jeod;

class BodyDetachTest : public BodyDetach
{
public:
    void set_dyn_subject(DynBody * in)
    {
        dyn_subject = in;
    }

    void set_mass_subject(MassBody * in)
    {
        mass_subject = in;
    }

    DynBody * get_dyn_subject()
    {
        return dyn_subject;
    }

    MassBody * get_mass_subject()
    {
        return mass_subject;
    }
};

TEST(BodyDetach, create)
{
    BodyDetach staticInst;
    BodyDetachTest * dynInst;
    dynInst = new BodyDetachTest();
    BodyDetach * dynInst2 = new BodyDetach();

    EXPECT_FALSE(dynInst->active);
    EXPECT_TRUE(dynInst->terminate_on_error);
    EXPECT_EQ(std::string(""), dynInst->action_name);

    EXPECT_EQ(std::string(""), dynInst->get_identifier());
    EXPECT_EQ(nullptr, dynInst->get_dyn_subject());
    EXPECT_EQ(nullptr, dynInst->get_mass_subject());

    delete dynInst2;
    delete dynInst;
}

TEST(BodyDetach, is_ready)
{
    BodyDetachTest * dynInst = new BodyDetachTest();

    EXPECT_FALSE(dynInst->is_ready());

    dynInst->active = true;

    EXPECT_TRUE(dynInst->is_ready());
}

TEST(BodyDetach, apply)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    MockMassBody mockMassBody;
    Mock::VerifyAndClear(&mockMessageHandler);

    {
        // Test detach dyn_subject from parent. Succeeds
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);

        BodyDetachTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_mass_subject(&mockMassBody);
        EXPECT_CALL(mockDynBody, detach()).Times(1).WillOnce(Return(true));
        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
    }
    {
        // Test detach mass_subject from parent. Succeeds
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);

        BodyDetachTest dynInst;
        dynInst.set_mass_subject(&mockMassBody);
        EXPECT_CALL(mockMassBody, detach()).Times(1).WillOnce(Return(true));
        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockMassBody);
    }
    {
        // Test detach dyn_subject from parent. Fails, terminate_on_error set
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);

        BodyDetachTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_mass_subject(&mockMassBody);
        EXPECT_CALL(mockDynBody, detach()).Times(1).WillOnce(Return(false));
        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
    }
    {
        // Test detach dyn_subject from parent. Fails, terminate_on_error not set
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Error, _, _, _, _, _, _)).Times(1);

        BodyDetachTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_mass_subject(&mockMassBody);
        dynInst.terminate_on_error = false;
        EXPECT_CALL(mockDynBody, detach()).Times(1).WillOnce(Return(false));
        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}
