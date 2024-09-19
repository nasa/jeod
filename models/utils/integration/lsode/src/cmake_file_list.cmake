set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
lsode_simple_second_order_ode_integrator.cc
lsode_data_classes.cc
lsode_first_order_ode_integrator__support.cc
lsode_first_order_ode_integrator__utility.cc
lsode_second_order_ode_integrator.cc
lsode_integrator_constructor.cc
lsode_control_data_interface.cc
lsode_generalized_second_order_ode_integrator.cc
lsode_integration_controls.cc
lsode_first_order_ode_integrator__manager.cc
lsode_first_order_ode_integrator__integrator.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
