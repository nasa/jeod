cmake_minimum_required(VERSION 3.11)

if(NOT DEFINED UNIT_TEST_SRC)
   message(FATAL_ERROR "UNIT_TEST_SRC not set")
endif()

if(NOT DEFINED UNIT_TEST_NAME)
   message(FATAL_ERROR "UNIT_TEST_NAME not set")
endif()

if(NOT DEFINED JEODLIB_INSTALL_DIR)
   if(NOT DEFINED ENV{JEODLIB_INSTALL_DIR})
      set(JEODLIB_INSTALL_DIR ${JEOD_HOME}/lib_jeod_unit_test)
   else()
      set(JEODLIB_INSTALL_DIR $ENV{JEODLIB_INSTALL_DIR})
   endif()
endif()

get_filename_component(JEOD_HOME "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)

include_directories(${JEOD_HOME}/tools)
include_directories(${JEOD_HOME}/models)
include_directories(${ER7_UTILS_HOME})

add_executable(${UNIT_TEST_NAME} ${UNIT_TEST_SRC})
foreach(SRC ${UNIT_TEST_SRC})
  set_source_files_properties(${SRC} PROPERTIES COMPILE_FLAGS "${UT_COVERAGE_INCLUDE_FLAGS} ${UT_COVERAGE_COMPILE_FLAGS} ${COMPILE_OPTIONS}")
endforeach()
target_link_libraries(${UNIT_TEST_NAME} ${JEODLIB_INSTALL_DIR}/libjeod.a ${UT_COVERAGE_LINK_FLAGS} dl pthread)

execute_process(COMMAND find models -type d -name mock
            WORKING_DIRECTORY ${JEOD_HOME}
 	   OUTPUT_VARIABLE MOCK_DIRS_RAW)
string(REPLACE "\n" ";" MOCK_DIRS ${MOCK_DIRS_RAW})

foreach(MOCK_DIR ${MOCK_DIRS})
  include_directories("${JEOD_HOME}/${MOCK_DIR}")
endforeach()

if(USE_TRICK_MATH)
   if(NOT DEFINED TRICK_HOME)
      if(NOT DEFINED ENV{TRICK_HOME})
         message(FATAL_ERROR "ENABLE_UNIT_TESTS and USE_TRICK_MATH set but TRICK_HOME not defined in environment or passed via -DTRICK_HOME=")
      else()
         set(TRICK_HOME $ENV{TRICK_HOME})
         set(ER7_UTILS_HOME $ENV{TRICK_HOME}/trick_source)
      endif()
   else()
      set(ER7_UTILS_HOME ${TRICK_HOME}/trick_source)
   endif()

  if(EXISTS ${TRICK_HOME}/include)
     if(EXISTS ${TRICK_HOME}/lib64)
        target_link_libraries(${UNIT_TEST_NAME} ${TRICK_HOME}/lib64/libtrick_math.a)
     else()
        target_link_libraries(${UNIT_TEST_NAME} ${TRICK_HOME}/lib/libtrick_math.a)
     endif()
  else()
     target_link_libraries(${UNIT_TEST_NAME} ${TRICK_HOME}/lib_${TRICK_HOST_CPU}/libtrick.a)
  endif()
  foreach(SRC ${UNIT_TEST_SRC})
    set_source_files_properties(${SRC} PROPERTIES COMPILE_FLAGS "-I${TRICK_HOME}/include -I${TRICK_HOME}/include/trick/compat" ${COMPILE_OPTIONS})
  endforeach()
endif()

install(TARGETS ${UNIT_TEST_NAME} DESTINATION  ${CMAKE_CURRENT_SOURCE_DIR})
