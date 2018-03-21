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
#pragma once

#include <gl/GLTypes.hpp>
#include <glm/glm.hpp>

namespace gl {

namespace detail {

class TextureWrapper
{
public:
    TextureWrapper(const glm::uvec3 &dim,
                   GLenum tex_type,
                   const float *array,
                   GLint internal_format,
                   GLenum format,
                   GLint filter_type,
                   GLint wrap_type);

    void bind() const;

    void set_filter_type(GLint filter_type);
    void set_wrap_type(GLint wrap_type);

    GLuint get_id() const;

private:
    std::shared_ptr<GLuint> texture_;
    GLenum tex_type_;
    GLint internal_format_;
    GLenum format_;
    GLint filter_type_;
    GLint wrap_type_;
};

} // namespace detail

Texture create_texture(const glm::uvec3 &dim,
                       GLenum tex_type,
                       const float *array = nullptr,
                       GLint internal_format = GL_RGBA32F,
                       GLenum format = GL_RGBA,
                       GLint filter_type = GL_NEAREST,
                       GLint wrap_type = GL_REPEAT);

} // namespace gl
