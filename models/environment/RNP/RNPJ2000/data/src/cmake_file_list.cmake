set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
data_rnp_j2000.cc
data_nutation_j2000.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_DATA_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
