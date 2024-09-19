set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
solar_min.cc
solar_mean.cc
data_met_wind_velocity.cc
solar_max.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_DATA_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
