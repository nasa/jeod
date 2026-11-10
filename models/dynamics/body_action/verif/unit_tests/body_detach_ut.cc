/*
 * body_detach_ut.cc
 *
 *  Created on: June 6, 2024
 *      Author: lmccartney
 */

#include "dyn_body_mock.hh"
#include "dyn_manager_mock.hh"
#include "dynamics/body_action/include/body_action_messages.hh"
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
        SCOPED_TRACE("Detach DynBody without action initialization");
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);

        BodyDetachTest dynInst;
        dynInst.set_subject_body(mockDynBody);
        ASSERT_EQ(nullptr, dynInst.get_mass_subject());
        EXPECT_CALL(mockDynBody, detach()).Times(1).WillOnce(Return(true));
        dynInst.apply(mockDynManager);
        EXPECT_EQ(&mockDynBody.mass, dynInst.get_mass_subject());
        EXPECT_EQ(1u, dynInst.shutdown_count);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
    }
    {
        SCOPED_TRACE("Detach standalone MassBody");
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Debug, _, _, _, _, _, _)).Times(1);

        BodyDetachTest dynInst;
        dynInst.set_subject_body(mockMassBody);
        EXPECT_CALL(mockMassBody, detach()).Times(1).WillOnce(Return(true));
        dynInst.apply(mockDynManager);
        EXPECT_EQ(&mockMassBody, dynInst.get_mass_subject());
        EXPECT_EQ(nullptr, dynInst.get_dyn_subject());
        EXPECT_EQ(1u, dynInst.shutdown_count);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockMassBody);
    }
    {
        SCOPED_TRACE("Detach fails with terminate_on_error set");
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);

        BodyDetachTest dynInst;
        dynInst.set_subject_body(mockDynBody);
        EXPECT_CALL(mockDynBody, detach()).Times(1).WillOnce(Return(false));
        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
    }
    {
        SCOPED_TRACE("Detach fails with terminate_on_error clear");
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Error, _, _, _, _, _, _)).Times(1);

        BodyDetachTest dynInst;
        dynInst.set_subject_body(mockDynBody);
        dynInst.terminate_on_error = false;
        EXPECT_CALL(mockDynBody, detach()).Times(1).WillOnce(Return(false));
        dynInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
    }
    {
        SCOPED_TRACE("Reassign initialized MassBody subject to DynBody");

        BodyDetachTest dynInst;
        dynInst.set_subject_body(mockMassBody);
        dynInst.initialize(mockDynManager);
        dynInst.set_subject_body(mockDynBody);
        ASSERT_EQ(nullptr, dynInst.get_mass_subject());

        EXPECT_CALL(mockMassBody, detach()).Times(0);
        EXPECT_CALL(mockDynBody, detach()).Times(1).WillOnce(Return(true));
        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Debug, _, _, _, BodyActionMessages::trace, _, _))
            .Times(1);

        dynInst.apply(mockDynManager);

        EXPECT_EQ(&mockDynBody.mass, dynInst.get_mass_subject());
        EXPECT_EQ(1u, dynInst.shutdown_count);

        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockMassBody);
    }
    {
        SCOPED_TRACE("Reassign initialized DynBody subject to MassBody");

        BodyDetachTest dynInst;
        dynInst.set_subject_body(mockDynBody);
        dynInst.initialize(mockDynManager);
        dynInst.set_subject_body(mockMassBody);

        EXPECT_CALL(mockDynBody, detach()).Times(0);
        EXPECT_CALL(mockMassBody, detach()).Times(1).WillOnce(Return(true));
        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Debug, _, _, _, BodyActionMessages::trace, _, _))
            .Times(1);

        dynInst.apply(mockDynManager);

        EXPECT_EQ(&mockMassBody, dynInst.get_mass_subject());
        EXPECT_EQ(nullptr, dynInst.get_dyn_subject());
        EXPECT_EQ(1u, dynInst.shutdown_count);

        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockMassBody);
    }
    {
        SCOPED_TRACE("Missing subject");

        BodyDetachTest dynInst;

        EXPECT_CALL(mockDynBody, detach()).Times(0);
        EXPECT_CALL(mockMassBody, detach()).Times(0);
        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Failure, _, _, _, BodyActionMessages::null_pointer, _, _))
            .Times(1);

        dynInst.apply(mockDynManager);

        EXPECT_EQ(nullptr, dynInst.get_mass_subject());
        EXPECT_EQ(0u, dynInst.shutdown_count);

        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockMassBody);
    }
    {
        SCOPED_TRACE("Mismatched subject pointers");

        BodyDetachTest dynInst;
        dynInst.set_subject_body(mockDynBody);
        dynInst.set_mass_subject(&mockMassBody);

        EXPECT_CALL(mockDynBody, detach()).Times(0);
        EXPECT_CALL(mockMassBody, detach()).Times(0);
        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Failure, _, _, _, BodyActionMessages::fatal_error, _, _))
            .Times(1);

        dynInst.apply(mockDynManager);

        EXPECT_EQ(0u, dynInst.shutdown_count);

        Mock::VerifyAndClear(&mockMessageHandler);
        Mock::VerifyAndClear(&mockDynBody);
        Mock::VerifyAndClear(&mockMassBody);
    }
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}
