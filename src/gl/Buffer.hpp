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
#include <vector>

namespace gl {

namespace detail {

template <typename T>
class BufferWrapper
{
public:
    BufferWrapper(const T *data, std::size_t num_elements, GLenum type, GLenum usage);

    void bind() const;
    void unbind() const;

    void update(size_t element_offset, const T *data, size_t num_elements) const;
    void update(size_t element_offset, const std::vector<T> &data) const;

    GLuint get_id() const;
    GLenum get_buffer_type() const;
    GLenum get_data_type() const;

private:
    std::shared_ptr<GLuint> buffer_;
    GLenum type_;
};

template <typename T>
BufferWrapper<T>::BufferWrapper(const T *data, std::size_t num_elements, GLenum type, GLenum usage) : type_(type)
{
    GLuint buf;
    glGenBuffers(1, &buf);
    buffer_ = std::shared_ptr<GLuint>(new GLuint(buf), [](auto *id) {
        glDeleteBuffers(1, id);
        delete id;
    });

    bind();
    glBufferData(type_, static_cast<GLsizeiptr>(num_elements * sizeof(T)), data, usage);
    unbind();
}

template <typename T>
void BufferWrapper<T>::bind() const
{
    glBindBuffer(type_, *buffer_);
}

template <typename T>
void BufferWrapper<T>::unbind() const
{
    glBindBuffer(type_, 0);
}

template <typename T>
void BufferWrapper<T>::update(size_t element_offset, const T *data, size_t num_elements) const
{
    constexpr auto type_size_bytes = sizeof(T);

    bind();
    glBufferSubData(type_,
                    static_cast<GLintptr>(element_offset * type_size_bytes),
                    static_cast<GLsizeiptr>(num_elements * type_size_bytes),
                    data);
    unbind();
}

template <typename T>
void BufferWrapper<T>::update(size_t element_offset, const std::vector<T> &data) const
{
    update(element_offset, data.data(), data.size());
}

template <typename T>
GLuint BufferWrapper<T>::get_id() const
{
    return *buffer_;
}

template <typename T>
GLenum BufferWrapper<T>::get_buffer_type() const
{
    return type_;
}

template <typename T>
GLenum BufferWrapper<T>::get_data_type() const
{
    if (std::is_same<T, unsigned char>::value) {
        return GL_UNSIGNED_BYTE;
    }
    if (std::is_same<T, unsigned short>::value) {
        return GL_UNSIGNED_SHORT;
    }
    if (std::is_same<T, unsigned int>::value) {
        return GL_UNSIGNED_INT;
    }
    if (std::is_same<T, float>::value) {
        return GL_FLOAT;
    }
    throw std::runtime_error("No OpenGL data type equivalent");
}

} // namespace detail

template <typename T>
Buffer<T>
create_buffer(const T *data, std::size_t num_elements, GLenum type = GL_ARRAY_BUFFER, GLenum usage = GL_STATIC_DRAW)
{
    return std::make_shared<detail::BufferWrapper<T>>(data, num_elements, type, usage);
}

template <typename T>
Buffer<T> create_buffer(const std::vector<T> &data, GLenum type = GL_ARRAY_BUFFER, GLenum usage = GL_STATIC_DRAW)
{
    return create_buffer(data.data(), data.size(), type, usage);
}

} // namespace gl
