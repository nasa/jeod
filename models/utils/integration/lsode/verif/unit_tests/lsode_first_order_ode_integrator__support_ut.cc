/*
 * lsode_first_order_ode_integrator__support_ut.cc
 */

#include "message_handler_mock.hh"
#include "utils/integration/lsode/include/lsode_first_order_ode_integrator.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(LsodeFirstOrderODEIntegrator, calculate_epsilon) {}

TEST(LsodeFirstOrderODEIntegrator, calculate_integration_coefficients) {}

TEST(LsodeFirstOrderODEIntegrator, interpolate_y) {}

TEST(LsodeFirstOrderODEIntegrator, jacobian_prep_init) {}

TEST(LsodeFirstOrderODEIntegrator, jacobian_prep_loop) {}

TEST(LsodeFirstOrderODEIntegrator, jacobian_prep_wrap_up) {}

TEST(LsodeFirstOrderODEIntegrator, linear_chord_iteration) {}

TEST(LsodeFirstOrderODEIntegrator, load_ew_values) {}
