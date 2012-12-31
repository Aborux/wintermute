project(Wintermute)
cmake_minimum_required(VERSION 2.8)

## Modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

## Main variables
set(PROJECT_LABEL "Wintermute")
set(WINTERMUTE_MAJOR_VERSION 0)
set(WINTERMUTE_MINOR_VERSION 1)
set(WINTERMUTE_VERSION "${WINTERMUTE_MAJOR_VERSION}.${WINTERMUTE_MINOR_VERSION}")

## Includes.
## There are placed here because they depend on the PROJECT_LABEL value.
include(WintermuteSettings)
include(WintermuteMacros)

## Options for Wintermute's build.
option(WINTER_DEPRECATED_WARNINGS "Should Wintermute report warnings about deprecated methods?" ON)

## Packages
find_package(Qt4 ${WINTER_QT_VERSION} COMPONENTS
    QtCore
    QtGui
    QtDBus
    REQUIRED)
find_package(ZMQ REQUIRED)
find_package(Pthreads REQUIRED)
find_package(Protobuf REQUIRED)

## Go through directories.
add_subdirectory(cmake)
add_subdirectory(data)
add_subdirectory(src)

if(WITH_TESTING)
    add_subdirectory(testing)
endif()
