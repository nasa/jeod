set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
euler_derived_state.cc
solar_beta_derived_state.cc
planetary_derived_state.cc
derived_state_messages.cc
orb_elem_derived_state.cc
relative_derived_state.cc
lvlh_relative_derived_state.cc
derived_state.cc
lvlh_derived_state.cc
ned_derived_state.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
