/*
 * gravity_interaction_ut.cc
 */

#include "environment/gravity/include/gravity_interaction.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(GravityInteraction, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    GravityInteraction staticInst;
    GravityInteraction * dynInst = new GravityInteraction;
    delete dynInst;
}

TEST(GravityInteraction, set_integ_frame) {}

TEST(GravityInteraction, add_control) {}

TEST(GravityInteraction, remove_control) {}

TEST(GravityInteraction, initialize_controls) {}

TEST(GravityInteraction, reset_controls) {}

TEST(GravityInteraction, sort_controls) {}
