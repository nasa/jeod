cmake_minimum_required(VERSION 3.14)
project(model_ut C CXX)

set(ENABLE_UNIT_TESTS TRUE)
include($ENV{JEOD_HOME}/bin/jeod/common_config.cmake)

include($ENV{JEOD_HOME}/models/utils/integration/verif/er7_utils_stubs/mock_config.cmake)

set(UNIT_TEST_SRC
mass_attach_ut.cc
mass_calc_composite_cm_ut.cc
mass_calc_composite_inertia_ut.cc
mass_detach_ut.cc
mass_point_init_ut.cc
mass_point_mass_inertia_ut.cc
mass_point_state_ut.cc
mass_point_ut.cc
mass_print_body_ut.cc
mass_print_tree_ut.cc
mass_properties_init_ut.cc
mass_reattach_ut.cc
mass_update_ut.cc
mass_ut.cc
${ER7_STUB_SRCS}
)
set(UNIT_TEST_NAME test_program)

include(${JEOD_HOME}/bin/jeod/unit_test.cmake)
target_link_libraries(${UNIT_TEST_NAME} gtest gtest_main gmock)
