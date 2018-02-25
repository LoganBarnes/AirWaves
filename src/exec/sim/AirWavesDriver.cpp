// ////////////////////////////////////////////////////////////
// Created on 2/25/18.
// Copyright (c) 2018. All rights reserved.
//
//  ___________________________$$$$$$$\__________
// | $$\    $$\                $$  __$$\      | ||
// |_$$ |___$$ |_$$\______$$\__$$ |__$$ |_____|_||
// | $$ |   $$ | $$$\    $$$ | $$$$$$$  |  () | ||
// |_\$$\__$$  |_$$$$\__$$$$ |_$$  ____/______|_||
// |  \$$\$$  /  $$\$$\$$ $$ | $$ |        () | ||
// |___\$$$  /___$$ \$$$  $$ |_$$ |___________|_||
// |    \$  /    $$ |\$  /$$ | \__|           | ||
// |_____\_/_____$$ |_\_/_$$ |________________|_||
//               \__|     \__|
// The Visual Music Project
// Created by Logan T. Barnes
// ////////////////////////////////////////////////////////////
#include "AirWavesDriver.hpp"
#include <exec/AirWaves.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include <iostream>
#include <chrono>

namespace vmp {

namespace {

std::unique_ptr<int, decltype(&delete_glfw)> init_glfw()
{
    // Set the error callback before any other GLFW calls so we get proper error reporting
    glfwSetErrorCallback([](int error, const char *description) {
        std::cerr << "ERROR: (" << error << ") " << description << std::endl;
    });

    std::unique_ptr<int, decltype(&delete_glfw)> glfw(new int(glfwInit()), delete_glfw);

    if (*glfw == 0) {
        throw std::runtime_error("GLFW init failed");
    }
    return glfw;
}

std::unique_ptr<GLFWwindow, decltype(&delete_window)>
init_window(int width, int height, const std::string &title, int samples)
{

#ifdef OFFSCREEN
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_OSMESA_CONTEXT_API);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    width = 640;
    height = 480;
#else
    if (width == 0 || height == 0) {
        const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        width = mode->width;
        height = mode->height;
    }

    if (title.empty()) {
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); // highest on mac :(
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif // __APPLE__

#endif

    glfwWindowHint(GLFW_SAMPLES, samples);

    std::unique_ptr<GLFWwindow, decltype(&delete_window)>
        window(glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr), delete_window);

    if (window == nullptr) {
        throw std::runtime_error("GLFW window creation failed");
    }

    glfwMakeContextCurrent(window.get());
    glfwSwapInterval(1);

    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0) {
        throw std::runtime_error("Failed load OpenGL Glad functions");
    }
    return window;
}

std::unique_ptr<bool, decltype(&delete_imgui)> init_imgui(GLFWwindow *window)
{
    std::unique_ptr<bool, decltype(&delete_imgui)> imgui(new bool(ImGui_ImplGlfwGL3_Init(window, false)), delete_imgui);

    if (!imgui) {
        throw std::runtime_error("ImGui initialization failed");
    }
    return imgui;
}

} // namespace

void delete_glfw(const int *status)
{
    glfwTerminate();
    delete status;
}

void delete_window(GLFWwindow *window)
{
    glfwDestroyWindow(window);
}
void delete_imgui(const bool *status)
{
    ImGui_ImplGlfwGL3_Shutdown();
    delete status;
}

AirWavesDriver::AirWavesDriver(const std::string &title, int width, int height, int samples)
    : glfw_(init_glfw()),
      window_(init_window(width, height, title, samples)),
      imgui_(init_imgui(window_.get())),
      air_waves_(nullptr)
{
    int w, h;
    glfwGetFramebufferSize(window_.get(), &w, &h);

    air_waves_ = std::make_unique<AirWaves>(w, h);
}

AirWavesDriver::~AirWavesDriver() = default;

void AirWavesDriver::run(double timestep, double max_timestep)
{
    auto current_time = std::chrono::steady_clock::now();
    double accumulator = 0.0;

    do {
        auto new_time = std::chrono::steady_clock::now();
        double frame_time = std::chrono::duration<double>{new_time - current_time}.count();
        current_time = new_time;

        frame_time = std::min(max_timestep, frame_time);

        if (!paused_) {
            accumulator += frame_time;

            while (accumulator >= timestep) {
                //                air_waves_.update();
                total_time_ += timestep;
                accumulator -= timestep;
                ++frames_;
            }
        }

        const double alpha = accumulator / timestep;

        render(alpha);

        if (paused_) {
            glfwWaitEvents();
        } else {
            glfwPollEvents();
        }
    } while (!glfwWindowShouldClose(window_.get()));
}

void AirWavesDriver::render(double alpha)
{
    int w, h;
    glfwGetFramebufferSize(window_.get(), &w, &h);

    glViewport(0, 0, w, h);

    // If the program is paused, ImGui may doesn't always register resize updates in a single frame.
    // Therefore we create the frame twice so we the updates will be present by the time we render.
    if (paused_) {
        ImGui_ImplGlfwGL3_NewFrame();
        air_waves_->configure_gui(w, h);
        ImGui::Render();
    }

    ImGui_ImplGlfwGL3_NewFrame();
    air_waves_->configure_gui(w, h);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    air_waves_->render(w, h, alpha);
    ImGui::Render();
    glfwSwapBuffers(window_.get());
}

} // namespace vmp
