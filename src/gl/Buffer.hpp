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

#include <gl/GLUtil.hpp>
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

    void update(size_t start_index, size_t num_elements, const T *data) const;
    void update(size_t start_index, const std::vector<T> &data) const;

    GLuint get_id() const;
    GLenum get_buffer_type() const;
    GLenum get_data_type() const;
    std::size_t get_num_elements() const;

    void get_data(T *data, std::size_t num_elements, std::size_t start_index = 0) const;
    void get_data(std::vector<T> *data, std::size_t start_index = 0) const;

private:
    std::shared_ptr<GLuint> buffer_;
    std::size_t num_elements_;
    GLenum buffer_type_;
    GLenum data_type_;
};

template <typename T>
BufferWrapper<T>::BufferWrapper(const T *data, std::size_t num_elements, GLenum type, GLenum usage)
    : num_elements_(num_elements), buffer_type_(type), data_type_(gl::gl_type<T>())
{
    GLuint buf;
    glGenBuffers(1, &buf);
    buffer_ = std::shared_ptr<GLuint>(new GLuint(buf), [](auto *id) {
        glDeleteBuffers(1, id);
        delete id;
    });

    bind();
    glBufferData(buffer_type_, static_cast<GLsizeiptr>(num_elements_ * sizeof(T)), data, usage);
    unbind();
}

template <typename T>
void BufferWrapper<T>::bind() const
{
    glBindBuffer(buffer_type_, get_id());
}

template <typename T>
void BufferWrapper<T>::unbind() const
{
    glBindBuffer(buffer_type_, 0);
}

template <typename T>
void BufferWrapper<T>::update(size_t start_index, size_t num_elements, const T *data) const
{
    assert(num_elements + start_index <= num_elements_);

    bind();
    glBufferSubData(buffer_type_,
                    static_cast<GLintptr>(start_index * sizeof(T)),
                    static_cast<GLsizeiptr>(num_elements * sizeof(T)),
                    data);
    unbind();
}

template <typename T>
void BufferWrapper<T>::update(size_t start_index, const std::vector<T> &data) const
{
    update(start_index, data.size(), data.data());
}

template <typename T>
GLuint BufferWrapper<T>::get_id() const
{
    return *buffer_;
}

template <typename T>
GLenum BufferWrapper<T>::get_buffer_type() const
{
    return buffer_type_;
}

template <typename T>
GLenum BufferWrapper<T>::get_data_type() const
{
    return data_type_;
}

template <typename T>
std::size_t BufferWrapper<T>::get_num_elements() const
{
    return num_elements_;
}

template <typename T>
void BufferWrapper<T>::get_data(T *data, std::size_t num_elements, std::size_t start_index) const
{
    assert(num_elements + start_index <= num_elements_);
    bind();
    glGetBufferSubData(buffer_type_, start_index * sizeof(T), num_elements * sizeof(T), data);
    unbind();
}

template <typename T>
void BufferWrapper<T>::get_data(std::vector<T> *data, std::size_t start_index) const
{
    assert(data);
    data->resize(num_elements_ - start_index);
    get_data(data->data(), data->size(), start_index);
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
