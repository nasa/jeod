/*
 * propagated_planet_ut.cc
 */

#include "environment/ephemerides/propagated_planet/include/propagated_planet.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(PropagatedEphemerisPlanet, create) {}

TEST(PropagatedEphemerisPlanet, update) {}

TEST(PropagatedEphemerisOrientation, create) {}

TEST(PropagatedEphemerisOrientation, update) {}

TEST(PropagatedPlanet, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    PropagatedPlanet staticInst;
    PropagatedPlanet * dynInst = new PropagatedPlanet;
    delete dynInst;
}

TEST(PropagatedPlanet, activate) {}

TEST(PropagatedPlanet, deactivate) {}

TEST(PropagatedPlanet, timestamp) {}

TEST(PropagatedPlanet, get_name) {}

TEST(PropagatedPlanet, initialize_model) {}

TEST(PropagatedPlanet, ephem_initialize) {}

TEST(PropagatedPlanet, set_commanded_mode) {}

TEST(PropagatedPlanet, set_mode) {}

TEST(PropagatedPlanet, ephem_activate) {}

TEST(PropagatedPlanet, ephem_build_tree) {}

TEST(PropagatedPlanet, ephem_update) {}
