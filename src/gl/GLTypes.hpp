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

template <int Dim>
class FramebufferWrapper;

class ProgramWrapper;
class TextureWrapper;
class VertexArrayWrapper;

} // namespace detail

class ProgramManager;

template <typename T>
class TCamera;

using Camera = TCamera<float>;

template <typename T>
using Buffer = std::shared_ptr<detail::BufferWrapper<T>>;

template <int Dim>
using Framebuffer = std::shared_ptr<detail::FramebufferWrapper<Dim>>;

using Program = std::shared_ptr<detail::ProgramWrapper>;
using Texture = std::shared_ptr<detail::TextureWrapper>;
using VertexArray = std::shared_ptr<detail::VertexArrayWrapper>;

template <typename VboType = float, typename IboType = unsigned>
struct Pipeline
{
    Program program{nullptr};
    Buffer<VboType> vbo{nullptr};
    Buffer<IboType> ibo{nullptr};
    VertexArray vao{nullptr};
    Texture tex{nullptr};
    Framebuffer<2> fbo{nullptr};
};

struct VAOElement
{
    std::string name;
    GLint size;
    GLenum type;
    void *pointer;
};

} // namespace gl
