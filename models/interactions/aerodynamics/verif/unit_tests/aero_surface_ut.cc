/*
 * aero_surface_ut.cc
 */

#include "interactions/aerodynamics/include/aero_surface.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(AeroSurface, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    AeroSurface staticInst;
    AeroSurface * dynInst = new AeroSurface;
    delete dynInst;
}

TEST(AeroSurface, allocate_array) {}

TEST(AeroSurface, allocate_interaction_facet) {}
