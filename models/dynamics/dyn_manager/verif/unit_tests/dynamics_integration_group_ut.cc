/*
 * dynamics_integration_group_ut.cc
 */

#include "dynamics/dyn_manager/include/dynamics_integration_group.hh"
#include "message_handler_mock.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(DynamicsIntegrationGroup, create)
{
    MockMessageHandler mockMessageHandler;

    EXPECT_CALL(mockMessageHandler, process_message(_, _, _, _, _, _, _)).Times(AnyNumber());
    DynamicsIntegrationGroup staticInst;
    DynamicsIntegrationGroup * dynInst = new DynamicsIntegrationGroup;
    delete dynInst;
}

TEST(DynamicsIntegrationGroup, register_base_contents) {}

TEST(DynamicsIntegrationGroup, create_group) {}

TEST(DynamicsIntegrationGroup, register_group) {}

TEST(DynamicsIntegrationGroup, initialize_group) {}

TEST(DynamicsIntegrationGroup, add_dyn_body) {}

TEST(DynamicsIntegrationGroup, delete_dyn_body) {}

TEST(DynamicsIntegrationGroup, prepare_for_integ_loop) {}

TEST(DynamicsIntegrationGroup, gravitation) {}

TEST(DynamicsIntegrationGroup, collect_derivatives) {}

TEST(DynamicsIntegrationGroup, reset_body_integrators) {}

TEST(DynamicsIntegrationGroup, integrate_bodies) {}
