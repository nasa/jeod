cmake_minimum_required(VERSION 3.14)

project(JEOD CXX C)

include(bin/jeod/common_config.cmake)

set(tools tools/test_harness)

set(ENABLE_UNIT_TESTS ${ENABLE_UNIT_TESTS})
set(TRICK_BUILD ${TRICK_BUILD})
set(DE4XX_ONLY ${DE4XX_ONLY})
if(NOT DE4XX_ONLY)
   execute_process(COMMAND find models ${tools} ${SPICE_FILTER} ${EXPERIMENTAL_FILTER} -name verif -prune -o -type d -name src -print 
                   WORKING_DIRECTORY ${JEOD_HOME}
                   OUTPUT_VARIABLE ALL_SRC_DIRS_RAW)
   execute_process(COMMAND find models ${tools} ${SPICE_FILTER} ${EXPERIMENTAL_FILTER} -name verif -prune -o -name data -prune -o -type d -name src -print 
                   WORKING_DIRECTORY ${JEOD_HOME}
                   OUTPUT_VARIABLE MODEL_SRC_DIRS_RAW)
   
   string(REPLACE "\n" ";" ALL_SRC_DIRS ${ALL_SRC_DIRS_RAW})
   string(REPLACE "\n" ";" MODEL_SRC_DIRS ${MODEL_SRC_DIRS_RAW})
   
   ## Separate data direcotries
   set(DATA_DIRS ${ALL_SRC_DIRS})
   foreach(SRC_DIR ${MODEL_SRC_DIRS})
     list(REMOVE_ITEM DATA_DIRS ${SRC_DIR})
   endforeach()
   
   set(SRCS)
   foreach(SRC_DIR ${ALL_SRC_DIRS})
     aux_source_directory(${JEOD_HOME}/${SRC_DIR} SRCS)
   endforeach()
   
   # Get and set list of data src files for compiling unoptimized
   set(DATA_SRCS)
   foreach(SRC_DIR ${DATA_DIRS})
     aux_source_directory(${JEOD_HOME}/${SRC_DIR} DATA_SRCS)
   endforeach()
   foreach(DATA_SRC ${DATA_SRCS})
     set_source_files_properties(${DATA_SRC} PROPERTIES COMPILE_OPTIONS -O0 ${COMPILE_OPTIONS})
   endforeach()
   
   if(NOT TRICK_BUILD)
      FILE(GLOB TRICKSRCS ${JEOD_HOME}/models/utils/sim_interface/src/trick*.cc)
      foreach(TRICKSRC ${TRICKSRCS})
         list(REMOVE_ITEM SRCS ${TRICKSRC})
      endforeach()
   endif()
   
   add_library(jeod STATIC ${SRCS})
   target_link_libraries(jeod dl)
   install(TARGETS jeod DESTINATION ${INSTALL_DIR}/lib${SUFFIX})
   if(ENABLE_UNIT_TESTS)
       message(STATUS "ENABLE_UNIT_TESTS TRUE")
       target_compile_options(jeod PUBLIC ${UT_COVERAGE_COMPILE_FLAGS})
       target_link_libraries(jeod ${UT_COVERAGE_LINK_FLAGS})
   endif()
endif()

set(DE_TGTS)
add_subdirectory(${JEOD_HOME}/models/environment/ephemerides/de4xx_ephem/data)
message(STATUS "Detected ${DE_TGTS}")
foreach(DE_TGT ${DE_TGTS})
   install(TARGETS ${DE_TGT} DESTINATION ${INSTALL_DIR}/lib${SUFFIX}/de4xx_lib)
endforeach()

