cmake_minimum_required(VERSION 3.14)

project(DE4XX_EPHEM CXX C)

get_filename_component(JEOD_HOME "${CMAKE_CURRENT_LIST_DIR}/../../../../.." ABSOLUTE)
include(${JEOD_HOME}/bin/jeod/common_config.cmake)

set(SRCGEN_DEPS ${JEOD_HOME}/models/environment/ephemerides/de4xx_ephem/data/generate_ephemeris.py
                ${JEOD_HOME}/models/environment/ephemerides/de4xx_ephem/data/eph_meta.cc.mako)

set(LIBRARY_OUTPUT_DIRECTORY de4xx_lib)

FILE(GLOB DE_DATA_ASCII_HEADERS ${JEOD_HOME}/models/environment/ephemerides/de4xx_ephem/ascii_full_set/header.*)
foreach(DE_DATA_ASCII_HEADER ${DE_DATA_ASCII_HEADERS})
  get_filename_component(DEID ${DE_DATA_ASCII_HEADER} LAST_EXT)
  string(REPLACE "." "" DEID ${DEID})
  FILE(GLOB DE_DATA_ASCII_SRCS ${JEOD_HOME}/models/environment/ephemerides/de4xx_ephem/ascii_full_set/ascp*.${DEID})
  set(DEDATASRCS)
  set(DEDATASRCCOUNT 0)
  foreach(DE_DATA_ASCII_SRC ${DE_DATA_ASCII_SRCS})
     list(APPEND DEDATASRCS ${JEOD_HOME}/models/environment/ephemerides/de4xx_ephem/data/data_src/de${DEID}_${DEDATASRCCOUNT}.cc)
     MATH(EXPR DEDATASRCCOUNT "${DEDATASRCCOUNT}+1")
  endforeach()
  set(DENAME de${DEID})
  message(STATUS "Found ASCII DE header = ${DE_DATA_ASCII_HEADER} DEID = ${DEID}")
  if(REGEN_DE4XX_DATA)
    message(STATUS "\tGenerated sources ${DEDATASRCS}")
    FILE(GLOB DE_DATA_ASCII_SRCS ${JEOD_HOME}/models/environment/ephemerides/de4xx_ephem/ascii_full_set/*.${DEID})
    add_custom_command(OUTPUT ${DEDATASRCS}
                       WORKING_DIRECTORY ${JEOD_HOME}/models/environment/ephemerides/de4xx_ephem/data
                       COMMAND ./generate_ephemeris.py -i ../ascii_full_set ${DEID}
                       DEPENDS ${SRCGEN_DEPS} ${DE_DATA_ASCII_SRCS})
  endif()
endforeach()


set(DE_TGTS)
FILE(GLOB DEDATASRCS ${JEOD_HOME}/models/environment/ephemerides/de4xx_ephem/data/data_src/de*.cc)
while(DEDATASRCS)
  list(POP_FRONT DEDATASRCS FIRSTDATASRC)
  get_filename_component(DENAME_COUNT ${FIRSTDATASRC} NAME_WLE)
  string(FIND ${DENAME_COUNT} "_" RETIDX)
  string(SUBSTRING ${DENAME_COUNT} 0 ${RETIDX} DENAME)
  string(REPLACE "de" "" DEID ${DENAME})
  message(STATUS "Found DE src files for DEID = ${DEID}")
  FILE(GLOB DATASRCS ${JEOD_HOME}/models/environment/ephemerides/de4xx_ephem/data/data_src/de${DEID}_*.cc)
  set_source_files_properties(${DATASRCS} PROPERTIES COMPILE_OPTIONS -O0 ${COMPILE_OPTIONS})
  add_library(${DENAME} SHARED ${DATASRCS})
  list(APPEND DE_TGTS ${DENAME})
  set_target_properties(${DENAME} PROPERTIES SUFFIX ".so")
  message(STATUS "\tList of src files for lib${DENAME}.so : ${DATASRCS}")
  list(REMOVE_ITEM DEDATASRCS ${DATASRCS})
endwhile()
set(DE_TGTS ${DE_TGTS} PARENT_SCOPE)
