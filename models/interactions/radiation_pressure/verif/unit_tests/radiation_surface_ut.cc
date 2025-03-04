/*
 * radiation_surface_ut.cc
 */

#include "interactions/radiation_pressure/include/radiation_surface.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(RadiationSurface, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    RadiationSurface staticInst;
    RadiationSurface * dynInst = new RadiationSurface;
    delete dynInst;
}

TEST(RadiationSurface, initialize) {}

TEST(RadiationSurface, allocate_array) {}

TEST(RadiationSurface, allocate_interaction_facet) {}

TEST(RadiationSurface, incident_radiation) {}

TEST(RadiationSurface, interact_with_third_body) {}

TEST(RadiationSurface, accumulate_thermal_sources) {}

TEST(RadiationSurface, thermal_integrator) {}

TEST(RadiationSurface, equalize_absorption_emission) {}

TEST(RadiationSurface, radiation_pressure) {}

TEST(RadiationSurface, initialize_runtime_values) {}

TEST(RadiationSurface, add_thermal_integrators_to) {}
