/*
 * radiation_surface_factory_ut.cc
 */

#include "interactions/radiation_pressure/include/radiation_surface_factory.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(RadiationSurfaceFactory, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    RadiationSurfaceFactory staticInst;
    RadiationSurfaceFactory * dynInst = new RadiationSurfaceFactory;
    delete dynInst;
}

TEST(RadiationSurfaceFactory, add_facet_params) {}
