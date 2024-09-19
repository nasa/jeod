set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
message_messages.cc
suppressed_code_message_handler.cc
message_handler.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
