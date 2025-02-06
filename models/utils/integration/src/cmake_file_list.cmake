set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
jeod_integration_group.cc
integration_messages.cc
jeod_integration_time.cc
generalized_second_order_ode_technique.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
