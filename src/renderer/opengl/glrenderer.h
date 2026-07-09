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

class FrameBuffer;
class FrameBufferManager;
class Window;
class Shader;
class GLShader;

namespace Lumos
{
    class AssetManager;
}

class RENDERER_API GLRenderer : public Renderer
{
public:
    GLRenderer(const Window &window);
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
    Lumos::IGraphicsDevice& getGraphicsDevice() override;
private:
    uint32_t m_width = 0;
    uint32_t m_height = 0;
    std::unordered_map<std::string, Shader*> m_shadercache;
    std::vector<RenderInfo> m_renderqueue;
    std::shared_ptr<FrameBufferManager> m_framebufferManager;
    std::unique_ptr<Lumos::IGraphicsDevice> m_graphicsdevice;
    const Window& m_window;
    void draw();
};
