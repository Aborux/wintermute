cmake_minimum_required(VERSION 2.8)
list(APPEND CMAKE_MODULE_PATH "/home/veles/cmake/")

find_package(ZMQ REQUIRED)
find_package(Pthreads REQUIRED)
find_package(Protobuf REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
execute_process(COMMAND /usr/bin/protoc -I=${CMAKE_CURRENT_SOURCE_DIR} --cpp_out=${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/simplemessage.proto
    )

add_executable(wntr-client
    wntr-client.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/simplemessage.pb.cc
    )
target_link_libraries(wntr-client
    ${ZMQ_LIBRARIES}
    ${PROTOBUF_LIBRARIES}
    ${PTHREADS_LIBRARY}
    )

