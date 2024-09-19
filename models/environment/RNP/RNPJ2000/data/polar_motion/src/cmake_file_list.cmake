set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
xpyp_monthly.cc
xpyp_daily.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_DATA_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
