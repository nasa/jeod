set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
earth_spherical.cc
earth_GGM05C.cc
earth_GGM02C.cc
moon_GRAIL150.cc
earth_solid_tides.cc
mars_MRO110B2.cc
sun_spherical.cc
moon_LP150Q.cc
moon_spherical.cc
earth_GEMT1.cc
mars_spherical.cc
jupiter_spherical.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_DATA_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
