set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
tai_to_ut1.cc
tai_to_utc.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_DATA_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
