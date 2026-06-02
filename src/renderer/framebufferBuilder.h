#pragma once
#include "framebuffer.h"

#ifdef RENDERER_EXPORT
#define RENDERER_API __declspec(dllexport)
#else
#define RENDERER_API __declspec(dllimport)
#endif

// builder class, higher levels abstraction, should handle framebuffer creation for multiple APIs
class RENDERER_API FramebufferBuilder
{
public:
    FramebufferBuilder& setSize(uint32_t width, uint32_t height);
    FramebufferBuilder& setSamples(uint32_t count);
    FramebufferBuilder& setColorAttachments(std::vector<Format> attachments);
    FramebufferBuilder& setDepthFormat(DepthFormat depthFormat, DepthFormatType type = DepthFormatType::Buffer);
    std::shared_ptr<FrameBuffer> build();
private:
    FramebufferSpec m_spec;
};
