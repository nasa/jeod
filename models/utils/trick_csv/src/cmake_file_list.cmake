set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
read_trk_csv.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
