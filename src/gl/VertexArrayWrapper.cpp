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
#include "VertexArrayWrapper.hpp"

namespace gl {

namespace detail {

void VertexArrayWrapper::bind() const
{
    glBindVertexArray(*vao_);
}

void VertexArrayWrapper::unbind() const
{
    glBindVertexArray(0);
}

/**
 * @warning assumes a VBO is bound
 */
void VertexArrayWrapper::set_attributes(GLuint program, GLsizei total_stride, const std::vector<VAOElement> &elements)
{
    bind();

    for (const auto &vaoElmt : elements) {
        int pos = glGetAttribLocation(program, vaoElmt.name.c_str());
        if (pos < 0) {
            std::stringstream msg;
            msg << "attrib location " << vaoElmt.name << " not found for program " << program;

            throw std::runtime_error(msg.str());
        }

        auto position = static_cast<GLuint>(pos);

        glEnableVertexAttribArray(position);
        switch (vaoElmt.type) {
        case GL_BYTE:
        case GL_UNSIGNED_BYTE:
        case GL_SHORT:
        case GL_UNSIGNED_SHORT:
        case GL_INT:
        case GL_UNSIGNED_INT:
            glVertexAttribIPointer(position,
                                   vaoElmt.size, // Num coordinates per position
                                   vaoElmt.type, // Type
                                   total_stride, // Stride, 0 = tightly packed
                                   vaoElmt.pointer // Array buffer offset
            );
            break;
        case GL_DOUBLE:
            glVertexAttribLPointer(position,
                                   vaoElmt.size, // Num coordinates per position
                                   vaoElmt.type, // Type
                                   total_stride, // Stride, 0 = tightly packed
                                   vaoElmt.pointer // Array buffer offset
            );
            break;
        default:
            glVertexAttribPointer(position,
                                  vaoElmt.size, // Num coordinates per position
                                  vaoElmt.type, // Type
                                  GL_FALSE, // Normalized
                                  total_stride, // Stride, 0 = tightly packed
                                  vaoElmt.pointer // Array buffer offset
            );
            break;
        }
    }

    unbind();
}

} // namespace detail

} // namespace gl
