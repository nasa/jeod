set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
foo.cc
template_messages.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
