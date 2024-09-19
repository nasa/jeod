set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
earth.cc
jupiter.cc
mars.cc
sun.cc
moon.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_DATA_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
