set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
mass_point_state.cc
mass_reattach.cc
mass_point_mass_inertia.cc
mass_messages.cc
mass_calc_composite_inertia.cc
mass_point.cc
mass_update.cc
mass_print_tree.cc
mass_print_body.cc
mass_point_init.cc
mass_calc_composite_cm.cc
mass.cc
mass_detach.cc
mass_attach.cc
mass_properties_init.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
