cmake_minimum_required(VERSION 3.10)

project(JEOD CXX C)

include(${CMAKE_CURRENT_LIST_DIR}/bin/jeod/common_config.cmake)

# Directory list obtained by using shell command :
# find . -name tools -prune -o -name verif -prune -o -name src -print | sort
set(JEOD_SRC_FILES)
set(JEOD_DATA_SRC_FILES)

set(SRC_DIRS
models/dynamics/body_action/src
models/dynamics/derived_state/src
models/dynamics/dyn_body/src
models/dynamics/dyn_manager/src
models/dynamics/mass/src
models/dynamics/rel_kin/src
models/environment/RNP/GenericRNP/src
models/environment/RNP/RNPJ2000/data/polar_motion/src
models/environment/RNP/RNPJ2000/data/src
models/environment/RNP/RNPJ2000/src
models/environment/RNP/RNPMars/data/src
models/environment/RNP/RNPMars/src
models/environment/atmosphere/MET/data/src
models/environment/atmosphere/MET/src
models/environment/atmosphere/base_atmos/src
models/environment/earth_lighting/src
models/environment/ephemerides/de4xx_ephem/src
models/environment/ephemerides/ephem_interface/src
models/environment/ephemerides/ephem_item/src
models/environment/ephemerides/ephem_manager/src
models/environment/ephemerides/propagated_planet/src
models/environment/gravity/data/src
models/environment/gravity/src
models/environment/planet/data/src
models/environment/planet/src
models/environment/spice/src
models/environment/time/data/src
models/environment/time/src
models/experimental/constraints/src
models/experimental/math/src
models/interactions/aerodynamics/data/src
models/interactions/aerodynamics/src
models/interactions/contact/src
models/interactions/gravity_torque/src
models/interactions/radiation_pressure/src
models/interactions/thermal_rider/src
models/utils/container/src
models/utils/integration/gauss_jackson/src
models/utils/integration/lsode/src
models/utils/integration/src
models/utils/lvlh_frame/src
models/utils/math/src
models/utils/memory/src
models/utils/message/src
models/utils/model_template/src
models/utils/named_item/src
models/utils/orbital_elements/src
models/utils/orientation/src
models/utils/planet_fixed/north_east_down/src
models/utils/planet_fixed/planet_fixed_posn/src
models/utils/quaternion/src
models/utils/ref_frames/src
models/utils/sim_interface/src
models/utils/surface_model/src
models/utils/trick_csv/src
)

if(ENABLE_UNIT_TESTS)
list(APPEND SRC_DIRS tools/test_harness/src)
endif()

FOREACH(SRC_DIR ${SRC_DIRS})
include(${SRC_DIR}/cmake_file_list.cmake)
ENDFOREACH()

foreach(DATA_SRC ${JEOD_DATA_SRC_FILES})
  set_source_files_properties(${DATA_SRC} PROPERTIES COMPILE_FLAGS -O0 ${COMPILE_OPTIONS})
endforeach()

if(ENABLE_UNIT_TESTS)
   foreach(SRC ${JEOD_SRC_FILES})
     set_source_files_properties(${SRC} PROPERTIES COMPILE_FLAGS ${UT_COVERAGE_COMPILE_FLAGS} ${COMPILE_OPTIONS})
   endforeach()
endif()

include_directories(models)

if(NOT DE4XX_ONLY)
add_library(jeod STATIC ${JEOD_SRC_FILES} ${JEOD_DATA_SRC_FILES})
install(TARGETS jeod DESTINATION ${INSTALL_DIR})
   if(ENABLE_UNIT_TESTS)
      # Add fexceptions back to source files that need it
      set_source_files_properties(${JEOD_HOME}/models/utils/memory/src/memory_manager_protected.cc PROPERTIES COMPILE_FLAGS -fexceptions)
      set_source_files_properties(${JEOD_HOME}/models/utils/sim_interface/src/checkpoint_input_manager.cc PROPERTIES COMPILE_FLAGS -fexceptions)
      set_source_files_properties(${JEOD_HOME}/models/utils/sim_interface/src/checkpoint_output_manager.cc PROPERTIES COMPILE_FLAGS -fexceptions)

      target_link_libraries(jeod ${UT_COVERAGE_LINK_FLAGS})
   endif()
endif()

set(DE_TGTS)
add_subdirectory(${JEOD_HOME}/models/environment/ephemerides/de4xx_ephem/data)
if(REGEN_DE4XX_DATA)
message(STATUS "Regenerated and Detected ${DE_TGTS}")
else()
message(STATUS "Detected ${DE_TGTS}")
endif()
foreach(DE_TGT ${DE_TGTS})
   install(TARGETS ${DE_TGT} DESTINATION ${INSTALL_DIR}/de4xx_lib)
endforeach()
