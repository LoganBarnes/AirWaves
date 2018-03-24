// ////////////////////////////////////////////////////////////
// Created on 3/24/18.
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
#include "GLInstance.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

namespace gl {

namespace testing {

GLInstance::GLInstance()
{
    // Set the error callback before any other GLFW calls so we get proper error reporting
    glfwSetErrorCallback([](int error, const char *description) {
        std::cerr << "ERROR: (" << error << ") " << description << std::endl;
    });

    if (glfwInit() == 0) {
        throw std::runtime_error("GLFW init failed");
    }

    // can only do OpenGL 2.1 with default linux OS Mesa install
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    int width = 640;
    int height = 480;

    window_ = glfwCreateWindow(width, height, "", nullptr, nullptr);

    if (window_ == nullptr) {
        glfwTerminate();
        throw std::runtime_error("GLFW window creation failed");
    }

    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1);

    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0) {
        glfwDestroyWindow(window_);
        glfwTerminate();
        throw std::runtime_error("Failed load OpenGL Glad functions");
    }

    std::cout << "\nOpenGL version " << GLVersion.major << "." << GLVersion.minor << " loaded.\n" << std::endl;
}
GLInstance::~GLInstance()
{
    glfwDestroyWindow(window_);
    glfwTerminate();
}
} // namespace testing

} // namespace gl
