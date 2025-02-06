set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
interaction_surface_factory.cc
facet.cc
flat_plate.cc
surface_model_messages.cc
surface_model.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
