set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
lvlh_frame_messages.cc
lvlh_frame.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
