set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
dyn_body_attached_body_constraint.cc
constrained_rigid_mass.cc
dyn_body_constraints_solver.cc
force_constraint_component.cc
dyn_body_pendulum_constraint.cc
constrained_point_mass.cc
torque_constraint_component.cc
constraint_component.cc
pendulum_constraint_component.cc
dyn_body_constraint.cc
constraint_frame.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
