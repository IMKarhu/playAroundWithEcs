#pragma once
#include "framebuffer.h"

#ifdef _WIN32
    #ifdef RENDERER_EXPORT
    #define RENDERER_API __declspec(dllexport)
    #else
    #define RENDERER_API __declspec(dllimport)
    #endif
#else
    #define RENDERER_API
#endif

class GLFramebuffer : public FrameBuffer
{
public:
    GLFramebuffer(const FramebufferSpec& spec);
    ~GLFramebuffer();

    void bind() override;
    void unbind() override;

    void resize(uint32_t width, uint32_t height) override;

    uint32_t colorAttachment() const override;
    FramebufferSpec &framebufferSpec() override;
private:
    void create();
    void invalidate();
    FramebufferSpec m_spec;
};
