#pragma once
#include <functional>
#include "renderInfo.h"
#include "renderPassDesc.h"
#include "assetmanagers/assetbase.h"

namespace Lumos
{
    class MeshManager;
    class FrameBufferManager;
    class AssetManager;
    class Renderer
    {
        public:
            virtual ~Renderer() = default;

            virtual void beginFrame() const = 0;
            virtual void endFrame() = 0;

            virtual void beginPass(const RenderPassDesc& desc, FrameBufferManager& framebuffermanager) = 0;
            virtual void endPass() = 0;

            virtual void submit(RenderInfo info) = 0;
            virtual void flush(std::function<void()> func, Lumos::AssetManager& assetmanager) = 0;
            virtual void createAndAddToShaderCache(std::string name,
                    const std::string vertpath,
                    const std::string fragpath) = 0;
            virtual Lumos::IGraphicsDevice& getGraphicsDevice() = 0;
            virtual void setDefaultFramebufferDimensios(uint32_t width, uint32_t height) = 0;
        private:
    };
}

