cmake_minimum_required(VERSION 3.11)

if(NOT DEFINED UNIT_TEST_SRC)
   message(FATAL_ERROR "UNIT_TEST_SRC not set")
endif()

if(NOT DEFINED UNIT_TEST_NAME)
   message(FATAL_ERROR "UNIT_TEST_NAME not set")
endif()

include(${JEOD_HOME}/bin/jeod/common_config.cmake)

include_directories(${JEOD_HOME}/tools)

add_executable(${UNIT_TEST_NAME} ${UNIT_TEST_SRC})
target_compile_options(${UNIT_TEST_NAME} PRIVATE -pg --coverage)
target_link_libraries(${UNIT_TEST_NAME} -pg --coverage ${JEOD_HOME}/lib${SUFFIX}/libjeod.a ${UT_COVERAGE_COMPILE_FLAGS} dl pthread)

if(USE_TRICK_MATH)
  if(EXISTS ${TRICK_HOME}/include)
     if(EXISTS ${TRICK_HOME}/lib64)
        target_link_libraries(${UNIT_TEST_NAME} ${TRICK_HOME}/lib64/libtrick_math.a)
     else()
        target_link_libraries(${UNIT_TEST_NAME} ${TRICK_HOME}/lib/libtrick_math.a)
     endif()
  else()
     target_link_libraries(${UNIT_TEST_NAME} ${TRICK_HOME}/lib_${TRICK_HOST_CPU}/libtrick.a)
  endif()
endif()

install(TARGETS ${UNIT_TEST_NAME} DESTINATION  ${CMAKE_CURRENT_SOURCE_DIR})
