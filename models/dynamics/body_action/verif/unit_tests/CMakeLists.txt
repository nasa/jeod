cmake_minimum_required(VERSION 3.14)
project(model_ut C CXX)

set(ENABLE_UNIT_TESTS TRUE)
include($ENV{JEOD_HOME}/bin/jeod/common_config.cmake)

include($ENV{JEOD_HOME}/models/utils/integration/verif/er7_utils_stubs/mock_config.cmake)

set(UNIT_TEST_SRC
body_action_ut.cc
body_attach_aligned_ut.cc
body_attach_matrix_ut.cc
body_attach_ut.cc
body_detach_specific_ut.cc
body_detach_ut.cc
body_reattach_ut.cc
dyn_body_frame_switch_ut.cc
dyn_body_init_lvlh_rot_state_ut.cc
dyn_body_init_lvlh_state_ut.cc
dyn_body_init_lvlh_trans_state_ut.cc
dyn_body_init_ned_rot_state_ut.cc
dyn_body_init_ned_state_ut.cc
dyn_body_init_ned_trans_state_ut.cc
dyn_body_init_orbit_ut.cc
dyn_body_init_planet_derived_ut.cc
dyn_body_init_rot_state_ut.cc
dyn_body_init_trans_state_ut.cc
dyn_body_init_ut.cc
dyn_body_init_wrt_planet_ut.cc
mass_body_init_ut.cc
${ER7_STUB_SRCS}
)
set(UNIT_TEST_NAME test_program)

include(${JEOD_HOME}/bin/jeod/unit_test.cmake)
target_link_libraries(${UNIT_TEST_NAME} gtest gtest_main gmock)
