#include "framebufferBuilder.h"

FramebufferBuilder& FramebufferBuilder::setSize(uint32_t width, uint32_t height)
{
    m_spec.width = width;
    m_spec.height = height;
    return *this;
}

FramebufferBuilder& FramebufferBuilder::setSamples(uint32_t count)
{
    m_spec.samples = count;
    return *this;
}

FramebufferBuilder&FramebufferBuilder::setColorAttachments(std::vector<Format> attachments)
{
    for (size_t i = 0; i < attachments.size(); ++i) {
        m_spec.attachments[i].format = attachments[i];
    }
    return *this;
}

FramebufferBuilder& FramebufferBuilder::setDepthFormat(DepthFormat depthFormat)
{
    m_spec.depthFormat = depthFormat;
    return *this;
}

std::shared_ptr<FrameBuffer> FramebufferBuilder::build() 
{
    return FrameBuffer::create(m_spec);
}
