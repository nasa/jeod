cmake_minimum_required(VERSION 3.14)
project(model_ut C CXX)

set(ENABLE_UNIT_TESTS TRUE)
include($ENV{JEOD_HOME}/bin/jeod/common_config.cmake)

include($ENV{JEOD_HOME}/models/utils/integration/verif/er7_utils_stubs/mock_config.cmake)

set(UNIT_TEST_SRC
dynamics_integration_group_ut.cc
dyn_bodies_primitives_ut.cc
dyn_manager_init_ut.cc
dyn_manager_ut.cc
gravitation_ut.cc
initialize_dyn_bodies_ut.cc
initialize_model_ut.cc
initialize_simulation_ut.cc
integ_group_primitives_ut.cc
mass_bodies_primitives_ut.cc
perform_actions_ut.cc
${ER7_STUB_SRCS}
)
set(UNIT_TEST_NAME test_program)

include(${JEOD_HOME}/bin/jeod/unit_test.cmake)
target_link_libraries(${UNIT_TEST_NAME} gtest gtest_main gmock)
