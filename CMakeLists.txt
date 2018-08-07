###############################################################
# Created on 10/21/2017.
# Copyright (c) 2017. All rights reserved.
#  ___________________________$$$$$$$\__________
# | $$\    $$\                $$  __$$\      | ||
# |_$$ |___$$ |_$$\______$$\__$$ |__$$ |_____|_||
# | $$ |   $$ | $$$\    $$$ | $$$$$$$  |  () | ||
# |_\$$\__$$  |_$$$$\__$$$$ |_$$  ____/______|_||
# |  \$$\$$  /  $$\$$\$$ $$ | $$ |        () | ||
# |___\$$$  /___$$ \$$$  $$ |_$$ |___________|_||
# |    \$  /    $$ |\$  /$$ | \__|           | ||
# |_____\_/_____$$ |_\_/_$$ |________________|_||
#               \__|     \__|
# The Visual Music Project
# Created by Logan Barnes
###############################################################
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(AirWaves)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(VMP_BUILD_TESTS "Build unit tests created with GoogleTest framework" OFF)
option(VMP_BUILD_GL_TESTS "Build unit tests for graphics classes too" ON)
option(VMP_UPDATE_REPOS "Update thirdpary git repos" OFF)
option(VMP_USE_DEV_FLAGS "Compile with all the flags" OFF)
option(VMP_USE_OSMESA "Use OS Mesa for offscreen rendering" OFF)

#############################
### Project Configuration ###
#############################
if (GLC_USE_DEV_FLAGS)
    if (NOT MSVC)
        set(INTENSE_FLAGS -Wall -Wextra -Werror -Wpedantic -Winit-self
                -Wold-style-cast -Woverloaded-virtual -Wsign-conversion
                -Wmissing-declarations -Wmissing-include-dirs -Wshadow)
    else ()
        set(INTENSE_FLAGS -WX)
    endif ()
endif ()

if (MSVC)
    add_definitions(-DNOMINMAX -D_CRT_SECURE_NO_WARNINGS) # silly microsoft
endif ()

configure_file(src/VMPConfig.hpp.in ${CMAKE_BINARY_DIR}/vmp/VMPConfig.hpp)

############################
### Visual Music Project ###
############################
add_subdirectory(thirdparty)

cmake_policy(SET CMP0009 NEW)
file(GLOB_RECURSE VMP_SOURCE_FILES
        LIST_DIRECTORIES false
        CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/vmp/*
        )

add_library(vmp ${VMP_SOURCE_FILES})
target_include_directories(vmp PUBLIC src ${CMAKE_BINARY_DIR})
target_link_libraries(vmp PUBLIC vmp-thirdparty)
target_compile_options(vmp PRIVATE ${INTENSE_FLAGS})

### Air Waves GUI ###
file(GLOB_RECURSE EXEC_SOURCE_FILES
        LIST_DIRECTORIES false
        CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/exec/*
        )

add_executable(AirWaves ${EXEC_SOURCE_FILES})
target_link_libraries(AirWaves vmp airwaves-thirdparty)
target_compile_options(AirWaves PRIVATE ${INTENSE_FLAGS})

if (${VMP_BUILD_TESTS})

    # add test coverage capabilities
    if (CMAKE_COMPILER_IS_GNUCXX AND CMAKE_BUILD_TYPE MATCHES "Debug")
        include(thirdparty/cmake/CodeCoverage.cmake)
        setup_target_for_coverage(${PROJECT_NAME}_coverage vmp-tests coverage)
        target_compile_options(vmp PUBLIC -fprofile-arcs -ftest-coverage)
        target_link_libraries(vmp PUBLIC gcov)
    endif ()

    file(GLOB_RECURSE TEST_SOURCE_FILES
            LIST_DIRECTORIES false
            CONFIGURE_DEPENDS
            ${CMAKE_CURRENT_SOURCE_DIR}/src/testing/*
            )

    add_executable(vmp-tests ${TEST_SOURCE_FILES})
    add_test(NAME vmp-test COMMAND vmp-tests)
    target_link_libraries(vmp-tests vmp gmock_main)
    target_compile_options(vmp-tests PRIVATE ${INTENSE_FLAGS})
endif ()
