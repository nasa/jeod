/*
 * planetary_derived_state_ut.cc
 *
 *  Created on: Aug 6, 2024
 *      Author: lmccartney
 */

#include "dyn_body_mock.hh"
#include "dyn_manager_mock.hh"
#include "dynamics/derived_state/include/planetary_derived_state.hh"
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

class PlanetaryDerivedStateTest : public PlanetaryDerivedState
{
public:
    Planet * find_planet(const DynManager & dyn_manager,
                         const std::string & planet_name,
                         const std::string & variable_name)
    {
        return PlanetaryDerivedState::find_planet(dyn_manager, planet_name, variable_name);
    }

    std::string get_state_identifier()
    {
        return state_identifier;
    }

    bool get_use_alt_pfix()
    {
        return use_alt_pfix;
    }
};

TEST(PlanetaryDerivedState, create)
{
    PlanetaryDerivedState staticInst;
    PlanetaryDerivedStateTest * dynInst;
    dynInst = new PlanetaryDerivedStateTest();
    PlanetaryDerivedState * dynInst2 = new PlanetaryDerivedState;

    EXPECT_EQ(nullptr, dynInst->subject);
    EXPECT_EQ("", dynInst->reference_name);
    EXPECT_EQ("", dynInst->get_state_identifier());

    delete dynInst2;
    delete dynInst;
}

TEST(PlanetaryDerivedState, initialize)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    Planet planet;
    {
        // active is false
        PlanetaryDerivedStateTest staticInst;
        EXPECT_CALL(mockDynManager, find_planet(_)).Times(1).WillOnce(Return(&planet));
        staticInst.initialize(mockDynBody, mockDynManager);
        Mock::VerifyAndClear(&mockDynManager);
    }
    {
        // active is true
        PlanetaryDerivedStateTest staticInst;
        staticInst.set_use_alt_pfix(true);
        EXPECT_CALL(mockDynManager, find_planet(_)).Times(1).WillOnce(Return(&planet));
        staticInst.initialize(mockDynBody, mockDynManager);
        Mock::VerifyAndClear(&mockDynManager);
    }
}

TEST(PlanetaryDerivedState, set_use_alt_pfix)
{
    PlanetaryDerivedStateTest staticInst;

    EXPECT_FALSE(staticInst.get_use_alt_pfix());

    staticInst.set_use_alt_pfix(true);

    EXPECT_TRUE(staticInst.get_use_alt_pfix());
}

TEST(PlanetaryDerivedState, update)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    Planet planet;

    PlanetaryDerivedStateTest staticInst;
    staticInst.subject = &mockDynBody;
    EXPECT_CALL(mockDynManager, find_planet(_)).Times(1).WillOnce(Return(&planet));
    staticInst.initialize(mockDynBody, mockDynManager);
    staticInst.update();
}
