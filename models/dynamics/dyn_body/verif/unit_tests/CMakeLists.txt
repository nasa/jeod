cmake_minimum_required(VERSION 3.14)
project(model_ut C CXX)

set(ENABLE_UNIT_TESTS TRUE)
include($ENV{JEOD_HOME}/bin/jeod/common_config.cmake)

include($ENV{JEOD_HOME}/models/utils/integration/verif/er7_utils_stubs/mock_config.cmake)

set(UNIT_TEST_SRC
body_force_collect_ut.cc
body_ref_frame_ut.cc
body_wrench_collect_ut.cc
dyn_body_attach_ut.cc
dyn_body_collect_ut.cc
dyn_body_detach_ut.cc
dyn_body_find_body_frame_ut.cc
dyn_body_initialize_model_ut.cc
dyn_body_integration_ut.cc
dyn_body_propagate_state_ut.cc
dyn_body_set_state_ut.cc
dyn_body_ut.cc
dyn_body_vehicle_point_ut.cc
force_ut.cc
structure_integrated_dyn_body_collect_ut.cc
structure_integrated_dyn_body_integration_ut.cc
structure_integrated_dyn_body_pt_accel_ut.cc
structure_integrated_dyn_body_solve_ut.cc
structure_integrated_dyn_body_ut.cc
torque_ut.cc
${ER7_STUB_SRCS}
)
set(UNIT_TEST_NAME test_program)

include(${JEOD_HOME}/bin/jeod/unit_test.cmake)
target_link_libraries(${UNIT_TEST_NAME} gtest gtest_main gmock)

#set_property(SOURCE body_force_collect_ut.cc APPEND_STRING PROPERTY COMPILE_FLAGS -fexceptions)
