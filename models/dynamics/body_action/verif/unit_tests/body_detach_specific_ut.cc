/*
 * body_detach_specific_ut.cc
 *
 *  Created on: June 13, 2024
 *      Author: lmccartney
 */

#include "dyn_body_mock.hh"
#include "dyn_manager_mock.hh"
#include "dynamics/body_action/include/body_detach_specific.hh"
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

class BodyDetachSpecificTest : public BodyDetachSpecific
{
public:
    void set_dyn_subject(DynBody * in)
    {
        dyn_subject = in;
    }

    void set_dyn_detach_from(DynBody * in)
    {
        dyn_detach_from = in;
    }

    void set_mass_subject(MassBody * in)
    {
        mass_subject = in;
    }

    void set_mass_detach_from(MassBody * in)
    {
        mass_detach_from = in;
    }

    DynBody * get_dyn_subject()
    {
        return dyn_subject;
    }

    DynBody * get_dyn_detach_from()
    {
        return dyn_detach_from;
    }

    MassBody * get_mass_subject()
    {
        return mass_subject;
    }

    MassBody * get_mass_detach_from()
    {
        return mass_detach_from;
    }
};

TEST(BodyDetachSpecific, create)
{
    BodyDetachSpecific staticInst;
    BodyDetachSpecificTest * dynInst;
    dynInst = new BodyDetachSpecificTest();
    BodyDetachSpecific * dynInst2 = new BodyDetachSpecific();

    EXPECT_FALSE(dynInst->active);
    EXPECT_TRUE(dynInst->terminate_on_error);
    EXPECT_EQ(std::string(""), dynInst->action_name);

    EXPECT_EQ(std::string(""), dynInst->get_identifier());
    EXPECT_EQ(nullptr, dynInst->get_dyn_subject());
    EXPECT_EQ(nullptr, dynInst->get_mass_subject());
    EXPECT_EQ(nullptr, dynInst->get_dyn_detach_from());
    EXPECT_EQ(nullptr, dynInst->get_mass_detach_from());

    delete dynInst2;
    delete dynInst;
}

TEST(BodyDetachSpecific, initialize)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    MockMassBody mockMassBody;
    Mock::VerifyAndClear(&mockMessageHandler);

    BodyDetachSpecificTest dynInst;
    dynInst.set_dyn_detach_from(&mockDynBody);
    dynInst.set_dyn_subject(&mockDynBody);
    dynInst.initialize(mockDynManager);
    Mock::VerifyAndClear(&mockMessageHandler);

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(BodyDetachSpecific, is_ready)
{
    BodyDetachSpecificTest * dynInst = new BodyDetachSpecificTest();

    EXPECT_FALSE(dynInst->is_ready());

    dynInst->active = true;

    EXPECT_TRUE(dynInst->is_ready());
}

TEST(BodyDetachSpecific, apply)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    MockDynBody mockDynBody2;
    MockMassBody mockMassBody;
    //	MockMassBody mockMassBody2;
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        // Test detach dyn_subject from DynBody parent. Succeeds
        // dyn_subject not null, dyn_detach_from not null
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);

        BodyDetachSpecificTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.set_dyn_detach_from(&mockDynBody2);
        EXPECT_CALL(mockDynBody, detach(Ref(mockDynBody2))).Times(1).WillOnce(Return(true));
        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
    }
    {
        // Test jettison mass_subject from DynBody parent. Succeeds
        // dyn_subject null, dyn_detact_from not null
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);

        BodyDetachSpecificTest dynInst;
        dynInst.set_dyn_detach_from(&mockDynBody);
        dynInst.set_mass_subject(&mockMassBody);
        EXPECT_CALL(mockDynBody, remove_mass_body(Ref(mockMassBody))).Times(1).WillOnce(Return(true));
        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
    }

    {
        // Test detach mass_subject from MassBody parent, dyn_subject null. Succeeds
        // dyn_subject null, dyn_detact_from null
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);

        BodyDetachSpecificTest dynInst;
        dynInst.set_mass_subject(&mockMassBody);
        dynInst.set_mass_detach_from(&mockMassBody);
        EXPECT_CALL(mockMassBody, detach(Ref(mockMassBody))).Times(1).WillOnce(Return(true));
        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockMassBody);
    }
    {
        // Test invalid setup. Fails, terminate_on_error set
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);

        BodyDetachSpecificTest dynInst;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.apply(mockDynManager);

        Mock::VerifyAndClear(&mockMessageHandler);
    }
    {
        // Test invalid setup. Fails, terminate_on_error not set
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Error, _, _, _, _, _, _)).Times(1);

        BodyDetachSpecificTest dynInst;
        dynInst.terminate_on_error = false;
        dynInst.set_dyn_subject(&mockDynBody);
        dynInst.apply(mockDynManager);

        Mock::VerifyAndClear(&mockMessageHandler);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(BodyDetachSpecific, set_detach_from_body)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    MockMassBody mockMassBody;
    Mock::VerifyAndClear(&mockMessageHandler);

    {
        // Test setting detach from body to DynBody
        BodyDetachSpecificTest dynInst;
        dynInst.set_detach_from_body(mockDynBody);
        EXPECT_EQ(nullptr, dynInst.get_mass_detach_from());
        EXPECT_EQ(&mockDynBody, dynInst.get_dyn_detach_from());
    }
    {
        // Test setting detach from body to MassBody
        BodyDetachSpecificTest dynInst;
        dynInst.set_detach_from_body(mockMassBody);
        EXPECT_EQ(nullptr, dynInst.get_dyn_detach_from());
        EXPECT_EQ(&mockMassBody, dynInst.get_mass_detach_from());
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}
