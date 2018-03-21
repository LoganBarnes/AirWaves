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

namespace detail {

template <typename T>
class BufferWrapper;

} // namespace detail

template <int Dim>
class Framebuffer;

class Program;
class ProgramManager;
class VertexArray;

template <typename T>
using Buffer = std::shared_ptr<detail::BufferWrapper<T>>;

template <int Dim>
using FramebufferHandle = std::shared_ptr<Framebuffer<Dim>>;

using ProgramHandle = std::shared_ptr<Program>;
using VertexArrayHandle = std::shared_ptr<Program>;

template <typename VboType = float, typename IboType = unsigned>
struct Pipeline
{
    ProgramHandle program{nullptr};
    Buffer<VboType> vbo{nullptr};
    Buffer<IboType> ibo{nullptr};
    VertexArrayHandle vao{nullptr};
    FramebufferHandle<2> fbo{nullptr};
};

struct VAOElement
{
    std::string name;
    GLint size;
    GLenum type;
    void *pointer;
};

} // namespace gl
