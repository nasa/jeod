set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
planet_fixed_posn.cc
planet_fixed_messages.cc
alt_lat_long_state.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
