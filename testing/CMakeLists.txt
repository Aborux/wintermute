cmake_minimum_required(VERSION 2.8)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(PROTOC_FLAGS
    -I${CMAKE_CURRENT_SOURCE_DIR}
    -I${PROJECT_SOURCE_DIR}/src/
    )
execute_process(COMMAND /usr/bin/protoc ${PROTOC_FLAGS} --cpp_out=${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/src/simplemessage.proto
    ${PROJECT_SOURCE_DIR}/src/agentinfo.proto
    ${PROJECT_SOURCE_DIR}/src/coremessages.proto
    )

add_executable(wntr-client
    wntr-client.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/simplemessage.pb.cc
    ${CMAKE_CURRENT_BINARY_DIR}/agentinfo.pb.cc
    ${CMAKE_CURRENT_BINARY_DIR}/coremessages.pb.cc
    )
target_link_libraries(wntr-client
    ${ZMQ_LIBRARIES}
    ${PROTOBUF_LIBRARIES}
    ${PTHREADS_LIBRARY}
    )

