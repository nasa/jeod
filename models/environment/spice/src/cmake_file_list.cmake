if(DEFINED JEOD_SPICE_DIR)
set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
spice_ephem.cc
spice_ephem_orient.cc
spice_ephem_point.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
endif(DEFINED JEOD_SPICE_DIR)
