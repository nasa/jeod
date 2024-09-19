set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
test_harness_messages.cc
test_sim_interface.cc
test_message_handler.cc
cmdline_parser.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
