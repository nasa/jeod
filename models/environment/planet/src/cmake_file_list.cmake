set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
base_planet.cc
planet.cc
planet_messages.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
