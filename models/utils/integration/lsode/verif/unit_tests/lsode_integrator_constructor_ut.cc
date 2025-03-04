/*
 * lsode_integrator_constructor_ut.cc
 */

#include "message_handler_mock.hh"
#include "utils/integration/lsode/include/lsode_integrator_constructor.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(LsodeIntegratorConstructor, create) {}

TEST(LsodeIntegratorConstructor, create_constructor) {}

TEST(LsodeIntegratorConstructor, create_copy) {}

TEST(LsodeIntegratorConstructor, create_integration_controls) {}

TEST(LsodeIntegratorConstructor, create_first_order_ode_integrator) {}

TEST(LsodeIntegratorConstructor, create_second_order_ode_integrator) {}

TEST(LsodeIntegratorConstructor, create_generalized_deriv_second_order_ode_integrator) {}
