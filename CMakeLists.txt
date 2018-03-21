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
cmake_minimum_required(VERSION 3.7.0 FATAL_ERROR)
project(AirWaves)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(VMP_BUILD_TESTS "Build unit tests created with GoogleTest framework" OFF)
option(VMP_BUILD_GL_TESTS "Build unit tests for graphics classes too" ON)
option(VMP_UPDATE_REPOS "Update thirdpary git repos" OFF)
option(VMP_USE_DEV_FLAGS "Compile with all the flags" OFF)
option(VMP_USE_OSMESA "Use OS Mesa for offscreen rendering" OFF)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

######################
### EXTERNAL REPOS ###
######################
include(DownloadProject)

if (VMP_UPDATE_REPOS)
    set(UPDATE_STATUS 0)
else ()
    set(UPDATE_STATUS 1)
endif ()

### RtAudio ###
download_project(PROJ rtAudio
        PREFIX thirdparty
        GIT_REPOSITORY https://github.com/LoganBarnes/rtaudio.git
        GIT_TAG master
        UPDATE_DISCONNECTED ${UPDATE_STATUS}
        QUIET
        )

# compile RtAudio with current project
add_subdirectory(${rtAudio_SOURCE_DIR} ${rtAudio_BINARY_DIR})
target_include_directories(rtaudio_static SYSTEM PUBLIC ${rtAudio_SOURCE_DIR})

### GLFW ###
download_project(PROJ glfw
        PREFIX thirdparty
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG master
        UPDATE_DISCONNECTED ${UPDATE_STATUS}
        QUIET
        )

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_USE_OSMESA ${VMP_USE_OSMESA} CACHE BOOL "" FORCE)

# compile glfw with current project
add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})

### GLM ###
download_project(PROJ glm
        PREFIX thirdparty
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG master
        UPDATE_DISCONNECTED ${UPDATE_STATUS}
        QUIET
        )

### ImGui ###
download_project(PROJ imgui
        PREFIX thirdparty
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.53
        UPDATE_DISCONNECTED ${UPDATE_STATUS}
        QUIET
        )

add_library(thirdparty
        # imgui
        ${imgui_SOURCE_DIR}/imconfig.h
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui.h
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_internal.h
        ${imgui_SOURCE_DIR}/stb_rect_pack.h
        ${imgui_SOURCE_DIR}/stb_textedit.h
        ${imgui_SOURCE_DIR}/stb_truetype.h
        src/thirdparty/imgui_impl_glfw_gl3.cpp
        src/thirdparty/imgui_impl_glfw_gl3.h
        # glad
        src/thirdparty/glad/glad.c
        src/thirdparty/glad/glad.h
        src/thirdparty/KHR/khrplatform.h
        )

target_include_directories(thirdparty SYSTEM PUBLIC
        ${glfw_SOURCE_DIR}/include
        ${imgui_SOURCE_DIR}
        ${glm_SOURCE_DIR}
        src/thirdparty
        )
target_link_libraries(thirdparty glfw)

### Graphics Library ###
# currently an internal directory with the
# intent of becoming its own project
set(GL_SOURCE
        src/gl/Buffer.hpp
        src/gl/FramebufferWrapper.cpp
        src/gl/FramebufferWrapper.hpp
        src/gl/GLTypes.hpp
        src/gl/Program.cpp
        src/gl/Program.hpp
        src/gl/ProgramManager.cpp
        src/gl/ProgramManager.hpp
        src/gl/VertexArrayWrapper.cpp
        src/gl/VertexArrayWrapper.hpp
        )

add_library(graphics ${GL_SOURCE})
target_include_directories(graphics PUBLIC src)
target_link_libraries(graphics thirdparty)

#############################
### Project Configuration ###
#############################
if (VMP_USE_DEV_FLAGS)
    if (NOT MSVC)
        set(INTENSE_FLAGS -Wall -Wextra -Winit-self -Winit-self
                -Wold-style-cast -Woverloaded-virtual -Werror
                -Wmissing-declarations -pedantic -Wuninitialized)
    else ()
        set(INTENSE_FLAGS -WX)
    endif ()
