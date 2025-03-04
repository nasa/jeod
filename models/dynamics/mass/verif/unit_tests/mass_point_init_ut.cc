/*
 * mass_point_init_ut.cc
 */

#include "dynamics/mass/include/mass_point_init.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(MassPointInit, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MassPointInit staticInst;
    MassPointInit * dynInst = new MassPointInit;
    delete dynInst;
}

TEST(MassPointInit, initialize_mass_point) {}
