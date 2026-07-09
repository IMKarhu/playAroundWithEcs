#include "glframebuffer.h"
#include <glad/glad.h>

#include <print>
#include <assert.h>

// needs more formats and not sure if this is even the right way to go about this
// also does not support multiple levels yet.
static void createTextureStorage(uint32_t& id, Format format, uint32_t width, uint32_t height)
{
    switch(format) {
        case Format::RGBA8:
            glTextureStorage2D(id, 1, GL_RGBA8, width, height);
            break;
        case Format::RGBA16F:
            glTextureStorage2D(id, 1, GL_RGBA16F, width, height);
            break;
        case Format::RGB16F:
            glTextureStorage2D(id, 1, GL_RGB16F, width, height);
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

static void createRenderBufferStorage(uint32_t &id, uint32_t &fboid, DepthFormat format, uint32_t width, uint32_t height)
{
    switch(format) {
        case DepthFormat::Depth24Stencil8:
            glNamedRenderbufferStorage(id, GL_DEPTH24_STENCIL8, width, height);
            glNamedFramebufferRenderbuffer(fboid, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, id);
            break;
        case DepthFormat::Depth32F:
            glNamedRenderbufferStorage(id, GL_DEPTH_COMPONENT32F, width, height);
            glNamedFramebufferRenderbuffer(fboid, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, id);
            break;
        default:
            std::println("failed to create renderbuffer storage");
    }
}


GLFramebuffer::GLFramebuffer(const FramebufferSpec& spec)
    :m_spec(spec)
{
    create();
}

GLFramebuffer::~GLFramebuffer()
{
}

void GLFramebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}
void GLFramebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLFramebuffer::resize(uint32_t width, uint32_t height)
{
    if (m_spec.width == width && m_spec.height == height)
        return;

    invalidate();
    m_spec.width = width;
    m_spec.height = height;
    create();
}

TextureHandle GLFramebuffer::colorAttachment(uint32_t index) const
{
    assert(index < m_colorattachments.size() && "index is bigger than attachment vector size");
    return m_colorattachments[index];
}

FramebufferSpec &GLFramebuffer::framebufferSpec()
{
    return m_spec;
}

void GLFramebuffer::create()
{
    glCreateFramebuffers(1, &m_fbo);
    m_colorattachments.resize(m_spec.attachments.size());
    for (size_t i = 0; i < m_colorattachments.size(); ++i) {
        std::println("framebuffer: {}, and format: {}", i, int(m_spec.attachments[i].format));
        glCreateTextures(GL_TEXTURE_2D, 1, &m_colorattachments[i].id);
        createTextureStorage(m_colorattachments[i].id, m_spec.attachments[i].format, m_spec.width, m_spec.height);
        glTextureParameteri(m_colorattachments[i].id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_colorattachments[i].id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(m_colorattachments[i].id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_colorattachments[i].id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glNamedFramebufferTexture(m_fbo, GL_COLOR_ATTACHMENT0 + i, m_colorattachments[i].id, 0);
    }

    if (m_spec.depthFormat != DepthFormat::None) {
        if ( m_spec.depthType == DepthFormatType::Texture) {
            glCreateTextures(GL_TEXTURE_2D, 1, &m_depthattachment.id);
            createTextureStorageDepth(m_depthattachment.id, m_spec.depthFormat, m_spec.width, m_spec.height);
            glTextureParameteri(m_depthattachment.id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(m_depthattachment.id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTextureParameteri(m_depthattachment.id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTextureParameteri(m_depthattachment.id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            switch(m_spec.depthFormat) {
                case DepthFormat::Depth24Stencil8:
                    glNamedFramebufferTexture(m_fbo, GL_DEPTH_STENCIL_ATTACHMENT, m_depthattachment.id, 0);
                    break;
                case DepthFormat::Depth32F:
                    glNamedFramebufferTexture(m_fbo, GL_DEPTH_ATTACHMENT, m_depthattachment.id, 0);
                    break;
            }
        }
        else if (m_spec.depthType == DepthFormatType::Buffer) {
            glCreateRenderbuffers(1, &m_rbo);
            createRenderBufferStorage(m_rbo, m_fbo, m_spec.depthFormat, m_spec.width, m_spec.height);
        }
    }

    if (!m_spec.attachments.empty()) {
        GLenum drawBuffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
        glNamedFramebufferDrawBuffers(m_fbo, m_colorattachments.size(), drawBuffers);
    }
    else if (m_spec.attachments.empty()) {
        //depth only pass
        glNamedFramebufferDrawBuffers(m_fbo, 1, GL_NONE);
    }

    if (glCheckNamedFramebufferStatus(m_fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::println("Framebuffer is not complete");
    }

}

void GLFramebuffer::invalidate()
{
    glDeleteFramebuffers(1, &m_fbo);
    for (auto attachment : m_colorattachments) {
        if (attachment.id) {
            glDeleteTextures(1, &attachment.id);
        }
    }
    if (m_depthattachment.id) {
        glDeleteTextures(1, &m_depthattachment.id);
    }
    if (m_rbo) {
        glDeleteRenderbuffers(1, &m_rbo);
    }
}
