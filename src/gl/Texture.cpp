// ////////////////////////////////////////////////////////////
// Created on 3/21/18.
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
#include "Texture.hpp"

namespace gl {

namespace detail {

TextureWrapper::TextureWrapper(const glm::uvec3 &dim,
                               GLenum tex_type,
                               const float *array,
                               GLint internal_format,
                               GLenum format,
                               GLint filter_type,
                               GLint wrap_type)
    : tex_type_{tex_type}, internal_format_{internal_format}, format_{format}
{
    GLuint tex;
    glGenTextures(1, &tex);
    texture_ = std::shared_ptr<GLuint>(new GLuint(tex), [](auto *id) {
        glDeleteTextures(1, id);
        delete id;
    });

    set_wrap_type(wrap_type);
    set_filter_type(filter_type);

    bind();

    switch (tex_type_) {
    case GL_TEXTURE_3D:
        glTexImage3D(tex_type_, 0, internal_format_, dim.x, dim.y, dim.z, 0, format_, GL_FLOAT, array);
        break;
    case GL_TEXTURE_2D:
        glTexImage2D(tex_type_, 0, internal_format_, dim.x, dim.y, 0, format_, GL_FLOAT, array);
        break;
    case GL_TEXTURE_1D:
        glTexImage1D(tex_type_, 0, internal_format_, dim.x, 0, format_, GL_FLOAT, array);
        break;
    default:
        assert(false);
        break;
    }
}

void TextureWrapper::bind() const
{
    glBindTexture(tex_type_, get_id());
}

void TextureWrapper::set_filter_type(GLint filter_type)
{
    filter_type_ = filter_type;

    bind();
    glTexParameteri(tex_type_, GL_TEXTURE_MIN_FILTER, filter_type_);
    glTexParameteri(tex_type_, GL_TEXTURE_MAG_FILTER, filter_type_);
}

void TextureWrapper::set_wrap_type(GLint wrap_type)
{
    wrap_type_ = wrap_type;
    bind();
    glTexParameteri(tex_type_, GL_TEXTURE_WRAP_S, wrap_type_);
    glTexParameteri(tex_type_, GL_TEXTURE_WRAP_T, wrap_type_);
}

GLuint TextureWrapper::get_id() const
{
    return *texture_;
}

} // namespace detail

Texture create_texture(unsigned width,
                       unsigned height,
                       const float *array,
                       GLint internal_format,
                       GLenum format,
                       GLint filter_type,
                       GLint wrap_type,
                       GLenum tex_type)
{
    return create_texture({width, height}, array, internal_format, format, filter_type, wrap_type, tex_type);
}

Texture create_texture(const glm::uvec2 &size,
                       const float *array,
                       GLint internal_format,
                       GLenum format,
                       GLint filter_type,
                       GLint wrap_type,
                       GLenum tex_type)
{
    return create_texture({size.x, size.y, 1}, tex_type, array, internal_format, format, filter_type, wrap_type);
}

Texture create_texture(const glm::uvec3 &dim,
                       GLenum tex_type,
                       const float *array,
                       GLint internal_format,
                       GLenum format,
                       GLint filter_type,
                       GLint wrap_type)
{
    return std::make_shared<detail::TextureWrapper>(dim,
                                                    tex_type,
                                                    array,
                                                    internal_format,
                                                    format,
                                                    filter_type,
                                                    wrap_type);
}

} // namespace gl
