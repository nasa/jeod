set(SUBDIR ${CMAKE_CURRENT_LIST_DIR})

set(SRCS
de405_28.cc
de440_7.cc
de405_18.cc
de440_3.cc
de405_19.cc
de440_2.cc
de405_29.cc
de440_6.cc
de405_16.cc
de405_22.cc
de405_5.cc
de405_26.cc
de405_12.cc
de405_1.cc
de440_9.cc
de405_27.cc
de405_13.cc
de405_0.cc
de440_8.cc
de405_17.cc
de405_23.cc
de405_4.cc
de405_24.cc
de405_10.cc
de440_10.cc
de405_3.cc
de405_14.cc
de405_20.cc
de405_7.cc
de405_30.cc
de405_15.cc
de405_21.cc
de405_6.cc
de405_25.cc
de405_11.cc
de405_2.cc
de405_9.cc
de440_1.cc
de421_1.cc
de440_5.cc
de421_0.cc
de440_4.cc
de405_8.cc
de440_0.cc
)

foreach(SRC ${SRCS})
list(APPEND JEOD_DATA_SRC_FILES ${SUBDIR}/${SRC})
endforeach()
