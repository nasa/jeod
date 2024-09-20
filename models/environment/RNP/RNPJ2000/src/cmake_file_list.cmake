set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
rnp_j2000.cc
polar_motion_j2000_init.cc
polar_motion_j2000.cc
nutation_j2000.cc
precession_j2000.cc
nutation_j2000_init.cc
rotation_j2000.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
