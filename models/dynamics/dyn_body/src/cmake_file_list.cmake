set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
dyn_body_detach.cc
dyn_body_attach.cc
dyn_body_find_body_frame.cc
dyn_body_collect.cc
structure_integrated_dyn_body_pt_accel.cc
dyn_body_integration.cc
torque.cc
dyn_body_initialize_model.cc
dyn_body_messages.cc
body_wrench_collect.cc
dyn_body_propagate_state.cc
body_force_collect.cc
dyn_body_vehicle_point.cc
dyn_body_set_state.cc
dyn_body.cc
structure_integrated_dyn_body_integration.cc
structure_integrated_dyn_body_solve.cc
structure_integrated_dyn_body.cc
force.cc
structure_integrated_dyn_body_collect.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
