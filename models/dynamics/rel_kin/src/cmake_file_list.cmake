set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
relative_kinematics.cc
rel_kin_messages.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
