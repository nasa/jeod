set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
gauss_jordan_solver.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
