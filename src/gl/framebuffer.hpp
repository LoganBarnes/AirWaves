#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>

namespace gl {

template <int Dim>
class Framebuffer
{
public:
    explicit Framebuffer(glm::vec<Dim, unsigned> dim,
                         const float *array = nullptr,
                         GLint internal_format = GL_RGBA32F,
                         GLenum format = GL_RGBA,
                         GLint filter_type = GL_NEAREST,
                         GLint wrap_type = GL_REPEAT);

    void bind() const;
    void unbind() const;

    template <typename UsageFunc>
    void use(const UsageFunc &usage_func) const;

    GLuint get_texture_id() const;
    GLuint get_framebuffer_id() const;

private:
    std::shared_ptr<GLuint> texture_;
    std::shared_ptr<GLuint> framebuffer_;
};

template <int Dim>
std::shared_ptr<Framebuffer<Dim>> create_shared_framebuffer(glm::vec<Dim, unsigned> dim,
                                                            const float *array = nullptr,
                                                            GLint internal_format = GL_RGBA32F,
                                                            GLenum format = GL_RGBA,
                                                            GLint filter_type = GL_NEAREST,
                                                            GLint wrap_type = GL_REPEAT);

} // namespace gl
