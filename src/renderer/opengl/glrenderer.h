#pragma once
#include "renderer.h"
#include "ecsImpl/components.h"
#include <memory>
#include <unordered_map>

#ifdef RENDERER_EXPORT
#define RENDERER_API __declspec(dllexport)
#else
#define RENDERER_API __declspec(dllimport)
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
    void renderScene(const RenderInfo &info) const override;
    void endFrame() const override;
    void createAndAddToShaderCache(std::string name,
                                   const std::string vertpath,
                                   const std::string fragpath) override;
    void createMeshPrimitive(std::string name,
                             std::vector<Vertex> vertices,
                             std::vector<uint32_t> indices,
                             uint32_t vbo,
                             uint32_t ebo) const override;
    void resizeFramebuffer(uint32_t width, uint32_t height);

private:
    std::unordered_map<std::string, Shader*> m_shadercache;
    //This might just be a temporary solution.
    //Instead of initializing same model multiple time
    //just create it once and cache vao, ebo and indice count
    mutable std::unordered_map<std::string, uint32_t> m_vaocache;
    mutable std::unordered_map<std::string, uint32_t> m_indexcache;
    std::shared_ptr<FrameBufferManager> m_framebufferManager;
};
