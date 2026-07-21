#pragma once
#include "renderer.h"
#include <memory>
#include <unordered_map>
#include <vector>

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
    class FrameBuffer;
    class FrameBufferManager;
    class Shader;
    class GLShader;
    class AssetManager;
    class RENDERER_API GLRenderer : public Renderer
    {
        public:
            GLRenderer();
            ~GLRenderer() override;

            void beginFrame() const override;
            void endFrame() override;

            void beginPass(const RenderPassDesc& desc, FrameBufferManager& framebuffermanager) override;
            void endPass() override;

            void submit(RenderInfo info) override;
            void flush(std::function<void()> func, Lumos::AssetManager& assetmanager) override;
            void createAndAddToShaderCache(std::string name,
                    const std::string vertpath,
                    const std::string fragpath) override;
            IGPUResourceFactory& getGPUResourceFactory() override;
            void setDefaultFramebufferDimensios(uint32_t width, uint32_t height) override;
        private:
            //these are for default framebuffer
            uint32_t m_width;
            uint32_t m_height;
            std::unordered_map<std::string, Shader*> m_shadercache;
            std::vector<RenderInfo> m_renderqueue;
            std::shared_ptr<FrameBufferManager> m_framebufferManager;
            std::unique_ptr<IGPUResourceFactory> m_graphicsdevice;
            void draw();
    };
}

