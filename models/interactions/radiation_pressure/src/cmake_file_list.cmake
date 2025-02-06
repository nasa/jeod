set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
radiation_facet.cc
radiation_surface.cc
radiation_base_facet.cc
radiation_source.cc
flat_plate_radiation_factory.cc
radiation_messages.cc
radiation_default_surface.cc
radiation_pressure__default_surface.cc
radiation_pressure__surface_model.cc
radiation_surface_factory.cc
radiation_third_body.cc
radiation_pressure.cc
flat_plate_radiation_facet.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
