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
#include "Framebuffer.hpp"
#include <gl/Texture.hpp>
#include <glm/gtc/vec1.hpp>

namespace gl {

namespace detail {

namespace {

std::shared_ptr<GLuint> create_framebuffer(const glm::uvec3 &dim, GLenum tex_type, GLuint texture)
{
    std::shared_ptr<GLuint> renderbuffer{nullptr};
    std::shared_ptr<GLuint> framebuffer{nullptr};

    // no depth texture; create a renderbuffer
    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    renderbuffer = std::shared_ptr<GLuint>(new GLuint(rbo), [](auto *id) {
        glDeleteRenderbuffers(1, id);
        delete id;
    });

    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    framebuffer = std::shared_ptr<GLuint>(new GLuint(fbo), [renderbuffer](auto *id) {
        glDeleteFramebuffers(1, id);
        delete id;
    });

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // set color attachment
    glBindTexture(tex_type, texture);

    switch (tex_type) {
    case GL_TEXTURE_3D:
        glFramebufferTexture3D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex_type, texture, 0, 0);
        break;
    case GL_TEXTURE_2D:
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex_type, texture, 0);
        break;
    case GL_TEXTURE_1D:
        glFramebufferTexture1D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex_type, texture, 0);
        break;
    default:
        assert(false);
        break;
    }

    glBindRenderbuffer(GL_RENDERBUFFER, *renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, dim.x, dim.y);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // attach a renderbuffer to depth attachment point
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, *renderbuffer);

    // Check the framebuffer is ok
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        framebuffer = nullptr;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return framebuffer;
} // create_framebuffer

} // namespace

template <>
FramebufferWrapper<1>::FramebufferWrapper(
    glm::uvec1 dim, const float *array, GLint internal_format, GLenum format, GLint filter_type, GLint wrap_type)
    : full_dim_{dim.x, 1, 1}
{
    texture_ = create_texture(full_dim_, GL_TEXTURE_1D, array, internal_format, format, filter_type, wrap_type);
    framebuffer_ = create_framebuffer(full_dim_, GL_TEXTURE_1D, texture_->get_id());
}

template <>
FramebufferWrapper<2>::FramebufferWrapper(
    glm::uvec2 dim, const float *array, GLint internal_format, GLenum format, GLint filter_type, GLint wrap_type)
    : full_dim_{dim.x, dim.y, 1}
{
    texture_ = create_texture(full_dim_, GL_TEXTURE_2D, array, internal_format, format, filter_type, wrap_type);
    framebuffer_ = create_framebuffer(full_dim_, GL_TEXTURE_2D, texture_->get_id());
}

template <>
FramebufferWrapper<3>::FramebufferWrapper(
    glm::uvec3 dim, const float *array, GLint internal_format, GLenum format, GLint filter_type, GLint wrap_type)
    : full_dim_{dim}
{
    texture_ = create_texture(full_dim_, GL_TEXTURE_3D, array, internal_format, format, filter_type, wrap_type);
    framebuffer_ = create_framebuffer(full_dim_, GL_TEXTURE_3D, texture_->get_id());
}

template <int Dim>
void FramebufferWrapper<Dim>::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, get_id());
}

template <int Dim>
void FramebufferWrapper<Dim>::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

template <int Dim>
Texture FramebufferWrapper<Dim>::get_texture() const
{
    return texture_;
}

template <int Dim>
GLuint FramebufferWrapper<Dim>::get_id() const
{
    return *framebuffer_;
}

template class FramebufferWrapper<1>;
template class FramebufferWrapper<2>;
template class FramebufferWrapper<3>;

} // namespace detail

template <int Dim>
Framebuffer<Dim> create_framebuffer(glm::vec<Dim, unsigned> dim,
                                    const float *array,
                                    GLint internal_format,
                                    GLenum format,
                                    GLint filter_type,
                                    GLint wrap_type)
{
    return std::make_shared<detail::FramebufferWrapper<Dim>>(dim,
                                                             array,
                                                             internal_format,
                                                             format,
                                                             filter_type,
                                                             wrap_type);
}

Framebuffer<2> create_framebuffer(unsigned width,
                                  unsigned height,
                                  const float *array,
                                  GLint internal_format,
                                  GLenum format,
                                  GLint filter_type,
                                  GLint wrap_type)
{
    return create_framebuffer<2>({width, height}, array, internal_format, format, filter_type, wrap_type);
}

template Framebuffer<1> create_framebuffer(glm::uvec1, const float *, GLint, GLenum, GLint, GLint);
template Framebuffer<2> create_framebuffer(glm::uvec2, const float *, GLint, GLenum, GLint, GLint);
template Framebuffer<3> create_framebuffer(glm::uvec3, const float *, GLint, GLenum, GLint, GLint);

} // namespace gl
