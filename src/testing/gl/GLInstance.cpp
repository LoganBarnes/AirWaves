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

// Can uncomment this to debug if something goes wrong.
// Otherwise it throws off code coverage calculations.
//#define CHECK_ERRORS

namespace gl {

namespace testing {

GLInstance::GLInstance()
{
#ifdef CHECK_ERRORS
    // Set the error callback before any other GLFW calls so we get proper error reporting
    glfwSetErrorCallback([](int error, const char *description) {
        std::cerr << "ERROR: (" << error << ") " << description << std::endl;
    });
#endif

    auto init_err_stat = glfwInit();

#ifdef CHECK_ERRORS
    if (init_err_stat == 0) {
        throw std::runtime_error("GLFW init failed");
    }
#endif

    // can only do OpenGL 2.1 with default linux OS Mesa install
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    int width = 640;
    int height = 480;

    window_ = glfwCreateWindow(width, height, "", nullptr, nullptr);

#ifdef CHECK_ERRORS
    if (window_ == nullptr) {
        glfwTerminate();
        throw std::runtime_error("GLFW window creation failed");
    }
#endif

    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1);

    auto glad_err_stat = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

#ifdef CHECK_ERRORS
    if (glad_err_stat == 0) {
        glfwDestroyWindow(window_);
        glfwTerminate();
        throw std::runtime_error("Failed load OpenGL Glad functions");
    }

    if (GLVersion.major < 2 && GLVersion.minor < 1) {
        std::cout << "\nOpenGL version " << GLVersion.major << "." << GLVersion.minor << " loaded.\n" << std::endl;
    }
#endif
}
GLInstance::~GLInstance()
{
    glfwDestroyWindow(window_);
    glfwTerminate();
}
} // namespace testing

} // namespace gl
