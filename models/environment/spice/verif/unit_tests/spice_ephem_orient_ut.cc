/*
 * spice_ephem_orient_ut.cc
 */

#include "environment/spice/include/spice_ephem_orient.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(SpiceEphemOrientation, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    SpiceEphemOrientation staticInst;
    SpiceEphemOrientation * dynInst = new SpiceEphemOrientation;
    delete dynInst;
}

TEST(SpiceEphemOrientation, update) {}

TEST(SpiceEphemOrientation, validate) {}

TEST(SpiceEphemOrientation, get_spice_transformation) {}
