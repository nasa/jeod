set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
gravitation.cc
mass_bodies_primitives.cc
dyn_manager_messages.cc
integ_group_primitives.cc
initialize_dyn_bodies.cc
initialize_simulation.cc
dyn_bodies_primitives.cc
dyn_manager.cc
perform_actions.cc
initialize_model.cc
dynamics_integration_group.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
