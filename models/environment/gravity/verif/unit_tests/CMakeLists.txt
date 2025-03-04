cmake_minimum_required(VERSION 3.14)
project(model_ut C CXX)

set(ENABLE_UNIT_TESTS TRUE)
include($ENV{JEOD_HOME}/bin/jeod/common_config.cmake)

include($ENV{JEOD_HOME}/models/utils/integration/verif/er7_utils_stubs/mock_config.cmake)

set(UNIT_TEST_SRC
gravity_controls_ut.cc
gravity_interaction_ut.cc
gravity_manager_ut.cc
gravity_source_ut.cc
spherical_harmonics_calc_nonspherical_ut.cc
spherical_harmonics_delta_coeffs_ut.cc
spherical_harmonics_gravity_controls_ut.cc
spherical_harmonics_gravity_source_ut.cc
spherical_harmonics_solid_body_tides_ut.cc
spherical_harmonics_tidal_effects_ut.cc
${ER7_STUB_SRCS}
)
set(UNIT_TEST_NAME test_program)

include(${JEOD_HOME}/bin/jeod/unit_test.cmake)
target_link_libraries(${UNIT_TEST_NAME} gtest gtest_main gmock)
