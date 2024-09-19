set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
data_rnp_mars.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_DATA_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
