set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
ephem_ref_frame.cc
ephem_messages.cc
simple_ephemerides.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
