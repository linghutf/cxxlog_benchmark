cmake_minimum_required(VERSION 2.6)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Protobuf REQUIRED)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ~/workspace/common_inc/google/protobuf
    /usr/local/include
)

PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS user.proto)


link_directories(
    /usr/local/lib
)

link_libraries(
    protobuf
    log4cplus
)

file(GLOB SRC_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h *.cc)


add_executable(user_encoder user_encoder.cc ${PROTO_SRCS})
add_executable(user_decoder user_decoder.cc ${PROTO_SRCS})
