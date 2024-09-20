set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
ephem_item.cc
ephem_orient_zxz.cc
ephem_point.cc
ephem_orient.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
