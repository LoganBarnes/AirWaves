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
#include <gl/Program.hpp>
#include <gl/Buffer.hpp>
#include <vector>
#include <sstream>

namespace gl {

namespace detail {

class VertexArrayWrapper
{
public:
    template <typename T>
    explicit VertexArrayWrapper(const Program &program,
                                const Buffer<T> &vbo,
                                GLsizei total_stride,
                                const std::vector<VAOElement> &elements);

    void bind() const;
    void unbind() const;

    GLuint get_id() const;

    template <typename IboType = unsigned>
    void render(GLenum mode, int start, int num_elements, const Buffer<IboType> &ibo = nullptr);

private:
    std::shared_ptr<GLuint> vao_;

    /**
     * @warning assumes a VBO is bound
     */
    void set_attributes(GLuint program_id, GLsizei total_stride, const std::vector<VAOElement> &elements);
};

template <typename T>
VertexArrayWrapper::VertexArrayWrapper(const Program &program,
                                       const Buffer<T> &vbo,
                                       const GLsizei total_stride,
                                       const std::vector<VAOElement> &elements)
{
    GLuint vao;
    glGenVertexArrays(1, &vao);
    vao_ = std::shared_ptr<GLuint>(new GLuint(vao), [](auto *id) {
        glDeleteVertexArrays(1, id);
        delete id;
    });

    vbo->bind();
    set_attributes(program->get_id(), total_stride, elements);
    vbo->unbind();
}

template <typename IboType>
void VertexArrayWrapper::render(GLenum mode, int start, int num_elements, const Buffer<IboType> &ibo)
{
    bind();

    if (ibo) {
        assert(ibo->get_data_type() == GL_UNSIGNED_BYTE || ibo->get_data_type() == GL_UNSIGNED_SHORT
               || ibo->get_data_type() == GL_UNSIGNED_INT);

        ibo->bind();
        glDrawElements(mode, num_elements, ibo->get_data_type(), reinterpret_cast<void *>(start * sizeof(IboType)));
        ibo->unbind();
    } else {
        glDrawArrays(mode, start, num_elements);
    }

    unbind();
}

} // namespace detail

template <typename T>
VertexArray create_vertex_array(const Program &program,
                                const Buffer<T> &vbo,
                                GLsizei total_stride,
                                const std::vector<VAOElement> &elements)
{
    return std::make_shared<detail::VertexArrayWrapper>(program, vbo, total_stride, elements);
}

} // namespace gl
