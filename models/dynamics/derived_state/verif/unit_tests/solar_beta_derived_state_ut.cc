/*
 * solar_beta_dervied_state_ut.cc
 *
 *  Created on: Aug 5, 2024
 *      Author: lmccartney
 */

#include "dyn_body_mock.hh"
#include "dyn_manager_mock.hh"
#include "dynamics/derived_state/include/solar_beta_derived_state.hh"
#include "ephem_ref_frame_mock.hh"
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

class SolarBetaDerivedStateTest : public SolarBetaDerivedState
{
public:
    Planet * find_planet(const DynManager & dyn_manager,
                         const std::string & planet_name,
                         const std::string & variable_name)
    {
        return SolarBetaDerivedState::find_planet(dyn_manager, planet_name, variable_name);
    }

    std::string get_state_identifier()
    {
        return state_identifier;
    }

    void modify_veh_wrt_planet()
    {
        veh_wrt_planet.trans.position[0] = 1;
        veh_wrt_planet.trans.position[1] = 1;
        veh_wrt_planet.trans.position[2] = 1;
        veh_wrt_planet.trans.velocity[0] = -2000;
        veh_wrt_planet.trans.velocity[1] = -10000;
        veh_wrt_planet.trans.velocity[2] = -5000;
    }
};

TEST(SolarBetaDerivedState, create)
{
    SolarBetaDerivedState staticInst;
    SolarBetaDerivedStateTest * dynInst;
    dynInst = new SolarBetaDerivedStateTest();
    SolarBetaDerivedState * dynInst2 = new SolarBetaDerivedState;

    EXPECT_EQ(nullptr, dynInst->subject);
    EXPECT_EQ("", dynInst->reference_name);
    EXPECT_EQ("", dynInst->get_state_identifier());

    delete dynInst2;
    delete dynInst;
}

TEST(SolarBetaDerivedState, initialize)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    Planet planet;

    {
        // active is false
        SolarBetaDerivedStateTest staticInst;
        staticInst.active = false;
        staticInst.initialize(mockDynBody, mockDynManager);
    }
    {
        // active is true
        SolarBetaDerivedStateTest staticInst;
        EXPECT_CALL(mockDynManager, find_planet(_)).Times(2).WillRepeatedly(Return(&planet));
        staticInst.initialize(mockDynBody, mockDynManager);
        Mock::VerifyAndClear(&mockDynManager);
    }
}

TEST(SolarBetaDerivedState, update)
{
    MockMessageHandler mockMessageHandler;
    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MockDynManager mockDynManager;
    MockDynBody mockDynBody;
    MockEphemerisRefFrame mockEphemerisRefFrame;
    Planet planet;
    {
        // active false, sun nullptr
        SolarBetaDerivedStateTest staticInst;
        staticInst.active = false;
        staticInst.update();
    }
    {
        // active true, sun nullptr
        SolarBetaDerivedStateTest staticInst;
        staticInst.sun = nullptr;
        staticInst.update();
    }
    {
        // active true, sun not nullptr
        SolarBetaDerivedStateTest staticInst;
        staticInst.sun = &planet;
        staticInst.planet = &planet;
        staticInst.subject = &mockDynBody;
        staticInst.update();
    }
    {
        // h_mag_sq > epsilon
        SolarBetaDerivedStateTest staticInst;
        staticInst.sun = &planet;
        staticInst.planet = &planet;
        staticInst.subject = &mockDynBody;
        staticInst.modify_veh_wrt_planet();
        staticInst.update();
    }
}
