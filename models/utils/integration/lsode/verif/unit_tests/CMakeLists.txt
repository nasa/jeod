cmake_minimum_required(VERSION 3.14)
project(model_ut C CXX)

set(ENABLE_UNIT_TESTS TRUE)
include($ENV{JEOD_HOME}/bin/jeod/common_config.cmake)

include($ENV{JEOD_HOME}/models/utils/integration/verif/er7_utils_stubs/mock_config.cmake)

set(UNIT_TEST_SRC
lsode_control_data_interface_ut.cc
lsode_data_classes_ut.cc
lsode_first_order_ode_integrator__integrator_ut.cc
lsode_first_order_ode_integrator__manager_ut.cc
lsode_first_order_ode_integrator__support_ut.cc
lsode_first_order_ode_integrator__utility_ut.cc
lsode_generalized_second_order_ode_integrator_ut.cc
lsode_integration_controls_ut.cc
lsode_integrator_constructor_ut.cc
lsode_second_order_ode_integrator_ut.cc
lsode_simple_second_order_ode_integrator_ut.cc
${ER7_STUB_SRCS}
)
set(UNIT_TEST_NAME test_program)

include(${JEOD_HOME}/bin/jeod/unit_test.cmake)
target_link_libraries(${UNIT_TEST_NAME} gtest gtest_main gmock)
