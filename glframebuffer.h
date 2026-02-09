#pragma once
#include "framebuffer.h"

class GLFramebuffer : public FrameBuffer
{
public:
    GLFramebuffer(FramebufferSpec& spec);
    ~GLFramebuffer();

    void bind() override;
    void unbind() override;

    void resize(uint32_t width, uint32_t height) override;

    uint32_t colorAttachment() const override;
    FramebufferSpec &framebufferSpec() override;
private:
    void create();
    FramebufferSpec m_spec;
};
