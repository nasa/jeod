/*
 * gravity_manager_ut.cc
 */

#include "environment/gravity/include/gravity_manager.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(GravityManager, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    GravityManager staticInst;
    GravityManager * dynInst = new GravityManager;
    delete dynInst;
}

TEST(GravityManager, find_grav_source) {}

TEST(GravityManager, add_grav_source) {}

TEST(GravityManager, initialize_model) {}

TEST(GravityManager, initialize_state) {}

TEST(GravityManager, gravitation) {}
