cmake_minimum_required(VERSION 3.14)
project(model_ut C CXX)

set(ENABLE_UNIT_TESTS TRUE)
include($ENV{JEOD_HOME}/bin/jeod/common_config.cmake)

include($ENV{JEOD_HOME}/models/utils/integration/verif/er7_utils_stubs/mock_config.cmake)

set(UNIT_TEST_SRC
flat_plate_radiation_facet_ut.cc
flat_plate_radiation_factory_ut.cc
radiation_base_facet_ut.cc
radiation_default_surface_ut.cc
radiation_facet_ut.cc
radiation_pressure__default_surface_ut.cc
radiation_pressure__surface_model_ut.cc
radiation_pressure_ut.cc
radiation_source_ut.cc
radiation_surface_factory_ut.cc
radiation_surface_ut.cc
radiation_third_body_ut.cc
${ER7_STUB_SRCS}
)
set(UNIT_TEST_NAME test_program)

include(${JEOD_HOME}/bin/jeod/unit_test.cmake)
target_link_libraries(${UNIT_TEST_NAME} gtest gtest_main gmock)
