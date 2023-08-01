cmake_minimum_required(VERSION 3.14)

project(DE4XX_EPHEM CXX C)

include($ENV{JEOD_HOME}/bin/jeod/common_config.cmake)

set(SRCGEN_DEPS ${JEOD_HOME}/models/environment/ephemerides/de4xx_ephem/data/generate_ephemeris.py
                ${JEOD_HOME}/models/environment/ephemerides/de4xx_ephem/data/eph_meta.cc.mako)

set(LIBRARY_OUTPUT_DIRECTORY de4xx_lib)

FILE(GLOB DE_DATA_ASCII_HEADERS ${JEOD_HOME}/models/environment/ephemerides/de4xx_ephem/ascii_full_set/header.*)
set(DE_TGTS)
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
  #set(DEDATASRC ${JEOD_HOME}/models/environment/ephemerides/de4xx_ephem/data/data_src/de${DEID}_0.cc)
  set(DENAME de${DEID})
  message(STATUS "Found DE header = ${DE_DATA_ASCII_HEADER} DEID = ${DEID}")
  message(STATUS "\tGenerated source targeted for ${DEDATASRCS}")
  message(STATUS "\tGenerated library targeted for lib${DENAME}.so")
  if(REGEN_DE4XX_DATA)
    message(STATUS "\tData sources found for ${DEDATASRCS}")
    FILE(GLOB DE_DATA_ASCII_SRCS ${JEOD_HOME}/models/environment/ephemerides/de4xx_ephem/ascii_full_set/*.${DEID})
    add_custom_command(OUTPUT ${DEDATASRCS}
                       WORKING_DIRECTORY ${JEOD_HOME}/models/environment/ephemerides/de4xx_ephem/data
                       COMMAND ./generate_ephemeris.py -i ../ascii_full_set ${DEID}
                       DEPENDS ${SRCGEN_DEPS} ${DE_DATA_ASCII_SRCS})
  endif()
  set_source_files_properties(${DEDATASRCS} PROPERTIES COMPILE_OPTIONS -O0 ${COMPILE_OPTIONS})
  add_library(${DENAME} SHARED ${DEDATASRCS})
  list(APPEND DE_TGTS ${DENAME})
  set_target_properties(${DENAME} PROPERTIES SUFFIX ".so")
endforeach()
set(DE_TGTS ${DE_TGTS} PARENT_SCOPE)
