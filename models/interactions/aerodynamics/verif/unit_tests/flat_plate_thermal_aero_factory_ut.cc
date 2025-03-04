/*
 * flat_plate_thermal_aero_factory_ut.cc
 */

#include "interactions/aerodynamics/include/flat_plate_thermal_aero_factory.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(FlatPlateThermalAeroFactory, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    FlatPlateThermalAeroFactory staticInst;
    FlatPlateThermalAeroFactory * dynInst = new FlatPlateThermalAeroFactory;
    delete dynInst;
}

TEST(FlatPlateThermalAeroFactory, is_correct_factory) {}
