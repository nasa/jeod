/*
 * lsode_first_order_ode_integrator__integrator_ut.cc
 */

#include "message_handler_mock.hh"
#include "utils/integration/lsode/include/lsode_first_order_ode_integrator.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(LsodeFirstOrderODEIntegrator, integrator_core) {}

TEST(LsodeFirstOrderODEIntegrator, integrator_reset_method_coeffs) {}

TEST(LsodeFirstOrderODEIntegrator, integrator_test_stepsize_change) {}

TEST(LsodeFirstOrderODEIntegrator, integrator_reset_yh) {}

TEST(LsodeFirstOrderODEIntegrator, integrator_predict) {}

TEST(LsodeFirstOrderODEIntegrator, integrator_reset_iteration_loop_part1) {}

TEST(LsodeFirstOrderODEIntegrator, integrator_reset_iteration_loop_part2) {}

TEST(LsodeFirstOrderODEIntegrator, integrator_corrector_iteration) {}

TEST(LsodeFirstOrderODEIntegrator, integrator_corrector_failed_part1) {}

TEST(LsodeFirstOrderODEIntegrator, integrator_corrector_failed_part2) {}

TEST(LsodeFirstOrderODEIntegrator, integrator_corrector_converged) {}

TEST(LsodeFirstOrderODEIntegrator, integrator_error_test_failed) {}

TEST(LsodeFirstOrderODEIntegrator, integrator_compute_new_order_prep) {}

TEST(LsodeFirstOrderODEIntegrator, integrator_compute_new_order) {}

TEST(LsodeFirstOrderODEIntegrator, integrator_compute_new_order_check_step_error) {}

TEST(LsodeFirstOrderODEIntegrator, integrator_set_new_order) {}

TEST(LsodeFirstOrderODEIntegrator, integrator_fail_reset_order_1_part1) {}

TEST(LsodeFirstOrderODEIntegrator, integrator_fail_reset_order_1_part2) {}

TEST(LsodeFirstOrderODEIntegrator, integrator_wrapup) {}

TEST(LsodeFirstOrderODEIntegrator, integrator_terminate) {}
