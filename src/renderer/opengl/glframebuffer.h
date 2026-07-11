#pragma once
#include "framebuffer.h"
#include "ecsImpl/components.h"

#ifdef _WIN32
    #ifdef RENDERER_EXPORT
    #define RENDERER_API __declspec(dllexport)
    #else
    #define RENDERER_API __declspec(dllimport)
    #endif
#else
    #define RENDERER_API
#endif

namespace Lumos
{
    class GLFramebuffer : public FrameBuffer
    {
        public:
            GLFramebuffer(const FramebufferSpec& spec);
            ~GLFramebuffer();

            void bind() override;
            void unbind() override;

            void resize(uint32_t width, uint32_t height) override;

            TextureHandle colorAttachment(uint32_t index) const override;
            FramebufferSpec &framebufferSpec() override;
        private:
            void create();
            void invalidate();
            TextureHandle m_depthattachment;
            std::vector<TextureHandle> m_colorattachments;
            uint32_t m_fbo;
            uint32_t m_rbo;
            FramebufferSpec m_spec;
    };
}// namespace Lumos

