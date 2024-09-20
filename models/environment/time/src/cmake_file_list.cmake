set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
time_converter_std_ude.cc
time_converter_dyn_ude.cc
time_converter_tai_utc.cc
time_converter_tai_ut1.cc
time_converter_tai_tdb.cc
time_manager__initialize.cc
time_converter_ut1_gmst.cc
time_ude.cc
time_messages.cc
time_gps.cc
time_dyn.cc
time_converter.cc
time_gmst.cc
time_ut1.cc
time_tdb.cc
time__add_type_update.cc
time_converter_dyn_tai.cc
time_manager_init.cc
time_standard.cc
time_manager.cc
time_utc.cc
time_converter_tai_tt.cc
time_met.cc
time_tai.cc
time_converter_dyn_tdb.cc
time.cc
time_converter_tai_gps.cc
time_tt.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
