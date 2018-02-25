#include "framebuffer.hpp"
#include <glm/gtc/vec1.hpp>

namespace gl {

namespace {

std::shared_ptr<GLuint> create_texture(const glm::uvec3 &dim,
                                       GLenum tex_type,
                                       const float *array = nullptr,
                                       GLint internal_format = GL_RGBA32F,
                                       GLenum format = GL_RGBA,
                                       GLint filter_type = GL_NEAREST,
                                       GLint wrap_type = GL_REPEAT)
{
    GLuint tex;
    glGenTextures(1, &tex);
    std::shared_ptr<GLuint> texture(new GLuint(tex), [](auto *pID) {
        glDeleteTextures(1, pID);
        delete pID;
    });

    glBindTexture(tex_type, tex);

    glTexParameteri(tex_type, GL_TEXTURE_WRAP_S, wrap_type);
    glTexParameteri(tex_type, GL_TEXTURE_WRAP_T, wrap_type);

    glTexParameteri(tex_type, GL_TEXTURE_MIN_FILTER, filter_type);
    glTexParameteri(tex_type, GL_TEXTURE_MAG_FILTER, filter_type);

    switch (tex_type) {
    case GL_TEXTURE_3D:
        glTexImage3D(tex_type, 0, internal_format, dim.x, dim.y, dim.z, 0, format, GL_FLOAT, array);
        break;
    case GL_TEXTURE_2D:
        glTexImage2D(tex_type, 0, internal_format, dim.x, dim.y, 0, format, GL_FLOAT, array);
        break;
    case GL_TEXTURE_1D:
        glTexImage1D(tex_type, 0, internal_format, dim.x, 0, format, GL_FLOAT, array);
        break;
    default:
        assert(false);
        break;
    }

    return texture;
} // create_texture

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
Framebuffer<1>::Framebuffer(
    glm::uvec1 dim, const float *array, GLint internal_format, GLenum format, GLint filter_type, GLint wrap_type)
{
    glm::uvec3 full_dim = {dim.x, 1, 1};
    texture_ = create_texture(full_dim, GL_TEXTURE_1D, array, internal_format, format, filter_type, wrap_type);
    framebuffer_ = create_framebuffer(full_dim, GL_TEXTURE_1D, *texture_);
}

template <>
Framebuffer<2>::Framebuffer(
    glm::uvec2 dim, const float *array, GLint internal_format, GLenum format, GLint filter_type, GLint wrap_type)
{
    glm::uvec3 full_dim = {dim.x, dim.y, 1};
    texture_ = create_texture(full_dim, GL_TEXTURE_2D, array, internal_format, format, filter_type, wrap_type);
    framebuffer_ = create_framebuffer(full_dim, GL_TEXTURE_2D, *texture_);
}

template <>
Framebuffer<3>::Framebuffer(
    glm::uvec3 dim, const float *array, GLint internal_format, GLenum format, GLint filter_type, GLint wrap_type)
{
    texture_ = create_texture(dim, GL_TEXTURE_3D, array, internal_format, format, filter_type, wrap_type);
    framebuffer_ = create_framebuffer(dim, GL_TEXTURE_3D, *texture_);
}

template <int Dim>
void Framebuffer<Dim>::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer_);
}

template <int Dim>
void Framebuffer<Dim>::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

template <int Dim>
template <typename UsageFunc>
void Framebuffer<Dim>::use(const UsageFunc &usage_func) const
{
    bind();
    usage_func();
    unbind();
}

template <int Dim>
GLuint Framebuffer<Dim>::get_texture_id() const
{
    return *texture_;
}

template <int Dim>
GLuint Framebuffer<Dim>::get_framebuffer_id() const
{
    return *framebuffer_;
}

template <int Dim>
std::shared_ptr<Framebuffer<Dim>> create_shared_framebuffer(glm::vec<Dim, unsigned> dim,
                                                            const float *array,
                                                            GLint internal_format,
                                                            GLenum format,
                                                            GLint filter_type,
                                                            GLint wrap_type)
{
    return std::make_shared<Framebuffer<Dim>>(dim, array, internal_format, format, filter_type, wrap_type);
}

template class Framebuffer<1>;
template class Framebuffer<2>;
template class Framebuffer<3>;

template std::shared_ptr<Framebuffer<1>>
create_shared_framebuffer(glm::uvec1, const float *, GLint, GLenum, GLint, GLint);
template std::shared_ptr<Framebuffer<2>>
create_shared_framebuffer(glm::uvec2, const float *, GLint, GLenum, GLint, GLint);
template std::shared_ptr<Framebuffer<3>>
create_shared_framebuffer(glm::uvec3, const float *, GLint, GLenum, GLint, GLint);

} // namespace gl