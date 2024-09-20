set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
ref_frame_manager.cc
ref_frame_messages.cc
ref_frame.cc
ref_frame_compute_relative_state.cc
subscription.cc
ref_frame_items.cc
ref_frame_state.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
