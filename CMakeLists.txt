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
##############################################################
cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(AirWaves LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

option(VMP_BUILD_TESTS OFF "Build unit tests created with GoogleTest framework")

######################
### EXTERNAL REPOS ###
######################
include(cmake/DownloadProject.cmake)

### SimulationDriver ###
download_project(PROJ sim
        PREFIX thirdparty
        GIT_REPOSITORY https://github.com/LoganBarnes/SimulationDriver.git
        GIT_TAG master
        UPDATE_DISCONNECTED 1
        QUIET
        )

set(SIM_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# compile sim with current project
add_subdirectory(${sim_SOURCE_DIR} ${sim_BINARY_DIR})

### RtAudio ###
download_project(PROJ rtAudio
        PREFIX thirdparty
        GIT_REPOSITORY https://github.com/LoganBarnes/rtaudio.git
        GIT_TAG master
        UPDATE_DISCONNECTED 1
        QUIET
        )

# compile RtAudio with current project
add_subdirectory(${rtAudio_SOURCE_DIR} ${rtAudio_BINARY_DIR})
target_include_directories(rtaudio_static SYSTEM PUBLIC ${rtAudio_SOURCE_DIR})

############################
### Visual Music Project ###
############################
if (NOT MSVC)
    set(INTENSE_FLAGS -Wall -Wextra -Winit-self -Winit-self
            -Wold-style-cast -Woverloaded-virtual -Werror
            -Wmissing-declarations -pedantic -Wuninitialized)
else ()
    set(INTENSE_FLAGS -WX)
endif ()

set(VMP_SOURCE_FILES
        src/vmp/Transport.cpp
        src/vmp/Transport.hpp
        )

add_library(vmp ${VMP_SOURCE_FILES})
target_include_directories(vmp PUBLIC src)
target_link_libraries(vmp SimDriver rtaudio_static)
target_compile_options(vmp PRIVATE ${INTENSE_FLAGS})

set(EXEC_SOURCE_FILES
        src/exec/AirWaves.cpp
        src/exec/AirWaves.hpp
        src/exec/main.cpp
        )
add_executable(AirWaves ${EXEC_SOURCE_FILES})
target_link_libraries(AirWaves vmp)
target_compile_options(AirWaves PRIVATE ${INTENSE_FLAGS})

add_executable(sound-test src/exec/simple_sound_test.cpp)
target_link_libraries(sound-test vmp)
target_compile_options(sound-test PRIVATE ${INTENSE_FLAGS})

if (${VMP_BUILD_TESTS})
    # Download and unpack googletest at configure time
    download_project(PROJ gtest
            PREFIX thirdparty
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG release-1.8.0
            UPDATE_DISCONNECTED 1
            QUIET
            )
    # Prevent overriding the parent project's compiler/linker settings on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # compile googletest with current project
    add_subdirectory(${gtest_SOURCE_DIR}/googletest ${gtest_BINARY_DIR}/googletest)

    set(TEST_SOURCE_FILES
            src/testing/AudioDeviceTests.cpp
            )

    add_executable(vmp-tests ${TEST_SOURCE_FILES})
    add_test(NAME empty-test COMMAND vmp-tests)
    target_link_libraries(vmp-tests vmp gtest_main)
endif ()
