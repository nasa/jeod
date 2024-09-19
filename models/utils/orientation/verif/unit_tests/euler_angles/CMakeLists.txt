cmake_minimum_required(VERSION 3.14)
project(model_ut C CXX)

set(ENABLE_UNIT_TESTS TRUE)
set(USE_TRICK_MATH TRUE)
include($ENV{JEOD_HOME}/bin/jeod/common_config.cmake)

set(UNIT_TEST_SRC  main.cc)
set(UNIT_TEST_NAME test_program)

include(${JEOD_HOME}/bin/jeod/unit_test.cmake)
target_link_libraries(${UNIT_TEST_NAME} gtest gtest_main gmock)