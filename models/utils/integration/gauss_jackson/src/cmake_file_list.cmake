set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
gauss_jackson_rational_coeffs.cc
gauss_jackson_generalized_second_order_ode_integrator.cc
gauss_jackson_state_machine.cc
gauss_jackson_integration_controls.cc
gauss_jackson_coeffs.cc
gauss_jackson_integrator_constructor.cc
gauss_jackson_coefficients_pair.cc
gauss_jackson_config.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
