/*
 * dervied_state_ut.cc
 *
 *  Created on: Nov 7, 2023
 *      Author: tbrain
 */

#include "dyn_body_mock.hh"
#include "dyn_manager_mock.hh"
#include "dynamics/derived_state/include/derived_state.hh"
#include "mass_mock.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::An;
using testing::AnyNumber;
using testing::ByRef;
using testing::Matcher;
using testing::Mock;
using testing::Return;
using testing::ReturnRef;
using testing::TypedEq;

using namespace jeod;

class DerivedStateTest : public DerivedState
{
public:
    Planet * find_planet(const DynManager & dyn_manager,
                         const std::string & planet_name,
                         const std::string & variable_name)
    {
        return DerivedState::find_planet(dyn_manager, planet_name, variable_name);
    }

    std::string get_state_identifier()
    {
        return state_identifier;
    }
};

TEST(DerivedState, create)
{
    DerivedState staticInst;
    DerivedStateTest * dynInst;
    dynInst = new DerivedStateTest();
    DerivedState * dynInst2 = new DerivedState;

    EXPECT_EQ(nullptr, dynInst->subject);
    EXPECT_EQ("", dynInst->reference_name);
    EXPECT_EQ("", dynInst->get_state_identifier());

    delete dynInst2;
    delete dynInst;
}

TEST(DerivedState, statelessCalls)
{
    // Special test for coverage only. Need to call something that doesn't have a state of variable to check?
    // Add it here.
    DerivedState staticInst;
    staticInst.update();
}

TEST(DerivedState, set_reference_name)
{
    DerivedStateTest staticInst;
    staticInst.set_reference_name(std::string("TestName"));
    EXPECT_EQ("TestName", staticInst.reference_name);
}

TEST(DerivedState, initialize)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    mockDynBody.set_name("TestDynBody");

    {
        DerivedStateTest staticInst;
        staticInst.reference_name = "refname";
        staticInst.initialize(mockDynBody, mockDynManager);

        EXPECT_EQ(&mockDynBody, staticInst.subject);
        EXPECT_EQ("DerivedStateTest.TestDynBody.refname", staticInst.get_state_identifier());
    }
    {
        DerivedStateTest staticInst;
        staticInst.initialize(mockDynBody, mockDynManager);

        EXPECT_EQ(&mockDynBody, staticInst.subject);
        EXPECT_EQ("DerivedStateTest.TestDynBody", staticInst.get_state_identifier());
    }
}

TEST(DerivedState, find_planet)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    Planet planet;
    Mock::VerifyAndClear(&mockMessageHandler);

    {
        // Test found planet. Succeeds
        EXPECT_CALL(mockDynManager, find_planet(_)).WillOnce(Return(&planet));
        DerivedStateTest staticInst;
        Planet * ret = staticInst.find_planet(mockDynManager, "planet_name", "variable_name");
        EXPECT_EQ(ret, &planet);
        Mock::VerifyAndClear(&mockDynManager);
    }

    {
        // Test missing planet case. Fails
        EXPECT_CALL(mockMessageHandler, process_message(MessageHandler::Failure, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(mockDynManager, find_planet(_)).WillOnce(Return(nullptr));
        DerivedStateTest staticInst;
        Planet * ret = staticInst.find_planet(mockDynManager, "planet_name", "variable_name");
        EXPECT_EQ(nullptr, ret);
        Mock::VerifyAndClear(&mockDynManager);
        Mock::VerifyAndClear(&mockMessageHandler);
    }

    // For non-unit destructor process_message calls.
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
}
