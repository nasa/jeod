set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
wind_velocity.cc
wind_velocity_base.cc
atmosphere_state.cc
atmosphere_messages.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
