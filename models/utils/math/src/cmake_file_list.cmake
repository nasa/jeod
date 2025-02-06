set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
dm_invert.cc
dm_invert_symm.cc
gauss_quadrature.cc
math_messages.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
