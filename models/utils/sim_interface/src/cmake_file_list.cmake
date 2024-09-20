set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
trick_sim_interface.cc
trick_memory_interface_alloc.cc
trick10_memory_interface.cc
trick_memory_interface.cc
checkpoint_output_manager.cc
simulation_interface.cc
trick_memory_interface_chkpnt.cc
trick_message_handler.cc
trick_dynbody_integ_loop.cc
trick_memory_interface_xlate.cc
checkpoint_input_manager.cc
sim_interface_messages.cc
trick_memory_interface_attrib.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
