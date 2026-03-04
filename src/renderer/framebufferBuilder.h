#pragma once
#include "framebuffer.h"

// builder class, higher levels abstraction, should handle framebuffer creation for multiple APIs
class FramebufferBuilder
{
public:
    FramebufferBuilder& setSize(uint32_t width, uint32_t height);
    FramebufferBuilder& setSamples(uint32_t count);
    FramebufferBuilder& setColorAttachments(std::vector<Format> attachments);
    FramebufferBuilder& setDepthFormat(DepthFormat depthFormat);
    std::shared_ptr<FrameBuffer> build();
private:
    FramebufferSpec m_spec;
};
