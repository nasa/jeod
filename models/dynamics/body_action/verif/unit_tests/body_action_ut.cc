/*
 * body_action_ut.cc
 *
 *  Created on: Oct 9, 2023
 *      Author: tbrain
 */

#include "dyn_body_mock.hh"
#include "dyn_manager_mock.hh"
#include "dynamics/body_action/include/body_action.hh"
#include "mass_mock.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

class BodyActionTest : public BodyAction
{
public:
    bool validate_body_inputs(DynBody *& dyn_body_in,
                              MassBody *& mass_body_in,
                              const std::string & body_base_name,
                              bool allow_failure = false) override
    {
        return BodyAction::validate_body_inputs(dyn_body_in, mass_body_in, body_base_name, allow_failure);
    }

    void validate_name(const std::string & variable_value,
                       const std::string & variable_name,
                       const std::string & variable_type)
    {
        BodyAction::validate_name(variable_value, variable_name, variable_type);
    }

    MassBody * get_mass_subject()
    {
        return mass_subject;
    }

    DynBody * get_dyn_subject()
    {
        return dyn_subject;
    }

    void set_mass_subject(MassBody * in)
    {
        mass_subject = in;
    }

    void set_dyn_subject(DynBody * in)
    {
        dyn_subject = in;
    }
};

TEST(BodyAction, create)
{
    BodyAction staticInst;
    BodyActionTest * dynInst;
    dynInst = new BodyActionTest();
    BodyAction * dynInst2 = new BodyAction();

    EXPECT_TRUE(dynInst->active);
    EXPECT_TRUE(dynInst->terminate_on_error);
    EXPECT_EQ(std::string(""), dynInst->action_name);

    EXPECT_EQ(std::string(""), dynInst->get_identifier());
    EXPECT_EQ(nullptr, dynInst->get_dyn_subject());
    EXPECT_EQ(nullptr, dynInst->get_mass_subject());

    delete dynInst2;
    delete dynInst;
}

TEST(BodyAction, statelessCalls)
{
    // Special test for coverage only. Need to call something that doesn't have a state of variable to check?
    // Add it here.
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;

    BodyAction staticInst;
    staticInst.apply(mockDynManager);
}

TEST(BodyAction, initialize)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    Mock::VerifyAndClear(&mockMessageHandler);

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(0);

    {
        // Test initialization with a non-empty action_name
        BodyAction action;
        action.action_name = "test";
        action.set_subject_body(mockDynBody);
        action.initialize(mockDynManager);
        EXPECT_EQ("jeod::BodyAction.test", action.get_identifier());
    }

    {
        // Test initialization with an empty action_name
        BodyAction action;
        action.set_subject_body(mockDynBody);
        action.initialize(mockDynManager);
        EXPECT_EQ("jeod::BodyAction.unnamed instance", action.get_identifier());
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(BodyAction, is_ready)
{
    BodyAction staticInst;

    EXPECT_TRUE(staticInst.is_ready());

    staticInst.active = false;

    EXPECT_FALSE(staticInst.is_ready());
}

TEST(BodyAction, set_subject_body)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynBody mockDynBody;
    MockMassBody mockMassBody;

    {
        // Test that setting the subject body to a DynBody clears the mass_subject pointer.
        BodyActionTest staticInst;
        staticInst.set_mass_subject(&mockMassBody);
        staticInst.set_subject_body(mockDynBody);
        EXPECT_EQ(staticInst.get_mass_subject(), nullptr);
        EXPECT_EQ(staticInst.get_dyn_subject(), &mockDynBody);
    }

    {
        // Test that setting the subject body to a MassBody clears the dyn_subject pointer.
        BodyActionTest staticInst;
        staticInst.set_dyn_subject(&mockDynBody);
        staticInst.set_subject_body(mockMassBody);
        EXPECT_EQ(staticInst.get_mass_subject(), &mockMassBody);
        EXPECT_EQ(staticInst.get_dyn_subject(), nullptr);
    }
}

