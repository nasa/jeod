set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
body_attach_aligned.cc
dyn_body_init_ned_state.cc
dyn_body_init.cc
dyn_body_frame_switch.cc
dyn_body_init_lvlh_state.cc
body_detach_specific.cc
body_attach_matrix.cc
body_detach.cc
body_attach.cc
body_action_messages.cc
mass_body_init.cc
body_action.cc
dyn_body_init_ned_rot_state.cc
dyn_body_init_lvlh_rot_state.cc
dyn_body_init_orbit.cc
body_reattach.cc
dyn_body_init_trans_state.cc
dyn_body_init_lvlh_trans_state.cc
dyn_body_init_ned_trans_state.cc
dyn_body_init_planet_derived.cc
dyn_body_init_rot_state.cc
dyn_body_init_wrt_planet.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
