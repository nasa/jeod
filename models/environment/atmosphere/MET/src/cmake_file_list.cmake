set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
MET_atmosphere_state_vars.cc
MET_atmosphere.cc
MET_atmosphere_state.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
