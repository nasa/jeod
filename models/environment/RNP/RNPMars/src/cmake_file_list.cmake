set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
precession_mars.cc
rnp_mars.cc
nutation_mars.cc
rotation_mars.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
