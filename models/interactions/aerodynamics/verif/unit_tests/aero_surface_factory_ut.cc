/*
 * aero_surface_factory_ut.cc
 */

#include "interactions/aerodynamics/include/aero_surface_factory.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(AeroSurfaceFactory, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    AeroSurfaceFactory staticInst;
    AeroSurfaceFactory * dynInst = new AeroSurfaceFactory;
    delete dynInst;
}

TEST(AeroSurfaceFactory, add_facet_params) {}
