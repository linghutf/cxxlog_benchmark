

PROJECT(decoder)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#ADD_DEFINITIONS("-g -Wall")
ADD_DEFINITIONS("-O2")

file(GLOB SRC_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *h *.cpp )

INCLUDE_DIRECTORIES(
    ~/workspace/common_inc/stt
    ~/workspace/common_inc/boost
    )

LINK_DIRECTORIES(
    ~/workspace/common_lib/stt
    ~/workspace/common_lib/boost
    )

LINK_LIBRARIES(
    Common
    textlog
    boost_system
    boost_thread
    rt
    )

#SET(TARGET decoder)
#ADD_EXECUTABLE(${TARGET} ${SRC_FILES})

SET(TARGET bench)
ADD_EXECUTABLE(${TARGET} ${SRC_FILES})
