/*
 * lsode_first_order_ode_integrator__manager_ut.cc
 */

#include "message_handler_mock.hh"
#include "utils/integration/lsode/include/lsode_first_order_ode_integrator.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using testing::_;
using testing::AnyNumber;
using testing::Mock;

using namespace jeod;

TEST(LsodeFirstOrderODEIntegrator, integrate) {}

TEST(LsodeFirstOrderODEIntegrator, process_entry_point_cycle_start) {}

TEST(LsodeFirstOrderODEIntegrator, manager_check_stop_conditions) {}

TEST(LsodeFirstOrderODEIntegrator, manager_initialize_calculation_part1) {}

TEST(LsodeFirstOrderODEIntegrator, manager_initialize_calculation_part2) {}

TEST(LsodeFirstOrderODEIntegrator, manager_integration_loop_part1) {}

TEST(LsodeFirstOrderODEIntegrator, manager_integration_loop_part2) {}

TEST(LsodeFirstOrderODEIntegrator, manager_integration_loop_part3) {}

TEST(LsodeFirstOrderODEIntegrator, reset_integrator) {}

TEST(LsodeFirstOrderODEIntegrator, manager_set_calculation_phase_eq_2_reload) {}
