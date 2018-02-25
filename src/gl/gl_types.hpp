#pragma once

#include <glad/glad.h>
#include <string>
#include <memory>

namespace gl {

struct VAOElement
{
    std::string name;
    GLint size;
    GLenum type;
    void *pointer;
};

template <int Dim>
class Framebuffer;

template <typename T>
class Buffer;

class VertexArray;

template <typename VboType = float, typename IboType = unsigned>
struct Pipeline
{
    std::shared_ptr<GLuint> program{nullptr};
    std::shared_ptr<Buffer<VboType>> vbo{nullptr};
    std::shared_ptr<Buffer<IboType>> ibo{nullptr};
    std::shared_ptr<VertexArray> vao{nullptr};
    std::shared_ptr<Framebuffer<2>> fbo{nullptr};
};

} // namespace