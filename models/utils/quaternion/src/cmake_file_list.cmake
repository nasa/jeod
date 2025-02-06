set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
quat_to_mat.cc
quat_to_eigenrot.cc
quat_norm.cc
quat.cc
quat_from_mat.cc
quat_messages.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
