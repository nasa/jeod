cmake_minimum_required(VERSION 3.14)
project(model_ut C CXX)

set(ENABLE_UNIT_TESTS TRUE)
include($ENV{JEOD_HOME}/bin/jeod/common_config.cmake)

include($ENV{JEOD_HOME}/models/utils/integration/verif/er7_utils_stubs/mock_config.cmake)

set(UNIT_TEST_SRC
checkpoint_input_manager_ut.cc
checkpoint_output_manager_ut.cc
simulation_interface_ut.cc
trick10_memory_interface_ut.cc
trick_dynbody_integ_loop_ut.cc
trick_memory_interface_alloc_ut.cc
trick_memory_interface_attrib_ut.cc
trick_memory_interface_chkpnt_ut.cc
trick_memory_interface_ut.cc
trick_memory_interface_xlate_ut.cc
trick_message_handler_ut.cc
trick_sim_interface_ut.cc
${ER7_STUB_SRCS}
)
set(UNIT_TEST_NAME test_program)

include(${JEOD_HOME}/bin/jeod/unit_test.cmake)
target_link_libraries(${UNIT_TEST_NAME} gtest gtest_main gmock)