TEST(BodyAction, validate_body_inputs)
{
    MockMessageHandler mockMessageHandler;

    DynBody * bodyPtr = nullptr;
    MassBody * massPtr = nullptr;

    bool ret = false;
    BodyActionTest action;

    {
        // Both input ptrs are null, do not allow_failure
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        ret = action.validate_body_inputs(bodyPtr, massPtr, "");
        EXPECT_FALSE(ret);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    {
        // Both input ptrs are null, allow_failure
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(0);
        ret = action.validate_body_inputs(bodyPtr, massPtr, "", true);
        EXPECT_FALSE(ret);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    // Reset and add mocks
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynBody mockDynBody;
    MockMassBody mockMassBody;
    Mock::VerifyAndClear(&mockMessageHandler);

    {
        // Both are non-null ptrs but do not belong to the same DynBody, do not allow failure
        bodyPtr = &mockDynBody;
        massPtr = &mockMassBody;
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        ret = action.validate_body_inputs(bodyPtr, massPtr, "");
        EXPECT_FALSE(ret);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    {
        // Both are non-null ptrs but do not belong to the same DynBody, allow failure
        bodyPtr = &mockDynBody;
        massPtr = &mockMassBody;
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(0);
        ret = action.validate_body_inputs(bodyPtr, massPtr, "", true);
        EXPECT_FALSE(ret);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    {
        // Both non-null ptrs and belong to the same body. Success
        bodyPtr = &mockDynBody;
        massPtr = &(mockDynBody.mass);
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(0);
        ret = action.validate_body_inputs(bodyPtr, massPtr, "");
        EXPECT_TRUE(ret);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    {
        // DynBody non-null ptr. Success
        bodyPtr = &mockDynBody;
        massPtr = nullptr;
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(0);
        ret = action.validate_body_inputs(bodyPtr, massPtr, "");
        EXPECT_TRUE(ret);
        EXPECT_EQ(&mockDynBody.mass, massPtr);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    {
        // MassBody non-null ptr. Success
        bodyPtr = nullptr;
        massPtr = &mockMassBody;
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(0);
        ret = action.validate_body_inputs(bodyPtr, massPtr, "");
        EXPECT_TRUE(ret);
        EXPECT_EQ(nullptr, bodyPtr);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}

TEST(BodyAction, is_same_subject_body)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynBody mockDynBody;
    MockMassBody mockMassBody;

    {
        // Test against DynBody subject
        BodyActionTest staticInst;
        staticInst.set_dyn_subject(&mockDynBody);
        EXPECT_FALSE(staticInst.is_same_subject_body(mockMassBody));
        EXPECT_TRUE(staticInst.is_same_subject_body(mockDynBody.mass));
    }

    {
        // Test against MassBody subject
        BodyActionTest staticInst;
        staticInst.set_mass_subject(&mockMassBody);
        EXPECT_TRUE(staticInst.is_same_subject_body(mockMassBody));
        EXPECT_FALSE(staticInst.is_same_subject_body(mockDynBody.mass));
    }

    {
        // Test against DynBody's mass variable
        BodyActionTest staticInst;
        staticInst.set_mass_subject(&mockDynBody.mass);
        EXPECT_FALSE(staticInst.is_same_subject_body(mockMassBody));
        EXPECT_TRUE(staticInst.is_same_subject_body(mockDynBody.mass));
    }
}

TEST(BodyAction, is_subject_dyn_body)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynBody mockDynBody;
    MockMassBody mockMassBody;

    {
        // Test with no subject set
        BodyActionTest staticInst;
        EXPECT_FALSE(staticInst.is_subject_dyn_body());
    }

    {
        // Test with DynBody subject
        BodyActionTest staticInst;
        staticInst.set_dyn_subject(&mockDynBody);
        EXPECT_TRUE(staticInst.is_subject_dyn_body());
    }

    {
        // Test with DynBody's mass variable
        BodyActionTest staticInst;
        staticInst.set_mass_subject(&mockDynBody.mass);
        EXPECT_TRUE(staticInst.is_subject_dyn_body());
    }

    {
        // Test with MassBody subject
        BodyActionTest staticInst;
        staticInst.set_mass_subject(&mockMassBody);
        EXPECT_FALSE(staticInst.is_subject_dyn_body());
    }
}

TEST(BodyAction, get_subject_dyn_body)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynBody mockDynBody;
    MockMassBody mockMassBody;

    {
        // Test with no subject set
        BodyActionTest staticInst;
        EXPECT_EQ(staticInst.get_subject_dyn_body(), nullptr);
    }

    {
        // Test with DynBody subject
        BodyActionTest staticInst;
        staticInst.set_dyn_subject(&mockDynBody);
        EXPECT_EQ(staticInst.get_subject_dyn_body(), &mockDynBody);
    }

    {
        // Test with DynBody's mass variable
        BodyActionTest staticInst;
        staticInst.set_mass_subject(&mockDynBody.mass);
        EXPECT_EQ(staticInst.get_subject_dyn_body(), &mockDynBody);
    }

    {
        // Test with MassBody subject
        BodyActionTest staticInst;
        staticInst.set_mass_subject(&mockMassBody);
        EXPECT_EQ(staticInst.get_subject_dyn_body(), nullptr);
    }
}

TEST(BodyAction, validate_name)
{
    BodyActionTest staticInst;

    MockMessageHandler mockMessageHandler;
    {
        // If first argument isn't empty, no error.
        EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(0);
        staticInst.validate_name("foo", "", "");
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    {
        // If first argument is empty, error.
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        staticInst.validate_name("", "", "");
        Mock::VerifyAndClear(&mockMessageHandler);
    }
}
