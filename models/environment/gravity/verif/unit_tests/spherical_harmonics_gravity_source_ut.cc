/*
 * spherical_harmonics_gravity_source_ut.cc
 */

#include "environment/gravity/include/spherical_harmonics_gravity_source.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(SphericalHarmonicsGravitySource, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    SphericalHarmonicsGravitySource staticInst;
    SphericalHarmonicsGravitySource * dynInst = new SphericalHarmonicsGravitySource;
    delete dynInst;
}

TEST(SphericalHarmonicsGravitySource, initialize_body) {}

TEST(SphericalHarmonicsGravitySource, find_deltacoeff) {}

TEST(SphericalHarmonicsGravitySource, add_deltacoeff) {}
