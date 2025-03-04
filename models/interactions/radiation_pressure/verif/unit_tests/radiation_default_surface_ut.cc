/*
 * radiation_default_surface_ut.cc
 */

#include "interactions/radiation_pressure/include/radiation_default_surface.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(RadiationDefaultSurface, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    RadiationDefaultSurface staticInst;
    RadiationDefaultSurface * dynInst = new RadiationDefaultSurface;
    delete dynInst;
}

TEST(RadiationDefaultSurface, initialize) {}

TEST(RadiationDefaultSurface, incident_radiation) {}

TEST(RadiationDefaultSurface, thermal_update) {}

TEST(RadiationDefaultSurface, add_thermal_integrator_to) {}
