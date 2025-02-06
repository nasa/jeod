set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
thermal_facet_rider.cc
thermal_integrable_object.cc
thermal_model_rider.cc
thermal_messages.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
