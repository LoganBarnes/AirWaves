###############################################################
# Created on 08/07/2018.
# Copyright (c) 2018. All rights reserved.
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
include(cmake/DownloadProject.cmake)

if (${VMP_UPDATE_REPOS})
    set(UPDATE_STATUS 0)
else ()
    set(UPDATE_STATUS 1)
endif ()

### RtAudio ###
download_project(PROJ rtAudio-dl
        PREFIX thirdparty
        GIT_REPOSITORY https://github.com/LoganBarnes/rtaudio.git
        GIT_TAG master
        UPDATE_DISCONNECTED ${UPDATE_STATUS}
        QUIET
        )

set(AUDIO_LINUX_PULSE ON CACHE BOOL "" FORCE)

# compile RtAudio with current project
add_subdirectory(${rtAudio-dl_SOURCE_DIR} ${rtAudio-dl_BINARY_DIR})


### GLC ###
download_project(PROJ glc-dl
        PREFIX thirdparty
        GIT_REPOSITORY https://gitlab.com/LoganBarnes/glc.git
        GIT_TAG master
        UPDATE_DISCONNECTED ${UPDATE_STATUS}
        QUIET
        )

set(GLC_UPDATE_REPOS ${VMP_UPDATE_REPOS} CACHE BOOL "" FORCE)
set(GLC_USE_DEV_FLAGS ${VMP_USE_DEV_FLAGS} CACHE BOOL "" FORCE)
set(GLC_USE_OSMESA ${VMP_USE_OSMESA} CACHE BOOL "" FORCE)

# compile glc with current project
add_subdirectory(${glc-dl_SOURCE_DIR} ${glc-dl_BINARY_DIR})

### GLFW ###
download_project(PROJ glfw-dl
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
add_subdirectory(${glfw-dl_SOURCE_DIR} ${glfw-dl_BINARY_DIR})

### ImGui ###
download_project(PROJ imgui-dl
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        PREFIX thirdparty
        GIT_TAG v1.60
        UPDATE_DISCONNECTED ${UPDATE_STATUS}
        QUIET
        )

if (${VMP_BUILD_TESTS})
    # Download and unpack googletest at configure time
    download_project(PROJ googletest-dl
            PREFIX thirdparty
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG master
            UPDATE_DISCONNECTED ${UPDATE_STATUS}
            QUIET
            )

    # Prevent overriding the parent project's compiler/linker settings on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # compile googletest with current project
    add_subdirectory(${googletest-dl_SOURCE_DIR} ${googletest-dl_BINARY_DIR} EXCLUDE_FROM_ALL)

    target_include_directories(gmock_main SYSTEM BEFORE INTERFACE
            "${googletest-dl_SOURCE_DIR}/googletest/include"
            "${googletest-dl_SOURCE_DIR}/googlemock/include"
            )

    include(CTest)
    enable_testing()
endif ()

##########################
### Thirdparty targets ###
##########################

### VMP ###
add_library(vmp-thirdparty INTERFACE)

target_include_directories(vmp-thirdparty SYSTEM INTERFACE ${rtAudio-dl_SOURCE_DIR})
target_link_libraries(vmp-thirdparty INTERFACE rtaudio_static glc)
target_compile_options(vmp-thirdparty INTERFACE -fPIC)

### AirWaves ###
add_library(airwaves-thirdparty
        ${imgui-dl_SOURCE_DIR}/imconfig.h
        ${imgui-dl_SOURCE_DIR}/imgui_demo.cpp
        ${imgui-dl_SOURCE_DIR}/imgui_draw.cpp
        ${imgui-dl_SOURCE_DIR}/imgui_internal.h
        ${imgui-dl_SOURCE_DIR}/imgui.cpp
        ${imgui-dl_SOURCE_DIR}/imgui.h
        ${imgui-dl_SOURCE_DIR}/stb_rect_pack.h
        ${imgui-dl_SOURCE_DIR}/stb_textedit.h
        ${imgui-dl_SOURCE_DIR}/stb_truetype.h
        ${imgui-dl_SOURCE_DIR}/examples/opengl3_example/imgui_impl_glfw_gl3.h
        ${imgui-dl_SOURCE_DIR}/examples/opengl3_example/imgui_impl_glfw_gl3.cpp
        )

target_include_directories(airwaves-thirdparty SYSTEM PUBLIC
        ${imgui-dl_SOURCE_DIR}/examples/opengl3_example
        ${imgui-dl_SOURCE_DIR}
        )

target_link_libraries(airwaves-thirdparty PUBLIC glfw vmp-thirdparty)

target_compile_options(airwaves-thirdparty PRIVATE -fPIC)
