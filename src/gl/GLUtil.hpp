// ////////////////////////////////////////////////////////////
// Created on 3/25/18.
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

namespace gl {

/**
 * @return 0 if type is not a common GL type
 */
template <typename T>
GLenum gl_type()
{
    if (std::is_same<T, GLboolean>::value) {
        return GL_BOOL;
    }
    if (std::is_same<T, GLbyte>::value) {
        return GL_BYTE;
    }
    if (std::is_same<T, GLshort>::value) {
        return GL_SHORT;
    }
    if (std::is_same<T, GLint>::value) {
        return GL_INT;
    }
    if (std::is_same<T, GLubyte>::value) {
        return GL_UNSIGNED_BYTE;
    }
    if (std::is_same<T, GLushort>::value) {
        return GL_UNSIGNED_SHORT;
    }
    if (std::is_same<T, GLuint>::value) {
        return GL_UNSIGNED_INT;
    }
    if (std::is_same<T, GLfloat>::value) {
        return GL_FLOAT;
    }
    if (std::is_same<T, GLdouble>::value) {
        return GL_DOUBLE;
    }
    return 0;
}

} // namespace gl
