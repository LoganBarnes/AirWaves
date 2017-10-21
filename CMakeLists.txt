cmake_minimum_required(VERSION 3.7)
project(AirWaves)

set(CMAKE_CXX_STANDARD 14)

option(VMP_BUILD_TESTS OFF "Build unit tests created with GoogleTest framework")

include(cmake/DownloadProject.cmake)

# Download and unpack simulation driver at configure time
download_project(PROJ sim
        PREFIX thirdparty
        GIT_REPOSITORY https://github.com/LoganBarnes/SimulationDriver.git
        GIT_TAG master
        UPDATE_DISCONNECTED 1
        QUIET
        )

set(SIM_BUILD_EXECS OFF CACHE BOOL "" FORCE)
set(SIM_BUILD_CUDA OFF CACHE BOOL "" FORCE)

# compile sim with current project
add_subdirectory(${sim_SOURCE_DIR} ${sim_BINARY_DIR})


# Download and unpack RtAudio at configure time
download_project(PROJ rtAudio
        PREFIX thirdparty
        GIT_REPOSITORY https://github.com/LoganBarnes/rtaudio.git
        GIT_TAG master
        UPDATE_DISCONNECTED 1
        QUIET
        )

# compile RtAudio with current project
add_subdirectory(${rtAudio_SOURCE_DIR} ${rtAudio_BINARY_DIR})
target_include_directories(rtaudio_static PUBLIC ${rtAudio_SOURCE_DIR})

set(VMP_SOURCE_FILES
        src/vmp/Transport.cpp
        src/vmp/Transport.hpp
        )

add_library(vmp ${VMP_SOURCE_FILES})
target_include_directories(vmp PUBLIC src)
target_link_libraries(vmp SimDriver rtaudio_static)

set(EXEC_SOURCE_FILES
        src/exec/AirWaves.cpp
        src/exec/AirWaves.hpp
        src/exec/main.cpp
        )
add_executable(AirWaves ${EXEC_SOURCE_FILES})
target_link_libraries(AirWaves vmp)

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
