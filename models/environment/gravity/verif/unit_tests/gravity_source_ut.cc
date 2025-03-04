/*
 * gravity_source_ut.cc
 */

#include "environment/gravity/include/gravity_source.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(GravitySource, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    GravitySource staticInst;
    GravitySource * dynInst = new GravitySource;
    delete dynInst;
}

TEST(GravitySource, initialize_state) {}
