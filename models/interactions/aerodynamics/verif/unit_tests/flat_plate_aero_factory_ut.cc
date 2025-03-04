/*
 * flat_plate_aero_factory_ut.cc
 */

#include "interactions/aerodynamics/include/flat_plate_aero_factory.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(FlatPlateAeroFactory, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    FlatPlateAeroFactory staticInst;
    FlatPlateAeroFactory * dynInst = new FlatPlateAeroFactory;
    delete dynInst;
}

TEST(FlatPlateAeroFactory, create_facet) {}

TEST(FlatPlateAeroFactory, is_correct_factory) {}
