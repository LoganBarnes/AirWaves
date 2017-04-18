cmake_minimum_required ( VERSION 3.7.1 )
project ( AirWaves )

option( BUILD_TESTS OFF "Build unit tests created with gmock/gtest framework"  )

# namespace used for project
set ( PROJECT_NAMESPACE cnc )

# The version number.
set ( PROJECT_VERSION_MAJOR 1 )
set ( PROJECT_VERSION_MINOR 0 )
set ( PROJECT_VERSION_PATCH 0 )

# required before DefaultProjectLibrary.cmake
set( SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src )

# other paths
set( SHARED_PATH ${PROJECT_SOURCE_DIR}/shared )
# set( RES_PATH    ${PROJECT_SOURCE_DIR}/res    )
# set( SHADER_PATH ${SHARED_PATH}/shaders       )


set( PROJECT_CONFIG_FILE ${SHARED_PATH}/src/common/ProjectConfig.hpp.in )


# set( USE_GLFW ON CACHE BOOL "" FORCE )
# set( USE_GLM  ON CACHE BOOL "" FORCE )
# set( USE_GUI  ON CACHE BOOL "" FORCE )

set( USE_GMOCK ${BUILD_TESTS} CACHE BOOL "" FORCE )


# set variables from shared simulation project
add_subdirectory( ${SHARED_PATH} )


# header dirs
set( PROJECT_INCLUDE_DIRS ${SRC_DIR} )

# # cpp files
# set(
#     PROJECT_SOURCE

#     # ignored by compilers but added to project files
#     ${SHARED_UNCRUSTIFY_FILE}
#     ${RES_PATH}/shaders/rasterize/shader.vert
#     ${RES_PATH}/shaders/rasterize/shader.frag
#     ${RES_PATH}/shaders/rasterize/flat/shader.frag
#     ${RES_PATH}/shaders/rasterize/round/shader.frag
#     ${RES_PATH}/shaders/stencil/shader.vert
#     ${RES_PATH}/shaders/stencil/shader.frag

#     ${SRC_DIR}/grid/Grid.hpp
#     ${SRC_DIR}/grid/Grid.cpp

#     ${SRC_DIR}/carver/Carver.hpp
#     ${SRC_DIR}/carver/Carver.cpp
#     ${SRC_DIR}/carver/GLCarver.hpp
#     ${SRC_DIR}/carver/GLCarver.cpp

#     ${SRC_DIR}/io/HeightsCallback.hpp
#     ${SRC_DIR}/io/HeightsCallback.cpp
#     ${SRC_DIR}/io/HeightsIOHandler.hpp
#     ${SRC_DIR}/io/HeightsIOHandler.cpp
#     ${SRC_DIR}/io/CarverRenderer.hpp
#     ${SRC_DIR}/io/CarverRenderer.cpp
#     )


set(
    TESTING_SOURCE
    ${SRC_DIR}/testing/AirWavesUnitTests.cpp
    )

list( APPEND TESTING_INCLUDE_DIRS ${SRC_DIR} )


# file with main function
set( PROJECT_MAIN ${SRC_DIR}/exec/AirWaves.cpp )

set ( FORCE_CUDA_STANDARD 11 )
set ( FORCE_CPP_STANDARD  14 )
include( ${SHARED_PATH}/cmake/DefaultProjectLibrary.cmake )

