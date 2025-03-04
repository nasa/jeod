/*
 * radiation_pressure_ut.cc
 */

#include "interactions/radiation_pressure/include/radiation_pressure.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(RadiationPressure, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    RadiationPressure staticInst;
    RadiationPressure * dynInst = new RadiationPressure;
    delete dynInst;
}

TEST(RadiationPressure, initialize_environment) {}

TEST(RadiationPressure, update) {}

TEST(RadiationPressure, third_body_adjustments) {}

TEST(RadiationPressure, add_third_body) {}

TEST(RadiationPressure, set_third_body_active) {}

TEST(RadiationPressure, set_third_body_inactive) {}

TEST(RadiationPressure, find_third_body) {}

TEST(RadiationPressure, set_calculate_forces) {}
