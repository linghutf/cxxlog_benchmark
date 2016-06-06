

PROJECT(decoder)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#ADD_DEFINITIONS("-g -Wall")
ADD_DEFINITIONS("-O2")

file(GLOB SRC_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *h *.cc )

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
    glog
    )

#SET(TARGET decoder)
#ADD_EXECUTABLE(${TARGET} ${SRC_FILES})

#SET(TARGET bench)
#ADD_EXECUTABLE(${TARGET} ${SRC_FILES})

ADD_EXECUTABLE(bench_fastlog bench_fastlog.cc)
ADD_EXECUTABLE(bench_sttlog bench_sttlog.cc)
ADD_EXECUTABLE(bench_glog bench_glog.cc)

ADD_EXECUTABLE(bench_fastlog_same bench_fastlog_same.cc)
ADD_EXECUTABLE(bench_sttlog_same bench_sttlog_same.cc)
ADD_EXECUTABLE(bench_glog_same bench_glog_same.cc)

ADD_EXECUTABLE(bench_fastlog_single bench_fastlog_single.cc)
ADD_EXECUTABLE(bench_sttlog_single bench_sttlog_single.cc)
ADD_EXECUTABLE(bench_glog_single bench_glog_single.cc)
