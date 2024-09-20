set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
named_item_demangle.cc
named_item.cc
named_item_messages.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
