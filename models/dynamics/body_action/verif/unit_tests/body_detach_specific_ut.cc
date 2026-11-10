/*
 * body_detach_specific_ut.cc
 *
 *  Created on: June 13, 2024
 *      Author: lmccartney
 */

#include "dyn_body_mock.hh"
#include "dyn_manager_mock.hh"
#include "dynamics/body_action/include/body_action_messages.hh"
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
    unsigned int shutdown_count{};

    void shutdown() override
    {
        ++shutdown_count;
        BodyAction::shutdown();
    }

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
    Mock::VerifyAndClear(&mockMessageHandler);
    {
        SCOPED_TRACE("Test detach dyn_subject from DynBody parent. Succeeds");
        // dyn_subject not null, dyn_detach_from not null
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);

        BodyDetachSpecificTest dynInst;
        dynInst.set_subject_body(mockDynBody);
        dynInst.set_detach_from_body(mockDynBody2);
        EXPECT_CALL(mockDynBody, detach(Ref(mockDynBody2))).Times(1).WillOnce(Return(true));
        dynInst.apply(mockDynManager);
        EXPECT_EQ(&mockDynBody.mass, dynInst.get_mass_subject());
        EXPECT_EQ(&mockDynBody2.mass, dynInst.get_mass_detach_from());
        EXPECT_EQ(1u, dynInst.shutdown_count);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
    }
    {
        SCOPED_TRACE("Test jettison mass_subject from DynBody parent. Succeeds");
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
        SCOPED_TRACE("Test detach mass_subject from MassBody parent, dyn_subject null. Succeeds");
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
        SCOPED_TRACE("DynBody subject cannot detach from standalone MassBody; terminate_on_error set");
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);

        BodyDetachSpecificTest dynInst;
        dynInst.set_subject_body(mockDynBody);
        dynInst.set_detach_from_body(mockMassBody);
        dynInst.apply(mockDynManager);

        Mock::VerifyAndClear(&mockMessageHandler);
    }
    {
        SCOPED_TRACE("DynBody subject cannot detach from standalone MassBody; terminate_on_error clear");
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Error, _, _, _, _, _, _)).Times(1);

        BodyDetachSpecificTest dynInst;
        dynInst.terminate_on_error = false;
        dynInst.set_subject_body(mockDynBody);
        dynInst.set_detach_from_body(mockMassBody);
        dynInst.apply(mockDynManager);

        Mock::VerifyAndClear(&mockMessageHandler);
    }
    {
        SCOPED_TRACE("Missing subject");

        BodyDetachSpecificTest dynInst;
        dynInst.set_detach_from_body(mockDynBody2);

        EXPECT_CALL(mockDynBody2, remove_mass_body(_)).Times(0);
        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Failure, _, _, _, BodyActionMessages::null_pointer, _, _))
            .Times(1);

        dynInst.apply(mockDynManager);

        EXPECT_EQ(0u, dynInst.shutdown_count);

        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody2);
    }
    {
        SCOPED_TRACE("Missing detach-from body even with terminate_on_error clear");

        BodyDetachSpecificTest dynInst;
        dynInst.set_subject_body(mockMassBody);
        dynInst.terminate_on_error = false;

        EXPECT_CALL(mockMassBody, detach(An<MassBody &>())).Times(0);
        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Failure, _, _, _, BodyActionMessages::null_pointer, _, _))
            .Times(1);

        dynInst.apply(mockDynManager);

        EXPECT_EQ(0u, dynInst.shutdown_count);

        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockMassBody);
    }
    {
        SCOPED_TRACE("Mismatched subject pointers");

        BodyDetachSpecificTest dynInst;
        dynInst.set_subject_body(mockDynBody);
        dynInst.set_mass_subject(&mockMassBody);
        dynInst.set_detach_from_body(mockDynBody2);

        EXPECT_CALL(mockDynBody, detach(Ref(mockDynBody2))).Times(0);
        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Failure, _, _, _, BodyActionMessages::fatal_error, _, _))
            .Times(1);

        dynInst.apply(mockDynManager);

        EXPECT_EQ(0u, dynInst.shutdown_count);

        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
    }
    {
        SCOPED_TRACE("Mismatched detach-from pointers");

        BodyDetachSpecificTest dynInst;
        dynInst.set_subject_body(mockDynBody);
        dynInst.set_detach_from_body(mockDynBody2);
        dynInst.set_mass_detach_from(&mockMassBody);

        EXPECT_CALL(mockDynBody, detach(Ref(mockDynBody2))).Times(0);
        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Failure, _, _, _, BodyActionMessages::fatal_error, _, _))
            .Times(1);

        dynInst.apply(mockDynManager);

        EXPECT_EQ(0u, dynInst.shutdown_count);

        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
    }
    {
        SCOPED_TRACE("Reassign initialized subject and detach-from body");

        BodyDetachSpecificTest dynInst;
        dynInst.set_subject_body(mockMassBody);
        dynInst.set_detach_from_body(mockDynBody);
        dynInst.initialize(mockDynManager);

        dynInst.set_subject_body(mockDynBody);
        dynInst.set_detach_from_body(mockDynBody2);
        ASSERT_EQ(nullptr, dynInst.get_mass_subject());
        ASSERT_EQ(nullptr, dynInst.get_mass_detach_from());

        EXPECT_CALL(mockDynBody, remove_mass_body(_)).Times(0);
        EXPECT_CALL(mockDynBody, detach(Ref(mockDynBody2))).Times(1).WillOnce(Return(true));
        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Debug, _, _, _, BodyActionMessages::trace, _, _))
            .Times(1);

        dynInst.apply(mockDynManager);

        EXPECT_EQ(&mockDynBody.mass, dynInst.get_mass_subject());
        EXPECT_EQ(&mockDynBody2.mass, dynInst.get_mass_detach_from());
        EXPECT_EQ(1u, dynInst.shutdown_count);

        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
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
        SCOPED_TRACE("Test setting detach from body to DynBody");
        BodyDetachSpecificTest dynInst;
        dynInst.set_detach_from_body(mockDynBody);
        EXPECT_EQ(nullptr, dynInst.get_mass_detach_from());
        EXPECT_EQ(&mockDynBody, dynInst.get_dyn_detach_from());
    }
    {
        SCOPED_TRACE("Test setting detach from body to MassBody");
        BodyDetachSpecificTest dynInst;
        dynInst.set_detach_from_body(mockMassBody);
        EXPECT_EQ(nullptr, dynInst.get_dyn_detach_from());
        EXPECT_EQ(&mockMassBody, dynInst.get_mass_detach_from());
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}
