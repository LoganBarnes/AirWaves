cmake_minimum_required(VERSION 3.8)
project(AirWaves)

set(CMAKE_CXX_STANDARD 14)

option(BUILD_TESTS OFF "Build unit tests created with gtest/gmock framework")

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)
include(cmake/DownloadProject.cmake)

# Download and unpack simulation driver at configure time
download_project(PROJ sim
        PREFIX thirdparty
        GIT_REPOSITORY https://gitlab.com/LoganBarnes/SimulationDriver.git
        GIT_TAG master
        UPDATE_DISCONNECTED 1
        QUIET
        )

# compile sim with current project
add_subdirectory(${sim_SOURCE_DIR} ${sim_BINARY_DIR})


# Download and unpack kfr at configure time
download_project(PROJ kfr
        PREFIX thirdparty
        GIT_REPOSITORY https://github.com/kfrlib/kfr.git
        GIT_TAG v1.2.0
        UPDATE_DISCONNECTED 1
        QUIET
        )

# compile kfr with current project
add_library(kfr INTERFACE)
target_include_directories(kfr SYSTEM INTERFACE ${kfr_SOURCE_DIR}/include)


set(SOURCE_FILES
        src/vmp/SimTest.cpp
        src/vmp/SimTest.hpp
        )

add_library(vmp ${SOURCE_FILES})
target_include_directories(vmp PUBLIC src)
target_link_libraries(vmp SimDriver kfr)

add_executable(AirWaves src/exec/main.cpp)
target_link_libraries(AirWaves vmp)
