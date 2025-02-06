set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
earth_lighting.cc
earth_lighting_messages.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
