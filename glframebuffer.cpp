#include "glframebuffer.h"
#include <glad/glad.h>

#include <print>


GLFramebuffer::GLFramebuffer(FramebufferSpec& spec)
    :m_spec(spec)
{
    std::println("creating framebuffer");
    create();
}

GLFramebuffer::~GLFramebuffer()
{
}

void GLFramebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_spec.fbo);
}
void GLFramebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLFramebuffer::resize(uint32_t width, uint32_t height)
{
    if (m_spec.width == width || m_spec.height == height)
        return;

    m_spec.width = width;
    m_spec.height = height;
    create();
}

uint32_t GLFramebuffer::colorAttachment() const
{
    return m_spec.colorAttachment;
}

FramebufferSpec &GLFramebuffer::framebufferSpec()
{
    return m_spec;
}

void GLFramebuffer::create()
{
    glGenFramebuffers(1, &m_spec.fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_spec.fbo);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_spec.colorAttachment);
    glTextureStorage2D(m_spec.colorAttachment, 1, GL_RGBA8, m_spec.width, m_spec.height);
    glTextureParameteri(m_spec.colorAttachment, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_spec.colorAttachment, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(m_spec.colorAttachment, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_spec.colorAttachment, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glNamedFramebufferTexture(m_spec.fbo, GL_COLOR_ATTACHMENT0, m_spec.colorAttachment, 0);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_spec.depthAttachment);
    glTextureStorage2D(m_spec.depthAttachment, 1, GL_DEPTH24_STENCIL8, m_spec.width, m_spec.height);
    glTextureParameteri(m_spec.depthAttachment, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_spec.depthAttachment, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(m_spec.depthAttachment, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_spec.depthAttachment, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glNamedFramebufferTexture(m_spec.fbo, GL_DEPTH_STENCIL_ATTACHMENT, m_spec.depthAttachment, 0);

    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
    glNamedFramebufferDrawBuffers(m_spec.fbo, 1, drawBuffers);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::println("Framebuffer is not complete");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

