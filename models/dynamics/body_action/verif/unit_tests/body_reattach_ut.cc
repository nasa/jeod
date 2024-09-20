/*
 * body_reattach_ut.cc
 *
 *  Created on: June 17, 2024
 *      Author: lmccartney
 */

#include "dyn_body_mock.hh"
#include "dyn_manager_mock.hh"
#include "dynamics/body_action/include/body_reattach.hh"
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

class BodyReattachTest : public BodyReattach
{
public:
    void set_mass_subject(MassBody * in)
    {
        mass_subject = in;
    }

    void set_dyn_subject(DynBody * in)
    {
        dyn_subject = in;
    }

    MassBody * get_mass_subject()
    {
        return mass_subject;
    }

    DynBody * get_dyn_subject()
    {
        return dyn_subject;
    }
};

TEST(BodyReattach, create)
{
    BodyReattach staticInst;
    BodyReattachTest * dynInst;
    dynInst = new BodyReattachTest();
    BodyReattach * dynInst2 = new BodyReattach();

    EXPECT_FALSE(dynInst->active);
    EXPECT_TRUE(dynInst->terminate_on_error);
    EXPECT_EQ(std::string(""), dynInst->action_name);

    EXPECT_EQ(std::string(""), dynInst->get_identifier());
    EXPECT_EQ(nullptr, dynInst->get_mass_subject());

    delete dynInst2;
    delete dynInst;
}

TEST(BodyReattach, apply)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockMassBody mockMassBody;
    Mock::VerifyAndClear(&mockMessageHandler);

    {
        // Test reattach mass_subject. Fails
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);

        BodyReattachTest dynInst;
        dynInst.set_mass_subject(&mockMassBody);
        dynInst.terminate_on_error = true;
        EXPECT_CALL(mockMassBody, reattach(_, _)).Times(1).WillOnce(Return(false));
        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockMassBody);
    }
    {
        // Test reattach mass_subject. Succeeds
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);

        BodyReattachTest dynInst;
        dynInst.set_mass_subject(&mockMassBody);
        dynInst.terminate_on_error = true;
        EXPECT_CALL(mockMassBody, reattach(_, _)).Times(1).WillOnce(Return(true));
        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockMassBody);
    }
    {
        // Test reattach mass_subject. Succeeds
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);

        BodyReattachTest dynInst;
        dynInst.set_mass_subject(&mockMassBody);
        dynInst.terminate_on_error = false;
        EXPECT_CALL(mockMassBody, reattach(_, _)).Times(1).WillOnce(Return(true));
        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockMassBody);
    }
    {
        // Test reattach mass_subject. Fails
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Error, _, _, _, _, _, _)).Times(1);

        BodyReattachTest dynInst;
        dynInst.set_mass_subject(&mockMassBody);
        dynInst.terminate_on_error = false;
        EXPECT_CALL(mockMassBody, reattach(_, _)).Times(1).WillOnce(Return(false));
        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockMassBody);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}
