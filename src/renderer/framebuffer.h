#pragma once
#include <cstdint>

struct FramebufferSpec
{
    uint32_t width;
    uint32_t height;
    uint32_t fbo;
    uint32_t colorAttachment;
    uint32_t depthAttachment;
};

class FrameBuffer
{
public:
    virtual ~FrameBuffer() = default;

    //I am not really a fan of having these bind and unbind functions.. but it works for now..
    virtual void bind() = 0;
    virtual void unbind() = 0;

    virtual void resize(uint32_t width, uint32_t height) = 0;

    virtual uint32_t colorAttachment() const = 0;
    virtual FramebufferSpec &framebufferSpec() = 0;
private:
};

