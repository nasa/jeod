cmake_minimum_required(VERSION 3.14)
project(model_ut C CXX)

set(ENABLE_UNIT_TESTS TRUE)
include($ENV{JEOD_HOME}/bin/jeod/common_config.cmake)

include($ENV{JEOD_HOME}/models/utils/integration/verif/er7_utils_stubs/mock_config.cmake)

set(UNIT_TEST_SRC
gauss_jackson_coefficients_pair_ut.cc
gauss_jackson_coeffs_ut.cc
gauss_jackson_config_ut.cc
gauss_jackson_generalized_second_order_ode_integrator_ut.cc
gauss_jackson_integration_controls_ut.cc
gauss_jackson_integrator_constructor_ut.cc
gauss_jackson_rational_coeffs_ut.cc
gauss_jackson_state_machine_ut.cc
${ER7_STUB_SRCS}
)
set(UNIT_TEST_NAME test_program)

include(${JEOD_HOME}/bin/jeod/unit_test.cmake)
target_link_libraries(${UNIT_TEST_NAME} gtest gtest_main gmock)
