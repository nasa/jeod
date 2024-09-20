set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
eigen_rotation.cc
euler_angles.cc
orientation_messages.cc
orientation.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
