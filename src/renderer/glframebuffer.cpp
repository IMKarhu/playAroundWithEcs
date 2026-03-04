#include "glframebuffer.h"
#include <glad/glad.h>

#include <print>

// needs more formats and not sure if this is even the right way to go about this
// also does not support multiple levels yet.
static void createTextureStorage(uint32_t& id, Format format, uint32_t width, uint32_t height)
{
    switch(format) {
        case Format::RGBA8:
            glTextureStorage2D(id, 1, GL_RGBA8, width, height);
            break;
        default:
            std::println("failed to create texture storage");
    }
}

static void createTextureStorageDepth(uint32_t& id, DepthFormat format, uint32_t width, uint32_t height)
{
    switch(format) {
        case DepthFormat::Depth24Stencil8:
            glTextureStorage2D(id, 1, GL_DEPTH24_STENCIL8, width, height);
            break;
        case DepthFormat::Depth32F:
            glTextureStorage2D(id, 1, GL_DEPTH_COMPONENT32F, width, height);
            break;
        default:
            std::println("failed to create texture storage");
    }
}


GLFramebuffer::GLFramebuffer(const FramebufferSpec& spec)
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

//TODO: is this correct? always get the first attachment, probably not. needs to be fixed
uint32_t GLFramebuffer::colorAttachment() const
{
    return m_spec.attachments[0].texture;
}

FramebufferSpec &GLFramebuffer::framebufferSpec()
{
    return m_spec;
}

void GLFramebuffer::create()
{
    glGenFramebuffers(1, &m_spec.fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_spec.fbo);

    for (size_t i = 0; i < m_spec.attachments.size(); ++i) {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_spec.attachments[i].texture);
        createTextureStorage(m_spec.attachments[i].texture, m_spec.attachments[i].format, m_spec.width, m_spec.height);
        // glTextureStorage2D(m_spec.attchments[i].texture, 1, (uint32_t)m_spec.attchments[i].format, m_spec.width, m_spec.height);
        glTextureParameteri(m_spec.attachments[i].texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_spec.attachments[i].texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(m_spec.attachments[i].texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_spec.attachments[i].texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glNamedFramebufferTexture(m_spec.fbo, GL_COLOR_ATTACHMENT0 + i, m_spec.attachments[i].texture, 0);
    }

    if (m_spec.depthFormat != DepthFormat::None) {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_spec.depthAttachment);
        createTextureStorageDepth(m_spec.depthAttachment, m_spec.depthFormat, m_spec.width, m_spec.height);
        // glTextureStorage2D(m_spec.depthAttachment, 1, (uint32_t)m_spec.depthFormat, m_spec.width, m_spec.height);
        glTextureParameteri(m_spec.depthAttachment, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_spec.depthAttachment, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(m_spec.depthAttachment, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_spec.depthAttachment, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        switch(m_spec.depthFormat) {
            case DepthFormat::Depth24Stencil8:
                glNamedFramebufferTexture(m_spec.fbo, GL_DEPTH_STENCIL_ATTACHMENT, m_spec.depthAttachment, 0);
                break;
            case DepthFormat::Depth32F:
                glNamedFramebufferTexture(m_spec.fbo, GL_DEPTH_ATTACHMENT, m_spec.depthAttachment, 0);
                break;
        }
    }

    if (m_spec.attachments.size() > 1) {
        GLenum drawBuffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
        glNamedFramebufferDrawBuffers(m_spec.fbo, m_spec.attachments.size(), drawBuffers);
    }
    else if (m_spec.attachments.empty()) {
        //depth only pass
        glNamedFramebufferDrawBuffers(m_spec.fbo, 1, GL_NONE);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::println("Framebuffer is not complete");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

