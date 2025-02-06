set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
planet_rnp.cc
planet_orientation.cc
RNP_messages.cc
planet_rotation.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
