set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
aerodynamics_messages.cc
default_aero.cc
aero_surface.cc
aero_surface_factory.cc
flat_plate_thermal_aero_factory.cc
flat_plate_aero_factory.cc
flat_plate_aero_facet.cc
aero_drag.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
