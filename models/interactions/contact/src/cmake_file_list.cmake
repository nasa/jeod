set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
point_contact_facet_factory.cc
line_point_contact_pair.cc
line_contact_facet.cc
point_contact_pair.cc
contact.cc
line_contact_pair.cc
contact_params.cc
contact_surface_factory.cc
point_contact_facet.cc
contact_facet.cc
pair_interaction.cc
line_contact_facet_factory.cc
contact_pair.cc
contact_surface.cc
spring_pair_interaction.cc
contact_messages.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
