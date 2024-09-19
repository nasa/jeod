set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
north_east_down.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
