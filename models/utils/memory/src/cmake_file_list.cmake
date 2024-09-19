set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
memory_type.cc
memory_manager_protected.cc
memory_manager_static.cc
memory_item.cc
memory_messages.cc
memory_manager.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
