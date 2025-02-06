set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
gravity_interaction.cc
gravity_controls.cc
spherical_harmonics_solid_body_tides.cc
gravity_manager.cc
gravity_source.cc
spherical_harmonics_delta_coeffs.cc
spherical_harmonics_gravity_controls.cc
spherical_harmonics_calc_nonspherical.cc
spherical_harmonics_gravity_source.cc
gravity_messages.cc
spherical_harmonics_tidal_effects.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
