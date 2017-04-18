cmake_minimum_required ( VERSION 3.7.1 )
project ( AirWaves )

option( BUILD_TESTS OFF "Build unit tests created with gmock/gtest framework"  )

###
### namespace used for project
###
set ( PROJECT_NAMESPACE vmp ) # visual music project

###
### The version number.
###
set ( PROJECT_VERSION_MAJOR 1 )
set ( PROJECT_VERSION_MINOR 0 )
set ( PROJECT_VERSION_PATCH 0 )

###
### required before DefaultProjectLibrary.cmake
###
set( SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src )

###
### other paths
###
set( SHARED_PATH ${PROJECT_SOURCE_DIR}/shared )
set( RES_PATH    ${PROJECT_SOURCE_DIR}/res    )
set( SHADER_PATH ${SHARED_PATH}/shaders       )


###
###
###
set( PROJECT_CONFIG_FILE ${SHARED_PATH}/src/common/ProjectConfig.hpp.in )


###
### Set options for the shared simulation
### project then add it to the current build
###
set( USE_GLFW ON CACHE BOOL "" FORCE )
set( USE_GLM  ON CACHE BOOL "" FORCE )
set( USE_GUI  ON CACHE BOOL "" FORCE )

set( USE_GMOCK ${BUILD_TESTS} CACHE BOOL "" FORCE )

add_subdirectory( ${SHARED_PATH} )


###
### header dirs
###
set(
    PROJECT_INCLUDE_DIRS

    ${SRC_DIR}/io
    )

###
### project source files that get
### comipled into the AirWaves lib
###
set(
    PROJECT_SOURCE

    # ignored by compilers but added to project files
    ${SHARED_UNCRUSTIFY_FILE}

    ${SRC_DIR}/io/AirWavesIOHandler.hpp
    ${SRC_DIR}/io/AirWavesIOHandler.cpp
    )


###
### Unit tests for the AirWaves lib
###
set(
    TESTING_SOURCE
    ${SRC_DIR}/testing/AirWavesUnitTests.cpp
    )

list( APPEND TESTING_INCLUDE_DIRS ${SRC_DIR} )


###
### file with main function to execute the program
###
set( PROJECT_MAIN ${SRC_DIR}/exec/AirWaves.cpp )

###
### Set the C++ standard for compilation then include
### a cmake file that will build and link all projects
### specific libraries, tests, and executables
###
set ( FORCE_CUDA_STANDARD 11 )
set ( FORCE_CPP_STANDARD  14 )
include( ${SHARED_PATH}/cmake/DefaultProjectLibrary.cmake )

