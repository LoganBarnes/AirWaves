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

#include <gl/GLTypes.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>

namespace gl {

namespace detail {

template <int Dim>
class FramebufferWrapper
{
public:
    explicit FramebufferWrapper(glm::vec<Dim, unsigned> dim,
                                const float *array = nullptr,
                                GLint internal_format = GL_RGBA32F,
                                GLenum format = GL_RGBA,
                                GLint filter_type = GL_NEAREST,
                                GLint wrap_type = GL_REPEAT);

    void bind() const;
    void unbind() const;

    template <typename UsageFunc>
    void use(const UsageFunc &usage_func) const;

    Texture get_texture() const;
    GLuint get_id() const;

private:
    Texture texture_;
    std::shared_ptr<GLuint> framebuffer_;
    glm::uvec3 full_dim_;
};

template <int Dim>
template <typename UsageFunc>
void FramebufferWrapper<Dim>::use(const UsageFunc &usage_func) const
{
    bind();
    glViewport(0, 0, full_dim_.x, full_dim_.y);
    usage_func();
    unbind();
}

} // namespace detail

template <int Dim>
Framebuffer<Dim> create_framebuffer(glm::vec<Dim, unsigned> dim,
                                    const float *array = nullptr,
                                    GLint internal_format = GL_RGBA32F,
                                    GLenum format = GL_RGBA,
                                    GLint filter_type = GL_NEAREST,
                                    GLint wrap_type = GL_REPEAT);

Framebuffer<2> create_framebuffer(unsigned width,
                                  unsigned height,
                                  const float *array = nullptr,
                                  GLint internal_format = GL_RGBA32F,
                                  GLenum format = GL_RGBA,
                                  GLint filter_type = GL_NEAREST,
                                  GLint wrap_type = GL_REPEAT);

} // namespace gl
