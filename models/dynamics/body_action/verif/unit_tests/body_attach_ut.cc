/*
 * body_attach_ut.cc
 *
 *  Created on: Nov 6, 2023
 *      Author: tbrain
 */

#include <array>

#include "dyn_body_mock.hh"
#include "dyn_manager_mock.hh"
#include "dynamics/body_action/include/body_attach.hh"
#include "mass_mock.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

class BodyAttachTest : public BodyAttach
{
public:
    MassBody * get_mass_parent()
    {
        return mass_parent;
    }

    DynBody * get_dyn_parent()
    {
        return dyn_parent;
    }

    RefFrame * get_ref_parent()
    {
        return ref_parent;
    }

    void set_mass_parent(MassBody * in)
    {
        mass_parent = in;
    }

    void set_dyn_parent(DynBody * in)
    {
        dyn_parent = in;
    }

    void set_ref_parent(RefFrame * in)
    {
        ref_parent = in;
    }
};

BodyAttach gInst;

TEST(BodyAttach, create)
{
    BodyAttach staticInst;
    BodyAttachTest * dynInst;
    dynInst = new BodyAttachTest();
    BodyAttach * dynInst2 = new BodyAttach();

    EXPECT_FALSE(dynInst->succeeded);
    EXPECT_TRUE(dynInst->active);
    EXPECT_EQ(nullptr, dynInst->get_dyn_parent());
    EXPECT_EQ(nullptr, dynInst->get_mass_parent());
    EXPECT_EQ(nullptr, dynInst->get_ref_parent());

    delete dynInst2;
    delete dynInst;
}

TEST(BodyAttach, initialize)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    RefFrame refFrame;
    Mock::VerifyAndClear(&mockMessageHandler);

    {
        // No parent of any type set for validate_body_inputs call and no parent RefFrame. Error
        BodyAttachTest staticInst;
        staticInst.set_subject_body(mockDynBody);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        staticInst.initialize(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    {
        // No parent of any type set for validate_body_inputs call but valie parent RefFrame. Success
        BodyAttachTest staticInst;
        staticInst.set_subject_body(mockDynBody);
        staticInst.set_ref_parent(&refFrame);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(0);
        staticInst.initialize(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    {
        // A parent is set for validate_body_inputs call. Success
        BodyAttachTest staticInst;
        staticInst.set_subject_body(mockDynBody);
        staticInst.set_dyn_parent(&mockDynBody);
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(0);
        staticInst.initialize(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(BodyAttach, set_parent_body)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynBody mockDynBody;
    MockMassBody mockMassBody;
    RefFrame refFrame;

    {
        // Test setting parent DynBody will clear MassBody parent and RefFrame parent pointers
        BodyAttachTest staticInst;
        staticInst.set_dyn_parent(nullptr);
        staticInst.set_mass_parent(&mockMassBody);
        staticInst.set_ref_parent(&refFrame);
        staticInst.set_parent_body(mockDynBody);
        EXPECT_EQ(staticInst.get_dyn_parent(), &mockDynBody);
        EXPECT_EQ(staticInst.get_mass_parent(), nullptr);
        EXPECT_EQ(staticInst.get_ref_parent(), nullptr);
    }

    {
        // Test setting parent MassBody will clear DynBody parent and RefFrame parent pointers
        BodyAttachTest staticInst;
        staticInst.set_dyn_parent(&mockDynBody);
        staticInst.set_mass_parent(nullptr);
        staticInst.set_ref_parent(&refFrame);
        staticInst.set_parent_body(mockMassBody);
        EXPECT_EQ(staticInst.get_dyn_parent(), nullptr);
        EXPECT_EQ(staticInst.get_mass_parent(), &mockMassBody);
        EXPECT_EQ(staticInst.get_ref_parent(), nullptr);
    }
}

TEST(BodyAttach, set_parent_frame)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynBody mockDynBody;
    MockMassBody mockMassBody;
    RefFrame refFrame;

    // Test setting parent RefRrame will clear MassBody parent and DynBody parent pointers
    BodyAttachTest staticInst;
    staticInst.set_dyn_parent(&mockDynBody);
    staticInst.set_mass_parent(&mockMassBody);
    staticInst.set_ref_parent(nullptr);
    staticInst.set_parent_frame(refFrame);
    EXPECT_EQ(staticInst.get_dyn_parent(), nullptr);
    EXPECT_EQ(staticInst.get_mass_parent(), nullptr);
    EXPECT_EQ(staticInst.get_ref_parent(), &refFrame);
}

TEST(BodyAttach, apply)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    mockDynBody.mass.set_name("TestDynBody");
    MockDynBody mockDynBody2;
    mockDynBody2.mass.set_name("TestDynBody 2");
    MockMassBody mockMassBody;
    mockMassBody.set_name("TestMassBody");
    MockMassBody mockMassBody2;
    mockMassBody2.set_name("TestMassBody 2");
    RefFrame refFrame;
    refFrame.set_name("TestRefFrame");
    Mock::VerifyAndClear(&mockMessageHandler);

    std::array<std::string, 3> expStringArgs({"", "", ""});

    {
        // Test succeeded action with a DynBody parent and subject. Not allowed to fail
        BodyAttachTest staticInst;
        staticInst.set_subject_body(mockDynBody);
        staticInst.set_dyn_parent(&mockDynBody2);
        staticInst.succeeded = true;
        expStringArgs[0] = "";
        expStringArgs[1] = mockDynBody.mass.name.c_str();
        expStringArgs[2] = mockDynBody2.mass.name.c_str();
        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Debug, _, _, _, _, _, AreVarArgumentsEqual(expStringArgs)))
            .Times(1);
        staticInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    {
        // Test failed action with a MassBody parent and subject. Not allowed to fail
        BodyAttachTest staticInst;
        staticInst.set_subject_body(mockMassBody);
        staticInst.set_mass_parent(&mockMassBody2);
        staticInst.succeeded = false;
        staticInst.terminate_on_error = true;
        expStringArgs[0] = "";
        expStringArgs[1] = mockMassBody.name.c_str();
        expStringArgs[2] = mockMassBody2.name.c_str();
        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Failure, _, _, _, _, _, AreVarArgumentsEqual(expStringArgs)))
            .Times(1);
        staticInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    {
        // Test failed action with a RefFrame parent but no subject. Allowed to fail.
        BodyAttachTest staticInst;
        staticInst.set_ref_parent(&refFrame);
        staticInst.succeeded = false;
        staticInst.terminate_on_error = false;
        expStringArgs[0] = "";
        expStringArgs[1] = "Unknown";
        expStringArgs[2] = refFrame.get_name();
        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Error, _, _, _, _, _, AreVarArgumentsEqual(expStringArgs)))
            .Times(1);
        staticInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    {
        // Test failed action with no parent or subject. Allowed to fail.
        BodyAttachTest staticInst;
        staticInst.succeeded = false;
        staticInst.terminate_on_error = false;
        expStringArgs[0] = "";
        expStringArgs[1] = "Unknown";
        expStringArgs[2] = "Unknown";
        EXPECT_CALL(mockMessageHandler,
                    process_message(MessageHandler::Error, _, _, _, _, _, AreVarArgumentsEqual(expStringArgs)))
            .Times(1);
        staticInst.apply(mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}
