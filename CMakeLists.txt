cmake_minimum_required(VERSION 3.7)
project(AirWaves)

set(CMAKE_CXX_STANDARD 14)

option(BUILD_TESTS OFF "Build unit tests created with Catch framework")

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

if (${BUILD_TESTS})
    # Download and unpack catch at configure time
    download_project(PROJ catch
            PREFIX thirdparty
            GIT_REPOSITORY https://github.com/philsquared/Catch.git
            GIT_TAG v1.10.0
            UPDATE_DISCONNECTED 1
            QUIET
            )

    set(TEST_SOURCE_FILES
            src/testing/CatchMain.cpp
            src/testing/AirWavesUnitTests.cpp
            )

    add_executable(vmp-tests ${TEST_SOURCE_FILES})
    add_test(NAME empty-test COMMAND vmp-tests)
    target_include_directories(vmp-tests PUBLIC ${catch_SOURCE_DIR}/include)
    target_link_libraries(vmp-tests vmp)
endif ()
