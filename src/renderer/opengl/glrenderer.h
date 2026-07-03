#pragma once
#include "renderer.h"
#include "ecsImpl/components.h"
#include <memory>
#include <unordered_map>

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


class RENDERER_API GLRenderer : public Renderer
{
public:
    GLRenderer(const Window &window);
    ~GLRenderer() override;

    void initialize() const override;
    void beginFrame() const override;
    void renderScene(const RenderInfo &info, const MeshManager& meshmanager) const override;
    void endFrame() const override;
    void renderToScreen(const RenderInfo &info, const MeshManager& meshmanager) const override;
    void createAndAddToShaderCache(std::string name,
                                   const std::string vertpath,
                                   const std::string fragpath) override;
    MeshResource createMeshPrimitive(std::vector<Vertex> vertices,
                                     std::vector<uint32_t> indices) const override;
    TextureHandle createTexture(const std::string& filepath, TextureImportSettings settings) const override;
    void resizeFramebuffer(uint32_t width, uint32_t height);

private:
    std::unordered_map<std::string, Shader*> m_shadercache;
    std::shared_ptr<FrameBufferManager> m_framebufferManager;
};
