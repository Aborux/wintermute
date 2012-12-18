cmake_minimum_required(VERSION 2.8)
project(WintermuteApplication)

include(WintermuteMacros)

find_package(ZMQ REQUIRED)
find_package(Protobuf REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
## Configures
configure_file("${PROJECT_SOURCE_DIR}/global.hpp.in"
               "${CMAKE_CURRENT_BINARY_DIR}/global.hpp" @ONLY)

file(GLOB_RECURSE WINTERMUTE_SOURCES "*.cpp")
file(GLOB PROTOBUF_FILES *.proto)

set(WINTERMUTE_INCLUDE_DIR "${WINTER_INCLUDE_INSTALL_DIR}")
set(WINTERMUTE_INCLUDE_DIRS ${WINTERMUTE_INCLUDE_DIR}
    ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
    ${QT_QTDBUS_INCLUDE_DIR}
    ${QT_QTGUI_INCLUDE_DIR}
    )
set(WINTERMUTE_LIBRARIES 
    ${QT_QTCORE_LIBRARY}
    ${QT_QTDBUS_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${ZMQ_LIBRARIES}
    ${PROTOBUF_LIBRARIES}
    )

include_directories(${WINTERMUTE_INCLUDE_DIRS})

wntr_automoc("${WINTERMUTE_SOURCES}")
wntr_make_docs(PATH "${PROJECT_SOURCE_DIR}")
wntr_protoc(${PROTOBUF_FILES})

## Targets
add_executable(wintermute ${WINTERMUTE_SOURCES})
target_link_libraries(wintermute ${WINTERMUTE_LIBRARIES})
set_target_properties(wintermute PROPERTIES
    DEFINE_SYMBOL "WINTER_EXPORT"
    VERSION ${WINTERMUTE_VERSION}
    COMPILE_DEFINITIONS "${WINTERMUTE_COMPILE_DEFINITIONS}")
#    COMPILE_FLAGS "${WINTERMUTE_COMPILE_FLAGS}")

## Installs
install(TARGETS wintermute
        RUNTIME DESTINATION "${WINTER_BIN_INSTALL_DIR}"
        COMPONENT bin)

wntr_install_headers(HEADERS_PATH "${PROJECT_SOURCE_DIR}"
                     DESTINATION  "${WINTER_INCLUDE_INSTALL_DIR}")

configure_file("${CMAKE_SOURCE_DIR}/cmake/Modules/FindWintermute.cmake.in"
               "${CMAKE_BINARY_DIR}/InstallFiles/FindWintermute.cmake" @ONLY)
