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

#include <glad/glad.h>
#include <string>
#include <memory>

namespace gl {

struct VAOElement
{
    std::string name;
    GLint size;
    GLenum type;
    void *pointer;
};

template <typename T>
class Buffer;

template <int Dim>
class Framebuffer;

class Program;
class ProgramManager;
class VertexArray;

template <typename VboType = float, typename IboType = unsigned>
struct Pipeline
{
    std::shared_ptr<GLuint> program{nullptr};
    std::shared_ptr<Buffer<VboType>> vbo{nullptr};
    std::shared_ptr<Buffer<IboType>> ibo{nullptr};
    std::shared_ptr<VertexArray> vao{nullptr};
    std::shared_ptr<Framebuffer<2>> fbo{nullptr};
};

} // namespace gl
