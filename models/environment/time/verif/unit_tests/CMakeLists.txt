cmake_minimum_required(VERSION 3.14)
project(model_ut C CXX)

set(ENABLE_UNIT_TESTS TRUE)
include($ENV{JEOD_HOME}/bin/jeod/common_config.cmake)

include($ENV{JEOD_HOME}/models/utils/integration/verif/er7_utils_stubs/mock_config.cmake)

set(UNIT_TEST_SRC
time__add_type_update_ut.cc
time_converter_dyn_tai_ut.cc
time_converter_dyn_tdb_ut.cc
time_converter_dyn_ude_ut.cc
time_converter_std_ude_ut.cc
time_converter_tai_gps_ut.cc
time_converter_tai_tdb_ut.cc
time_converter_tai_tt_ut.cc
time_converter_tai_ut1_ut.cc
time_converter_tai_utc_ut.cc
time_converter_ut1_gmst_ut.cc
time_converter_ut.cc
time_dyn_ut.cc
time_gmst_ut.cc
time_gps_ut.cc
time_manager__initialize_ut.cc
time_manager_init_ut.cc
time_manager_ut.cc
time_met_ut.cc
time_standard_ut.cc
time_tai_ut.cc
time_tdb_ut.cc
time_tt_ut.cc
time_ude_ut.cc
time_ut1_ut.cc
time_ut.cc
time_utc_ut.cc
${ER7_STUB_SRCS}
)
set(UNIT_TEST_NAME test_program)

include(${JEOD_HOME}/bin/jeod/unit_test.cmake)
target_link_libraries(${UNIT_TEST_NAME} gtest gtest_main gmock)
