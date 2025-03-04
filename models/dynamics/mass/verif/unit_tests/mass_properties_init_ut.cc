/*
 * mass_properties_init_ut.cc
 */

#include "dynamics/mass/include/mass_properties_init.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(MassPropertiesInit, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    MassPropertiesInit staticInst;
    MassPropertiesInit * dynInst = new MassPropertiesInit;
    delete dynInst;
}

TEST(MassPropertiesInit, initialize_mass_properties) {}
