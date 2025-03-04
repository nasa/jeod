/*
 * flat_plate_radiation_factory_ut.cc
 */

#include "interactions/radiation_pressure/include/flat_plate_radiation_factory.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(FlatPlateRadiationFactory, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    FlatPlateRadiationFactory staticInst;
    FlatPlateRadiationFactory * dynInst = new FlatPlateRadiationFactory;
    delete dynInst;
}

TEST(FlatPlateRadiationFactory, create_facet) {}

TEST(FlatPlateRadiationFactory, is_correct_factory) {}