endif ()

if (MSVC)
    add_definitions(-DNOMINMAX -D_CRT_SECURE_NO_WARNINGS) # silly microsoft
endif()

configure_file(src/VmpConfig.hpp.in ${CMAKE_BINARY_DIR}/vmp/VMPConfig.hpp)

############################
### Visual Music Project ###
############################

set(VMP_SOURCE_FILES
        src/vmp/audio/sources/SawSource.hpp
        src/vmp/audio/sources/SineSource.hpp
        src/vmp/audio/MainStream.cpp
        src/vmp/audio/MainStream.hpp
        src/vmp/audio/Output.cpp
        src/vmp/audio/Output.hpp
        src/vmp/audio/Sound.hpp
        src/vmp/audio/Source.cpp
        src/vmp/audio/Source.hpp
        src/vmp/VMP.cpp
        src/vmp/VMP.hpp
        src/vmp/VmpTypes.hpp

        src/shaders/orb.frag
        src/shaders/orb.vert
        )

add_library(vmp ${VMP_SOURCE_FILES})
target_include_directories(vmp PUBLIC src ${CMAKE_BINARY_DIR})
target_link_libraries(vmp rtaudio_static graphics)
target_compile_options(vmp PRIVATE ${INTENSE_FLAGS} $<$<CONFIG:Debug>:${PROFILE_FLAGS}>)

set(EXEC_SOURCE_FILES
        src/exec/sim/AirWavesDriver.cpp
        src/exec/sim/AirWavesDriver.hpp

        src/exec/AirWaves.cpp
        src/exec/AirWaves.hpp
        src/exec/main.cpp
        src/exec/Transport.cpp
        src/exec/Transport.hpp
        )

add_executable(AirWaves ${EXEC_SOURCE_FILES})
target_link_libraries(AirWaves vmp)
target_compile_options(AirWaves PRIVATE ${INTENSE_FLAGS})

add_executable(sound-test src/exec/simple_sound_test.cpp)
target_link_libraries(sound-test vmp)
target_compile_options(sound-test PRIVATE ${INTENSE_FLAGS})

if (${VMP_BUILD_TESTS})
    # Download and unpack googletest at configure time
    download_project(PROJ googletest
            PREFIX thirdparty
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG master
            UPDATE_DISCONNECTED ${UPDATE_STATUS}
            QUIET
            )

    # Prevent overriding the parent project's compiler/linker settings on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # compile googletest with current project
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)

    target_include_directories(gmock_main SYSTEM BEFORE INTERFACE
            "${googletest_SOURCE_DIR}/googletest/include"
            "${googletest_SOURCE_DIR}/googlemock/include")

    # add test coverage capabilities
    if (CMAKE_COMPILER_IS_GNUCXX AND CMAKE_BUILD_TYPE MATCHES "Debug")
        include(CodeCoverage)
        setup_target_for_coverage(${PROJECT_NAME}_coverage vmp-tests coverage)
        set(PROFILE_FLAGS -fprofile-arcs -ftest-coverage)
        set(PROFILE_LIBS gcov)
        target_compile_options(vmp PRIVATE ${PROFILE_FLAGS})
        target_link_libraries(vmp ${PROFILE_LIBS})
    endif ()

    include(CTest)
    enable_testing()

    set(TEST_SOURCE_FILES
            src/testing/AudioDeviceTests.cpp
            )

    if (VMP_BUILD_GL_TESTS)
        set(TEST_SOURCE_FILES
                ${TEST_SOURCE_FILES}

                src/testing/gl/ProgramManagerTests.cpp
                src/testing/gl/shaders/shader.frag
                src/testing/gl/shaders/shader.vert
                )
    endif ()

    add_executable(vmp-tests ${TEST_SOURCE_FILES})
    add_test(NAME vmp-test COMMAND vmp-tests)
    target_link_libraries(vmp-tests vmp gmock_main ${PROFILE_LIBS})
    target_compile_options(vmp-tests PRIVATE ${INTENSE_FLAGS} ${PROFILE_FLAGS})
endif ()
