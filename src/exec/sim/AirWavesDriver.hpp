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
#pragma once

#include <string>
#include <memory>

struct GLFWwindow;

namespace vmp {

void delete_glfw(const int *status);
void delete_window(GLFWwindow *window);
void delete_imgui(const bool *status);

class AirWaves;

class AirWavesDriver
{
public:
    explicit AirWavesDriver(const std::string &title = "AirWaves", int width = 0, int height = 0, int samples = 4);
    ~AirWavesDriver();

    void run(double timestep = 1.0 / 60.0, double max_timestep = 0.1);

private:
    std::unique_ptr<int, decltype(&delete_glfw)> glfw_;
    std::unique_ptr<GLFWwindow, decltype(&delete_window)> window_;
    std::unique_ptr<bool, decltype(&delete_imgui)> imgui_;

    std::unique_ptr<AirWaves> air_waves_;

    bool paused_{false};

    std::size_t frames_{0};
    double total_time_{0.0};

    void register_callbacks();
    void render(double alpha);

    void handle_resize(int width, int height);
    void handle_mouse_button_event(GLFWwindow *window, int button, int action, int mods);
    void handle_scroll_event(GLFWwindow *window, double xoffset, double yoffset);
    void handle_key_event(GLFWwindow *window, int key, int scancode, int action, int mods);
    void handle_char_event(GLFWwindow *window, unsigned codepoint);
};

} // namespace vmp
