/*
 * jeod_integration_group_ut.cc
 */

#include "message_handler_mock.hh"
#include "utils/integration/include/jeod_integration_group.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(JeodIntegrationGroup, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    JeodIntegrationGroup staticInst;
    JeodIntegrationGroup * dynInst = new JeodIntegrationGroup;
    delete dynInst;
}

TEST(JeodIntegrationGroup, register_classes) {}

TEST(JeodIntegrationGroup, add_integrable_object) {}

TEST(JeodIntegrationGroup, remove_integrable_object) {}

TEST(JeodIntegrationGroup, initialize_group) {}
