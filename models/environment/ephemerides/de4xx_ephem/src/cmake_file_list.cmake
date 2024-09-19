set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
de4xx_ephem.cc
de4xx_file_init.cc
de4xx_file.cc
de4xx_file_update.cc
de4xx_ephem_dynmanager.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
