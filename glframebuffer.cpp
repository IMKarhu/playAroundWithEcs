#include "glframebuffer.h"
#include <glad/glad.h>

#include <print>


GLFramebuffer::GLFramebuffer(FramebufferSpec& spec)
    :m_spec(spec)
{
    create();
    std::println("creating framebuffer");
}

GLFramebuffer::~GLFramebuffer()
{
}

void GLFramebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_spec.m_fbo);
}
void GLFramebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLFramebuffer::resize(uint32_t width, uint32_t height)
{
    if (m_spec.m_width == width || m_spec.m_height == height)
        return;

    m_spec.m_width = width;
    m_spec.m_height = height;
    create();
}

uint32_t GLFramebuffer::colorAttachment() const
{
    return m_spec.m_colorAttachment;
}

void GLFramebuffer::create()
{
    glGenFramebuffers(1, &m_spec.m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_spec.m_fbo);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_spec.m_colorAttachment);
    glBindTexture(GL_TEXTURE_2D, m_spec.m_colorAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_spec.m_width, m_spec.m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_spec.m_colorAttachment, 0);

    // glCreateTextures(GL_TEXTURE_2D, 1, &m_depthAttachment);
    // glBindTexture(GL_TEXTURE_2D, m_depthAttachment);
    // glTextureStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_width, m_height);
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::println("Framebuffer is not complete");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

